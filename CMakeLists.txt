cmake_minimum_required(VERSION 3.10)
project(VulkanWindow)
set(CMAKE_CXX_STANDARD 17)

# Force static linking of everything including C++ runtime
set(CMAKE_BUILD_TYPE Release)

# Compiler-specific static flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /O2 /DNDEBUG")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
else()
    # MinGW/GCC - force completely static
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -static-libgcc -static-libstdc++")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG -static-libgcc")
endif()

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/helpers.cpp
    src/vertex.cpp
    # src/shaders.cpp   # Uncomment if/when you add this file
)

add_executable(VulkanWindow ${SOURCES})

# Link Vulkan and other dependencies as needed
find_package(Vulkan REQUIRED)
target_link_libraries(VulkanWindow Vulkan::Vulkan)

# If using GLFW, add:
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(VulkanWindow glfw)

# Windows-specific - no console window + static linking
if(WIN32)
    if(MSVC)
        set_target_properties(VulkanWindow PROPERTIES
            LINK_FLAGS "/SUBSYSTEM:WINDOWS"
        )
    else()
        # MinGW - force completely static including C++ runtime
        target_link_libraries(VulkanWindow PRIVATE
            -static-libgcc
            -static-libstdc++
        )
        set_target_properties(VulkanWindow PROPERTIES
            LINK_FLAGS "-mwindows -static -static-libgcc -static-libstdc++"
        )
    endif()
endif()