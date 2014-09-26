#ifndef HTN_H_
#define HTN_H_

#include <vector>

class Task
{
public:

	Task() : m_completed(false) {}
	virtual ~Task() {}

	virtual bool isCompleted() { return m_completed; }

	virtual bool isPreConditionMet() { return true; }

	virtual bool performTask() = 0;

protected:

	bool m_completed;
};

//a compound task must have its tasks completed in order
class CompoundTask : public Task
{
public:

	CompoundTask() {}
	virtual ~CompoundTask() { for(auto task : m_tasks) delete task; }

	void addTask(Task* a_task) { m_tasks.push_back(a_task); }

	virtual bool performTask()
	{
		//if(m_completed != true &&
		//	isPreConditionMet() == true)
		//{
			//perform subtasks
			for(auto task : m_tasks)
			{
				if(task->isCompleted() == false &&
					task->performTask() == false)
				{
					return false;
				}
			}
			return true;
			//m_completed = true;
		//}
		//return m_completed;
	}

protected:

	std::vector<Task*> m_tasks;
};

//a simultask performs each of its tasks regardless if the previous hasn't yet been completed
class SimulTask : public Task
{
public:

	SimulTask() {}
	virtual ~SimulTask() { for(auto task : m_tasks) delete task; }

	void addTask(Task* a_task) { m_tasks.push_back(a_task); }

	virtual bool performTask()
	{
		//if(m_completed != true &&
		//	isPreConditionMet() == true)
		//{
			bool completed = true;
			//perform subtasks
			for(auto task : m_tasks)
			{
				if(task->isCompleted() == false)
				{
					//if even one of the tasks is not complete, m_completed will become false
					if(completed)
					{
						completed = task->performTask();
					}
					else
					{
						task->performTask();
					}
				}
			}
			return completed;
		//}
		//return m_completed;
	}

protected:

	std::vector<Task*> m_tasks;
};

#endif