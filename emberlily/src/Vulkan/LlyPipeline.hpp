#pragma once

#include <string>
#include <vector>

#include "LlyDevice.hpp"
#include "LlyModel.hpp"

namespace ember
{
    
struct PipelineConfigInfo 
{
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};
    
class LlyPipeline
{
public:
    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    LlyPipeline() = default;
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
