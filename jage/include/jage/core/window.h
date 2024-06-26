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

		/**
		 * Creates a new window using SDL with specified attributes and initializes an OpenGL context.
		 * @return True if the window and OpenGL context were created successfully, false otherwise.
		 */
		bool Create();

		/**
		 * Shuts down the window, ensuring all associated resources are properly released.
		 */
		void Shutdown();

		/**
		 * Processes all pending window events, and updates input states.
		 */
		void PollEvents();

		/**
		 * Retrieves the current size of the window.
		 * @param w Reference to store the width of the window.
		 * @param h Reference to store the height of the window.
		 */
		void GetSize(int& w, int& h) const;

		/**
		 * Prepares the rendering pipeline for rendering a new frame, typically by clearing buffers.
		 */
		void BeginRender();

		/**
		 * Completes the rendering of a frame and swaps the front and back buffers.
		 */
		void EndRender();

	private:
		SDL_Window* window_;
		SDL_GLContext glContext_;
	};
}
