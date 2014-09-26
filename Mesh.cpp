#include "Mesh.h"
#include "Gizmos.h"

Mesh::Mesh() : m_control(nullptr)
{
	m_elecTiles = new ElectrifiedTile[5];
	m_deployedElecTile = 0;

	// LEVEL DATA LEGEND
	// o = Nothing
	// F = floor
	// DOORS (upper case closed, lower case open)
	// Q, W, E, R, T => 1, 2, 3, 4, 5
	// A => 1&2
	// S => 3&4
	// KEYS
	// Y, U, I, O, P => 1, 2, 3, 4, 5

	char levelData[24][11] = {{'o','o','o','o','o','o','o','o','o','T','o'},
							  {'o','o','o','o','F','F','F','o','F','F','F'},
							  {'o','o','o','o','F','F','F','o','F','F','F'},
							  {'o','o','o','o','F','F','F','o','F','F','F'},
							  {'o','o','o','o','o','F','o','o','o','F','o'},
							  {'o','o','o','F','F','F','F','F','F','F','o'},
							  {'o','o','o','e','o','o','o','r','o','o','o'},
							  {'o','o','F','F','F','o','F','F','F','o','o'},
							  {'o','o','F','F','F','o','F','F','F','o','o'},
							  {'o','o','F','Y','F','o','F','U','F','o','o'},
							  {'o','o','F','F','F','o','F','F','F','o','o'},
							  {'o','o','F','F','F','o','F','F','F','o','o'},
							  {'o','o','o','Q','o','o','o','W','o','o','o'},
							  {'o','o','o','F','F','F','F','F','o','o','o'},
							  {'o','o','o','o','o','A','o','o','o','o','o'},
							  {'o','F','F','F','F','F','F','F','F','F','o'},
							  {'o','w','o','o','o','o','o','o','o','q','o'},
							  {'F','F','F','o','F','F','F','o','F','F','F'},
							  {'F','F','F','o','F','F','F','o','F','F','F'},
							  {'F','I','F','o','F','P','F','o','F','O','F'},
							  {'F','F','F','o','F','F','F','o','F','F','F'},
							  {'F','F','F','o','F','F','F','o','F','F','F'},
							  {'o','R','o','o','o','S','o','o','o','E','o'},
							  {'o','F','F','F','F','F','F','F','F','F','o'}, };

	//create quad nodes from the level data
	for(int y = 0; y < 24; ++y)
	{
		for(int x = 0; x < 11; ++x)
		{
			NavMeshNode* newNode;
			Door* newDoor;
			Key* newKey;
			switch(levelData[y][x])
			{
			case 'o':
				break;
			case 'F':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);
				break;
			case 'Q':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, -1);

				//create door, attach node to door
				newDoor = new Door(newNode, 1);
				m_doors.push_back(newDoor);
				break;
			case 'W':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, -1);

				//create door, attach node to door
				newDoor = new Door(newNode, 2);
				m_doors.push_back(newDoor);
				break;
			case 'E':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, -1);

				//create door, attach node to door
				newDoor = new Door(newNode, 3);
				m_doors.push_back(newDoor);
				break;
			case 'R':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, -1);

				//create door, attach node to door
				newDoor = new Door(newNode, 4);
				m_doors.push_back(newDoor);
				break;
			case 'T':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, -1);

				//create door, attach node to door
				newDoor = new Door(newNode, 5);
				m_doors.push_back(newDoor);
				break;
			case 'q':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);

				//create door, attach node to door
				newDoor = new Door(newNode, 1, true);
				m_doors.push_back(newDoor);
				break;
			case 'w':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);

				//create door, attach node to door
				newDoor = new Door(newNode, 2, true);
				m_doors.push_back(newDoor);
				break;
			case 'e':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);

				//create door, attach node to door
				newDoor = new Door(newNode, 3, true);
				m_doors.push_back(newDoor);
				break;
			case 'r':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);

				//create door, attach node to door
				newDoor = new Door(newNode, 4, true);
				m_doors.push_back(newDoor);
				break;
			case 't':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);

				//create door, attach node to door
				newDoor = new Door(newNode, 5, true);
				m_doors.push_back(newDoor);
				break;
			case 'A':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, -1);

				//requires keys 1 AND 2
				newDoor = new Door(newNode, 6);
				m_doors.push_back(newDoor);
				break;
			case 'S':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, -1);

				//requires keys 3 AND 4
				newDoor = new Door(newNode, 7);
				m_doors.push_back(newDoor);
				break;
			case 'Y':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);

				newKey = new Key(newNode, 1);
				m_keys.push_back(newKey);
				break;
			case 'U':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);

				newKey = new Key(newNode, 2);
				m_keys.push_back(newKey);
				break;
			case 'I':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);

				newKey = new Key(newNode, 3);
				m_keys.push_back(newKey);
				break;
			case 'O':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);

				newKey = new Key(newNode, 4);
				m_keys.push_back(newKey);
				break;
			case 'P':
				newNode = new NavMeshNode();
				setUpNewNode(newNode, x, y, 1);

				newKey = new Key(newNode, 5);
				m_keys.push_back(newKey);
				break;
			default:
				break;
			}
		}
	}

	//connect the nodes by setting their links

	for(auto start : m_graph)
	{
		for(auto end : m_graph)
		{
			//don't compare a node against itself
			if(start == end)
			{
				continue;
			}

			if( (start->vertices[0] == end->vertices[0] && start->vertices[1] == end->vertices[1]) ||
				(start->vertices[0] == end->vertices[1] && start->vertices[1] == end->vertices[2]) ||
				(start->vertices[0] == end->vertices[2] && start->vertices[1] == end->vertices[3]) ||
				(start->vertices[0] == end->vertices[3] && start->vertices[1] == end->vertices[0]) ||
				(start->vertices[0] == end->vertices[1] && start->vertices[1] == end->vertices[0]) ||
				(start->vertices[0] == end->vertices[2] && start->vertices[1] == end->vertices[1]) ||
				(start->vertices[0] == end->vertices[3] && start->vertices[1] == end->vertices[2]) ||
				(start->vertices[0] == end->vertices[0] && start->vertices[1] == end->vertices[3]) )
			{
				start->links[0] = end;
			}

			if( (start->vertices[1] == end->vertices[0] && start->vertices[2] == end->vertices[1]) ||
				(start->vertices[1] == end->vertices[1] && start->vertices[2] == end->vertices[2]) ||
				(start->vertices[1] == end->vertices[2] && start->vertices[2] == end->vertices[3]) ||
				(start->vertices[1] == end->vertices[3] && start->vertices[2] == end->vertices[0]) ||
				(start->vertices[1] == end->vertices[1] && start->vertices[2] == end->vertices[0]) ||
				(start->vertices[1] == end->vertices[2] && start->vertices[2] == end->vertices[1]) ||
				(start->vertices[1] == end->vertices[3] && start->vertices[2] == end->vertices[2]) ||
				(start->vertices[1] == end->vertices[0] && start->vertices[2] == end->vertices[3]) )
			{
				start->links[1] = end;
			}

			if( (start->vertices[2] == end->vertices[0] && start->vertices[3] == end->vertices[1]) ||
				(start->vertices[2] == end->vertices[1] && start->vertices[3] == end->vertices[2]) ||
				(start->vertices[2] == end->vertices[2] && start->vertices[3] == end->vertices[3]) ||
				(start->vertices[2] == end->vertices[3] && start->vertices[3] == end->vertices[0]) ||
				(start->vertices[2] == end->vertices[1] && start->vertices[3] == end->vertices[0]) ||
				(start->vertices[2] == end->vertices[2] && start->vertices[3] == end->vertices[1]) ||
				(start->vertices[2] == end->vertices[3] && start->vertices[3] == end->vertices[2]) ||
				(start->vertices[2] == end->vertices[0] && start->vertices[3] == end->vertices[3]) )
			{
				start->links[2] = end;
			}

			if( (start->vertices[3] == end->vertices[0] && start->vertices[0] == end->vertices[1]) ||
				(start->vertices[3] == end->vertices[1] && start->vertices[0] == end->vertices[2]) ||
				(start->vertices[3] == end->vertices[2] && start->vertices[0] == end->vertices[3]) ||
				(start->vertices[3] == end->vertices[3] && start->vertices[0] == end->vertices[0]) ||
				(start->vertices[3] == end->vertices[1] && start->vertices[0] == end->vertices[0]) ||
				(start->vertices[3] == end->vertices[2] && start->vertices[0] == end->vertices[1]) ||
				(start->vertices[3] == end->vertices[3] && start->vertices[0] == end->vertices[2]) ||
				(start->vertices[3] == end->vertices[0] && start->vertices[0] == end->vertices[3]) )
			{
				start->links[3] = end;
			}
		}
	}
}

Mesh::~Mesh()
{
	if(m_control != nullptr)
	{
		delete m_control;
	}

	for(auto node : m_graph)
	{
		delete node;
	}

	for(auto door : m_doors)
	{
		delete door;
	}

	for(auto key : m_keys)
	{
		delete key;
	}

	delete [] m_elecTiles;
}

void Mesh::setUpNewNode(NavMeshNode* a_node, int a_x, int a_y, int a_weight)
{
	a_node->links[0] = nullptr;
	a_node->links[1] = nullptr;
	a_node->links[2] = nullptr;
	a_node->links[3] = nullptr;

	a_node->position = glm::vec3(1 * a_x - 6, 0, 1 * a_y - 12);

	a_node->vertices[0] = a_node->position + glm::vec3(-0.5,0,-0.5);
	a_node->vertices[1] = a_node->position + glm::vec3(0.5,0,-0.5);
	a_node->vertices[2] = a_node->position + glm::vec3(0.5,0,0.5);
	a_node->vertices[3] = a_node->position + glm::vec3(-0.5,0,0.5);

	a_node->cost.f = 0;
	a_node->cost.g = 0;
	a_node->cost.h = 0;

	a_node->weight = a_weight;
	a_node->alteredWeight = false;

	m_graph.push_back(a_node);
}

void Mesh::update()
{
	m_control->update();

	controlEvents();

	//draw floor
	for(unsigned int node = 0; node < m_graph.size(); ++node)
	{
		Gizmos::addAABBFilled(m_graph[node]->position, glm::vec3(0.5,0.05,0.5), glm::vec4(0.7, 0.3, 0.4, 1));
	}

	//draw doors
	for(auto door : m_doors)
	{
		int keyNo = door->getKeyNo();
		Key* key;
		if(keyNo <= 4)
		{
			key = getKey(keyNo);
		}
		else
		{
			key = getKey(keyNo - 2);
		}
		glm::vec3 extents;
		if(door->isOpen())
		{
			extents = glm::vec3(0.1,0.05,0.5);
		}
		else
		{
			extents = glm::vec3(0.5,0.05,0.1);
		}

		glm::vec4 colour;
		float blendValue;
		if(keyNo <= 4)
		{
			switch(key->getStatus())
			{
			case KeyStatus::DEFAULT:
			case KeyStatus::OWNED:
				Gizmos::addAABBFilled(door->getAttachedNode()->position + glm::vec3(0,0.1,0), extents, door->getColour());
				break;
			case KeyStatus::FLASHING:
				blendValue = fmod(Utility::getTotalTime(), 1.0f);
				colour = glm::mix(key->getColour(), glm::vec4(1,1,0,1), blendValue);
				Gizmos::addAABBFilled(door->getAttachedNode()->position + glm::vec3(0,0.1,0), extents, colour);
				break;
			default:
				break;
			}
		}
		else
		{
			Gizmos::addAABBFilled(door->getAttachedNode()->position + glm::vec3(0,0.1,0), extents, door->getColour());
		}
	}

	//draw un-owned keys
	for(auto key : m_keys)
	{
		glm::vec4 colour;
		float blendValue;
		switch(key->getStatus())
		{
		case KeyStatus::DEFAULT:
			Gizmos::addSphere(key->getAttachedNode()->position + glm::vec3(0,0.2,0), 4, 4, 0.3f, key->getColour());
			break;
		case KeyStatus::FLASHING:
			blendValue = fmod(Utility::getTotalTime(), 1.0f);
			colour = glm::mix(key->getColour(), glm::vec4(1,1,0,1), blendValue);
			Gizmos::addSphere(key->getAttachedNode()->position + glm::vec3(0,0.2,0), 4, 4, 0.3f, colour);
			break;
		case KeyStatus::OWNED:
		default:
			break;
		}
	}

	//electrified tiles
	for(int i = 0; i < 5; ++i)
	{
		Mesh::NavMeshNode* tile = (m_elecTiles + i)->getAttachedNode();
		if(tile != nullptr)
		{
			Gizmos::addAABBFilled(tile->position + glm::vec3(0,0.075,0), glm::vec3(0.5,0.05,0.5), glm::vec4(1,1,0,0.5));
		}
	}
}

void Mesh::controlEvents()
{
	//reset weight change bool
	for(auto tile : m_graph)
	{
		tile->alteredWeight = false;
	}

	glm::vec3 target = m_control->getTarget();
	bool eventOnTarget = m_control->event();

	//check for target boundaries
	if(target.x < -6.5f && target.z < -12.5f || target.x > 4.5f && target.z > 11.5f)
	{
		//do nothing
	}
	else
	{
		if(eventOnTarget)
		{
			//check for doors
			for(auto door : m_doors)
			{
				NavMeshNode* attachedNode = door->getAttachedNode();
				if(target.x > attachedNode->vertices[0].x && target.z > attachedNode->vertices[0].z && target.x < attachedNode->vertices[2].x && target.z < attachedNode->vertices[2].z)
				{
					//can only open/close doors with keys that are flashing
					int keyNo = door->getKeyNo();
					if(keyNo <= 4)
					{
						Key* key = getKey(keyNo);
						if(key->getStatus() == KeyStatus::FLASHING)
						{
							door->flipDoor();
						}
					}
					eventOnTarget = false;
					break;
				}
			}

			//check for floors
			if(eventOnTarget)
			{
				for(auto tile : m_graph)
				{
					if(target.x > tile->vertices[0].x && target.z > tile->vertices[0].z && target.x < tile->vertices[2].x && target.z < tile->vertices[2].z)
					{
						if(tile->isElectrified())
						{
							break;
						}
						//set tile to electrified
						(m_elecTiles + m_deployedElecTile)->setAttachedNode(tile);
						if(m_deployedElecTile == 4)
						{
							m_deployedElecTile = 0;
						}
						else
						{
							++m_deployedElecTile;
						}
						break;
					}
				}
			}
		}
		for(auto tile : m_graph)
		{
			if(target.x > tile->vertices[0].x && target.z > tile->vertices[0].z && target.x < tile->vertices[2].x && target.z < tile->vertices[2].z)
			{
				Gizmos::addAABBFilled(tile->position + glm::vec3(0,0.075,0), glm::vec3(0.5,0.05,0.5), glm::vec4(1,0,1,0.5));
				break;
			}
		}
	}
}

Mesh::NavMeshNode* Mesh::calculateAStar(unsigned int a_agentID, NavMeshNode* a_start, NavMeshNode* a_end, bool a_ignoreDoors)
{
	//an agent should already have an allocated space to store its path. if it doesn't, return here to avoid errors
	if(a_agentID >= m_graph[0]->next.size())
	{
		return nullptr;
	}

	reset(a_agentID);

	std::vector<NavMeshNode*> openList;
	std::vector<NavMeshNode*> closedList;

	openList.push_back(a_end);
	NavMeshNode* current;

	while(openList.size() > 0)
	{
		std::sort(openList.begin(), openList.end(), Mesh::sortFunction);
		//get node from open list with lowest cost
		current = openList[0];

		if(current == a_start)
		{
			//winnar
			return a_start;
		}

		//the node with the lowest cost has been found, remove from the open list, add to closed list
		openList.erase(openList.begin());
		closedList.push_back(current);

		//for each of current's neighbours
		for(int i = 0; i < 4; ++i)
		{
			//if nullptr, then there isn't a neighbour on that edge
			if(current->links[i] != nullptr)
			{
				//ignore weights with negative values. they indicate closed doors
				//maybe come back and implement a "check inventory system" if needed?
				if(current->links[i]->weight < 0)
				{
					if(a_ignoreDoors == false)
					{
						continue;
					}
				}
				//if the given neighbour is not in closed
				if(!isInList(closedList, current->links[i]))
				{
					//if the given neighbour is not in open
					if(!isInList(openList, current->links[i]))
					{
						//add to open
						//set the neighbour's previous pointer as current
						openList.push_back(current->links[i]);
					}
					//update cost and previous
					current->links[i]->next[a_agentID] = current;
					calculateCost(current, current->links[i], a_start);
				}
			}
		}
	}

	return nullptr;
}

bool Mesh::isInList(std::vector<NavMeshNode*>& a_list, NavMeshNode* a_node)
{
	for(auto node : a_list)
	{
		if(node == a_node)
		{
			return true;
		}
	}
	return false;
}

void Mesh::calculateCost(NavMeshNode* a_current, NavMeshNode* a_adjacent, NavMeshNode* a_end)
{
	a_adjacent->cost.g = a_current->cost.g + a_adjacent->weight;
	a_adjacent->cost.h = getHeuristic(a_adjacent, a_end);
	a_adjacent->cost.f = a_adjacent->cost.g + a_adjacent->cost.h;
}

unsigned int Mesh::getHeuristic(NavMeshNode* a_node, NavMeshNode* a_end)
{
	float distance = glm::distance(a_node->position, a_end->position);
	return (unsigned int)distance;
}

unsigned int Mesh::requestID()
{
	//for all the nodes in the graph, add an extra previous to each
	//return the id number
	for(auto node : m_graph)
	{
		node->next.push_back(nullptr);
	}
	return m_graph[0]->next.size() - 1;
}

void Mesh::reset(unsigned int a_agentID)
{
	for(auto node : m_graph)
	{
		node->cost.g = 0;
		node->next[a_agentID] = nullptr;
	}
}

Key* Mesh::getKey(int a_keyNo)
{
	for(auto key : m_keys)
	{
		if(key->getKeyNo() == a_keyNo)
		{
			return key;
		}
	}
	return m_keys[0];
}