#include "jage/input/joystick.h"
#include "jage/log.h"

#include <algorithm>

#include <sdl2/SDL_events.h>
#include <sdl2/SDL_gamecontroller.h>

namespace jage::input
{
	std::unordered_map<int, std::unique_ptr<JoyStick::Controller>> JoyStick::available_joysticks_;
	float JoyStick::dead_zone_ = 0.2f;

	void JoyStick::OnJoystickConnected(SDL_ControllerDeviceEvent& e)
	{
		if (const auto device_index = e.which; SDL_IsGameController(device_index))
		{
			auto c = std::make_unique<Controller>();
			c->gc = SDL_GameControllerOpen(device_index);
			if (c->gc)
			{
				c->joystick_index = device_index;
				std::fill(c->buttons.begin(), c->buttons.end(), false);
				std::fill(c->last_buttons.begin(), c->last_buttons.end(), false);
				std::fill(c->axes.begin(), c->axes.end(), 0.f);
				std::fill(c->last_axes.begin(), c->last_axes.end(), 0.f);

				int map_index = GetNextFreeIndex();
				JAGE_TRACE("Joystick connected: map_index({}), device({})", map_index, device_index)
				available_joysticks_[map_index] = std::move(c);
			}
			else
			{
				JAGE_ERROR("Error opening controller with Device Index: {}: {}", device_index, SDL_GetError())
			}
		}
	}

	void JoyStick::OnJoystickDisconnected(SDL_ControllerDeviceEvent& e)
	{
		int device_index = e.which;
		for (auto it = available_joysticks_.begin(); it != available_joysticks_.end(); ++it)
		{
			Controller* c = it->second.get();
			if (c->joystick_index == device_index)
			{
				JAGE_TRACE("Joystick disconnected: {}", device_index);
				SDL_GameControllerClose(c->gc);
				available_joysticks_.erase(it);
				break;
			}
		}
	}

	void JoyStick::Shutdown()
	{
		for (auto it = available_joysticks_.begin(); it != available_joysticks_.end();)
		{
			Controller* c = it->second.get();
	
			SDL_GameControllerClose(c->gc);
			available_joysticks_.erase(it);
		}
	}

	void JoyStick::Update()
	{
		for (auto& available_joystick : available_joysticks_)
		{
			Controller* c = available_joystick.second.get();
			JAGE_ASSERT(c && c->gc, "Invalid game controller in available_joysticks_ map")

			if (c && c->gc)
			{
				c->last_buttons = c->buttons;
				c->last_axes = c->axes;

				for (unsigned int i = 0; i < static_cast<int>(Button::Count); i++)
				{
					c->buttons[i] = SDL_GameControllerGetButton(c->gc, static_cast<SDL_GameControllerButton>(i));
				}
				for (unsigned int i = 0; i < static_cast<int>(Axis::Count); i++)
				{
					// SDL ranges axes from -32768 to 32767
					c->axes[i] = std::clamp(SDL_GameControllerGetAxis(c->gc, static_cast<SDL_GameControllerAxis>(i)) / 32767.f, -1.f, 1.f);
				}
			}
		}
	}

	bool JoyStick::IsJoystickAvailable(int joystickId)
	{
		return available_joysticks_.count(joystickId) > 0;
	}

	bool JoyStick::GetButton(int joystick_id, Button button)
	{
		auto it = available_joysticks_.find(joystick_id);
		if (it != available_joysticks_.end())
		{
			return it->second->buttons[static_cast<int>(button)];
		}

		JAGE_ERROR("Joystick with id {} is not available!", joystick_id)
		return false;
	}

	bool JoyStick::GetButtonDown(int joystick_id, Button button)
	{
		auto it = available_joysticks_.find(joystick_id);
		if (it != available_joysticks_.end())
		{
			return it->second->buttons[static_cast<int>(button)] && !it->second->last_buttons[static_cast<int>(button)];
		}

		JAGE_ERROR("Joystick with id {} is not available!", joystick_id)
		return false;
	}

	bool JoyStick::GetButtonUp(int joystick_id, Button button)
	{
		auto it = available_joysticks_.find(joystick_id);
		if (it != available_joysticks_.end())
		{
			return !it->second->buttons[static_cast<int>(button)] && it->second->last_buttons[static_cast<int>(button)];
		}

		JAGE_ERROR("Joystick with id {} is not available!", joystick_id)
		return false;
	}

	float JoyStick::GetAxis(int joystick_id, Axis axis)
	{
		auto it = available_joysticks_.find(joystick_id);
		if (it != available_joysticks_.end())
		{
			float val = it->second->axes[static_cast<int>(axis)];
			return abs(val) > dead_zone_ ? val : 0.f;
		}

		JAGE_ERROR("Joystick with id {} is not available!", joystick_id)
		return 0.f;
	}

	int JoyStick::GetNextFreeIndex()
	{
		int ret = -1;

		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (available_joysticks_.count(i) == 0)
			{
				ret = i;
				break;
			}
		}

		return ret;
	}
}
