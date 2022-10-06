#pragma once

#include <memory>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "LlyDevice.hpp"

namespace ember
{
    
class LlyModel
{
public:
    struct Vertex {
        glm::vec2 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    LlyModel(std::shared_ptr<LlyDevice> device, const std::vector<Vertex>& vertices);
    ~LlyModel();

    // Delete copy contructors
    LlyModel(const LlyModel&) = delete;
    LlyModel& operator=(const LlyModel&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);
private:
    void createVertexBuffers(const std::vector<Vertex>& vertices);

    std::shared_ptr<LlyDevice> device_;
    VkBuffer vertexBuffer_;
    VkDeviceMemory vertexBufferMemory_;
    uint32_t vertexCount_;
};
    
} // namespace ember
