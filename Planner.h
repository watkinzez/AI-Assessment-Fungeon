#ifndef PLANNER_H_
#define PLANNER_H_

#include "HTN.h"
#include "Agent.h"
#include "Blackboard.h"

class Planner
{
public:

	Planner();
	virtual ~Planner();

	void update();

	static Mesh* m_mesh;

protected:

	Task* m_task;
};

class StubTask : public Task
{
public:

	StubTask() { m_completed = false; }
	virtual ~StubTask() {}

	bool performTask()
	{
		m_completed = true;
		return m_completed;
	}
};

//a one-off task to unlock a door that requires more than one key.
class UnlockDoor : public Task
{
public:

	UnlockDoor(Door* a_door) : m_door(a_door) { m_completed = false; }
	virtual ~UnlockDoor() {}

	bool isCompleted()
	{
		m_completed = m_door->isOpen();
		return m_completed;
	}

	bool performTask()
	{
		m_door->openDoor();

		m_completed = true;
		return m_completed;
	}

protected:

	Door* m_door;
};

class GoalTask : public Task
{
public:

	GoalTask(Goal* a_goal, int a_toOpenDoor = 0) : m_goal(a_goal), m_request(nullptr), m_toOpenDoor(a_toOpenDoor) { m_completed = false; }
	virtual ~GoalTask() {}

	void hireSubscriber()
	{
		int numSubscribers = m_request->getNumSubscribers();
		if(numSubscribers <= 0)
		{
			return;
		}

		//hire the subscriber closest to the goal!
		float distance2 = FLT_MAX;
		Agent* hiredAgent = nullptr;

		Goal* goal;
		if(dynamic_cast<GetKey*>(m_request))
		{
			goal = dynamic_cast<GetKey*>(m_request)->getKey();
		}
		if(dynamic_cast<OpenDoor*>(m_request))
		{
			goal = dynamic_cast<OpenDoor*>(m_request)->getDoor();
		}
		if(dynamic_cast<GoToDoor*>(m_request))
		{
			goal = dynamic_cast<GoToDoor*>(m_request)->getDoor();
		}

		for(int i = 0; i < numSubscribers; ++i)
		{
			Agent* currentAgent = m_request->getSubscriber(i);
			if(currentAgent->hasGoal())
			{
				//the agent has already been hired for another request, ignore
				continue;
			}
			glm::vec3 diff = goal->getAttachedNode()->position - currentAgent->getPosition();
			float currentDistance2 = glm::length2(diff);
			if(currentDistance2 < distance2)
			{
				hiredAgent = currentAgent;
				distance2 = currentDistance2;
			}
		}

		if(hiredAgent != nullptr)
		{
			hiredAgent->setGoal(m_request);
			hiredAgent->setGoalNode(goal->getAttachedNode());
			m_request->hireAgent(hiredAgent);
		}
	}

	//checks if the hired agent can perform the task. else, fire them
	void checkCapability()
	{
		Agent* hiredAgent = m_request->getEmployee();

		//an agent may be fired if they can no longer reach their goal (no path), or if they are dying
		if(hiredAgent->hasDestination() == false || hiredAgent->isDying())
		{
			hiredAgent->setGoal(nullptr);
			hiredAgent->setGoalNode(nullptr);
			m_request->fireAgent();
		}
	}

	void removeRequest()
	{
		Agent* hiredAgent = m_request->getEmployee();
		if(hiredAgent != nullptr)
		{
			// "fire" agent
			hiredAgent->setGoal(nullptr);
			hiredAgent->setGoalNode(nullptr);
			m_request->fireAgent();
		}
		Blackboard* blackboard = Blackboard::getInstance();
		blackboard->removeRequest(m_request);
		m_request = nullptr;
	}

	bool isCompleted()
	{
		//overriding default implementation
		//check to see if the goal is obtained
		//if the goal is a door, it should be open
		//if the goal is a key, it should have an owner
		//if these are true, m_completed remains the same
		//else, it is set to false
		if(dynamic_cast<Door*>(m_goal))
		{
			m_completed = dynamic_cast<Door*>(m_goal)->isOpen();
		}
		if(dynamic_cast<Key*>(m_goal))
		{
			m_completed = dynamic_cast<Key*>(m_goal)->isOwned();
		}
		if(m_completed && m_request != nullptr)
		{
			//it's been completed through other means (player unlocking door), remove request
			removeRequest();
		}
		return m_completed;
	}

	bool performTask()
	{
		if(m_request == nullptr)
		{
			//if the request hasn't been created yet, create it
			Blackboard* blackboard = Blackboard::getInstance();
			if(dynamic_cast<Key*>(m_goal))
			{
				m_request = blackboard->createRequest(dynamic_cast<Key*>(m_goal));
			}
			if(dynamic_cast<Door*>(m_goal))
			{
				m_request = blackboard->createRequest(dynamic_cast<Door*>(m_goal), m_toOpenDoor);
			}
		}

		if(m_request->isCompleted() == true)
		{
			//if the request is complete, remove it
			removeRequest();
			m_completed = true;
			return m_completed;
		}

		if(m_request->isHired() == false)
		{
			hireSubscriber();
		}
		else
		{
			checkCapability();
		}

		return m_completed;
	}

protected:

	Goal* m_goal;
	Request* m_request;
	
	int m_toOpenDoor;
};

#endif