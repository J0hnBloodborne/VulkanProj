$shaderSourceDir = "src/shaders"
$shaderDestDir = "VulkanApp_Release/shaders"

if (!(Test-Path $shaderDestDir)) {
    New-Item -ItemType Directory -Path $shaderDestDir | Out-Null
}

# Compile mesh vertex shader
if (Test-Path "$shaderSourceDir/mesh.vert.glsl") {
    & glslc -fshader-stage=vertex "$shaderSourceDir/mesh.vert.glsl" -o "$shaderDestDir/mesh.vert.spv"
    Write-Host "Compiled mesh.vert.glsl to mesh.vert.spv" -ForegroundColor Green
} else {
    Write-Host "mesh.vert.glsl not found in src/shaders/" -ForegroundColor Yellow
}

# Compile mesh fragment shader
if (Test-Path "$shaderSourceDir/lambert.frag.glsl") {
    & glslc -fshader-stage=fragment "$shaderSourceDir/lambert.frag.glsl" -o "$shaderDestDir/lambert.frag.spv"
    Write-Host "Compiled lambert.frag.glsl to lambert.frag.spv" -ForegroundColor Green
} else {
    Write-Host "lambert.frag.glsl not found in src/shaders/" -ForegroundColor Yellow
}
