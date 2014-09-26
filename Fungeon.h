#ifndef __Fungeon_H_
#define __Fungeon_H_

#include "Application.h"
#include <glm/glm.hpp>

#include "Agent.h"
#include "Planner.h"

// Derived application class that wraps up all globals neatly
class Fungeon : public Application
{
public:

	Fungeon();
	virtual ~Fungeon();

protected:

	virtual bool onCreate(int a_argc, char* a_argv[]);
	virtual void onUpdate(float a_deltaTime);
	virtual void onDraw();
	virtual void onDestroy();

	std::vector<Agent*> m_agents;
	Planner* m_planner;
	PlayerControl* m_control;

	glm::mat4	m_cameraMatrix;
	glm::mat4	m_projectionMatrix;
};

#endif // __Fungeon_H_