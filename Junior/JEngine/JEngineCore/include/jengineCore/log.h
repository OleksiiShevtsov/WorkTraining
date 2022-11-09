#pragma once

#include <spdlog/spdlog.h>

namespace jengine 
{
#ifdef NDEBUG

#define JLOG_INFO(...)
#define JLOG_WARN(...)
#define JLOG_ERROR(...)
#define JLOG_CRITICAL(...)

#else

#define JLOG_INFO(...)		spdlog::info(__VA_ARGS__)
#define JLOG_WARN(...)		spdlog::warn(__VA_ARGS__)
#define JLOG_ERROR(...)		spdlog::error(__VA_ARGS__)
#define JLOG_CRITICAL(...)	spdlog::critical(__VA_ARGS__)

#endif
}
