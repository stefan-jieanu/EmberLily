#include "LlyPipeline.hpp"

#include <fstream>
#include "Core/Asserts.hpp"

namespace ember
{

PipelineConfigInfo LlyPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
{
    PipelineConfigInfo configInfo{};
    return configInfo;
}

LlyPipeline::LlyPipeline(
    LlyDevice& device, 
    const PipelineConfigInfo& configInfo,
    const std::string& vertFilepath,
    const std::string& fragFilepath)
    : device_(device)
{
    createGraphicsPipeline(configInfo, vertFilepath, fragFilepath);
}

LlyPipeline::~LlyPipeline()
{

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
}

void LlyPipeline::createShaderModule(
    const std::vector<char>& code,
    VkShaderModule* shaderModule)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkResult ok = vkCreateShaderModule(device_.device(), &createInfo, nullptr, shaderModule);
    EM_CORE_ASSERT(ok == VK_SUCCESS, "Failed to create shader module");
}

} // namespace ember
