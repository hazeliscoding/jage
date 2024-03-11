#pragma once

#include <spdlog/spdlog.h>

#define JAGE_DEFAULT_LOGGER_NAME "jagelogger"

#if defined(JAGE_PLATFORM_WINDOWS)
#define JAGE_BREAK __debugbreak();
#elif defined (JAGE_PLATFORM_MAC)
#define JAGE_BREAK __builtin_debugtrap();
#else
#define JAGE_BREAK __builtin_trap();
#endif

#ifndef JAGE_CONFIG_RELEASE
#define JAGE_TRACE(...)	if (spdlog::get(JAGE_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(JAGE_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__);}
#define JAGE_DEBUG(...)	if (spdlog::get(JAGE_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(JAGE_DEFAULT_LOGGER_NAME)->debug(__VA_ARGS__);}
#define JAGE_INFO(...)		if (spdlog::get(JAGE_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(JAGE_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__);}
#define JAGE_WARN(...)		if (spdlog::get(JAGE_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(JAGE_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__);}
#define JAGE_ERROR(...)	if (spdlog::get(JAGE_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(JAGE_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__);}
#define JAGE_FATAL(...)	if (spdlog::get(JAGE_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(JAGE_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__);}
#define JAGE_ASSERT(x, msg) if ((x)) {} else { JAGE_FATAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", #x, msg, __FILE__, __LINE__); JAGE_BREAK }
#else
// Disable logging for release builds
#define JAGE_TRACE(...)	(void)0
#define JAGE_DEBUG(...)	(void)0
#define JAGE_INFO(...)		(void)0
#define JAGE_WARN(...)		(void)0
#define JAGE_ERROR(...)	(void)0
#define JAGE_FATAL(...)	(void)0
#endif