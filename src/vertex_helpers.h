#pragma once
#include <vulkan/vulkan.h>
#include <array>
#include <vector>

struct Vertex;

// Vertex helper function declarations
namespace VertexHelpers {
    VkVertexInputBindingDescription getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
}
