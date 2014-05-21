#include "InputManager.h"
#include <iostream>

namespace RootEngine
{
	namespace InputManager
	{
		RootEngine::SubsystemSharedContext g_context;
		InputManager* InputManager::s_inputSys = nullptr;

		InputManager::InputManager()
		{
			m_window = nullptr;
		}

		void InputManager::Startup(void)
		{
			m_lockDownInput = false;
			int err = SDL_SetRelativeMouseMode(SDL_FALSE);
			m_globMousePos = glm::ivec2(0, 0);
			m_deltaMousePos = glm::ivec2(0, 0);

			for(int i = 0; i < MAX_KEYS; i++)
				m_keyState[i] = KeyState::UP;

			g_context.m_logger->LogText(LogTag::INPUT, LogLevel::INIT_PRINT, "InputManager subsystem initialized!");
		}


		void InputManager::Shutdown(void)
		{
			delete s_inputSys;
		}

		void InputManager::HandleInput(SDL_Event& p_event)
		{
			if(m_lockDownInput)
				return;

			switch(p_event.type)
			{
			case SDL_MOUSEWHEEL:
				m_scroll += p_event.wheel.y;
				break;
			case SDL_KEYDOWN:
				if(!p_event.key.repeat)
				{
					m_keyState[p_event.key.keysym.scancode] = KeyState::DOWN;
				}
				break;
			case SDL_KEYUP:
				m_keyState[p_event.key.keysym.scancode] = KeyState::UP;
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_keyState[p_event.button.button-SDL_BUTTON_LEFT+MouseButton::LEFT] = KeyState::DOWN;
				break;
			case SDL_MOUSEBUTTONUP:
				m_keyState[p_event.button.button-SDL_BUTTON_LEFT+MouseButton::LEFT] = KeyState::UP;
				break;

			case SDL_MOUSEMOTION:

				//m_deltaMousePos.x = p_event.motion.x - m_globMousePos.x;
				//m_deltaMousePos.y = p_event.motion.y - m_globMousePos.y;
				if(SDL_GetRelativeMouseMode())
				{
					m_globMousePos.x = p_event.motion.x;
					m_globMousePos.y = p_event.motion.y;

					m_deltaMousePos.x = p_event.motion.xrel;
					m_deltaMousePos.y = p_event.motion.yrel;
				}
				else
				{
					m_globMousePos.x = -p_event.motion.x;
					m_globMousePos.y = -p_event.motion.y;

					m_deltaMousePos.x = -p_event.motion.xrel;
					m_deltaMousePos.y = -p_event.motion.yrel;
				}
				if(m_lockMouseEnabled)
				{
					int w, h;
					if(m_window != nullptr)
					{
						SDL_GetWindowSize(m_window, &w, &h);
					}
					else
					{
						w = 1280;
						h = 720;
					}
					
					SDL_WarpMouseInWindow(NULL, w/2, h/2);
				}
				break;
			default:
				g_context.m_logger->LogText(LogTag::INPUT, LogLevel::MASS_DATA_PRINT, "Event %d did not match any case", p_event.type);
			}
		}

		void InputManager::SetMousePos(const glm::ivec2& p_mousePos)
		{
			m_deltaMousePos = m_globMousePos - p_mousePos;

			m_globMousePos = p_mousePos;
		}

		KeyState::KeyState InputManager::GetKeyState(SDL_Scancode p_key)
		{
			if(m_keyState[p_key] == KeyState::DOWN)
			{
				if(m_prevKeyState[p_key] == KeyState::DOWN)
				{
					return KeyState::DOWN;
				}
				else if(m_prevKeyState[p_key] == KeyState::UP)
				{
					return KeyState::DOWN_EDGE;
				}
			}
			else if(m_keyState[p_key] == KeyState::UP)
			{
				if(m_prevKeyState[p_key] == KeyState::DOWN)
				{
					return KeyState::UP_EDGE;
				}
				else if(m_prevKeyState[p_key] == KeyState::UP)
				{
					return KeyState::UP;
				}
			}

			assert(false);
			return KeyState::UP;
		}

		KeyState::KeyState InputManager::GetKeyState(MouseButton::MouseButton p_button)
		{
			return GetKeyState((SDL_Scancode)p_button);
		}

		glm::ivec2 InputManager::GetGlobalMousePos()
		{
			if(g_context.m_configManager->GetConfigValueAsBool("settings-mouse-invert"))
				return glm::ivec2(m_globMousePos.x, -m_globMousePos.y);
			return m_globMousePos;
		}

		glm::ivec2 InputManager::GetDeltaMousePos()
		{
			glm::ivec2 temp = m_deltaMousePos;
			if(g_context.m_configManager->GetConfigValueAsBool("settings-mouse-invert"))
				temp.y = -temp.y;
			return temp;
		}

		InputManager* InputManager::GetInstance()
		{ 
			if(!s_inputSys)
				s_inputSys = new InputManager();

				return s_inputSys;
		}

		void InputManager::Reset()
		{
			m_deltaMousePos = glm::vec2(0, 0);
			m_scroll = 0;

			memcpy(m_prevKeyState, m_keyState, sizeof(KeyState::KeyState) * MAX_KEYS);
		}

		void InputManager::LockMouseToCenter(bool p_enable)
		{
			m_lockMouseEnabled = p_enable;
			//SDL_SetRelativeMouseMode((SDL_bool) p_enable);
			SDL_ShowCursor(!p_enable);
		}
		//Get scroll count, negative is scroll down (backwards) and positive is scroll up (forward)
		int InputManager::GetScroll()
		{
			return m_scroll;
		}

		void InputManager::SetWindow( SDL_Window* p_window )
		{
			m_window = p_window;
		}

	}

}

RootEngine::InputManager::InputInterface* CreateInputSystem(RootEngine::SubsystemSharedContext p_context)
{
	RootEngine::InputManager::g_context = p_context;

	return RootEngine::InputManager::InputManager::GetInstance();
}


