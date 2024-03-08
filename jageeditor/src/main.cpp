#include <iostream>

#include "jage/engine.h"

int main()
{
    jage::Engine& engine = jage::Engine::instance();
    engine.Initialize();
    engine.Shutdown();

    return 0;
}