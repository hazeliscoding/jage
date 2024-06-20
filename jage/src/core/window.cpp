#include "core/window.h"
#include "input/mouse.h"
#include "input/keyboard.h"
#include "engine.h"
#include "log.h"

#include <iostream>

#include <sdl2/SDL.h>
#include <glad/glad.h>

namespace jage::core
{
	Window::Window()
		: window_(nullptr)
	{
	}

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

		window_ = SDL_CreateWindow("JAGE (Just Another Game Engine) v0.1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if (!window_)
		{
			JAGE_ERROR("Error creating window: {}", SDL_GetError())
			ret = false;
		}

#ifdef JAGE_PLATFORM_MAC
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_SetWindowMinimumSize(window_, 200, 200);

		glContext_ = SDL_GL_CreateContext(window_);
		if (glContext_ == nullptr)
		{
			JAGE_ERROR("Error creating OpenGL context: {}", SDL_GetError());
			return false;
		}

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
				default:
					break;
			}
		}

		// Update input
		input::Mouse::Update();
		input::Keyboard::Update();
	}

	void Window::GetSize(int& w, int& h)
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
