#pragma once

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
		
	private:
	};
}