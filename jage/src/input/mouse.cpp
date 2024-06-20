#include "input/mouse.h"
#include "log.h"

#include <algorithm>

#include <sdl2/SDL_mouse.h>

namespace jage::input
{
	int Mouse::x_ = 0;
	int Mouse::x_last_ = 0;

	int Mouse::y_ = 0;
	int Mouse::y_last_ = 0;

	std::array<bool, Mouse::button_count_> Mouse::buttons_;
	std::array<bool, Mouse::button_count_> Mouse::buttons_down_;
	std::array<bool, Mouse::button_count_> Mouse::buttons_up_;

	void Mouse::Initialize()
	{
		std::fill(buttons_.begin(), buttons_.end(), false);
		std::fill(buttons_down_.begin(), buttons_down_.end(), false);
		std::fill(buttons_up_.begin(), buttons_up_.end(), false);
	}

	void Mouse::Update()
	{
		std::fill(buttons_down_.begin(), buttons_down_.end(), false);
		std::fill(buttons_up_.begin(), buttons_up_.end(), false);

		Uint32 state = SDL_GetMouseState(&x_, &y_);
		for (int i = 0; i < button_count_; i++)
		{
			bool wasDown = buttons_[i];
			buttons_[i] = state & SDL_BUTTON(i + 1);
			bool isDown = buttons_[i];

			if (wasDown && !isDown)
			{
				buttons_up_[i] = true;
			}
			else if (!wasDown && isDown)
			{
				buttons_down_[i] = true;
			}
		}
	}

	bool Mouse::Button(int button)
	{
		JAGE_ASSERT(button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST, "Invalid mouse button!");
		if (button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST)
		{
			return buttons_[button - 1];
		}

		return false;
	}

	bool Mouse::ButtonDown(int button)
	{
		JAGE_ASSERT(button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST, "Invalid mouse button!");
		if (button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST)
		{
			return buttons_down_[button - 1];
		}

		return false;
	}

	bool Mouse::ButtonUp(int button)
	{
		JAGE_ASSERT(button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST, "Invalid mouse button!");
		if (button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST)
		{
			return buttons_up_[button - 1];
		}

		return false;
	}
}
