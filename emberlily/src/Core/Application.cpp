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

    window_ = std::make_shared<LlyWindow>(config_.title, config_.width, config_.height);
    window_->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

    Application::initialized = true;

    device_ = std::make_shared<LlyDevice>(window_);
    swapChain_ = std::make_unique<LlySwapChain>(device_, window_->getExtent());

    loadModels();
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

Application::~Application()
{
    vkDestroyPipelineLayout(device_->device(), pipelineLayout_, nullptr);
}

void Application::Run()
{
    while(state_.isRunning)
    {
        state_.isRunning = !window_->shouldWindowClose();

        window_->update();
        drawFrame();
    }

    vkDeviceWaitIdle(device_->device());

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
    EM_LOG_DEBUG("New window size: {},{}", e.GetWidth(), e.GetHeight());
    
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

void Application::loadModels()
{
    std::vector<LlyModel::Vertex> vertices {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    model_ = std::make_unique<LlyModel>(device_, vertices);
}

void Application::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    VkResult ok = vkCreatePipelineLayout(device_->device(), &pipelineLayoutInfo, nullptr, &pipelineLayout_);
    EM_CORE_ASSERT(ok == VK_SUCCESS, "Could not create pipeline layout!");
}

void Application::createPipeline()
{
    PipelineConfigInfo configInfo{};

    LlyPipeline::defaultPipelineConfigInfo(swapChain_->width(), swapChain_->height(), configInfo);

    configInfo.renderPass = swapChain_->getRenderPass();
    configInfo.pipelineLayout = pipelineLayout_;
    pipeline_ = std::make_shared<LlyPipeline>(
        device_,
        configInfo,
        "../../../../shaders/bin/simple_shader.vert.spv", 
        "../../../../shaders/bin/simple_shader.frag.spv");
}

void Application::createCommandBuffers()
{
    commandBuffers_.resize(swapChain_->imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device_->getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

    VkResult ok = vkAllocateCommandBuffers(device_->device(), &allocInfo, commandBuffers_.data());
    EM_CORE_ASSERT(ok == VK_SUCCESS, "Couldn't create command buffer!");
}

void Application::drawFrame()
{
    uint32_t imageIndex;
    auto result = swapChain_->acquireNextImage(&imageIndex);
    EM_CORE_ASSERT((result != VK_SUCCESS || result != VK_SUBOPTIMAL_KHR), "Not good aquire next image from swap chain");

    result = swapChain_->submitCommandBuffers(&commandBuffers_[imageIndex], &imageIndex);
    EM_CORE_ASSERT(result == VK_SUCCESS, "Failed to sbumit command buffer");
}

void Application::recreateSwapChain()
{
    auto extent = window_->getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = window_->getExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(device_->device());
    swapChain_ = std::make_unique<LlySwapChain>(device_->device(), extent);
    createPipeline();
}

void Application::recordCommandBuffer(int imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkResult ok = vkBeginCommandBuffer(commandBuffers_[imageIndex], &beginInfo);
    EM_CORE_ASSERT(ok == VK_SUCCESS, "Failed to begin recording command buffer!");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain_->getRenderPass();
    renderPassInfo.framebuffer = swapChain_->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain_->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers_[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    pipeline_->bind(commandBuffers_[imageIndex]);
    // vkCmdDraw(commandBuffers_[i], 3, 1, 0, 0);
    model_->bind(commandBuffers_[imageIndex]);
    model_->draw(commandBuffers_[imageIndex]);

    vkCmdEndRenderPass(commandBuffers_[imageIndex]);

    ok = vkEndCommandBuffer(commandBuffers_[imageIndex]);
    EM_CORE_ASSERT(ok == VK_SUCCESS, "Failed to end recording command buffer!");
}

} // namespace ember
