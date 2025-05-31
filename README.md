# Vulkan Pyramid Sample

This project demonstrates a minimal but robust Vulkan-powered 3D pyramid with per-face texturing and proper depth buffering. The code is organized to handle swapchain recreation, validation layers, and best practices for modern Vulkan.

---

## Features

- **3D Pyramid Rendering:** Renders a pyramid using indexed vertex buffers.
- **Texturing:** Each face of the pyramid is textured using UV mapping.
- **Depth Buffer:** Implements a depth buffer for correct 3D rendering and Z-ordering.
- **Validation Layers:** Enabled for error checking and debugging.
- **Swapchain Recreation:** Handles window resizing and surface changes without crashing.
- **Cross-Platform:** Uses GLFW for window/context creation.

---

## Setup & Build

### Prerequisites

- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- [vcpkg](https://github.com/Microsoft/vcpkg) package manager
- CMake 3.10+
- C++17 compatible compiler (MSVC, GCC, or Clang)

### Build Instructions

1. **Install dependencies with vcpkg:**
   ```bash
   vcpkg install glfw3 vulkan glm stb
   ```

2. **Configure and build:**
   ```bash
   cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="vcpkg/scripts/buildsystems/vcpkg.cmake"
   cmake --build build --config Release
   ```

3. **Create distribution package:**
   ```bash
   ./build_release_new.ps1
   ```

This creates a `VulkanApp_Release/` folder with the executable, required DLLs, shaders, and textures ready for distribution.

For some reason powershell doesn't copy textures or shaders, so just copy those over yourself.
texture.jpg and shaders/frag.spv,vert.spv

---

## Running The Sample

Run the executable from the distribution package:

```bash
cd VulkanApp_Release
./VulkanWindow.exe
```

**Controls:**
- Hold **Left Mouse Button** + Move Mouse: Rotate the tetrahedron
- Close window: Exit application

**Requirements:**
- Windows 10 or later
- Graphics card with Vulkan support  
- Updated graphics drivers
- The executable includes all required DLLs

---

## Key Implementation Details

### Swapchain & Depth Buffer

- The swapchain is created **without** `VK_IMAGE_USAGE_STORAGE_BIT`. Only `VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT` and optionally transfer bits are used.
- The depth buffer format is selected at runtime using a helper that queries supported formats (including stencil support if available).
- All depth resources (image, view, memory) are recreated with the swapchain.

### Validation Layers

- The Khronos validation layer is enabled by default unless built in release mode.
- All Vulkan API errors, warnings, and performance suggestions will print to `stderr`.

### Command Pool & Buffers

- Command pool and command buffers are **destroyed and recreated** as part of swapchain recreation, ensuring no invalid handle usage.

### Modern C++ Practices

- Uses `std::vector`, `std::array`, and RAII where possible.
- Error handling is done via exceptions.

---

## Troubleshooting

- **Validation Layer Errors:** Read the stderr output. Most issues are flagged with clear messages.
- **VK_IMAGE_USAGE_STORAGE_BIT Errors:** This sample does not use it on swapchain images. If you see these errors, clean and rebuild.
- **Invalid VkCommandPool Errors:** Always recreate command pool and buffers after swapchain recreation.
- **Black Screen or Flicker:** Ensure depth buffer is created, attached, and cleared each frame.

---

## License

MIT. Use it, learn from it, do whatever.

---

## Contact

For questions, open an issue or reach out via GitHub.