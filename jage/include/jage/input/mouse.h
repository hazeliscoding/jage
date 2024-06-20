#pragma once

#include <array>

namespace jage::input
{
	class Mouse
	{
	public:
		/**
		 * Initializes mouse states, setting all button states to false.
		 */
		static void Initialize();

		/**
		 * Updates the state of the mouse by recording current positions and button states.
		 * Positions and button states are fetched using SDL functions.
		 */
		static void Update();

		static int X() { return x_; }
		static int Y() { return y_; }

		static int DX() { return x_ - x_last_; }
		static int DY() { return y_ - y_last_; }

		/**
		 * Checks if a specific mouse button is currently pressed.
		 * @param button The button index to check.
		 * @return True if the button is pressed, false otherwise.
		 */
		static bool Button(int button);

		/**
		 * Checks if a specific mouse button was pressed this frame.
		 * @param button The button index to check.
		 * @return True if the button was pressed this frame, false otherwise.
		 */
		static bool ButtonDown(int button);

		/**
		 * Checks if a specific mouse button was released this frame.
		 * @param button The button index to check.
		 * @return True if the button was released this frame, false otherwise.
		 */
		static bool ButtonUp(int button);
	private:
		constexpr static int button_count_ = 5; // SDL supports up to 5 buttons

		static int x_, x_last_;
		static int y_, y_last_;

		static std::array<bool, button_count_> buttons_;
		static std::array<bool, button_count_> buttons_last_;
	};
}

enum
{
	JAGE_INPUT_MOUSE_FIRST = 1,
	JAGE_INPUT_MOUSE_LEFT = JAGE_INPUT_MOUSE_FIRST,
	JAGE_INPUT_MOUSE_MIDDLE = 2,
	JAGE_INPUT_MOUSE_RIGHT = 3,
	JAGE_INPUT_MOUSE_X1 = 4,
	JAGE_INPUT_MOUSE_X2 = 5,
	JAGE_INPUT_MOUSE_LAST = 5
};