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
	private:
		SDL_Window* window_;
	};
}
