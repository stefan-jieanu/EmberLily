#include "LlyWindow.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"

namespace ember
{

static void GLFWErrorCallback(int error, const char* description)
{
    EM_LOG_ERROR("GLFW error ({0}): {1}", error, description);
}

LlyWindow::LlyWindow(const std::string& title, int width, int height)
{
    data_.title = title;
    data_.width = width;
    data_.height = height;

    EM_LOG_INFO("Creating window {0} ({1}, {2})", data_.title, data_.width, data_.height);

    int success = glfwInit();
    EM_CORE_ASSERT(success, "Could not initialize GLFW!");

    glfwSetErrorCallback(GLFWErrorCallback);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(
        (int)data_.width,
        (int)data_.height,
        data_.title.c_str(), 
        nullptr, nullptr);

    // glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, &data_);
    // setVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        data.width = width;
        data.height = height;

        WindowResizeEvent event(width, height);
        data.eventCallback(event);
    });

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        WindowCloseEvent event;
        data.eventCallback(event);
    });

    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int modes)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action)
        {
        case GLFW_PRESS:
        {
            KeyPressedEvent event((KeyCode)key);
            data.eventCallback(event);
            break;
        }
        case GLFW_RELEASE:
        {
            KeyReleasedEvent event((KeyCode)key);
            data.eventCallback(event);
            break;
        }
        case GLFW_REPEAT:
        {
            KeyRepeatEvent event((KeyCode)key);
            data.eventCallback(event);
            break;
        }
        }
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int modes)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action)
        {
        case GLFW_PRESS:
        {
            MouseButtonPressedEvent event(button);
            data.eventCallback(event);
            break;
        }
        case GLFW_RELEASE:
        {
            MouseButtonReleasedEvent event(button);
            data.eventCallback(event);
            break;
        }
        }
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.eventCallback(event);
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xPos, double yPos)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xPos, (float)yPos);
        data.eventCallback(event);
    });
}

LlyWindow::~LlyWindow()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void LlyWindow::update()
{
    glfwPollEvents();
    // glfwSwapBuffers(window_);
}

void LlyWindow::setVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else 
        glfwSwapInterval(0);

    data_.VSync = enabled;
}

void LlyWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
    VkResult ok = glfwCreateWindowSurface(instance, window_, nullptr, surface);

    EM_CORE_ASSERT(
        ok == VK_SUCCESS,
        "Unable to create vulkan surface for GLFW window!"
    );
}

} // namespace ember
