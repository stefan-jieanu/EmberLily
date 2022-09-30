@echo off
cd shaders
mkdir bin
cd ..

C:\VulkanSDK\1.3.224.1\Bin\glslc.exe shaders/simple_shader.frag -o shaders/bin/simple_shader.frag.spv
C:\VulkanSDK\1.3.224.1\Bin\glslc.exe shaders/simple_shader.vert -o shaders/bin/simple_shader.vert.spv
