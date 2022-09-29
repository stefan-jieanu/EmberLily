#include "Application.hpp"

namespace ember
{

bool Application::initialized = false;

Application::Application(const ApplicationConfig& config)
    : config_(config)
{
    // First init the logger
    Logger::Init();

    if (Application::initialized) {
        EM_LOG_ERROR("Application already initialized");
        return;
    }

    state_.isRunning = true;
    state_.isSuspended = false;

    window_ = std::make_unique<Window>(config_.title, config_.width, config_.height);

    Application::initialized = true;
}

Application::~Application()
{

}

void Application::Run()
{
    while(state_.isRunning)
    {
        state_.isRunning = !window_->shouldWindowClose();

        window_->update();
    }

    // Explicitly set it to false here too in case it's set 
    // in another part of the application 
    state_.isRunning = false;
}

} // namespace ember
