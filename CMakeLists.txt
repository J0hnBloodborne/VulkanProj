cmake_minimum_required(VERSION 3.10)
project(VulkanWindow)
set(CMAKE_CXX_STANDARD 17)

# Default to Debug build for development
set(CMAKE_BUILD_TYPE Debug)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(SOURCES
    src/main.cpp
    src/stb_image_impl.cpp
    # Add other .cpp files as needed
)

add_executable(VulkanWindow ${SOURCES})

target_link_libraries(VulkanWindow PRIVATE Vulkan::Vulkan glfw)