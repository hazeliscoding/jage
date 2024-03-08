#include "jage/engine.h"

int main()
{
    jage::Engine& engine = jage::Engine::instance();
    engine.Run();

    return 0;
}