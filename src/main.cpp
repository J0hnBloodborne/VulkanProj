#include "HelloTriangleApplication.h"
#include "setup.h"
#include "pipeline.h"
#include "buffers.h"
#include "commandsync.h"
#include "descriptors.h"
#include "swapchain.h"
#include "textures.h"
#include "camera.h"
#include "cube_mesh.h"
#include "window.h"
#include "sphere_mesh.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

void HelloTriangleApplication::run() {
    initWindow();
    initVulkan();
    mainloop();
    cleanup();
}

void HelloTriangleApplication::initVulkan() {
    createInstance();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createCommandPool();
    createSwapChain();
    createImageViews();
    createDepthResources();
    createRenderPass();
    createDescriptorSetLayout();
    createGraphicsPipeline();
    createFramebuffers();
    createUniformBuffers();
    createTextureImage();
    createTextureImageView();
    createTextureSampler();
    createDescriptorPool();
    createDescriptorSets();

    // Generate mesh data first
    generateSphereMesh(sphereVertices, sphereIndices);
    generateCubeMesh(platformVertices, platformIndices);

    // Create buffers using mesh data
    ::createVertexBuffer(device, physicalDevice, commandPool, graphicsQueue,
        sphereVertices, sphereVertexBuffer, sphereVertexBufferMemory);
    ::createIndexBuffer(device, physicalDevice, commandPool, graphicsQueue,
        sphereIndices, sphereIndexBuffer, sphereIndexBufferMemory);
    ::createVertexBuffer(device, physicalDevice, commandPool, graphicsQueue,
        platformVertices, platformVertexBuffer, platformVertexBufferMemory);
    ::createIndexBuffer(device, physicalDevice, commandPool, graphicsQueue,
        platformIndices, platformIndexBuffer, platformIndexBufferMemory);
    // Debug: Print buffer handles after creation
    std::cout << "sphereVertexBuffer: " << sphereVertexBuffer << std::endl;
    std::cout << "sphereIndexBuffer: " << sphereIndexBuffer << std::endl;
    std::cout << "platformVertexBuffer: " << platformVertexBuffer << std::endl;
    std::cout << "platformIndexBuffer: " << platformIndexBuffer << std::endl;

    // Now create command buffers and sync objects
    createCommandBuffers();
    createSyncObjects();
    fallingSphere = GameObject(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f));
    groundPlatform = GameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f));
    groundPlatform.modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 1.0f, 10.0f));

    // Set initial camera orbit angles to look at sphere center
    rotationX = 0.0f; // elevation
    rotationY = 0.0f; // azimuth
    // Set initial camera position using orbit
    updateCameraOrbit(orbitRadius, rotationY, rotationX);
}

void HelloTriangleApplication::cleanup() {
    // Wait for device to be idle before destroying resources
    vkDeviceWaitIdle(device);

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }
    vkDestroySampler(device, textureSampler, nullptr);
    std::cout << "Texture sampler destroyed." << std::endl;
    vkDestroyImageView(device, depthImageView, nullptr);
    std::cout << "Depth image view destroyed." << std::endl;
    vkDestroyImage(device, depthImage, nullptr);
    std::cout << "Depth image destroyed." << std::endl;
    vkFreeMemory(device, depthImageMemory, nullptr);
    std::cout << "Depth image memory freed." << std::endl;
    vkDestroyImageView(device, textureImageView, nullptr);
    std::cout << "Texture image view destroyed." << std::endl;
    vkDestroyImage(device, textureImage, nullptr);
    std::cout << "Texture image destroyed." << std::endl;
    vkFreeMemory(device, textureImageMemory, nullptr);
    std::cout << "Texture image memory freed." << std::endl;
    for (size_t i = 0; i < uniformBuffers.size(); i++) {
        vkUnmapMemory(device, uniformBuffersMemory[i]);
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }
    std::cout << "Uniform buffers destroyed." << std::endl;
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    std::cout << "Descriptor pool destroyed." << std::endl;
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
    std::cout << "Descriptor set layout destroyed." << std::endl;
    vkDestroyBuffer(device, indexBuffer, nullptr);
    std::cout << "Index buffer destroyed." << std::endl;
    vkFreeMemory(device, indexBufferMemory, nullptr);   
    std::cout << "Index buffer memory freed." << std::endl;
    vkDestroyBuffer(device, vertexBuffer, nullptr);
    std::cout << "Vertex buffer destroyed." << std::endl;
    vkFreeMemory(device, vertexBufferMemory, nullptr);
    std::cout << "Vertex buffer memory freed." << std::endl;
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
    }
    std::cout << "Semaphores destroyed." << std::endl;
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
    }
    std::cout << "Fence destroyed." << std::endl;
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }        std::cout << "Fence destroyed." << std::endl;
    for (auto commandBuffer : commandBuffers) {
        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }
    std::cout << "Command buffers freed." << std::endl;
    vkDestroyCommandPool(device, commandPool, nullptr);
    std::cout << "Command pool destroyed." << std::endl;
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    std::cout << "Framebuffers destroyed." << std::endl;
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    std::cout << "Graphics pipeline destroyed." << std::endl;
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    std::cout << "Pipeline layout destroyed." << std::endl;
    vkDestroyRenderPass(device, renderPass, nullptr);
    std::cout << "Render pass destroyed." << std::endl;
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(device, imageView, nullptr);        }        std::cout << "Image views destroyed." << std::endl;
    vkDestroySwapchainKHR(device, swapChain, nullptr);
    std::cout << "Swap chain destroyed." << std::endl;
    vkDestroySurfaceKHR(instance, surface, nullptr);
    std::cout << "Surface destroyed." << std::endl;
    vkDestroyDevice(device, nullptr);
    std::cout << "Logical device destroyed." << std::endl;
    vkDestroyInstance(instance, nullptr);
    std::cout << "Vulkan instance destroyed." << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void HelloTriangleApplication::drawFrame() {
    // 1. Wait for the previous frame to finish
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    // 2. Acquire an image from the swapchain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    // 3. If window resized or swapchain out of date, bail and recreate
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // 4. Only reset fence if we're actually submitting work
    vkResetFences(device, 1, &inFlightFences[currentFrame]);        // 5. Record command buffer for this image
    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    updateUniformBuffer(currentFrame, fallingSphere.modelMatrix);
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

    VkDeviceSize offsets[] = {0};
    // Debug: Print camera and object info
    // std::cout << "Camera position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
    // std::cout << "Camera front: " << front.x << ", " << front.y << ", " << front.z << std::endl;
    // std::cout << "Sphere position: " << fallingSphere.position.x << ", " << fallingSphere.position.y << ", " << fallingSphere.position.z << std::endl;
    // std::cout << "Platform position: " << groundPlatform.position.x << ", " << groundPlatform.position.y << ", " << groundPlatform.position.z << std::endl;
    // std::cout << "Sphere indices: " << sphereIndices.size() << std::endl;
    // std::cout << "Platform indices: " << platformIndices.size() << std::endl;
    // std::cout << "Drawing sphere..." << std::endl;
    // Draw falling sphere
    // vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, &sphereVertexBuffer, offsets);
    // vkCmdBindIndexBuffer(commandBuffers[currentFrame], sphereIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
    // updateUniformBuffer(currentFrame, fallingSphere.modelMatrix);
    // vkCmdDrawIndexed(commandBuffers[currentFrame], static_cast<uint32_t>(sphereIndices.size()), 1, 0, 0, 0);
    //std::cout << "Drawing platform..." << std::endl;
    // Draw ground platform
    // vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, &platformVertexBuffer, offsets);
    // vkCmdBindIndexBuffer(commandBuffers[currentFrame], platformIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
    // updateUniformBuffer(currentFrame, groundPlatform.modelMatrix);
    // vkCmdDrawIndexed(commandBuffers[currentFrame], static_cast<uint32_t>(platformIndices.size()), 1, 0, 0, 0);

    // 6. Submit command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    // 7. Present the image
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    // 8. Handle resize, suboptimal, or out of date swapchain
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    // 9. Advance to next frame
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    // Physics update: gravity and bounce
    double currentTime = glfwGetTime();
    float dt = static_cast<float>(currentTime - lastFrameTime);
    lastFrameTime = currentTime;
    if (isSphereFalling) {
        sphereVelocity.y += gravity * dt;
        fallingSphere.position.y += sphereVelocity.y * dt;
        if (fallingSphere.position.y - sphereRadius <= platformY) {
            fallingSphere.position.y = platformY + sphereRadius;
            if (fabs(sphereVelocity.y) > 0.1f) {
                sphereVelocity.y = -sphereVelocity.y * restitution;
            } else {
                sphereVelocity.y = 0.0f;
                isSphereFalling = false;
            }
        }
        fallingSphere.modelMatrix = glm::translate(glm::mat4(1.0f), fallingSphere.position);
    }
}

int main() {
    HelloTriangleApplication app;
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cin.get();
        return EXIT_FAILURE;
    }
    std::cin.get();
    return EXIT_SUCCESS;
}