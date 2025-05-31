cmake_minimum_required(VERSION 3.10)
project(VulkanWindow)
set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)

add_executable(VulkanWindow src/main.cpp)
target_link_libraries(VulkanWindow PRIVATE Vulkan::Vulkan glfw glm::glm)