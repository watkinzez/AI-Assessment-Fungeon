#include "Agent.h"
#include "Gizmos.h"

Mesh* Agent::m_mesh;

Agent::Agent()
{
	m_agentID = m_mesh->requestID();
	m_destination = nullptr;
	m_goal = nullptr;
	m_dying = 0.0f;

	m_currentNode = m_mesh->getSpawnPoint();
	m_transform[3].xyz = m_currentNode->position;
	m_colour = glm::vec4(1,1,1,1);

	m_goalNode = nullptr;

	m_blackboard = Blackboard::getInstance();

	//construct behaviour
	Selector* selector = new Selector();
	Sequence* sequence = new Sequence();

	HasTask* hasTask = new HasTask();
	UpdatePath* updatePath = new UpdatePath();
	CheckBlackboard* checkBlackboard = new CheckBlackboard();
	HasPath* hasPath = new HasPath();
	WithinRange* withinRange = new WithinRange(0.1f);
	MoveToDestination* moveToDestination = new MoveToDestination(3);
	AtEndOfPath* atEndOfPath = new AtEndOfPath();
	PerformAction* performAction = new PerformAction();
	PickNextNodeInPath* pickNextNodeInPath = new PickNextNodeInPath();
	OnElecTile* onElecTile = new OnElecTile();
	Dying* dying = new Dying();
	Respawn* respawn = new Respawn();
	
	sequence->addChild(atEndOfPath);
	sequence->addChild(performAction);

	selector->addChild(sequence);
	selector->addChild(pickNextNodeInPath);

	sequence = new Sequence();
	sequence->addChild(withinRange);
	sequence->addChild(selector);

	selector = new Selector();
	selector->addChild(sequence);
	selector->addChild(moveToDestination);

	sequence = new Sequence();
	sequence->addChild(hasPath);
	sequence->addChild(selector);

	Sequence* sequence2 = new Sequence();
	sequence2->addChild(hasTask);
	sequence2->addChild(updatePath);
	sequence2->addChild(sequence);

	selector = new Selector();
	selector->addChild(sequence2);
	selector->addChild(checkBlackboard);

	Selector* selector2 = new Selector();
	sequence = new Sequence();

	selector2->addChild(dying);
	selector2->addChild(respawn);

	sequence->addChild(onElecTile);
	sequence->addChild(selector2);

	selector2 = new Selector();
	selector2->addChild(sequence);
	selector2->addChild(selector);

	m_behaviour = selector2;
}

Agent::~Agent()
{
	delete m_behaviour;

	//maybe put this in fungeon.cpp, but here for now
	Blackboard::deleteInstance();
}

void Agent::setPath()
{
	for(auto key : m_heldKeys)
	{
		//key 5 is a master key, ignores locked doors
		if(key->getKeyNo() == 5)
		{
			m_destination = m_mesh->calculateAStar(m_agentID, m_currentNode, m_goalNode, true);
		}
	}
	m_destination = m_mesh->calculateAStar(m_agentID, m_currentNode, m_goalNode, false);
}

void Agent::drawPath()
{
	Mesh::NavMeshNode* node = m_destination;
	while(node != nullptr)
	{
		switch(m_agentID)
		{
		case 0:
			Gizmos::addAABBFilled(node->position, glm::vec3(0.4,0.1,0.4), glm::vec4(0, 1, 0, 1));
			break;
		case 1:
			Gizmos::addAABBFilled(node->position, glm::vec3(0.4,0.1,0.4), glm::vec4(0, 0, 1, 1));
			break;
		};
		
		node = node->next[m_agentID];
	}
}

void Agent::update(float a_deltaTime)
{
	if(m_behaviour != nullptr)
	{
		m_behaviour->execute(this);
	}

	Gizmos::addAABBFilled(m_transform[3].xyz, glm::vec3(0.3), m_colour);

	int heldKeyCount = m_heldKeys.size();
	for(int i = 0; i < heldKeyCount; ++i)
	{
		Gizmos::addSphere(m_transform[3].xyz + glm::vec3(0, i + 1, 0), 4, 4, 0.3f, m_heldKeys[i]->getColour());
	}

	drawPath();
}

//goes through all the requests and subscribes to any that the agent is eligible for
void Agent::checkBlackboardRequests()
{
	int numberofRequests = m_blackboard->getNumberofRequests();
	Request* request;
	for(int i = 0; i < numberofRequests; ++i)
	{
		request = m_blackboard->getRequest(i);
		if(request->isHired())
		{
			//don't bother subscribing for a request that already has an employee
			continue;
		}
		if(dynamic_cast<GetKey*>(request))
		{
			//the way i've designed the game, requests to get keys should only happen
			//in a state where every minion should be able to retrieve them.
			//thus, everyone is eligible and the agent will automatically subscribe.

			//if problems arise that i haven't forseen, the agent could attempt to create a path to the key.
			//if that succeeds, they are eligible. else, they are not
			if(request->hasSubscribed(this) == false)
			{
				request->subscribe(this);
			}
		}
		if(dynamic_cast<OpenDoor*>(request))
		{
			//if the agent has the key to unlock the door, subscribe
			if( checkForKey( dynamic_cast<OpenDoor*>(request)->getDoor() ) )
			{
				if(request->hasSubscribed(this) == false)
				{
					request->subscribe(this);
				}
			}
		}
		if(dynamic_cast<GoToDoor*>(request))
		{
			//if the agent has at least one of the keys needed to unlock the door, subscribe
			if( checkForKey( dynamic_cast<GoToDoor*>(request)->getDoor(), dynamic_cast<GoToDoor*>(request)->getWithKey() ) )
			{
				if(request->hasSubscribed(this) == false)
				{
					request->subscribe(this);
				}
			}
		}
	}
}

//checks if the agent has the key to unlock a given door
bool Agent::checkForKey(Door* a_door, bool a_withKey)
{
	//case 6 and 7 are special cases. they require more than one key to unlock the given door
	
	switch(a_door->getKeyNo())
	{
	case 6:
		//requires keys 1 or 2
		{
			for(auto key : m_heldKeys)
			{
				int keyNo = key->getKeyNo();
				if(a_withKey)
				{
					if(keyNo == 1)
					{
						return true;
					}
				}
				else
				{
					if(keyNo == 2)
					{
						return true;
					}
				}
			}
			break;
		}
	case 7:
		//requires keys 3 or 4
		{
			for(auto key : m_heldKeys)
			{
				int keyNo = key->getKeyNo();
				if(a_withKey)
				{
					if(keyNo == 3)
					{
						return true;
					}
				}
				else
				{
					if(keyNo == 4)
					{
						return true;
					}
				}
			}
			break;
		}
	default:
		{
			for(auto key : m_heldKeys)
			{
				if(key->getKeyNo() == a_door->getKeyNo())
				{
					return true;
				}
			}
			break;
		}
	}
	return false;
}

//tag the goal as completed, and remove its pointer from the agent
void Agent::completeTask()
{
	m_goal->complete();
	m_goal = nullptr;
	m_goalNode = nullptr;
}

bool Agent::checkPathForChange()
{
	if(hasDestination() == false)
	{
		return false;
	}

	//m_destination = immediate next node
	Mesh::NavMeshNode* pathNode = m_destination;
	while(pathNode != nullptr)
	{
		pathNode = pathNode->next[m_agentID];
		if(pathNode != nullptr && pathNode->alteredWeight)
		{
			//if path node has been altered, return true
			return true;
		}
	}
	return false;
}

bool Agent::onElecTile()
{
	if(m_currentNode->weight == 3)
	{
		m_dying = 1.0f;
		return true;
	}
	return false;
}

//animation for dying. returns true once animation has completed
bool Agent::dyingAnimation()
{
	float deltaTime = Utility::getDeltaTime();
	m_dying -= deltaTime;

	if(isDying() == false)
	{
		//reset y position
		m_transform[3].y = 0;
		m_colour.a = 1;
		return true;
	}

	m_transform[3].y += deltaTime;
	m_colour.a -= deltaTime;

	return false;
}

void Agent::respawn()
{
	//loop to disown all held keys
	for(auto key : m_heldKeys)
	{
		key->disowned();
	}
	m_heldKeys.clear();

	m_currentNode = m_mesh->getSpawnPoint();
	m_transform[3].xyz = m_currentNode->position;

	m_destination = nullptr;
}