cmake_minimum_required(VERSION 3.10)
project(VulkanWindow)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Release)

# Compiler-specific static flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /O2 /DNDEBUG")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -static-libgcc -static-libstdc++")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG -static-libgcc")
endif()

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Add include directory for src/
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(SOURCES
    src/main.cpp
    src/stb_image_impl.cpp
)

add_executable(VulkanWindow ${SOURCES})

target_link_libraries(VulkanWindow PRIVATE Vulkan::Vulkan glfw)

# Windows-specific - no console window + static linking
if(WIN32)
    if(MSVC)
        set_target_properties(VulkanWindow PROPERTIES
            LINK_FLAGS "/SUBSYSTEM:WINDOWS"
        )
    else()
        target_link_libraries(VulkanWindow PRIVATE
            -static-libgcc
            -static-libstdc++
        )
        set_target_properties(VulkanWindow PROPERTIES
            LINK_FLAGS "-mwindows -static -static-libgcc -static-libstdc++"
        )
    endif()
endif()