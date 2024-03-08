#include "jage/engine.h"

int main()
{
    jage::Engine& engine = jage::Engine::instance();
    if (engine.Initialize())
    {
        while (true)
        {
		}
    }
    engine.Shutdown();

    return 0;
}