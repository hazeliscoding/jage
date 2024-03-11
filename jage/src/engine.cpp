#include "engine.h"
#include "log.h"

#include <iostream>

#include <sdl2/SDL.h>

namespace jage 
{
    Engine* Engine::instance_ = nullptr;

    Engine::Engine()
	    : isRunning_(false)
		, isInitialized_(false)
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
            // Core loop
            while (isRunning_)
            {
                window_.PollEvents();
            }
        }

		Shutdown();
    }

    bool Engine::Initialize()
    {
        bool ret = false;

        JAGE_ASSERT(!isInitialized_, "Attempting to call Engine::Initialize() more than once!")

        if (!isInitialized_)
        {
            logManager_.Initialize();
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
                    ret = true;
                    isRunning_ = true;
                    isInitialized_ = true;
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
        isRunning_ = false;
        isInitialized_ = false;

        // Managers - usually in reverse order
        logManager_.Shutdown();

        // Shutdown SDL
        window_.Shutdown();
	    SDL_Quit();
    }

}