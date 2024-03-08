#include "engine.h"

#include <iostream>

#include <sdl2/SDL.h>

namespace jage 
{
    Engine* Engine::instance_ = nullptr;

    Engine::Engine()
    {
		GetInfo();
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
#ifdef JAGE_CONFIG_DEBUG
        std::cout << "Configuration: DEBUG" << std::endl;
#endif
#ifdef JAGE_CONFIG_RELEASE
        std::cout << "Configuration: RELEASE" << std::endl;
#endif
#ifdef JAGE_PLATFORM_WINDOWS
        std::cout << "Platform: WINDOWS" << std::endl;
#endif
#ifdef JAGE_PLATFORM_LINUX
        std::cout << "Platform: LINUX" << std::endl;
#endif
#ifdef JAGE_PLATFORM_MAC
        std::cout << "Platform: MAC" << std::endl;
#endif
    }

    void Engine::Run()
    {
        if (Initialize())
        {
            // Core loop
            while (true)
            {
                window_.PollEvents();
            }
        }

		Shutdown();
    }

    bool Engine::Initialize()
    {
        bool ret = true;

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cerr << "Failed to Initialize SDL2: " << SDL_GetError() << '\n';
            ret = false;
		}
        else
        {
            SDL_version version;
            SDL_VERSION(&version)
            std::cout << "SDL2 version: " << static_cast<int>(version.major) << '.' << static_cast<int>(version.minor) << '.' << static_cast<int>(version.patch) <<
                '\n';

            if (!window_.Create())
            {
				ret = false;
			}
        }

        if (!ret)
        {
	        std::cerr << "Failed to initialize engine. Shutting down..." << '\n';
            Shutdown();
        }

		return ret;
    }

    void Engine::Shutdown()
    {
        window_.Shutdown();
	    SDL_Quit();
    }

}