#pragma once

namespace jage::managers
{
	class LogManager
	{
	public:
		LogManager() = default;
		~LogManager() = default;

		void Initialize();
		void Shutdown();
	};
}
