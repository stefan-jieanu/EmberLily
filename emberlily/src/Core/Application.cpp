#include "Application.hpp"

namespace ember
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

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
    window_->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

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

void Application::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));
    dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressed));
    dispatcher.Dispatch<KeyRepeatEvent>(BIND_EVENT_FN(Application::OnKeyRepeat));
    dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Application::OnKeyReleased));
    dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(Application::OnMouseButtonPressed));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(Application::OnMouseButtonReleased));
    dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Application::OnMouseMoved));
    dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(Application::OnMouseScrolled));
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    return false;
}
	
bool Application::OnWindowResize(WindowResizeEvent& e)
{
    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
        minimized_ = true;
        return false;
    }

    minimized_ = false;
    
    return false;
}

bool Application::OnKeyPressed(KeyPressedEvent& e)
{
    if (e.GetKeyCode() == KeyCode::Escape) {
        state_.isRunning = false;
    }
    return false;
}

bool Application::OnKeyRepeat(KeyRepeatEvent& e)
{
    return false;
}

bool Application::OnKeyReleased(KeyReleasedEvent& e)
{
    return false;
}

bool Application::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
    return false;
}

bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
    return false;
}

bool Application::OnMouseMoved(MouseMovedEvent& e)
{
    return false;
}

bool Application::OnMouseScrolled(MouseScrolledEvent& e)
{
    return false;
}

} // namespace ember
