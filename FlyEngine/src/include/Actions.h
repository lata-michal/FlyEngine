#pragma once

#include "pch.h"

namespace feng {

	namespace {

		enum class KeyActions : char
		{
			/*Move actions: mask -> 0xF*/
			FORWARD = 1, 
			BACKWARD = 2,
			LEFT = 4,
			RIGHT = 8,
			/*other actions: */
			FULLSCREEN = 16,
			EXIT = 32
		};

		enum class MouseActions : char
		{
			LOOKAROUND = 1,
		};

		enum class ScrollActions : char
		{
			ZOOM = 1,
		};

		KeyActions operator| (KeyActions lhs, KeyActions rhs)
		{
			return static_cast<KeyActions>(static_cast<std::underlying_type<KeyActions>::type>(lhs) | static_cast<std::underlying_type<KeyActions>::type>(rhs));
		}

		KeyActions operator& (KeyActions lhs, KeyActions rhs)
		{
			return static_cast<KeyActions>(static_cast<std::underlying_type<KeyActions>::type>(lhs) & static_cast<std::underlying_type<KeyActions>::type>(rhs));
		}

		KeyActions operator& (KeyActions lhs, int rhs)
		{
			return static_cast<KeyActions>(static_cast<std::underlying_type<KeyActions>::type>(lhs) & static_cast<std::underlying_type<KeyActions>::type>(rhs));
		}

		MouseActions operator| (MouseActions lhs, MouseActions rhs)
		{
			return static_cast<MouseActions>(static_cast<std::underlying_type<MouseActions>::type>(lhs) | static_cast<std::underlying_type<MouseActions>::type>(rhs));
		}

		MouseActions operator& (MouseActions lhs, MouseActions rhs)
		{
			return static_cast<MouseActions>(static_cast<std::underlying_type<MouseActions>::type>(lhs) & static_cast<std::underlying_type<MouseActions>::type>(rhs));
		}

		ScrollActions operator| (ScrollActions lhs, ScrollActions rhs)
		{
			return static_cast<ScrollActions>(static_cast<std::underlying_type<ScrollActions >::type>(lhs) | static_cast<std::underlying_type<ScrollActions >::type>(rhs));
		}

		ScrollActions operator& (ScrollActions lhs, ScrollActions rhs)
		{
			return static_cast<ScrollActions>(static_cast<std::underlying_type<ScrollActions>::type>(lhs) & static_cast<std::underlying_type<ScrollActions >::type>(rhs));
		}
	}

	class Actions
	{
	private:
		std::unordered_map<int, KeyActions> m_KeyBindings;
		std::unordered_map<int, MouseActions> m_MouseBindings;
		std::unordered_map<int, ScrollActions> m_ScrollBindings;
	public:
		virtual void Move(KeyActions direction, float deltaTime) = 0;
		virtual void LookAround(float xoffset, float yoffset, bool constrainPitch = true) = 0;
		virtual void Zoom(float yoffset) { }
		virtual void OtherAction(KeyActions action, float deltaTime) { }

		virtual void setKeyBindings(const std::unordered_map<int, KeyActions>& keyBindings) { m_KeyBindings = keyBindings; }
		virtual void setMouseBindings(const std::unordered_map<int, MouseActions>& mouseBindings) { m_MouseBindings = mouseBindings; }
		virtual void setScrollBindings(const std::unordered_map<int, ScrollActions>& scrollBindings) { m_ScrollBindings = scrollBindings; }

		virtual void AddKeyBinding(int key, KeyActions action) { m_KeyBindings[key] = action; }
		virtual void AddMouseBinding(int key, MouseActions action) { m_MouseBindings[key] = action; }
		virtual void AddScrollBinding(int key, ScrollActions action) { m_ScrollBindings[key] = action; }

		virtual const std::unordered_map<int, KeyActions>& GetKeyBindings() const { return m_KeyBindings; }
		virtual const std::unordered_map<int, MouseActions>& GetMouseBindings() const { return m_MouseBindings; }
		virtual const std::unordered_map<int, ScrollActions>& GetScrollBindings() const { return m_ScrollBindings; }
	};

}