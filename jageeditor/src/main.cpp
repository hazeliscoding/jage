#include <iostream>

#include "jage/engine.h"

int main()
{
    jage::GetInfo();

    jage::Initialize();
    jage::Shutdown();

#ifdef JAGE_PLATFORM_WINDOWS
    system("pause");
#endif

    return 0;
}