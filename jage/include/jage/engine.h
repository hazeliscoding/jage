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

        void Quit() { isRunning_ = false; }
    private:
        [[nodiscard]] bool Initialize();
        void Shutdown();
        void GetInfo();

    private:
        bool isRunning_;
        core::Window window_;

        Engine();

        static Engine* instance_;
    };
}