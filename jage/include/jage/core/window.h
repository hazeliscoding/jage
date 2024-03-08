#pragma once

struct SDL_Window;

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
	};
}
