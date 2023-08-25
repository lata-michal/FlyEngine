#include "Camera.h"

using namespace feng;

Camera::Camera(const glm::vec3& position, const glm::vec3& worldUp, float pitch, float yaw, float movementSpeed, float zoom)
	: m_Position(position), m_WorldUp(worldUp), m_Pitch(pitch), m_Yaw(yaw), m_MovementSpeed(movementSpeed), m_Zoom(zoom)
{
	UpdateCamera();
}

Camera::Camera(float x, float y, float z, const glm::vec3& worldUp, float pitch, float yaw, float movementSpeed, float zoom)
	: m_Position(glm::vec3(x, y, z)), m_WorldUp(worldUp), m_Pitch(pitch), m_Yaw(yaw), m_MovementSpeed(movementSpeed), m_Zoom(zoom)
{
	UpdateCamera();
}

void Camera::Move(KeyActions direction, float deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;

	if (static_cast<bool>(direction & KeyActions::FORWARD))
	{
		m_Position += (m_Front * velocity);
	}
	else if (static_cast<bool>(direction & KeyActions::BACKWARD))
	{
		m_Position -= (m_Front * velocity);
	}
	else if (static_cast<bool>(direction & KeyActions::LEFT))
	{
		m_Position -= (m_Right * velocity);
	}
	else if (static_cast<bool>(direction & KeyActions::RIGHT))
	{
		m_Position += (m_Right * velocity);
	}

	// m_Position.y = 0.0f; lock it at some value for fps like camera
	UpdateCamera();
}

void Camera::LookAround(float xoffset, float yoffset, bool constrainPitch)
{
	m_Pitch += yoffset;
	m_Yaw += xoffset;

	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	UpdateCamera();
}

void Camera::Zoom(float yoffset)
{
	m_Zoom -= yoffset;

	if (m_Zoom < 30.0f)
		m_Zoom = 30.0f;
	if (m_Zoom > 120.0f)
		m_Zoom = 120.0f;
}

void feng::Camera::ForceMove(char direction, float deltaTime, float movementSpeed)
{
	float velocity = movementSpeed * deltaTime;

	if (direction == 1)
	{
		m_Position += (m_Front * velocity);
	}
	else if (direction == 2)
	{
		m_Position -= (m_Front * velocity);
	}
	else if (direction == 3)
	{
		m_Position -= (m_Right * velocity);
	}
	else if (direction == 4)
	{
		m_Position += (m_Right * velocity);
	}

	// m_Position.y = 0.0f; lock it at some value for fps like camera
	UpdateCamera();
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return m_LookAt;
}

const glm::mat4& feng::Camera::GetViewMatrixMirror() const
{
	return m_LookAtMirror;
}

const glm::vec3& Camera::GetCameraPosition() const
{
	return m_Position;
}

const glm::vec3& Camera::GetCameraFront() const
{
	return m_Front;
}

const float Camera::GetZoom() const
{
	return m_Zoom;
}

void Camera::UpdateCamera()
{
	m_Front = glm::normalize(glm::vec3(glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch)), sin(glm::radians(m_Pitch)), sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch))));

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));

	m_LookAt = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	m_LookAtMirror = glm::lookAt(m_Position, m_Position - m_Front, m_Up);
}
