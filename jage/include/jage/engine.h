#pragma once

#include "core/window.h"

namespace jage
{
    class Engine
    {
    public:
        static Engine& instance();

        ~Engine() = default;

        [[nodiscard]] bool Initialize();
        void Shutdown();
    private:
        void GetInfo();

    private:
        core::Window window_;

        Engine();

        static Engine* instance_;
    };
}