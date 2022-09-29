#pragma once

#include <string>
#include <functional>

#include <GLFW/glfw3.h>
#include "Asserts.hpp"
#include "Defines.hpp"

namespace ember
{

class Window
{
public:
    // using EventCallbackFn = std::function<void(Event&)>;
    Window() {}
    Window(const std::string& title, int width, int height);
    ~Window(); 

    // Delete copy constructor and copy operator
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void update();

    inline unsigned int getWidth() const { return data_.width; }
    inline unsigned int getHeight() const { return data_.height; }
    inline int shouldWindowClose() const { return glfwWindowShouldClose(window_); }

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
