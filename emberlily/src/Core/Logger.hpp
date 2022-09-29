#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_DEBUG_ON
#define SPDLOG_TRACE_ON

#include <functional>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

#include "Defines.hpp"

// Undefine these spdlog defines as they are only necessary as a tweak before
// including the library No need to have them defined to clutter the rest of the
// program
#undef SPDLOG_ACTIVE_LEVEL
#undef SPDLOG_DEBUG_ON
#undef SPDLOG_TRACE_ON

namespace ember
{

class Logger
{
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetLogger()
    {
        return Logger::logger;
    }

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
    {
        return Logger::coreLogger;
    }


private:
    static std::shared_ptr<spdlog::logger> logger;
    static std::shared_ptr<spdlog::logger> coreLogger;
    static bool initialized;
};

} // namespace ember

// Client log macros
#define LOG_TRACE(...)                                                         \
    SPDLOG_LOGGER_TRACE(::ember::Logger::GetLogger(), __VA_ARGS__)
#define LOG_DEBUG(...)                                                         \
    SPDLOG_LOGGER_DEBUG(::ember::Logger::GetLogger(), __VA_ARGS__)
#define LOG_INFO(...)                                                          \
    SPDLOG_LOGGER_INFO(::ember::Logger::GetLogger(), __VA_ARGS__)
#define LOG_WARN(...)                                                          \
    SPDLOG_LOGGER_WARN(::ember::Logger::GetLogger(), __VA_ARGS__)
#define LOG_ERROR(...)                                                         \
    SPDLOG_LOGGER_ERROR(::ember::Logger::GetLogger(), __VA_ARGS__)

// EmberLily log macros
#define EM_LOG_TRACE(...)                                                         \
    SPDLOG_LOGGER_TRACE(::ember::Logger::GetCoreLogger(), __VA_ARGS__)
#define EM_LOG_DEBUG(...)                                                         \
    SPDLOG_LOGGER_DEBUG(::ember::Logger::GetCoreLogger(), __VA_ARGS__)
#define EM_LOG_INFO(...)                                                          \
    SPDLOG_LOGGER_INFO(::ember::Logger::GetCoreLogger(), __VA_ARGS__)
#define EM_LOG_WARN(...)                                                          \
    SPDLOG_LOGGER_WARN(::ember::Logger::GetCoreLogger(), __VA_ARGS__)
#define EM_LOG_ERROR(...)                                                         \
    SPDLOG_LOGGER_ERROR(::ember::Logger::GetCoreLogger(), __VA_ARGS__)