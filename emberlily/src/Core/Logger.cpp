#include "Logger.hpp"

namespace ember
{

std::shared_ptr<spdlog::logger> Logger::logger;
std::shared_ptr<spdlog::logger> Logger::coreLogger;

void Logger::Init()
{
    spdlog::set_pattern("%^[%n]: [%s, %#]: [%l]: %v%$");

    Logger::logger = spdlog::stdout_color_mt("App");
    Logger::logger->set_level(spdlog::level::trace);

    Logger::coreLogger = spdlog::stdout_color_mt("EmberLily");
    Logger::coreLogger->set_level(spdlog::level::trace);

    EM_LOG_INFO("Initialized Log");
}

} // namespace ember