#pragma once

#include <memory>
#include <string>
#include "Asserts.hpp"
#include "Defines.hpp"
#include "Vulkan/LlyWindow.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"

namespace ember
{

class Application
{
public:
    struct ApplicationConfig
    {
        unsigned short width;
        unsigned short height;
        std::string title;
    };

    struct ApplicationState
    {
        bool isRunning, isSuspended;
        unsigned short width;
        unsigned short height;
        float deltaTime;
    };

    Application(const ApplicationConfig& config = ApplicationConfig{1280, 720, "EmberLily"});
    ~Application();

    void Run();
    // Event handling callbacks
    void OnEvent(Event& e);
    virtual bool OnWindowClose(WindowCloseEvent& e);
    virtual bool OnWindowResize(WindowResizeEvent& e);
    virtual bool OnKeyPressed(KeyPressedEvent& e);
    virtual bool OnKeyRepeat(KeyRepeatEvent& e);
    virtual bool OnKeyReleased(KeyReleasedEvent& e);
    virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
    virtual bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
    virtual bool OnMouseMoved(MouseMovedEvent& e);
    virtual bool OnMouseScrolled(MouseScrolledEvent& e);

private:
    static bool initialized;

    bool minimized_;

    ApplicationConfig config_;
    ApplicationState state_;
    std::shared_ptr<LlyWindow> window_;
};

} // namespace ember
