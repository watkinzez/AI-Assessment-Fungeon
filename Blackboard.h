#ifndef BLACKBOARD_H_
#define BLACKBOARD_H_

#include <vector>
#include <glm/glm.hpp>

#include "MemoryLeaks.h"

class Request;
class Agent;
class Goal;
class Key;
class Door;

class Blackboard
{
public:

	Blackboard();
	virtual ~Blackboard();

	static Blackboard* getInstance();
	static void deleteInstance();

	//the planner uses these to post requests to the blackboard
	Request* createRequest(Key* a_key);
	Request* createRequest(Door* a_door, int a_toOpenDoor);

	//the agents use these to check the requests
	int getNumberofRequests() { return m_requests.size(); }
	Request* getRequest(int a_requestNo) { return m_requests[a_requestNo]; }
	
	//the planner uses this to remove the request once completed
	void removeRequest(Request* a_request);

protected:

	static Blackboard *instance;

	std::vector<Request*> m_requests;
};

class Request
{
public:

	Request() {}
	virtual ~Request() {}

	int getNumSubscribers() { return m_subscribers.size(); }
	Agent* getSubscriber(int a_num) { return m_subscribers[a_num]; }
	Agent* getEmployee() { return m_employee; }
	bool isHired() { return m_employee != nullptr; }
	bool isCompleted() { return m_completed == true; }

	void complete() { m_completed = true; }
	void fireAgent() { m_employee = nullptr; }

	void remove(Agent* a_agent)
	{
		for(unsigned int i = 0; i < m_subscribers.size(); ++i)
		{
			if(a_agent == m_subscribers[i])
			{
				m_subscribers.erase(m_subscribers.begin() + i);
				break;
			}
		}
	}

	bool hasSubscribed(Agent* a_agent)
	{
		for(unsigned int i = 0; i < m_subscribers.size(); ++i)
		{
			if(a_agent == m_subscribers[i])
			{
				return true;
			}
		}
		return false;
	}

	void subscribe(Agent* a_agent)
	{
		m_subscribers.push_back(a_agent);
	}

	void hireAgent(Agent* a_agent)
	{
		m_subscribers.clear();
		m_employee = a_agent;
	}

protected:

	std::vector<Agent*> m_subscribers;
	Agent* m_employee;
	bool m_completed;
};

class GetKey : public Request
{
public:

	GetKey(Key* a_key) : m_key(a_key)
	{
		m_employee = nullptr;
		m_completed = false;
	}
	virtual ~GetKey() {}

	Key* getKey() { return m_key; }

protected:

	Key* m_key;
};

class GoToDoor : public Request
{
public:

	GoToDoor(Door* a_door, bool a_withKey) : m_door(a_door), m_withKey(a_withKey)
	{
		m_employee = nullptr;
		m_completed = false;
	}
	virtual ~GoToDoor() {}

	Door* getDoor() { return m_door; }
	bool getWithKey() { return m_withKey; }

protected:

	Door* m_door;
	bool m_withKey;
};

class OpenDoor : public Request
{
public:

	OpenDoor(Door* a_door) : m_door(a_door)
	{
		m_employee = nullptr;
		m_completed = false;
	}
	virtual ~OpenDoor() {}

	Door* getDoor() { return m_door; }

protected:

	Door* m_door;
};

#endif