# Clay Vulkan Demo

This Visual Studio 2022 friendly sample shows how to pair Clay's layout engine with a Vulkan swapchain. It uses GLFW for the Win32 surface and stitches Clay render commands through the Vulkan renderer adapter.

## Build prerequisites
- [Vulkan SDK](https://vulkan.lunarg.com/doc/sdk) installed and the `VULKAN_SDK` environment variable set.
- Visual Studio 2022 with CMake and the MSVC toolset.
- Git submodules are not required because GLFW is fetched automatically through CMake's `FetchContent`.

## Configure and build (Visual Studio 2022)
1. Open a **Developer PowerShell for VS 2022** so the compiler environment variables are available.
2. Generate a build directory:
   ```pwsh
   cmake -S examples/vulkan-demo -B build/vulkan-demo -G "Visual Studio 17 2022" -A x64
   ```
3. Build the executable:
   ```pwsh
   cmake --build build/vulkan-demo --config Debug
   ```
4. Run the demo from the build tree (the app opens a GLFW window):
   ```pwsh
   build/vulkan-demo/Debug/clay_examples_vulkan_demo.exe
   ```

## Notes and future work
- The code is intentionally verbose and commented with links to the Vulkan SDK reference and the ImGui wiki for font atlas/descriptor sharing advice.
- Swapchain, render pass, and command buffer setup are present, but pipeline configuration uses placeholders so you can wire your own shaders.
- TODOs remain for dynamic descriptor management and richer font handling; these are natural next steps after the basic window + swapchain bring-up.