#pragma once

#include <memory>
#include "LlyDevice.hpp"

namespace ember
{
    
class LlyModel
{
public:
    LlyModel();
    ~LlyModel();

    // Delete copy contructors
    LlyModel(const LlyModel&) = delete;
    LlyModel& operator=(const LlyModel&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);
private:
    std::shared_ptr<LlyDevice> device_;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;
};
    
} // namespace ember
