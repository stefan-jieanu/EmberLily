#include "Logger.hpp"

namespace ember
{

std::shared_ptr<spdlog::logger> Logger::logger;
std::shared_ptr<spdlog::logger> Logger::coreLogger;
bool Logger::initialized = false;

void Logger::Init()
{
    if (Logger::initialized) {
        EM_LOG_WARN("Logger already initialized");
        return;
    }

    spdlog::set_pattern("%^[%n]: [%s, %#]: [%l]: %v%$");

    Logger::logger = spdlog::stdout_color_mt("App");
    Logger::logger->set_level(spdlog::level::trace);

    Logger::coreLogger = spdlog::stdout_color_mt("EmberLily");
    Logger::coreLogger->set_level(spdlog::level::trace);

    Logger::initialized = true;
    EM_LOG_INFO("Initialized Log");
}

} // namespace ember