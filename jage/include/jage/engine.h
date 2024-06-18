#pragma once

#include "core/window.h"
#include "managers/logmanager.h"
#include "managers/rendermanager.h"

namespace jage
{
    class Engine
    {
    public:
        static Engine& instance();

        ~Engine() = default;

        void Run();

        void Quit() { is_running_ = false; }

        // Managers
        managers::RenderManager& GetRenderManager() { return render_manager_; }
    private:
        [[nodiscard]] bool Initialize();
        void Shutdown();
        void GetInfo();

    private:
        bool is_running_;
        bool is_initialized_;
        core::Window window_;

        // Managers
        managers::LogManager log_manager_;
        managers::RenderManager render_manager_;

        // Singleton
        Engine();
        static Engine* instance_;
    };
}