#pragma once

#include "vertex.h"
#include <vector>
#include <glm/glm.hpp>

inline void generateCubeMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
    vertices = {
        // pos                color                 uv
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // Red
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    };
    indices = {
        0,1,2, 2,3,0, // back
        4,5,6, 6,7,4, // front
        4,5,1, 1,0,4, // bottom
        7,6,2, 2,3,7, // top
        4,0,3, 3,7,4, // left
        5,1,2, 2,6,5  // right
    };
}
