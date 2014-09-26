#include "PlayerControl.h"

#define DEFAULT_SCREENWIDTH 1280
#define DEFAULT_SCREENHEIGHT 720

PlayerControl::PlayerControl(GLFWwindow* a_window, glm::mat4& a_cameraMatrix, glm::mat4& a_projectionMatrix) : m_cameraMatrix(a_cameraMatrix),
																												m_projectionMatrix(a_projectionMatrix)
{
	m_plane = glm::vec4(0,1,0,0);
	m_mouseDown = false;
	m_event = false;

	m_window = a_window;
}

PlayerControl::~PlayerControl()
{
}

void PlayerControl::update()
{
	pickRayDirection();
	rayPlaneIntersection();

	if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		m_mouseDown = true;
	}
	else
	{
		if(m_mouseDown)
		{
			//event on current position
			m_event = true;
		}
		m_mouseDown = false;
	}
}

bool PlayerControl::event()
{
	if(m_event)
	{
		m_event = false;
		return true;
	}
	return false;
}

void PlayerControl::pickRayDirection()
{
	double x = 0;
	double y = 0;
	glfwGetCursorPos(m_window, &x, &y);

	glm::vec3 screenPos(x / DEFAULT_SCREENWIDTH * 2 - 1, (y / DEFAULT_SCREENHEIGHT * 2 - 1) * -1, -1);

	screenPos.x /= m_projectionMatrix[0][0];
	screenPos.y /= m_projectionMatrix[1][1];

	m_rayDir = glm::normalize(m_cameraMatrix * glm::vec4(screenPos, 0)).xyz();
}

void PlayerControl::rayPlaneIntersection()
{
	float d = (m_plane.w - glm::dot(m_cameraMatrix[3].xyz(), m_plane.xyz())) / glm::dot(m_rayDir, m_plane.xyz());

	m_target = m_cameraMatrix[3].xyz() + m_rayDir * d;
}