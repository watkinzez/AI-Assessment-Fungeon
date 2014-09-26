#include "Planner.h"

Mesh* Planner::m_mesh;

Planner::Planner()
{
	CompoundTask* compoundTask = new CompoundTask();
	SimulTask* simulTask = new SimulTask();

	Key* key = m_mesh->getKey(1);
	GoalTask* k1 = new GoalTask(key);

	key = m_mesh->getKey(2);
	GoalTask* k2 = new GoalTask(key);

	key = m_mesh->getKey(3);
	GoalTask* k3 = new GoalTask(key);

	key = m_mesh->getKey(4);
	GoalTask* k4 = new GoalTask(key);

	key = m_mesh->getKey(5);
	GoalTask* k5 = new GoalTask(key);

	Door* door = m_mesh->getDoor(4);
	GoalTask* d1 = new GoalTask(door);

	door = m_mesh->getDoor(5);
	GoalTask* d2 = new GoalTask(door);

	door = m_mesh->getDoor(6);
	GoalTask* d12 = new GoalTask(door);
	/*GoalTask* d12First = new GoalTask(door, 1);
	GoalTask* d12Second = new GoalTask(door, 2);
	UnlockDoor* unlockD12 = new UnlockDoor(door);*/

	door = m_mesh->getDoor(11);
	GoalTask* d3 = new GoalTask(door);

	door = m_mesh->getDoor(9);
	GoalTask* d4 = new GoalTask(door);

	door = m_mesh->getDoor(10);
	GoalTask* d34 = new GoalTask(door);
	/*GoalTask* d34First = new GoalTask(door, 1);
	GoalTask* d34Second = new GoalTask(door, 2);
	UnlockDoor* unlockD34 = new UnlockDoor(door);*/

	door = m_mesh->getDoor(1);
	GoalTask* d5 = new GoalTask(door);

	door = m_mesh->getDoor(2);
	GoalTask* openDoor1 = new GoalTask(door);

	door = m_mesh->getDoor(3);
	GoalTask* openDoor2 = new GoalTask(door);

	door = m_mesh->getDoor(7);
	GoalTask* openDoor3 = new GoalTask(door);

	door = m_mesh->getDoor(8);
	GoalTask* openDoor4 = new GoalTask(door);

	//--------------------------//

	compoundTask->addTask(k1);
	compoundTask->addTask(d1);
	simulTask->addTask(compoundTask);

	compoundTask = new CompoundTask();
	compoundTask->addTask(k2);
	compoundTask->addTask(d2);
	simulTask->addTask(compoundTask);

	SimulTask* simulTask2 = new SimulTask();
	simulTask2->addTask(openDoor1);
	simulTask2->addTask(openDoor2);

	CompoundTask* firstBranch = new CompoundTask();
	firstBranch->addTask(simulTask2);
	firstBranch->addTask(simulTask);
	firstBranch->addTask(d12);

	////
	/*simulTask = new SimulTask();
	simulTask->addTask(d12First);
	simulTask->addTask(d12Second);
	compoundTask = new CompoundTask();
	compoundTask->addTask(simulTask);
	compoundTask->addTask(unlockD12);*/
	////

	//firstBranch->addTask(compoundTask);

	//---------------------//

	simulTask = new SimulTask();

	compoundTask = new CompoundTask();
	compoundTask->addTask(k3);
	compoundTask->addTask(d3);
	simulTask->addTask(compoundTask);

	compoundTask = new CompoundTask();
	compoundTask->addTask(k4);
	compoundTask->addTask(d4);
	simulTask->addTask(compoundTask);

	simulTask2 = new SimulTask();
	simulTask2->addTask(openDoor3);
	simulTask2->addTask(openDoor4);

	CompoundTask* secondBranch = new CompoundTask();
	secondBranch->addTask(simulTask2);
	secondBranch->addTask(simulTask);
	secondBranch->addTask(d34);

	////
	/*simulTask = new SimulTask();
	simulTask->addTask(d34First);
	simulTask->addTask(d34Second);
	compoundTask = new CompoundTask();
	compoundTask->addTask(simulTask);
	compoundTask->addTask(unlockD34);*/
	////

	//secondBranch->addTask(compoundTask);

	compoundTask = new CompoundTask();
	compoundTask->addTask(firstBranch);
	compoundTask->addTask(secondBranch);

	CompoundTask* fullTree = new CompoundTask();
	fullTree->addTask(compoundTask);
	fullTree->addTask(k5);
	fullTree->addTask(d5);

	m_task = fullTree;
}

Planner::~Planner()
{
	delete m_task;
}

void Planner::update()
{
	if(m_task != nullptr)
	{
		m_task->performTask();
	}
}