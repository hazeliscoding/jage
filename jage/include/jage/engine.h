#pragma once

#include "core/window.h"

namespace jage
{
    class Engine
    {
    public:
        static Engine& instance();

        ~Engine() = default;

        void Run();
    private:
        [[nodiscard]] bool Initialize();
        void Shutdown();
        void GetInfo();

    private:
        core::Window window_;

        Engine();

        static Engine* instance_;
    };
}