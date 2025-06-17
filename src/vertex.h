#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 uv;
    
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescription;
    }
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, uv);

        return attributeDescriptions;
    }
};

const std::vector<Vertex> vertices = {
    // Base triangle (bottom face) - Counter-clockwise when viewed from above
    {{ 0.0f, -0.5f,  0.5f},  {1.0f, 0.0f, 0.0f}, {0.5f, 1.0f}},   // 0: Front center
    {{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},   // 1: Back left
    {{ 0.5f, -0.5f, -0.5f},  {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},   // 2: Back right
    // Apex
    {{ 0.0f,  0.5f,  0.0f},  {1.0f, 1.0f, 0.0f}, {0.5f, 0.5f}},   // 3: Top apex
    // Additional vertices for proper face normals
    {{ 0.0f, -0.5f,  0.5f},  {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},   // 4: Front center (different UV)
    {{-0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},   // 5: Back left (different UV)
    {{ 0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},   // 6: Back right (different UV)
};
const std::vector<uint16_t> indices = {
    // Base triangle
    0, 1, 2,
    // Side faces
    4, 3, 5,    // Front to left side
    0, 3, 1,    // Front to right side  
    1, 3, 2,    // Left to back side
    2, 3, 0     // Right to back side
};

struct UniformBufferObject {
    glm::mat4 mvp;
};