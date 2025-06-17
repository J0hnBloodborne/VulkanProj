#pragma once
#include "vertex.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>
#include <optional>
#include <chrono>
#include <string>
#include <set>
#include <stdexcept>
#include <cstdint>
#include <map>
#include <limits>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "GameObject.h"


const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public:
    void run();
    void initVulkan();
    void cleanup();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void createCommandPool();
    void createCommandBuffers();
    void recordCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void createSyncObjects();
    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();
    void createSwapChain();
    void createImageViews();
    void createDepthResources();
    void cleanupSwapChain();
    void recreateSwapChain();
    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();
    void updateUniformBuffer(uint32_t currentImage, const glm::mat4& modelMatrix);
    void mainloop();
    void initWindow();
    void drawFrame();
    void createInstance();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void setupDebugMessenger();
    int rateDeviceSuitability(VkPhysicalDevice device);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void turn(float xoffset, float yoffset);
    // ...add any other public methods you use...

    // Member variables
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
    VkQueue presentQueue;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    uint32_t graphicsFamilyIndex;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    size_t currentFrame = 0;
    bool framebufferResized = false;
    double lastMouseX = 400.0;
    double lastMouseY = 300.0;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    bool firstMouse = true;
    bool mouseButtonPressed = false;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;
    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    glm::vec3 position = glm::vec3(0.0f, 2.0f, 15.0f); // Start camera further away
    glm::vec3 front = glm::vec3(0.0f, -0.2f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 5.0f;
    float orbitRadius = 30.0f; // Camera starts further away and can be changed
    GameObject fallingSphere;
    GameObject groundPlatform;

    // Sphere buffers
    std::vector<Vertex> sphereVertices;
    std::vector<uint32_t> sphereIndices;
    VkBuffer sphereVertexBuffer;
    VkDeviceMemory sphereVertexBufferMemory;
    VkBuffer sphereIndexBuffer;
    VkDeviceMemory sphereIndexBufferMemory;

    // Platform buffers
    std::vector<Vertex> platformVertices;
    std::vector<uint32_t> platformIndices;
    VkBuffer platformVertexBuffer;
    VkDeviceMemory platformVertexBufferMemory;
    VkBuffer platformIndexBuffer;
    VkDeviceMemory platformIndexBufferMemory;

    // Camera orbit around the ball
    inline void updateCameraOrbit(float radius, float azimuth, float elevation) {
        // Spherical coordinates
        float x = fallingSphere.position.x + radius * cos(elevation) * sin(azimuth);
        float y = fallingSphere.position.y + radius * sin(elevation);
        float z = fallingSphere.position.z + radius * cos(elevation) * cos(azimuth);
        position = glm::vec3(x, y, z);
        front = glm::normalize(fallingSphere.position - position);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    // ...add any other member variables you use...
};
