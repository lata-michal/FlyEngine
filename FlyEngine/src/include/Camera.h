#pragma once

#include "pch.h"

#include "Actions.h"

namespace feng {

	class Camera : public Actions
	{
	private:
		glm::mat4 m_LookAt;
		glm::mat4 m_LookAtMirror;
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;

		float m_Pitch;
		float m_Yaw;

		float m_MovementSpeed;
		float m_Zoom;
	public:
		Camera(const glm::vec3& position, const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float pitch = 0.0f, float yaw = -90.0f, float movementSpeed = 3.0f, float zoom = 45.0f);
		Camera(float x, float y, float z, const glm::vec3& worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float pitch = 0.0f, float yaw = -90.0f, float movementSpeed = 3.0f, float zoom = 45.0f);

		void Move(KeyActions direction, float deltaTime) override;
		void LookAround(float xoffset, float yoffset, bool constrainPitch = true) override;
		void Zoom(float yoffset) override;

		void ForceMove(char direction, float deltaTime, float movementSpeed);

		const glm::mat4& GetViewMatrix() const;
		const glm::mat4& GetViewMatrixMirror() const;
		const glm::vec3& GetCameraPosition() const;
		const glm::vec3& GetCameraFront() const;
		const float GetZoom() const;
	private:
		void UpdateCamera();
	};

}