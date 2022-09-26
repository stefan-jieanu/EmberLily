#include "Window.hpp"

namespace ember
{

static void GLFWErrorCallback(int error, const char* description)
{
    EM_LOG_ERROR("GLFW error ({0}): {1}", error, description);
}

Window::Window(const std::string& title, int width, int height)
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

    glfwMakeContextCurrent(window_);
    glfwSetWindowUserPointer(window_, &data_);
    setVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    });

    glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    });

    glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode, int action, int modes)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    });

    glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int modes)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    });

    glfwSetScrollCallback(window_, [](GLFWwindow* window, double xOffset, double yOffset)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    });

    glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double xPos, double yPos)
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    });
}

Window::~Window()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void Window::update()
{
    glfwPollEvents();
    // glfwSwapBuffers(window_);
}

void Window::setVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else 
        glfwSwapInterval(0);

    data_.VSync = enabled;
}

} // namespace ember
