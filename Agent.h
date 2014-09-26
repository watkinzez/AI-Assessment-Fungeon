#ifndef AGENT_H_
#define AGENT_H_

#include "Blackboard.h"
#include "Mesh.h"
#include "Behaviour.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

#include "Utilities.h"

class Agent
{
public:

	Agent();
	virtual ~Agent();

	void setPath();

	virtual void update(float a_deltaTime);

	void drawPath();

	void checkBlackboardRequests();
	bool checkForKey(Door* a_door, bool a_withKey = false);
	void completeTask();
	bool checkPathForChange();
	bool onElecTile();
	bool dyingAnimation();
	void respawn();

	const glm::vec3 getPosition() const { return m_transform[3].xyz; }
	const glm::vec3 getDestination() const { return (m_destination == nullptr) ? glm::vec3(0) : m_destination->position; }
	const Mesh::NavMeshNode* checkNextDestination() const { return m_destination->next[m_agentID]; }
	Request* getGoal() { return m_goal; }
	bool hasGoal() { return (m_goal != nullptr); }
	bool hasDestination() { return (m_destination != nullptr); }
	bool isDying() { return (m_dying > 0.0f); };
	
	void setNextDestination() { m_destination = m_destination->next[m_agentID]; }
	void setPosition(const glm::vec3& a_position) { m_transform[3].xyz = a_position; }
	void updateCurrentNode() { m_currentNode = m_destination; }
	void setGoal(Request* a_goal) { m_goal = a_goal; }
	void setGoalNode(Mesh::NavMeshNode* a_node) { m_goalNode = a_node; }
	void addKey(Key* a_key) { a_key->owned(); m_heldKeys.push_back(a_key); }

	static Mesh* m_mesh;

protected:

	unsigned int m_agentID;
	glm::mat4 m_transform;
	glm::vec4 m_colour;
	Mesh::NavMeshNode* m_destination; // destination is only the immediate next node.
	std::vector<Key*> m_heldKeys;
	Behaviour* m_behaviour;
	Request* m_goal;
	Mesh::NavMeshNode* m_currentNode;
	Mesh::NavMeshNode* m_goalNode; // goalNode is the final target.
	float m_dying;

	Blackboard* m_blackboard;
};

//checks if within a given range from the current destination
class WithinRange : public Behaviour
{
public:

	WithinRange(float a_range) : m_range2(a_range * a_range) {}
	virtual ~WithinRange() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		float dist2;

		dist2 = glm::distance2(a_agent->getPosition(), a_agent->getDestination());

		if(dist2 <= m_range2)
		{
			a_agent->updateCurrentNode();
			return BR_SUCCESS;
		}
		return BR_FAILURE;
	}

	float m_range2;
};

//checks if there are more nodes in the current path to traverse
class AtEndOfPath : public Behaviour
{
public:

	AtEndOfPath() {}
	virtual ~AtEndOfPath() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		if(a_agent->checkNextDestination() == nullptr)
		{
			a_agent->setNextDestination(); // sets m_destination to nullptr
			return BR_SUCCESS;
		}
		return BR_FAILURE;
	}
};

//assigns a new destination from the current path
class PickNextNodeInPath : public Behaviour
{
public:

	PickNextNodeInPath() {}
	virtual ~PickNextNodeInPath() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		a_agent->setNextDestination();
		return BR_SUCCESS;
	}
};

//move closer to the current destination
class MoveToDestination : public Behaviour
{
public:

	MoveToDestination(float a_speed) : m_speed(a_speed) {}
	virtual ~MoveToDestination() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		glm::vec3 pos = a_agent->getPosition();
		glm::vec3 dir = glm::normalize(a_agent->getDestination() - pos);
		a_agent->setPosition(pos + dir * Utility::getDeltaTime() * m_speed);
		return BR_SUCCESS;
	}

	float m_speed;
};

class PerformAction : public Behaviour
{
public:

	PerformAction() {}
	virtual ~PerformAction() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		if(!a_agent->hasGoal())
		{
			return BR_FAILURE;
		}

		if(dynamic_cast<GetKey*>(a_agent->getGoal()))
		{
			//add key to held keys
			Key* key = dynamic_cast<GetKey*>(a_agent->getGoal())->getKey();
			a_agent->addKey(key);
		}

		if(dynamic_cast<OpenDoor*>(a_agent->getGoal()))
		{
			//open door
			Door* door = dynamic_cast<OpenDoor*>(a_agent->getGoal())->getDoor();
			door->openDoor();
		}

		//set goal to completed, remove goal, remove goal node
		a_agent->completeTask();

		return BR_SUCCESS;
	}
};

class HasTask : public Behaviour
{
public:

	HasTask() {}
	virtual ~HasTask() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		if(a_agent->hasGoal())
		{
			return BR_SUCCESS;
		}
		return BR_FAILURE;
	}
};

class UpdatePath : public Behaviour
{
public:

	UpdatePath() {}
	virtual~ UpdatePath() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		if(a_agent->hasDestination() == false)
		{
			//doesn't yet have a path to get to its destination, generate one!
			a_agent->setPath();
		}
		//UpdatePath - check if properties of the path nodes have been changed. if so, create a new path.
		if(a_agent->checkPathForChange())
		{
			a_agent->updateCurrentNode();
			a_agent->setPath();
		}
		return BR_SUCCESS;
	}
};

class HasPath : public Behaviour
{
public:

	HasPath() {}
	virtual ~HasPath() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		//the agent can no longer accomplish the task (closed door etc), so it stops.
		if(a_agent->hasDestination())
		{
			return BR_SUCCESS;
		}
		return BR_FAILURE;
	}
};

//checks the blackboard for requests to subscribe to
class CheckBlackboard : public Behaviour
{
public:

	CheckBlackboard() {}
	virtual ~CheckBlackboard() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		a_agent->checkBlackboardRequests();
		return BR_SUCCESS;
	}
};

//checks if the agent is on an electrified tile
class OnElecTile : public Behaviour
{
public:

	OnElecTile() {}
	virtual ~OnElecTile() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		if(a_agent->isDying())
		{
			return BR_SUCCESS;
		}
		if(a_agent->onElecTile())
		{
			return BR_SUCCESS;
		}
		return BR_FAILURE;
	}
};

//plays a dying animation, returns true once the dying animation has finished
class Dying : public Behaviour
{
public:

	Dying() {}
	virtual ~Dying() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		//dying animation will return false if not completed
		if(a_agent->dyingAnimation() == false)
		{
			return BR_SUCCESS;
		}
		return BR_FAILURE;
	}
};

//resets position, goals etc
class Respawn : public Behaviour
{
public:

	Respawn() {}
	virtual ~Respawn() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		a_agent->respawn();
		return BR_SUCCESS;
	}
};

#endif