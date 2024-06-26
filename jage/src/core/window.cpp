#include "jage/core/window.h"
#include "jage/input/mouse.h"
#include "jage/input/keyboard.h"
#include "jage/input/joystick.h"
#include "jage/engine.h"
#include "jage/log.h"

#include <iostream>

#include <sdl2/SDL.h>
#include <glad/glad.h>


namespace jage::core
{
    /**
     * Constructor for the Window class. Initializes the window_ member to nullptr.
     */
    Window::Window()
        : window_(nullptr)
    {
    }

    /**
     * Destructor for the Window class. Ensures proper shutdown if the window is still open.
     */
    Window::~Window()
    {
        if (window_)
        {
            Shutdown();
        }
    }

    bool Window::Create()
    {
        bool ret = true;

        // Create an SDL window centered on the screen with OpenGL and resizable attributes
        window_ = SDL_CreateWindow("JAGE (Just Another Game Engine) v0.1.0",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        if (!window_)
        {
            JAGE_ERROR("Error creating window: {}", SDL_GetError())
                ret = false;
        }

#ifdef JAGE_PLATFORM_MAC
        // Set OpenGL context attributes for compatibility on macOS
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

        // Set OpenGL context attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // Set minimum window size
        SDL_SetWindowMinimumSize(window_, 200, 200);

        // Create an OpenGL context for the window
        glContext_ = SDL_GL_CreateContext(window_);
        if (glContext_ == nullptr)
        {
            JAGE_ERROR("Error creating OpenGL context: {}", SDL_GetError());
            return false;
        }

        // Initialize GLAD for loading OpenGL functions
        gladLoadGLLoader(SDL_GL_GetProcAddress);

        return ret;
    }

    void Window::Shutdown()
    {
        if (window_)
        {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
        }
    }

    void Window::PollEvents()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                Engine::instance().Quit();
                break;
            case SDL_CONTROLLERDEVICEADDED:
                input::JoyStick::OnJoystickConnected(e.cdevice);
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                input::JoyStick::OnJoystickDisconnected(e.cdevice);
                break;
            default:
                break;
            }
        }

        // Update input states for mouse and keyboard
        input::Mouse::Update();
        input::Keyboard::Update();
    }

    void Window::GetSize(int& w, int& h) const
    {
        SDL_GetWindowSize(window_, &w, &h);
    }

    void Window::BeginRender()
    {
        Engine::instance().GetRenderManager().Clear();
    }

    void Window::EndRender()
    {
        SDL_GL_SwapWindow(window_);
    }
}
