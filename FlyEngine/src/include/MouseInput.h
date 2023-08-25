#pragma once

#include "pch.h"

namespace feng {

	struct MouseMove
	{
		float offsetX;
		float offsetY;

		MouseMove(float offsetX, float offsetY)
		{
			this->offsetX = offsetX;
			this->offsetY = offsetY;
		}
	};

	class MouseInput
	{
	private:
		bool m_FirstMouse;
		bool m_MouseMoved;
		bool m_Scrolled;

		MouseMove m_Move;

		float m_Sensitivity;
		float m_lastX;
		float m_lastY;
		float m_Scroll;
	public:
		MouseInput()
			: m_FirstMouse(true), m_MouseMoved(false), m_Scrolled(false), m_Move(0.0f, 0.0f), m_Sensitivity(0.025f), m_lastX(0.0f), m_lastY(0.0f), m_Scroll(0.0f) { }

		void SetSensitivity(float sensitivity) { m_Sensitivity = sensitivity; }
		void SetMovedMouse(bool moved) { m_MouseMoved = moved; }
		void SetScrolled(bool scrolled) { m_Scrolled = scrolled; }

		bool QueryMovedMouse() const { return m_MouseMoved; }
		bool QueryScrolled() const { return m_Scrolled; }

		const MouseMove& GetMouseMove() const { return m_Move; }
		float GetScroll() const { return m_Scroll; }

		void MouseMoved(float xpos, float ypos)
		{
			if (m_FirstMouse)
			{
				m_lastX = xpos;
				m_lastY = ypos;
				m_FirstMouse = false;
			}

			m_Move.offsetX = xpos - m_lastX;
			m_Move.offsetY = m_lastY - ypos;

			m_lastX = xpos;
			m_lastY = ypos;

			m_Move.offsetX *= m_Sensitivity;
			m_Move.offsetY *= m_Sensitivity;
		}

		void Scrolled(float ypos)
		{
			m_Scroll = ypos;
		}
	};

}