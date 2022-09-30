#pragma once

#include <string>
#include <vector>

#include "LlyDevice.hpp"

namespace ember
{
    
struct PipelineConfigInfo{};
    
class LlyPipeline
{
public:
    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    LlyPipeline(
        LlyDevice& device,
        const PipelineConfigInfo& configInfo,
        const std::string& vertFilepath,
        const std::string& fragFilepath);
    ~LlyPipeline();

    // Delete copy contructors
    LlyPipeline(const LlyPipeline&) = delete;
    void operator=(const LlyPipeline&) = delete;

private:
    static std::vector<char> readFile(const std::string& filepath);
    void createGraphicsPipeline(
        const PipelineConfigInfo& configInfo,
        const std::string& vertFilepath,
        const std::string& fragFilepath);

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    LlyDevice& device_;
    VkPipeline graphicsPipeline_;
    VkShaderModule vertShaderModule_;
    VkShaderModule fragShaderModule_;
};

} // namespace ember
