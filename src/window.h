#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include "HelloTriangleApplication.h"

inline void processInput(GLFWwindow* window, HelloTriangleApplication* app, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) app->moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) app->moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) app->moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) app->moveRight(deltaTime);
}

inline void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;
    static float lastX = 0.0f;
    static float lastY = 0.0f;
    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }
    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;
    lastX = (float)xpos;
    lastY = (float)ypos;
    HelloTriangleApplication* app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    if (app) app->turn(xoffset, yoffset);
}

inline void HelloTriangleApplication::initWindow() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(800, 600, "Vulkan Flex", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
}

inline void HelloTriangleApplication::mainloop() {
    float lastFrameTime = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrameTime = (float)glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        glfwPollEvents();
        processInput(window, this, deltaTime);
        drawFrame();
    }
}

inline void HelloTriangleApplication::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}
