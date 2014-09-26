#ifndef PLAYERCONTROL_H_
#define PLAYERCONTROL_H_

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Application.h"
#include <Gizmos.h>

#include "MemoryLeaks.h"

class PlayerControl
{
public:

	PlayerControl(GLFWwindow* a_window, glm::mat4& a_cameraMatrix, glm::mat4& a_projectionMatrix);
	virtual ~PlayerControl();

	void update();
	bool event();
	glm::vec3& getTarget() { return m_target; }

protected:

	void pickRayDirection();
	void rayPlaneIntersection();

	GLFWwindow* m_window;
	bool m_mouseDown;
	bool m_event;
	glm::vec3 m_rayDir;
	glm::vec4 m_plane;
	glm::vec3 m_target;

	glm::mat4& m_cameraMatrix;
	glm::mat4& m_projectionMatrix;
};

#endif