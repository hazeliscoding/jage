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

		void GetSize(int& w, int& h);

		void BeginRender();
		void EndRender();

	private:
		SDL_Window* window_;
		SDL_GLContext glContext_;
	};
}
