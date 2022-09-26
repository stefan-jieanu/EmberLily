#pragma once

#include <string>
#include <functional>

#include <GLFW/glfw3.h>
#include "Asserts.hpp"

namespace ember
{

class Window
{
public:
    // using EventCallbackFn = std::function<void(Event&)>;

    Window(const std::string& title="EmberLily", int width = 1280, int height = 720);
    ~Window(); 

    // Delete copy constructor and copy operator
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void update();

    inline unsigned int getWidth() const { return data_.width; }
    inline unsigned int getHeight() const { return data_.height; }

    // Window attributes
    // virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    void setVSync(bool enabled);
    inline bool isVSync() const { return data_.VSync; }

    void* getNativeWindow() const;

private:
    struct WindowData
    {
        std::string title;
        unsigned int width, height;
        bool VSync;
    } data_;

    GLFWwindow* window_;
};

} // namespace ember
