#pragma once

namespace jage
{
    class Engine
    {
    public:
        static Engine& instance();

        ~Engine() = default;

        bool Initialize();
        void Shutdown();
    private:
        void GetInfo();

        Engine();

        static Engine* instance_;
    };
}