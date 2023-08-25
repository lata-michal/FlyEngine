#pragma once

#include "pch.h"

#include "Log.h"

namespace feng {

	class Timer
	{
	private:
		float m_ObjectCreateTime;
		float m_DeltaTime;
		float m_LastFrameTime;
		float m_FramesTime;

		uint32_t m_FPS;
		uint32_t m_FramesCount;
	public:
		Timer() : m_ObjectCreateTime(static_cast<float>(glfwGetTime())), m_DeltaTime(0.0f), m_LastFrameTime(0.0f), m_FramesTime(0.0f), m_FPS(0), m_FramesCount(0) { }
		~Timer() { if (glfwGetCurrentContext()) { Log::Print("OPERATION TOOK " + std::to_string((static_cast<float>(glfwGetTime()) - m_ObjectCreateTime) * 1000.0f) + " MS\n"); }; }

		void UpdateDelta() { m_DeltaTime = static_cast<float>(glfwGetTime()) - m_LastFrameTime; m_LastFrameTime = static_cast<float>(glfwGetTime()); }
		void UpdateFPS() { (m_FramesCount > 99 ? (m_FPS = static_cast<uint32_t>(std::roundl(static_cast<float>(m_FramesCount) / m_FramesTime)), m_FramesTime = 0.0f, m_FramesCount = 0) : (m_FramesTime += m_DeltaTime, m_FramesCount++)); }

		inline uint32_t GetFPS() const { return m_FPS; }
		inline const std::string GetStringFPS() const { return std::to_string(m_FPS); }
		inline float GetDeltaTime() const { return m_DeltaTime; }
		inline float GetTimeSince() const { return static_cast<float>(glfwGetTime()) - m_ObjectCreateTime; }
	};

}