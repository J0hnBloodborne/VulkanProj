#include "HelloTriangleApplication.h"
#include "pipeline.h"
#include "buffers.h"
#include "commandsync.h"
#include "descriptors.h"
#include "swapchain.h"
#include "textures.h"
#include "camera.h"
#include "window.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

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
    createVertexBuffer();
    createIndexBuffer();
    createUniformBuffers();
    createTextureImage();
    createTextureImageView();
    createTextureSampler();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();
    createSyncObjects();
}

void HelloTriangleApplication::cleanup() {
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
    updateUniformBuffer(currentFrame);
    recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

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
}

void HelloTriangleApplication::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport(validationLayers)) {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
    std::cout << "Vulkan instance created successfully." << std::endl;
}

void HelloTriangleApplication::createSurface() {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
    std::cout << "Window surface created successfully." << std::endl;
}

void HelloTriangleApplication::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) {
        if (isDeviceSuitable(device, surface)) {
            int score = rateDeviceSuitability(device);
            candidates.insert(std::make_pair(score, device));
        }
    }

    if (!candidates.empty() && candidates.rbegin()->first > 0) {
        physicalDevice = candidates.rbegin()->second;
        std::cout << "Physical device selected with score: " << candidates.rbegin()->first << std::endl;
    } else {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

void HelloTriangleApplication::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.geometryShader = VK_TRUE; // Enable geometry shader
    deviceFeatures.samplerAnisotropy = VK_TRUE; // Enable anisotropic filtering

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    } else {
        std::cout << "Logical device created successfully." << std::endl;
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    std::cout << "Graphics and present queues retrieved successfully." << std::endl;
    
    graphicsFamilyIndex = indices.graphicsFamily.value();
}

void HelloTriangleApplication::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

int HelloTriangleApplication::rateDeviceSuitability(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 0;

    // Favor discrete GPUs
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    // Texture size = more points
    score += deviceProperties.limits.maxImageDimension2D;

    // No geometry shader, no dice
    if (!deviceFeatures.geometryShader) {
        return 0;
    }

    return score;
}

VkExtent2D HelloTriangleApplication::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }
}

void HelloTriangleApplication::cleanupSwapChain() {
    vkDestroyImageView(device, depthImageView, nullptr);
    vkDestroyImage(device, depthImage, nullptr);
    vkFreeMemory(device, depthImageMemory, nullptr);
    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
    }
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        vkDestroyImageView(device, swapChainImageViews[i], nullptr);
    }
    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void HelloTriangleApplication::recreateSwapChain() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(device);

    cleanupSwapChain();
    createSwapChain();
    createImageViews();
    createDepthResources();
    createFramebuffers();
}

int main() {
    HelloTriangleApplication app;
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}