cmake_minimum_required(VERSION 3.10)
project(VulkanWindow)
set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(VulkanWindow WIN32 src/main.cpp)

target_link_libraries(VulkanWindow PRIVATE 
    Vulkan::Vulkan 
    glfw 
    glm::glm
)

# Windows-specific - no console window
if(WIN32)
    if(MSVC)
        set_target_properties(VulkanWindow PROPERTIES
            LINK_FLAGS "/SUBSYSTEM:WINDOWS"
        )
    else()
        # MinGW - no console window
        set_target_properties(VulkanWindow PROPERTIES
            LINK_FLAGS "-mwindows"
        )
    endif()
endif()