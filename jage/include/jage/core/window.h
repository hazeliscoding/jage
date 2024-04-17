#pragma once

struct SDL_Window;
using SDL_GLContext = void*;

namespace jage::core
{
	class Window
	{
	public:
		Window();
		~Window();

		bool Create();
		void Shutdown();

		void PollEvents();
	private:
		SDL_Window* window_;
		SDL_GLContext glContext_;
	};
}
