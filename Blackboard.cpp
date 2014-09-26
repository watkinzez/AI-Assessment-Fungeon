#include "Blackboard.h"

Blackboard* Blackboard::instance = nullptr;

Blackboard* Blackboard::getInstance()
{
	if(instance == nullptr)
	{
		instance = new Blackboard();
	}
	return instance;
}

void Blackboard::deleteInstance()
{
	if(instance == nullptr)
	{
		return;
	}
	delete instance;
	instance = nullptr;
}

Blackboard::Blackboard()
{
}

Blackboard::~Blackboard()
{
	for(auto request : m_requests)
	{
		delete request;
	}
}

Request* Blackboard::createRequest(Key* a_key)
{
	Request* newRequest = new GetKey(a_key);
	m_requests.push_back(newRequest);

	return newRequest;
}

Request* Blackboard::createRequest(Door* a_door, int a_toOpenDoor = 0)
{
	Request* newRequest;
	switch(a_toOpenDoor)
	{
	default:
	case 0:
		newRequest = new OpenDoor(a_door);
		break;
	case 1:
		newRequest = new GoToDoor(a_door, true);
		break;
	case 2:
		newRequest = new GoToDoor(a_door, false);
		break;
	}
	m_requests.push_back(newRequest);

	return newRequest;
}

void Blackboard::removeRequest(Request* a_request)
{
	for(unsigned int i = 0; i < m_requests.size(); ++i)
	{
		if(a_request == m_requests[i])
		{
			m_requests.erase(m_requests.begin() + i);
			delete a_request;
			break;
		}
	}
}