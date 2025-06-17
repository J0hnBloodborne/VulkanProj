#pragma once
#include "HelloTriangleApplication.h"
#include <vector>
#include "vertex.h" // Make sure Vertex is visible

inline void generateSphereMesh(
    std::vector<Vertex>& vertices,
    std::vector<uint32_t>& indices,
    float radius = 1.0f,
    unsigned int sectorCount = 32,
    unsigned int stackCount = 16
) 
{
    vertices.clear();
    indices.clear();

    for (unsigned int i = 0; i <= stackCount; ++i) {
        float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stackCount; // from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle); // r * cos(u)
        float z = radius * sinf(stackAngle);  // r * sin(u)

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * glm::pi<float>() / sectorCount; // from 0 to 2pi

            float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            glm::vec3 pos = glm::vec3(x, y, z);
            glm::vec3 normal = glm::normalize(pos);

            // Use normal as color for visualization, mapped to [0,1]
            glm::vec3 color = normal * 0.5f + 0.5f;

            float u = (float)j / sectorCount;
            float v = (float)i / stackCount;
            glm::vec2 uv = glm::vec2(u, v);

            vertices.push_back({pos, color, uv});
        }
    }

    // Indices
    for (unsigned int i = 0; i < stackCount; ++i) {
        unsigned int k1 = i * (sectorCount + 1);     // beginning of current stack
        unsigned int k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}