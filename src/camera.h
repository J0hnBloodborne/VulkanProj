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

inline void HelloTriangleApplication::updateUniformBuffer(uint32_t currentImage) {
    UniformBufferObject ubo{};
    ubo.mvp = glm::perspective(glm::radians(45.0f),
                            swapChainExtent.width / (float)swapChainExtent.height,
                            0.1f, 10.0f);
    ubo.mvp[1][1] *= -1;
    ubo.mvp = ubo.mvp * glm::lookAt(position, position + front, up);
    ubo.mvp = ubo.mvp * glm::lookAt(glm::vec3(2,2,2), glm::vec3(0,0,0), glm::vec3(0,1,0));
    ubo.mvp = ubo.mvp * glm::rotate(glm::mat4(1.0f), rotationX, glm::vec3(1.0f, 0.0f, 0.0f));
    ubo.mvp = ubo.mvp * glm::rotate(glm::mat4(1.0f), rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
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
    }
    float xoffset = xpos - app->lastMouseX;
    float yoffset = app->lastMouseY - ypos;
    app->lastMouseX = xpos;
    app->lastMouseY = ypos;
    float sensitivity = 0.005f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    app->rotationY += xoffset;
    app->rotationX += yoffset;
    if (app->rotationX > 1.57f)
        app->rotationX = 1.57f;
    if (app->rotationX < -1.57f)
        app->rotationX = -1.57f;
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
    position += speed * front * deltaTime;
}
inline void HelloTriangleApplication::moveBackward(float deltaTime) {
    position -= speed * front * deltaTime;
}
inline void HelloTriangleApplication::moveLeft(float deltaTime) {
    position -= glm::normalize(glm::cross(front, up)) * speed * deltaTime;
}
inline void HelloTriangleApplication::moveRight(float deltaTime) {
    position += glm::normalize(glm::cross(front, up)) * speed * deltaTime;
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
