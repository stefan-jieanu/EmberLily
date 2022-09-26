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
