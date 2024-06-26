#include "jage/input/joystick.h"
#include "jage/log.h"

#include <algorithm>

#include <sdl2/SDL_events.h>
#include <sdl2/SDL_gamecontroller.h>

namespace jage::input
{
	std::unordered_map<int, std::unique_ptr<JoyStick::Controller>> JoyStick::available_joysticks_;

	void JoyStick::OnJoystickConnected(SDL_ControllerDeviceEvent& e)
	{
	}

	void JoyStick::OnJoystickDisconnected(SDL_ControllerDeviceEvent& e)
	{
	}

	void JoyStick::Shutdown()
	{
	}

	void JoyStick::Update()
	{
	}

	bool JoyStick::GetButton(int joystick_id, Button button)
	{
	}

	bool JoyStick::GetButtonDown(int joystick_id, Button button)
	{
	}

	bool JoyStick::GetButtonUp(int joystick_id, Button button)
	{
	}

	float JoyStick::GetAxis(int joystick_id, Axis axis)
	{
	}

	int JoyStick::GetNextFreeIndex()
	{
	}
}
