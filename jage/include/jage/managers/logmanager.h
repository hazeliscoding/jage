#pragma once

namespace jage::managers
{
	class LogManager
	{
	public:
		LogManager() = default;
		~LogManager() = default;

		static void Initialize();
		static void Shutdown();
	};
}
