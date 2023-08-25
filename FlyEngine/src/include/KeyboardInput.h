#pragma once

#include "pch.h"

namespace feng {

	class KeyboardInput
	{
	public:
		std::unordered_map<uint32_t, bool> m_Keys;
		std::unordered_set<uint32_t> m_Pressed;
	private:
		bool m_State;
	public:
		KeyboardInput()
			: m_State(false) { }

		void SetState(bool state) { m_State = state; }

		bool QueryState() const { return m_State; }
	};

}