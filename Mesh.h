#ifndef MESH_H_
#define MESH_H_

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include "PlayerControl.h"
#include <Utilities.h>
#include <math.h>

class Door;
class Key;
class ElectrifiedTile;

enum KeyStatus
{
	DEFAULT,
	OWNED,
	FLASHING
};

class Mesh
{
public:

	struct NavMeshNode;

	Mesh();
	virtual ~Mesh();

	void update();

	NavMeshNode* calculateAStar(unsigned int a_agentID, NavMeshNode* a_start, NavMeshNode* a_end, bool a_ignoreDoors);
	unsigned int requestID();
	Key* getKey(int a_keyNo);

	Door* getDoor(int a_doorNo) { return m_doors[a_doorNo - 1]; }
	NavMeshNode* getSpawnPoint() const { return m_graph[8]; }

	void setControl(PlayerControl* a_control) { m_control = a_control; }

	struct Cost
	{
		unsigned int f;
		unsigned int g;
		unsigned int h;
	};

	struct NavMeshNode
	{
		glm::vec3 position;
		glm::vec3 vertices[4];

		Cost cost;

		int weight;
		bool alteredWeight;

		NavMeshNode* links[4];

		std::vector<NavMeshNode*> next;

		void changeWeight(int a_weight)
		{
			weight = a_weight;
			alteredWeight = true;
		}

		bool isElectrified()
		{
			return (weight == 3);
		}
	};

protected:

	static bool sortFunction(NavMeshNode* i, NavMeshNode* j) { return (i->cost.f < j->cost.f); }

	void setUpNewNode(NavMeshNode* a_node, int x, int y, int a_weight);

	bool isInList(std::vector<NavMeshNode*>& a_list, NavMeshNode* a_node);
	void calculateCost(NavMeshNode* a_current, NavMeshNode* a_adjacent, NavMeshNode* a_end);
	unsigned int getHeuristic(NavMeshNode* a_adjacent, NavMeshNode* a_end);
	void reset(unsigned int a_agentID);

	void controlEvents();

	std::vector<NavMeshNode*> m_graph;
	std::vector<Door*> m_doors;
	std::vector<Key*> m_keys;
	ElectrifiedTile* m_elecTiles;
	int m_deployedElecTile;

	PlayerControl* m_control;
};

class ElectrifiedTile
{
public:

	ElectrifiedTile() : m_attachedNode(nullptr) {}
	virtual ~ElectrifiedTile() {}

	void setAttachedNode(Mesh::NavMeshNode* a_node)
	{
		if(m_attachedNode != nullptr)
		{
			//restore old tile to normal weight first
			m_attachedNode->changeWeight(1);
		}
		m_attachedNode = a_node;
		m_attachedNode->changeWeight(3);
	}

	Mesh::NavMeshNode* getAttachedNode() { return m_attachedNode; }

protected:

	Mesh::NavMeshNode* m_attachedNode;
};

class Goal
{
public:

	Goal() {}
	virtual ~Goal() {}

	void setColour()
	{
		switch(m_keyNo)
		{
		case 1:
			m_colour = glm::vec4(1, 0, 0, 1);
			break;
		case 2:
			m_colour = glm::vec4(0, 1, 0, 1);
			break;
		case 3:
			m_colour = glm::vec4(0, 0, 1, 1);
			break;
		case 4:
			m_colour = glm::vec4(0, 1, 1, 1);
			break;
		case 5:
			m_colour = glm::vec4(1, 0, 1, 1);
			break;
		default:
			m_colour = glm::vec4(0.4, 0.4, 0.4, 1);
			break;
		}
	}

	Mesh::NavMeshNode* getAttachedNode() { return m_attachedNode; }
	int getKeyNo() { return m_keyNo; }

	glm::vec4& getColour() { return m_colour; }

protected:

	Mesh::NavMeshNode* m_attachedNode;
	int m_keyNo;
	glm::vec4 m_colour;
};

class Door : public Goal
{
public:

	Door(Mesh::NavMeshNode* a_node, int a_unlockedByKey, bool a_isOpen = false, bool a_toBeOpened = true) : m_isOpen(a_isOpen),
																											m_toBeOpened(a_toBeOpened)
	{
		m_attachedNode = a_node;
		m_keyNo = a_unlockedByKey;
		setColour();
	}
	virtual ~Door() {}

	void flipDoor() { (m_isOpen)? closeDoor() : openDoor(); }

	//a door has a node it is attached to. when a door opens and closes, it changes the node's weight.
	void openDoor() 
	{
		m_isOpen = true;
		m_attachedNode->changeWeight(1);
	}

	void closeDoor()
	{
		m_isOpen = false;
		m_attachedNode->changeWeight(-1); // negative value indicates impassable
	}

	bool isOpen() { return m_isOpen; }

protected:

	bool m_isOpen;

	//determines whether the task is to open the door or simply go to it
	bool m_toBeOpened;
};

class Key : public Goal
{
public:

	Key(Mesh::NavMeshNode* a_node, int a_keyNo) : m_status(KeyStatus::DEFAULT)
	{
		m_attachedNode = a_node;
		m_keyNo = a_keyNo;
		setColour();
	}
	virtual ~Key() {}

	KeyStatus& getStatus() { return m_status; }

	void owned() { m_status = KeyStatus::OWNED; }
	void disowned() { m_status = KeyStatus::FLASHING; }

	bool isOwned() { return m_status == KeyStatus::OWNED; }

protected:

	//determines whether a key is currently being carried by an agent or not
	KeyStatus m_status;
};

#endif MESH_H_