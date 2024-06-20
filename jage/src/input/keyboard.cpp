#include "input/keyboard.h"
#include "log.h"

#include <sdl2/SDL_keyboard.h>
#include <sdl2/SDL_stdinc.h>

namespace jage::input
{
    std::array<bool, Keyboard::key_count_> Keyboard::keys_;
    std::array<bool, Keyboard::key_count_> Keyboard::keys_last_;

    void Keyboard::Initialize()
    {
        // Set all elements of the keys and keys_last arrays to false
        std::fill(keys_.begin(), keys_.end(), false);
        std::fill(keys_last_.begin(), keys_last_.end(), false);
    }

    void Keyboard::Update()
    {
        // Copy current keys states to the previous keys state
        keys_last_ = keys_;

        // Get the current key states from SDL
        const Uint8* state = SDL_GetKeyboardState(nullptr);

        // Update the keys array with the new state information from SDL
        for (int i = JAGE_INPUT_KEY_FIRST; i < key_count_; i++)
        {
            keys_[i] = state[i] != 0; // Convert SDL key state to boolean
        }
    }

    bool Keyboard::Key(int key)
    {
        // Ensure the key index is within bounds
        JAGE_ASSERT(key >= JAGE_INPUT_KEY_FIRST && key < key_count_, "Invalid keyboard key!")

        // Return the state of the key if it is within bounds
        if (key >= JAGE_INPUT_KEY_FIRST && key < key_count_)
        {
            return keys_[key];
        }
        return false;
    }

    bool Keyboard::KeyDown(int key)
    {
        // Ensure the key index is within bounds
        JAGE_ASSERT(key >= JAGE_INPUT_KEY_FIRST && key < key_count_, "Invalid keyboard key!")

        // Check if the key was pressed this frame but not the previous frame
        if (key >= JAGE_INPUT_KEY_FIRST && key < key_count_)
        {
            return keys_[key] && !keys_last_[key];
        }
        return false;
    }

    bool Keyboard::KeyUp(int key)
    {
        // Ensure the key index is within bounds
        JAGE_ASSERT(key >= JAGE_INPUT_KEY_FIRST && key < key_count_, "Invalid keyboard key!")

        // Check if the key was released this frame
        if (key >= JAGE_INPUT_KEY_FIRST && key < key_count_)
        {
            return !keys_[key] && keys_last_[key];
        }
        return false;
    }
}
