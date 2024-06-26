#pragma once

#include <array>
#include <unordered_map>
#include <memory>

struct SDL_ControllerDeviceEvent;
typedef struct _SDL_GameController SDL_GameController;

namespace jage::input
{
	class JoyStick
	{
	public:
		// Adapted from SDL - see SDL_GameControllerAxis
		enum class Axis
		{
			LeftStickHorizontal,
			LeftStickVertical,
			RightStickHorizontal,
			RightStickVertical,
			LeftTrigger,
			RightTrigger,
			Count
		};

		// Adapted from SDL - see SDL_GameControllerButton
		enum class Button
		{
			A,
			B,
			X,
			Y,
			Back,
			Guide,
			Start,
			LS,
			RS,
			LB,
			RB,
			DPAD_Up,
			DPAD_Down,
			DPAD_Left,
			DPAD_Right,
			Count
		};

	public:
		static void OnJoystickConnected(SDL_ControllerDeviceEvent& e);
		static void OnJoystickDisconnected(SDL_ControllerDeviceEvent& e);
		static void Shutdown();
		static void Update();

		static bool GetButton(int joystick_id, Button button);
		static bool GetButtonDown(int joystick_id, Button button);
		static bool GetButtonUp(int joystick_id, Button button);
		static float GetAxis(int joystick_id, Axis axis);

	private:
		static int GetNextFreeIndex();
		
	private:
		struct Controller
		{
			SDL_GameController* gc = nullptr;

			std::array<bool, static_cast<int>(Button::Count)> buttons;
			std::array<bool, static_cast<int>(Button::Count)> last_buttons;
			std::array<bool, static_cast<int>(Axis::Count)> axes;
			std::array<bool, static_cast<int>(Axis::Count)> last_axes;
		};

		static std::unordered_map<int, std::unique_ptr<Controller>> available_joysticks_;
	};
}