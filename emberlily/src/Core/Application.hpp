#pragma once

#include <memory>
#include <string>

#include "Asserts.hpp"
#include "Defines.hpp"
#include "Vulkan/LlyWindow.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Vulkan/LlyDevice.hpp"
#include "Vulkan/LlyPipeline.hpp"
#include "Vulkan/LlySwapChain.hpp"
#include "GameObject.hpp"

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

    void loadGameObjects();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void recreateSwapChain();
    void recordCommandBuffer(int imageIndex);
    void renderGameObjects(VkCommandBuffer commandBuffer);

    bool minimized_;

    ApplicationConfig config_;
    ApplicationState state_;
    std::shared_ptr<LlyWindow> window_;
    std::shared_ptr<LlyDevice> device_;
    std::unique_ptr<LlySwapChain> swapChain_;
    std::shared_ptr<LlyPipeline> pipeline_;
    VkPipelineLayout pipelineLayout_;
    std::vector<VkCommandBuffer> commandBuffers_;
    std::vector<GameObject> gameObjects_;
};

} // namespace ember
