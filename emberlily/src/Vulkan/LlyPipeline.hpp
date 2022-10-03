#pragma once

#include <string>
#include <vector>

#include "LlyDevice.hpp"

namespace ember
{
    
struct PipelineConfigInfo 
{
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};
    
class LlyPipeline
{
public:
    static void defaultPipelineConfigInfo(uint32_t width, uint32_t height, PipelineConfigInfo& configInfo);

    LlyPipeline(
        std::shared_ptr<LlyDevice> device,
        const PipelineConfigInfo& configInfo,
        const std::string& vertFilepath,
        const std::string& fragFilepath);
    ~LlyPipeline();

    // Delete copy contructors
    LlyPipeline(const LlyPipeline&) = delete;
    LlyPipeline& operator=(const LlyPipeline&) = delete;

    void bind(VkCommandBuffer commandBuffer);

private:
    static std::vector<char> readFile(const std::string& filepath);
    void createGraphicsPipeline(
        const PipelineConfigInfo& configInfo,
        const std::string& vertFilepath,
        const std::string& fragFilepath);

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    std::shared_ptr<LlyDevice> device_;
    VkPipeline graphicsPipeline_;
    VkShaderModule vertShaderModule_;
    VkShaderModule fragShaderModule_;
};

} // namespace ember
