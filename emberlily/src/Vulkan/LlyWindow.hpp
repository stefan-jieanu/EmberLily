#pragma once

#include <string>
#include <functional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Core/Asserts.hpp"
#include "Core/Defines.hpp"
#include "Events/Event.hpp"

namespace ember
{

class LlyWindow
{
public:
    using EventCallbackFn = std::function<void(Event&)>;
    LlyWindow() {}
    LlyWindow(const std::string& title, int width, int height);
    ~LlyWindow(); 

    // Delete copy constructor and copy operator
    LlyWindow(const LlyWindow&) = delete;
    LlyWindow& operator=(const LlyWindow&) = delete;

    void update();

    inline unsigned int getWidth() const { return data_.width; }
    inline unsigned int getHeight() const { return data_.height; }
    inline int shouldWindowClose() const { return glfwWindowShouldClose(window_); }

    // Window attributes
    inline void SetEventCallback(const EventCallbackFn& callback) { data_.eventCallback = callback; }
    void setVSync(bool enabled);
    inline bool isVSync() const { return data_.VSync; }

    void* getNativeWindow() const;
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
    struct WindowData
    {
        std::string title;
        unsigned int width, height;
        bool VSync;

        EventCallbackFn eventCallback;
    } data_;

    GLFWwindow* window_;
};

} // namespace ember
