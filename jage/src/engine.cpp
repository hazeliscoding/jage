#include "engine.h"
#include "log.h"

#include <iostream>

#include <sdl2/SDL.h>

#include "graphics/mesh.h"
#include "graphics/shader.h"

namespace jage 
{
    Engine* Engine::instance_ = nullptr;

    Engine::Engine()
	    : is_running_(false)
		, is_initialized_(false)
    {
    }

    Engine& Engine::instance()
    {
		if (!instance_)
		{
			instance_ = new Engine();
		}

        return *instance_;
    }

    void Engine::GetInfo()
    {
        JAGE_TRACE("JAGE v{}.{}", 0, 1);
#ifdef JAGE_CONFIG_DEBUG
        JAGE_DEBUG("Configuration: DEBUG");
#endif
#ifdef JAGE_CONFIG_RELEASE
        JAGE_DEBUG("Configuration: RELEASE");
#endif
#ifdef JAGE_PLATFORM_WINDOWS
        JAGE_WARN("Platform: WINDOWS");
#endif
#ifdef JAGE_PLATFORM_LINUX
        JAGE_WARN("Platform: LINUX");
#endif
#ifdef JAGE_PLATFORM_MAC
        JAGE_WARN("Platform: MAC");
#endif
    }

    void Engine::Run()
    {
        if (Initialize())
        {
            // Test Mesh
            float vertices[]
            {
                 0.5f,  0.5f, 0.f,
                 0.5f, -0.5f, 0.f,
                -0.5f, -0.5f, 0.f,
                -0.5f,  0.5f, 0.f
            };
            uint32_t elements[]
            {
                0, 3, 1,
                1, 3, 2
            };
            std::shared_ptr<graphics::Mesh> mesh = std::make_shared<graphics::Mesh>(&vertices[0], 4, 3, &elements[0], 6);

            // Test Shader
            const char* vertexShader = R"(
				#version 410 core
				layout (location = 0) in vec3 position;
				out vec3 vpos;
				void main()
				{
					vpos = position + vec3(0.5, 0.5, 0);
					gl_Position = vec4(position, 1.0);
				}
			)";

            const char* fragmentShader = R"(
				#version 410 core
				out vec4 outColor;
				in vec3 vpos;
				uniform vec3 color = vec3(0.0);
				void main()
				{
					outColor = vec4(vpos, 1.0);
				}
			)";
            std::shared_ptr<graphics::Shader> shader = std::make_shared<graphics::Shader>(vertexShader, fragmentShader);
            shader->SetUniformFloat3("color", 1, 0, 0);

            render_manager_.SetWireframeMode(true);

            // Core loop
            while (is_running_)
            {
                window_.PollEvents();

                window_.BeginRender();

                auto rc = std::make_unique<graphics::rendercommands::RenderMesh>(mesh, shader);
                render_manager_.Submit(std::move(rc));
                render_manager_.Flush();

                window_.EndRender();
            }
        }

		Shutdown();
    }

    bool Engine::Initialize()
    {
        bool ret = false;

        JAGE_ASSERT(!is_initialized_, "Attempting to call Engine::Initialize() more than once!")

        if (!is_initialized_)
        {
            GetInfo();

            if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            {
                JAGE_ERROR("Error initializing SDL2: {}", SDL_GetError());
            }
            else
            {
                SDL_version version;
                SDL_VERSION(&version)
                JAGE_INFO("SDL {}.{}.{}", static_cast<int32_t>(version.major), static_cast<int32_t>(version.minor), static_cast<int32_t>(version.patch));

                if (window_.Create())
                {
                    // Initialize managers
                    jage::managers::RenderManager::Initialize();
                    jage::managers::LogManager::Initialize();

                    ret = true;
                    is_running_ = true;
                    is_initialized_ = true;
                }
            }

            if (!ret)
            {
                JAGE_ERROR("Engine initialization failed. Shutting down.")
                Shutdown();
            }
        }

        return ret;
    }

    void Engine::Shutdown()
    {
        is_running_ = false;
        is_initialized_ = false;

        // Managers - usually in reverse order
        render_manager_.Shutdown();
        log_manager_.Shutdown();

        // Shutdown SDL
        window_.Shutdown();
	    SDL_Quit();
    }

}
