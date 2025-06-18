#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <cstring>
#include <iostream>
#include "HelloTriangleApplication.h"
#include "helpers.h"
#include "vertex.h"
#include "vertex_helpers.h"

inline void HelloTriangleApplication::updateCameraOrbit(float radius, float azimuth, float elevation) {
        // Spherical coordinates
        float x = radius * cos(elevation) * sin(azimuth);
        float y = radius * sin(elevation);
        float z = radius * cos(elevation) * cos(azimuth);
        position = glm::vec3(x, y, z);
        // front remains as set by mouse/orbit logic, do not force look-at sphere
    }
    
inline void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage, const glm::mat4& modelMatrix) {
    UniformBufferObject ubo{};
    ubo.mvp = glm::perspective(glm::radians(45.0f),
    (float)swapChainExtent.width / (float)swapChainExtent.height,
                            0.1f, 100.0f);
    ubo.mvp[1][1] *= -1;
    ubo.mvp = ubo.mvp * glm::lookAt(position, position + front, up);
    ubo.mvp = ubo.mvp * modelMatrix;
    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

inline void HelloTriangleApplication::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    if (!app->mouseButtonPressed) {
        app->firstMouse = true;
        return;
    }
    if (app->firstMouse) {
        app->lastMouseX = xpos;
        app->lastMouseY = ypos;
        app->firstMouse = false;
        return; // Don't update camera on the first click
    }
    float xoffset = xpos - app->lastMouseX;
    float yoffset = app->lastMouseY - ypos;
    app->lastMouseX = xpos;
    app->lastMouseY = ypos;
    float sensitivity = 0.005f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    app->rotationY += xoffset;   // azimuth
    app->rotationX += yoffset;   // elevation
    if (app->rotationX > 1.57f)
        app->rotationX = 1.57f;
    if (app->rotationX < -1.57f)
        app->rotationX = -1.57f;
    // Only update orbit angles, do not reset radius or position
    app->updateCameraOrbit(app->orbitRadius, app->rotationY, app->rotationX);
}

inline void HelloTriangleApplication::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            app->mouseButtonPressed = true;
            app->firstMouse = true;
        } else if (action == GLFW_RELEASE) {
            app->mouseButtonPressed = false;
        }
    }
}

// WASD and mouse look for HelloTriangleApplication
inline void HelloTriangleApplication::moveForward(float deltaTime) {
    orbitRadius -= speed * deltaTime;
    if (orbitRadius < 1.0f) orbitRadius = 1.0f;
    updateCameraOrbit(orbitRadius, rotationY, rotationX);
}
inline void HelloTriangleApplication::moveBackward(float deltaTime) {
    orbitRadius += speed * deltaTime;
    updateCameraOrbit(orbitRadius, rotationY, rotationX);
}
inline void HelloTriangleApplication::moveLeft(float deltaTime) {
    rotationY -= 1.5f * deltaTime;
    updateCameraOrbit(orbitRadius, rotationY, rotationX);
}
inline void HelloTriangleApplication::moveRight(float deltaTime) {
    rotationY += 1.5f * deltaTime;
    updateCameraOrbit(orbitRadius, rotationY, rotationX);
}
inline void HelloTriangleApplication::turn(float xoffset, float yoffset) {
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw   += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
}