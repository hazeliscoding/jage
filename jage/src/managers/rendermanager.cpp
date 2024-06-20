#include "managers/rendermanager.h"
#include "graphics/helpers.h"

#include "log.h"

#include <glad/glad.h>

namespace jage::managers
{
	RenderManager::RenderManager()
	{

	}

	RenderManager::~RenderManager()
	{
	}

	void RenderManager::Initialize()
	{
		// Log OpenGL vendor, renderer, and version information
		JAGE_INFO("OpenGL Info:\n  Vendor:\t{}\n  Renderer:\t{}\n  Version:\t{}",
			(const char*)glGetString(GL_VENDOR),
			(const char*)glGetString(GL_RENDERER),
			(const char*)glGetString(GL_VERSION))

		// Enable depth testing to ensure pixels closer to the camera obscure those further away
		glEnable(GL_DEPTH_TEST); JAGE_CHECK_GL_ERROR
		glDepthFunc(GL_LEQUAL); JAGE_CHECK_GL_ERROR // Use the less or equal depth test 

		// Enable blending to support alpha transparency
		glEnable(GL_BLEND); JAGE_CHECK_GL_ERROR
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); JAGE_CHECK_GL_ERROR

		// Set the initial background color to cornflower blue
		SetClearColour(
			static_cast<float>(0x64) / static_cast<float>(0xFF),  // Convert hex to float for red
			static_cast<float>(0x95) / static_cast<float>(0xFF),  // Convert hex to float for green
			static_cast<float>(0xED) / static_cast<float>(0xFF),  // Convert hex to float for blue
			1  // Alpha value
		);
	}

	void RenderManager::Shutdown()
	{
		// Clear all remaining render commands in the queue
		while (!render_commands_.empty())
		{
			render_commands_.pop();
		}
	}

	void RenderManager::Clear()
	{
		// Check for leftover commands which should have been flushed
		JAGE_ASSERT(render_commands_.empty(), "Unfinished render commands in queue!");

		// Clear any remaining commands just in case
		while (!render_commands_.empty())
		{
			render_commands_.pop();
		}

		// Clear the color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); JAGE_CHECK_GL_ERROR
	}

	void RenderManager::SetClearColour(const float r, const float g, const float b, const float a)
	{
		// Set the clear color for OpenGL
		glClearColor(r, g, b, a); JAGE_CHECK_GL_ERROR
	}

	void RenderManager::SetWireframeMode(const bool enabled)
	{
		// Enable or disable wireframe mode
		if (enabled)
		{
			// Set mode to wireframe
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); JAGE_CHECK_GL_ERROR
		}
		else
		{
			// Set mode to filled polygons
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); JAGE_CHECK_GL_ERROR
		}
	}

	void RenderManager::Submit(std::unique_ptr<graphics::rendercommands::RenderCommand> rc)
	{
		// Add a new render command to the queue
		render_commands_.push(std::move(rc));
	}

	void RenderManager::Flush()
	{
		// Execute all render commands in the queue
		while (!render_commands_.empty())
		{
			auto rc = std::move(render_commands_.front());  // Move the front command for execution
			render_commands_.pop();  // Remove the command from the queue
			rc->Execute();  // Execute the render command
		}
	}
}
	