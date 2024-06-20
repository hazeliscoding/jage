#include "input/mouse.h"
#include "log.h"

#include <algorithm>

#include <sdl2/SDL_mouse.h>

namespace jage::input
{
    int Mouse::x_ = 0;  // Current mouse X position
    int Mouse::x_last_ = 0;  // Previous mouse X position

    int Mouse::y_ = 0;  // Current mouse Y position
    int Mouse::y_last_ = 0;  // Previous mouse Y position

    std::array<bool, Mouse::button_count_> Mouse::buttons_;  // Current state of mouse buttons
    std::array<bool, Mouse::button_count_> Mouse::buttons_last_;  // Previous state of mouse buttons

    void Mouse::Initialize()
    {
        // Set all button states to false initially
        std::fill(buttons_.begin(), buttons_.end(), false);
        std::fill(buttons_last_.begin(), buttons_last_.end(), false);
    }

    void Mouse::Update()
    {
        // Record last known positions
        x_last_ = x_;
        y_last_ = y_;

        // Record last button states
        buttons_last_ = buttons_;  // TODO: Consider using memcpy for potential optimization

        // Fetch current mouse state from SDL
        const Uint32 state = SDL_GetMouseState(&x_, &y_);

        // Update button states based on SDL state flags
        for (int i = 0; i < button_count_; i++)
        {
            buttons_[i] = (state & SDL_BUTTON(i + 1)) != 0;  // Convert button bit flag to boolean
        }
    }

    bool Mouse::Button(int button)
    {
        // Ensure button index is within valid range
        JAGE_ASSERT(button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST, "Invalid mouse button!");

        // Check button state
        if (button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST)
        {
            return buttons_[button - 1];  // Adjust for zero-based index
        }
        return false;
    }

    bool Mouse::ButtonDown(int button)
    {
        // Ensure button index is within valid range
        JAGE_ASSERT(button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST, "Invalid mouse button!");

        // Check if the button was pressed this frame but not the last
        if (button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST)
        {
            return buttons_[button - 1] && !buttons_last_[button - 1];  // Adjust for zero-based index
        }
        return false;
    }

    bool Mouse::ButtonUp(int button)
    {
        // Ensure button index is within valid range
        JAGE_ASSERT(button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST, "Invalid mouse button!");

        // Check if the button was released this frame
        if (button >= JAGE_INPUT_MOUSE_FIRST && button <= JAGE_INPUT_MOUSE_LAST)
        {
            return !buttons_[button - 1] && buttons_last_[button - 1];  // Adjust for zero-based index
        }
        return false;
    }
}
