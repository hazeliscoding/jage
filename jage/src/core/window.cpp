#include "core/window.h"
#include "engine.h"
#include "log.h"

#include <iostream>

#include <sdl2/SDL.h>

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

		window_ = SDL_CreateWindow("JAGE (Just Another Game Engine) v0.1.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
		if (!window_)
		{
			JAGE_ERROR("Error creating window: {}", SDL_GetError())
			ret = false;
		}

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
	}
}
