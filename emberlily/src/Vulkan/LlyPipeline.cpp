#include "LlyPipeline.hpp"

#include <fstream>
#include "Core/Asserts.hpp"

namespace ember
{

void LlyPipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
{
    configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    // configInfo.viewport.x = 0.0f;
    // configInfo.viewport.y = 0.0f;
    // configInfo.viewport.width = static_cast<float>(width);
    // configInfo.viewport.height = static_cast<float>(height);
    // configInfo.viewport.minDepth = 0.0f;
    // configInfo.viewport.maxDepth = 1.0f;

    // configInfo.scissor.offset = {0, 0};
    // configInfo.scissor.extent = {width, height};

    configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    configInfo.viewportInfo.viewportCount = 1;
    configInfo.viewportInfo.pViewports = nullptr;
    configInfo.viewportInfo.scissorCount = 1;
    configInfo.viewportInfo.pScissors = nullptr;

    configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    configInfo.rasterizationInfo.lineWidth = 1.0f;
    configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
    configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
    configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
    configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

    configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
    configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
    configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
    configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

    configInfo.colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
    configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
    configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
    configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
    configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

    configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
    configInfo.colorBlendInfo.attachmentCount = 1;
    configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
    configInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
    configInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

    configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
    configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
    configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    configInfo.depthStencilInfo.front = {};  // Optional
    configInfo.depthStencilInfo.back = {};   // Optional

    configInfo.dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStateEnables.data();
    configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStateEnables.size());
    configInfo.dynamicStateInfo.flags = 0;
}

LlyPipeline::LlyPipeline(
    std::shared_ptr<LlyDevice> device, 
    const PipelineConfigInfo& configInfo,
    const std::string& vertFilepath,
    const std::string& fragFilepath)
    : device_(device)
{
    createGraphicsPipeline(configInfo, vertFilepath, fragFilepath);
}

LlyPipeline::~LlyPipeline()
{
    vkDestroyShaderModule(device_->device(), vertShaderModule_, nullptr);
    vkDestroyShaderModule(device_->device(), fragShaderModule_, nullptr);

    vkDestroyPipeline(device_->device(), graphicsPipeline_, nullptr);
}

void LlyPipeline::bind(VkCommandBuffer commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline_);
}

std::vector<char> LlyPipeline::readFile(const std::string& filepath)
{
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};

    EM_CORE_ASSERT(file.is_open(), "Could not open shader file!");

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

void LlyPipeline::createGraphicsPipeline(
    const PipelineConfigInfo& configInfo,
    const std::string& vertFilepath,
    const std::string& fragFilepath)
{
    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    createShaderModule(vertCode, &vertShaderModule_);
    createShaderModule(fragCode, &fragShaderModule_);

    VkPipelineShaderStageCreateInfo shaderStages[2];
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertShaderModule_;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragShaderModule_;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    auto bindingDesctriptions = LlyModel::Vertex::getBindingDescriptions();
    auto atttributeDescriptions = LlyModel::Vertex::getAttributeDescriptions();
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(atttributeDescriptions.size());
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDesctriptions.size());;
    vertexInputInfo.pVertexAttributeDescriptions = atttributeDescriptions.data();
    vertexInputInfo.pVertexBindingDescriptions = bindingDesctriptions.data();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
    pipelineInfo.pViewportState = &configInfo.viewportInfo;
    pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
    pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
    pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
    pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
    pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;

    pipelineInfo.layout = configInfo.pipelineLayout;
    pipelineInfo.renderPass = configInfo.renderPass;
    pipelineInfo.subpass = configInfo.subpass;

    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkResult ok = vkCreateGraphicsPipelines(device_->device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline_);
    EM_CORE_ASSERT(ok == VK_SUCCESS, "Could not create graphics pipeline!");
}

void LlyPipeline::createShaderModule(
    const std::vector<char>& code,
    VkShaderModule* shaderModule)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkResult ok = vkCreateShaderModule(device_->device(), &createInfo, nullptr, shaderModule);
    EM_CORE_ASSERT(ok == VK_SUCCESS, "Failed to create shader module");
}

} // namespace ember
