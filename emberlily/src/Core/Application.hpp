#pragma once

#include <memory>
#include <string>
#include "Asserts.hpp"
#include "Defines.hpp"
#include "Window.hpp"

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

private:
    static bool initialized;

    ApplicationConfig config_;
    ApplicationState state_;
    std::unique_ptr<Window> window_;
};

} // namespace ember
