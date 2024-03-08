#include "engine.h"

#include <iostream>

namespace jage 
{
    int Add(int a, int b) { return a + b; }

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
}