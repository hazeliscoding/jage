#pragma once

#include "jage/graphics/rendercommands.h"

#include <queue>
#include <memory>

namespace jage::managers
{
	class RenderManager
	{
	public:
		RenderManager() = default;
		~RenderManager() = default;

		void Initialize();
		void Shutdown();

		void Clear();
		void SetClearColour(float r, float g, float b, float a);
		void SetWireframeMode(bool enabled);

		void Submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rc);

		// Execute submitted RenderCommands in the order they were received.
		// We can extend the API if we need to mitigate performance impact
		void Flush();

	private:
		std::queue<std::unique_ptr<graphics::rendercommands::RenderCommand>> render_commands_;
	};
}