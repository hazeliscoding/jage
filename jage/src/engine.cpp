#include "engine.h"

#include <iostream>

#include <sdl2/SDL.h>

namespace jage 
{
    void GetInfo()
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

    bool Initialize()
    {
        bool ret = true;

		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cerr << "Failed to initialize SDL2: " << SDL_GetError() << '\n';
            ret = false;
		}
        else
        {
            SDL_version version;
            SDL_VERSION(&version);
            std::cout << "SDL2 version: " << static_cast<int>(version.major) << '.' << static_cast<int>(version.minor) << '.' << static_cast<int>(version.patch) <<
                '\n';
        }

		return ret;
    }

    void Shutdown()
    {
	    SDL_Quit();
    }

}