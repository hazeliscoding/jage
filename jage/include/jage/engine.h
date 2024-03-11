#pragma once

#include "core/window.h"
#include "managers/logmanager.h"

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
        bool isInitialized_;
        core::Window window_;

        // Managers
        managers::LogManager logManager_;

        // Singleton
        Engine();
        static Engine* instance_;
    };
}