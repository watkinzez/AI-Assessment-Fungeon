#ifndef BEHAVIOUR_H_
#define BEHAVIOUR_H_

enum BEHAVIOUR_RESULT
{
	BR_SUCCESS = 0,
	BR_FAILURE
};

class Agent;

class Behaviour
{
public:

	Behaviour() {}
	virtual ~Behaviour() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent) = 0;
};

class Composite : public Behaviour
{
public:

	Composite() {}
	virtual ~Composite() { for (auto child : m_children) delete child; }

	void addChild(Behaviour* a_child) { m_children.push_back(a_child); }

protected:

	std::vector<Behaviour*> m_children;
};

class Sequence : public Composite
{
public:

	Sequence() {}
	virtual ~Sequence() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		for(auto child : m_children)
		{
			if(child->execute(a_agent) == BR_FAILURE)
			{
				return BR_FAILURE;
			}
		}
		return BR_SUCCESS;
	}
};

class Selector : public Composite
{
public:

	Selector() {}
	virtual ~Selector() {}

	virtual BEHAVIOUR_RESULT execute(Agent* a_agent)
	{
		for(auto child : m_children)
		{
			if(child->execute(a_agent) == BR_SUCCESS)
			{
				return BR_SUCCESS;
			}
		}
		return BR_FAILURE;
	}
};

#endif