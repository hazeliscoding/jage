#pragma once

#include <array>

namespace jage::input
{
	class Mouse
	{
	public:
		static void Initialize();
		static void Update();

		static int X() { return x_; }
		static int Y() { return y_; }

		static bool Button(int button);
		static bool ButtonDown(int button);
		static bool ButtonUp(int button);
	private:
		constexpr static int button_count_ = 5; // SDL supports up to 5 buttons

		static int x_, x_last_;
		static int y_, y_last_;

		static std::array<bool, button_count_> buttons_;
		static std::array<bool, button_count_> buttons_down_;
		static std::array<bool, button_count_> buttons_up_;
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