# package_release.ps1 - Create distribution package with DLLs

Write-Host "Creating Vulkan distribution package..." -ForegroundColor Green

# Build (don't touch build folder)
Write-Host "Building executable..."
cmake --build build --config Release

# Create release directory
Write-Host "Creating distribution package..."
if (!(Test-Path "VulkanApp_Release")) { New-Item -ItemType Directory -Path "VulkanApp_Release" }
if (!(Test-Path "VulkanApp_Release/shaders")) { New-Item -ItemType Directory -Path "VulkanApp_Release/shaders" }

# Copy executable
Copy-Item "build/VulkanWindow.exe" "VulkanApp_Release/" -Force

# Copy DLLs from vcpkg
$vcpkgBin = "vcpkg_installed/x64-windows/bin"
if (Test-Path "$vcpkgBin/glfw3.dll") { 
    Copy-Item "$vcpkgBin/glfw3.dll" "VulkanApp_Release/" -Force 
    Write-Host "Copied glfw3.dll" -ForegroundColor Green
}

# Copy ALL DLLs from vcpkg (including any runtime dependencies)
if (Test-Path "$vcpkgBin") {
    Get-ChildItem "$vcpkgBin/*.dll" | ForEach-Object {
        Copy-Item $_.FullName "VulkanApp_Release/" -Force
        Write-Host "Copied $($_.Name)" -ForegroundColor Green
    }
}

# Also copy MinGW runtime DLLs if they exist
$mingwPaths = @(
    "C:\msys64\ucrt64\bin",
    "C:\msys64\mingw64\bin", 
    "C:\mingw64\bin"
)

$neededDlls = @("libgcc_s_seh-1.dll", "libstdc++-6.dll", "libwinpthread-1.dll")

foreach ($path in $mingwPaths) {
    if (Test-Path $path) {
        foreach ($dll in $neededDlls) {
            $dllPath = Join-Path $path $dll
            if (Test-Path $dllPath) {
                Copy-Item $dllPath "VulkanApp_Release/" -Force
                Write-Host "Copied $dll from MinGW" -ForegroundColor Green
            }
        }
        break
    }
}

# Copy shaders
if (Test-Path "shaders/*.spv") { 
    Copy-Item "shaders/*.spv" "VulkanApp_Release/shaders/" -Force 
    Write-Host "Copied shaders" -ForegroundColor Green
} else {
    Write-Host "Warning: No .spv shaders found" -ForegroundColor Yellow
}

# Copy texture
if (Test-Path "texture.jpg") { 
    Copy-Item "texture.jpg" "VulkanApp_Release/" -Force 
    Write-Host "Copied texture.jpg" -ForegroundColor Green
} else {
    Write-Host "Info: No texture.jpg found (will use default)" -ForegroundColor Cyan
}

Write-Host ""
Write-Host "Distribution package created in VulkanApp_Release/" -ForegroundColor Green
Write-Host "Ready to zip and distribute!" -ForegroundColor Green