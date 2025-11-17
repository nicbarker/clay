// Vulkan + Clay demo
// Vulkan SDK documentation: https://vulkan.lunarg.com/doc/sdk
// ImGui integration ideas for font atlases and descriptor reuse: https://github.com/ocornut/imgui/wiki

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#define CLAY_IMPLEMENTATION
extern "C" {
#include "../../clay.h"
#include "../../renderers/vulkan/clay_renderer_vulkan.c"
}

namespace
{
    const uint32_t s_DefaultWidth = 1280;
    const uint32_t s_DefaultHeight = 720;
    const std::vector<const char*> s_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> m_GraphicsFamily;
        std::optional<uint32_t> m_PresentFamily;

        bool IsComplete() const
        {
            return m_GraphicsFamily.has_value() && m_PresentFamily.has_value();
        }
    };

    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR m_Capabilities{};
        std::vector<VkSurfaceFormatKHR> m_Formats{};
        std::vector<VkPresentModeKHR> m_PresentModes{};
    };
} // namespace

class VulkanDemoApp
{
public:
    void Run()
    {
        try
        {
            InitializeWindow();
            InitializeVulkan();
            InitializeClay();
            MainLoop();
            Cleanup();
        }
        catch (const std::exception& l_Error)
        {
            std::cerr << "Demo failed: " << l_Error.what() << std::endl;
            throw;
        }
    }

private:
    GLFWwindow* m_Window = nullptr;
    VkInstance m_Instance = VK_NULL_HANDLE;
    VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
    VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice m_Device = VK_NULL_HANDLE;
    VkQueue m_GraphicsQueue = VK_NULL_HANDLE;
    VkQueue m_PresentQueue = VK_NULL_HANDLE;
    VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;
    std::vector<VkImage> m_SwapchainImages{};
    std::vector<VkImageView> m_SwapchainImageViews{};
    VkFormat m_SwapchainImageFormat = VK_FORMAT_B8G8R8A8_SRGB;
    VkExtent2D m_SwapchainExtent{};
    VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> m_SwapchainFramebuffers{};
    VkCommandPool m_CommandPool = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> m_CommandBuffers{};
    VkSemaphore m_ImageAvailableSemaphore = VK_NULL_HANDLE;
    VkSemaphore m_RenderFinishedSemaphore = VK_NULL_HANDLE;
    VkFence m_InFlightFence = VK_NULL_HANDLE;

    // Clay renderer bindings
    ClayVulkanRenderer m_ClayRenderer{};
    Clay_Arena m_ClayArena{};

    void InitializeWindow()
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(static_cast<int>(s_DefaultWidth), static_cast<int>(s_DefaultHeight), "Clay Vulkan Demo", nullptr, nullptr);
        if (m_Window == nullptr)
        {
            throw std::runtime_error("Failed to create GLFW window");
        }
    }

    void InitializeVulkan()
    {
        CreateInstance();
        CreateSurface();
        PickPhysicalDevice();
        CreateLogicalDevice();
        CreateSwapchain();
        CreateImageViews();
        CreateRenderPass();
        CreateFramebuffers();
        CreateCommandPool();
        AllocateCommandBuffers();
        CreateSyncObjects();
    }

    void InitializeClay()
    {
        uint64_t l_TotalMemorySize = Clay_MinMemorySize();
        m_ClayArena = Clay_CreateArenaWithCapacityAndMemory(l_TotalMemorySize, malloc(l_TotalMemorySize));
        
        // Build the layout dimensions explicitly to keep MSVC happy with aggregate initialization rules.
        Clay_Dimensions l_LayoutDimensions{};
        l_LayoutDimensions.width = static_cast<float>(m_SwapchainExtent.width);
        l_LayoutDimensions.height = static_cast<float>(m_SwapchainExtent.height);

        Clay_Initialize(m_ClayArena, l_LayoutDimensions, Clay_ErrorHandler{ 0 });

        Clay_VulkanRenderer_Init(&m_ClayRenderer, m_Device, m_RenderPass, VK_NULL_HANDLE);
        Clay_VulkanRenderer_SetPipelines(&m_ClayRenderer, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE);
        // TODO: Plug descriptor pool + layout management for textures/fonts instead of placeholder pipeline handles.
        Clay_VulkanRenderer_RegisterTextMeasure(&m_ClayRenderer);
    }

    void MainLoop()
    {
        while (!glfwWindowShouldClose(m_Window))
        {
            glfwPollEvents();

            int l_Width = 0;
            int l_Height = 0;
            glfwGetFramebufferSize(m_Window, &l_Width, &l_Height);
            m_SwapchainExtent.width = static_cast<uint32_t>(l_Width);
            m_SwapchainExtent.height = static_cast<uint32_t>(l_Height);
            
            Clay_Dimensions l_LayoutDimensions{};
            l_LayoutDimensions.width = static_cast<float>(l_Width);
            l_LayoutDimensions.height = static_cast<float>(l_Height);
            Clay_SetLayoutDimensions(l_LayoutDimensions);

            Clay_RenderCommandArray l_Commands = CreateLayout();
            DrawFrame(l_Commands);
        }

        vkDeviceWaitIdle(m_Device);
    }

    void Cleanup()
    {
        vkDeviceWaitIdle(m_Device);

        vkDestroyFence(m_Device, m_InFlightFence, nullptr);
        vkDestroySemaphore(m_Device, m_RenderFinishedSemaphore, nullptr);
        vkDestroySemaphore(m_Device, m_ImageAvailableSemaphore, nullptr);

        for (VkFramebuffer l_Framebuffer : m_SwapchainFramebuffers)
        {
            vkDestroyFramebuffer(m_Device, l_Framebuffer, nullptr);
        }
        for (VkImageView l_ImageView : m_SwapchainImageViews)
        {
            vkDestroyImageView(m_Device, l_ImageView, nullptr);
        }

        vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
        vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
        vkDestroyDevice(m_Device, nullptr);
        vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
        vkDestroyInstance(m_Instance, nullptr);

        glfwDestroyWindow(m_Window);
        glfwTerminate();

        // Clay_Arena owns memory allocated with malloc above; release the backing allocation explicitly.
        free(m_ClayArena.memory);
    }

    void CreateInstance()
    {
        VkApplicationInfo l_AppInfo{};
        l_AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        l_AppInfo.pApplicationName = "Clay Vulkan Demo";
        l_AppInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        l_AppInfo.pEngineName = "Clay";
        l_AppInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
        l_AppInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo l_CreateInfo{};
        l_CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        l_CreateInfo.pApplicationInfo = &l_AppInfo;

        uint32_t l_GlfwExtensionCount = 0;
        const char** l_Extensions = glfwGetRequiredInstanceExtensions(&l_GlfwExtensionCount);
        l_CreateInfo.enabledExtensionCount = l_GlfwExtensionCount;
        l_CreateInfo.ppEnabledExtensionNames = l_Extensions;

        if (vkCreateInstance(&l_CreateInfo, nullptr, &m_Instance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create Vulkan instance; see Vulkan SDK setup guidance.");
        }
    }

    void CreateSurface()
    {
        if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface");
        }
    }

    void PickPhysicalDevice()
    {
        uint32_t l_DeviceCount = 0;
        vkEnumeratePhysicalDevices(m_Instance, &l_DeviceCount, nullptr);
        if (l_DeviceCount == 0)
        {
            throw std::runtime_error("No Vulkan-capable GPU found");
        }

        std::vector<VkPhysicalDevice> l_Devices(l_DeviceCount);
        vkEnumeratePhysicalDevices(m_Instance, &l_DeviceCount, l_Devices.data());

        for (const VkPhysicalDevice& it_Device : l_Devices)
        {
            if (IsDeviceSuitable(it_Device))
            {
                m_PhysicalDevice = it_Device;
                break;
            }
        }

        if (m_PhysicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("No suitable GPU found");
        }
    }

    void CreateLogicalDevice()
    {
        QueueFamilyIndices l_Indices = FindQueueFamilies(m_PhysicalDevice);

        std::vector<VkDeviceQueueCreateInfo> l_QueueCreateInfos{};
        std::set<uint32_t> l_UniqueQueueFamilies = { l_Indices.m_GraphicsFamily.value(), l_Indices.m_PresentFamily.value() };

        float l_QueuePriority = 1.0f;
        for (uint32_t it_FamilyIndex : l_UniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo l_QueueCreateInfo{};
            l_QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            l_QueueCreateInfo.queueFamilyIndex = it_FamilyIndex;
            l_QueueCreateInfo.queueCount = 1;
            l_QueueCreateInfo.pQueuePriorities = &l_QueuePriority;
            l_QueueCreateInfos.push_back(l_QueueCreateInfo);
        }

        VkPhysicalDeviceFeatures l_DeviceFeatures{};
        VkDeviceCreateInfo l_CreateInfo{};
        l_CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        l_CreateInfo.queueCreateInfoCount = static_cast<uint32_t>(l_QueueCreateInfos.size());
        l_CreateInfo.pQueueCreateInfos = l_QueueCreateInfos.data();
        l_CreateInfo.pEnabledFeatures = &l_DeviceFeatures;
        l_CreateInfo.enabledExtensionCount = static_cast<uint32_t>(s_DeviceExtensions.size());
        l_CreateInfo.ppEnabledExtensionNames = s_DeviceExtensions.data();

        if (vkCreateDevice(m_PhysicalDevice, &l_CreateInfo, nullptr, &m_Device) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create logical device");
        }

        vkGetDeviceQueue(m_Device, l_Indices.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);
        vkGetDeviceQueue(m_Device, l_Indices.m_PresentFamily.value(), 0, &m_PresentQueue);
    }

    void CreateSwapchain()
    {
        SwapchainSupportDetails l_Support = QuerySwapchainSupport(m_PhysicalDevice);

        VkSurfaceFormatKHR l_SurfaceFormat = ChooseSwapSurfaceFormat(l_Support.m_Formats);
        VkPresentModeKHR l_PresentMode = ChoosePresentMode(l_Support.m_PresentModes);
        VkExtent2D l_Extent = ChooseSwapExtent(l_Support.m_Capabilities);

        uint32_t l_ImageCount = l_Support.m_Capabilities.minImageCount + 1;
        if (l_Support.m_Capabilities.maxImageCount > 0 && l_ImageCount > l_Support.m_Capabilities.maxImageCount)
        {
            l_ImageCount = l_Support.m_Capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR l_CreateInfo{};
        l_CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        l_CreateInfo.surface = m_Surface;
        l_CreateInfo.minImageCount = l_ImageCount;
        l_CreateInfo.imageFormat = l_SurfaceFormat.format;
        l_CreateInfo.imageColorSpace = l_SurfaceFormat.colorSpace;
        l_CreateInfo.imageExtent = l_Extent;
        l_CreateInfo.imageArrayLayers = 1;
        l_CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices l_Indices = FindQueueFamilies(m_PhysicalDevice);
        uint32_t l_QueueFamilyIndices[] = { l_Indices.m_GraphicsFamily.value(), l_Indices.m_PresentFamily.value() };

        if (l_Indices.m_GraphicsFamily != l_Indices.m_PresentFamily)
        {
            l_CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            l_CreateInfo.queueFamilyIndexCount = 2;
            l_CreateInfo.pQueueFamilyIndices = l_QueueFamilyIndices;
        }
        else
        {
            l_CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            l_CreateInfo.queueFamilyIndexCount = 0;
            l_CreateInfo.pQueueFamilyIndices = nullptr;
        }

        l_CreateInfo.preTransform = l_Support.m_Capabilities.currentTransform;
        l_CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        l_CreateInfo.presentMode = l_PresentMode;
        l_CreateInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(m_Device, &l_CreateInfo, nullptr, &m_Swapchain) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create swapchain");
        }

        vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &l_ImageCount, nullptr);
        m_SwapchainImages.resize(l_ImageCount);
        vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &l_ImageCount, m_SwapchainImages.data());

        m_SwapchainImageFormat = l_SurfaceFormat.format;
        m_SwapchainExtent = l_Extent;
    }

    void CreateImageViews()
    {
        m_SwapchainImageViews.resize(m_SwapchainImages.size());

        for (size_t it_Index = 0; it_Index < m_SwapchainImages.size(); ++it_Index)
        {
            VkImageViewCreateInfo l_CreateInfo{};
            l_CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            l_CreateInfo.image = m_SwapchainImages[it_Index];
            l_CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            l_CreateInfo.format = m_SwapchainImageFormat;
            l_CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            l_CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            l_CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            l_CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            l_CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            l_CreateInfo.subresourceRange.baseMipLevel = 0;
            l_CreateInfo.subresourceRange.levelCount = 1;
            l_CreateInfo.subresourceRange.baseArrayLayer = 0;
            l_CreateInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_Device, &l_CreateInfo, nullptr, &m_SwapchainImageViews[it_Index]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create image views");
            }
        }
    }

    void CreateRenderPass()
    {
        VkAttachmentDescription l_ColorAttachment{};
        l_ColorAttachment.format = m_SwapchainImageFormat;
        l_ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        l_ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        l_ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        l_ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        l_ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        l_ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        l_ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference l_ColorAttachmentRef{};
        l_ColorAttachmentRef.attachment = 0;
        l_ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription l_Subpass{};
        l_Subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        l_Subpass.colorAttachmentCount = 1;
        l_Subpass.pColorAttachments = &l_ColorAttachmentRef;

        VkRenderPassCreateInfo l_RenderPassInfo{};
        l_RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        l_RenderPassInfo.attachmentCount = 1;
        l_RenderPassInfo.pAttachments = &l_ColorAttachment;
        l_RenderPassInfo.subpassCount = 1;
        l_RenderPassInfo.pSubpasses = &l_Subpass;

        if (vkCreateRenderPass(m_Device, &l_RenderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create render pass");
        }
    }

    void CreateFramebuffers()
    {
        m_SwapchainFramebuffers.resize(m_SwapchainImageViews.size());

        for (size_t it_Index = 0; it_Index < m_SwapchainImageViews.size(); ++it_Index)
        {
            VkImageView l_Attachments[] = { m_SwapchainImageViews[it_Index] };

            VkFramebufferCreateInfo l_FramebufferInfo{};
            l_FramebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            l_FramebufferInfo.renderPass = m_RenderPass;
            l_FramebufferInfo.attachmentCount = 1;
            l_FramebufferInfo.pAttachments = l_Attachments;
            l_FramebufferInfo.width = m_SwapchainExtent.width;
            l_FramebufferInfo.height = m_SwapchainExtent.height;
            l_FramebufferInfo.layers = 1;

            if (vkCreateFramebuffer(m_Device, &l_FramebufferInfo, nullptr, &m_SwapchainFramebuffers[it_Index]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create framebuffer");
            }
        }
    }

    void CreateCommandPool()
    {
        QueueFamilyIndices l_QueueFamilyIndices = FindQueueFamilies(m_PhysicalDevice);

        VkCommandPoolCreateInfo l_PoolInfo{};
        l_PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        l_PoolInfo.queueFamilyIndex = l_QueueFamilyIndices.m_GraphicsFamily.value();
        l_PoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(m_Device, &l_PoolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create command pool");
        }
    }

    void AllocateCommandBuffers()
    {
        m_CommandBuffers.resize(m_SwapchainFramebuffers.size());

        VkCommandBufferAllocateInfo l_AllocInfo{};
        l_AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        l_AllocInfo.commandPool = m_CommandPool;
        l_AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        l_AllocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

        if (vkAllocateCommandBuffers(m_Device, &l_AllocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers");
        }
    }

    void CreateSyncObjects()
    {
        VkSemaphoreCreateInfo l_SemaphoreInfo{};
        l_SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo l_FenceInfo{};
        l_FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        l_FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(m_Device, &l_SemaphoreInfo, nullptr, &m_ImageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(m_Device, &l_SemaphoreInfo, nullptr, &m_RenderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(m_Device, &l_FenceInfo, nullptr, &m_InFlightFence) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create synchronization objects");
        }
    }

    void DrawFrame(const Clay_RenderCommandArray& renderCommands)
    {
        vkWaitForFences(m_Device, 1, &m_InFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(m_Device, 1, &m_InFlightFence);

        uint32_t l_ImageIndex = 0;
        vkAcquireNextImageKHR(m_Device, m_Swapchain, UINT64_MAX, m_ImageAvailableSemaphore, VK_NULL_HANDLE, &l_ImageIndex);

        vkResetCommandBuffer(m_CommandBuffers[l_ImageIndex], 0);
        RecordCommandBuffer(m_CommandBuffers[l_ImageIndex], l_ImageIndex, renderCommands);

        VkSemaphore l_WaitSemaphores[] = { m_ImageAvailableSemaphore };
        VkPipelineStageFlags l_WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSemaphore l_SignalSemaphores[] = { m_RenderFinishedSemaphore };

        VkSubmitInfo l_SubmitInfo{};
        l_SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        l_SubmitInfo.waitSemaphoreCount = 1;
        l_SubmitInfo.pWaitSemaphores = l_WaitSemaphores;
        l_SubmitInfo.pWaitDstStageMask = l_WaitStages;
        l_SubmitInfo.commandBufferCount = 1;
        l_SubmitInfo.pCommandBuffers = &m_CommandBuffers[l_ImageIndex];
        l_SubmitInfo.signalSemaphoreCount = 1;
        l_SubmitInfo.pSignalSemaphores = l_SignalSemaphores;

        if (vkQueueSubmit(m_GraphicsQueue, 1, &l_SubmitInfo, m_InFlightFence) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to submit draw command buffer");
        }

        VkPresentInfoKHR l_PresentInfo{};
        l_PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        l_PresentInfo.waitSemaphoreCount = 1;
        l_PresentInfo.pWaitSemaphores = l_SignalSemaphores;
        l_PresentInfo.swapchainCount = 1;
        l_PresentInfo.pSwapchains = &m_Swapchain;
        l_PresentInfo.pImageIndices = &l_ImageIndex;

        vkQueuePresentKHR(m_PresentQueue, &l_PresentInfo);
    }

    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, const Clay_RenderCommandArray& renderCommands)
    {
        VkCommandBufferBeginInfo l_BeginInfo{};
        l_BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &l_BeginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer");
        }

        VkClearValue l_ClearColor{};
        l_ClearColor.color = { { 0.1f, 0.1f, 0.12f, 1.0f } };

        VkRenderPassBeginInfo l_RenderPassInfo{};
        l_RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        l_RenderPassInfo.renderPass = m_RenderPass;
        l_RenderPassInfo.framebuffer = m_SwapchainFramebuffers[imageIndex];
        l_RenderPassInfo.renderArea.offset = { 0, 0 };
        l_RenderPassInfo.renderArea.extent = m_SwapchainExtent;
        l_RenderPassInfo.clearValueCount = 1;
        l_RenderPassInfo.pClearValues = &l_ClearColor;

        vkCmdBeginRenderPass(commandBuffer, &l_RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Clay renderer translates render commands into Vulkan draw calls. Pipelines are left as TODOs.
        Clay_Vulkan_Render(&m_ClayRenderer, renderCommands, commandBuffer);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer");
        }
    }

    Clay_RenderCommandArray CreateLayout()
    {
        Clay_BeginLayout();

        // Shared sizing config to expand containers along both axes.
        Clay_Sizing l_Expand{};
        l_Expand.width = CLAY_SIZING_GROW(0);
        l_Expand.height = CLAY_SIZING_GROW(0);

        // Root container stacks children vertically and adds generous padding.
        CLAY(CLAY_ID("Root"), {
            .layout = {
                .sizing = l_Expand,
                .padding = CLAY_PADDING_ALL(12),
                .childGap = 12,
                .childAlignment = {.x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP },
                .layoutDirection = CLAY_TOP_TO_BOTTOM
            },
            .backgroundColor = { 18, 18, 20, 255 }
            })
        {
            // Header keeps children centered and reserves a fixed height.
            CLAY(CLAY_ID("Header"), {
                .layout = {
                    .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(64) },
                    .padding = CLAY_PADDING_ALL(0),
                    .childGap = 0,
                    .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT
                },
                .backgroundColor = { 32, 64, 96, 255 }
                })
            {
                // Keep designated fields in declaration order for MSVC aggregate compatibility.
                Clay_TextElementConfig* l_HeaderTextConfig = CLAY_TEXT_CONFIG({
                    .userData = nullptr,
                    .textColor = { 245, 245, 245, 255 },
                    .fontId = 0,
                    .fontSize = 28
                    });
                CLAY_TEXT(CLAY_STRING("Clay Vulkan Demo"), l_HeaderTextConfig);
            }

            // Body splits the space horizontally into a sidebar and viewport.
            CLAY(CLAY_ID("Body"), {
                .layout = {
                    .sizing = l_Expand,
                    .padding = CLAY_PADDING_ALL(0),
                    .childGap = 10,
                    .childAlignment = {.x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP },
                    .layoutDirection = CLAY_LEFT_TO_RIGHT
                }
                })
            {
                // Sidebar lists current wiring status.
                CLAY(CLAY_ID("Sidebar"), {
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_FIXED(240), .height = CLAY_SIZING_GROW(0) },
                        .padding = CLAY_PADDING_ALL(10),
                        .childGap = 8,
                        .childAlignment = {.x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM
                    },
                    .backgroundColor = { 40, 40, 44, 255 }
                    })
                {
                    // Keep designated fields in declaration order for MSVC aggregate compatibility.
                    Clay_TextElementConfig* l_StatusTextConfig = CLAY_TEXT_CONFIG({
                        .userData = nullptr,
                        .textColor = { 200, 220, 255, 255 },
                        .fontId = 0,
                        .fontSize = 20
                        });
                    CLAY_TEXT(CLAY_STRING("Swapchain + Clay wiring ready.\nTODO: upload fonts and textures."), l_StatusTextConfig);
                }

                // Viewport stands in for rendered content.
                CLAY(CLAY_ID("Viewport"), {
                    .layout = {
                        .sizing = l_Expand,
                        .padding = CLAY_PADDING_ALL(0),
                        .childGap = 0,
                        .childAlignment = {.x = CLAY_ALIGN_X_LEFT, .y = CLAY_ALIGN_Y_TOP },
                        .layoutDirection = CLAY_TOP_TO_BOTTOM
                    },
                    .backgroundColor = { 28, 28, 32, 255 }
                    })
                {
                    // Keep designated fields in declaration order for MSVC aggregate compatibility.
                    Clay_TextElementConfig* l_ContentTextConfig = CLAY_TEXT_CONFIG({
                        .userData = nullptr,
                        .textColor = { 240, 240, 240, 255 },
                        .fontId = 0,
                        .fontSize = 22
                        });
                    CLAY_TEXT(CLAY_STRING("Render scene would appear here."), l_ContentTextConfig);
                }
            }
        }

        return Clay_EndLayout();
    }

    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device)
    {
        QueueFamilyIndices l_Indices{};

        uint32_t l_QueueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &l_QueueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> l_QueueFamilies(l_QueueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &l_QueueFamilyCount, l_QueueFamilies.data());

        uint32_t it_Index = 0;
        for (const VkQueueFamilyProperties& it_QueueFamily : l_QueueFamilies)
        {
            if (it_QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                l_Indices.m_GraphicsFamily = it_Index;
            }

            VkBool32 l_PresentSupport = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, it_Index, m_Surface, &l_PresentSupport);
            if (l_PresentSupport)
            {
                l_Indices.m_PresentFamily = it_Index;
            }

            if (l_Indices.IsComplete())
            {
                break;
            }
            ++it_Index;
        }

        return l_Indices;
    }

    bool IsDeviceSuitable(VkPhysicalDevice device)
    {
        QueueFamilyIndices l_Indices = FindQueueFamilies(device);

        bool l_ExtensionsSupported = CheckDeviceExtensionSupport(device);
        bool l_SwapchainAdequate = false;
        if (l_ExtensionsSupported)
        {
            SwapchainSupportDetails l_SwapchainSupport = QuerySwapchainSupport(device);
            l_SwapchainAdequate = !l_SwapchainSupport.m_Formats.empty() && !l_SwapchainSupport.m_PresentModes.empty();
        }

        return l_Indices.IsComplete() && l_ExtensionsSupported && l_SwapchainAdequate;
    }

    bool CheckDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t l_ExtensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &l_ExtensionCount, nullptr);

        std::vector<VkExtensionProperties> l_AvailableExtensions(l_ExtensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &l_ExtensionCount, l_AvailableExtensions.data());

        std::set<std::string> l_RequiredExtensions(s_DeviceExtensions.begin(), s_DeviceExtensions.end());

        for (const VkExtensionProperties& it_Extension : l_AvailableExtensions)
        {
            l_RequiredExtensions.erase(it_Extension.extensionName);
        }

        return l_RequiredExtensions.empty();
    }

    SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device)
    {
        SwapchainSupportDetails l_Details{};
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &l_Details.m_Capabilities);

        uint32_t l_FormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &l_FormatCount, nullptr);
        if (l_FormatCount != 0)
        {
            l_Details.m_Formats.resize(l_FormatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &l_FormatCount, l_Details.m_Formats.data());
        }

        uint32_t l_PresentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &l_PresentModeCount, nullptr);
        if (l_PresentModeCount != 0)
        {
            l_Details.m_PresentModes.resize(l_PresentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &l_PresentModeCount, l_Details.m_PresentModes.data());
        }

        return l_Details;
    }

    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const VkSurfaceFormatKHR& it_AvailableFormat : availableFormats)
        {
            if (it_AvailableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && it_AvailableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return it_AvailableFormat;
            }
        }

        return availableFormats.front();
    }

    VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (VkPresentModeKHR it_PresentMode : availablePresentModes)
        {
            if (it_PresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return it_PresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }

        VkExtent2D l_ActualExtent = { s_DefaultWidth, s_DefaultHeight };
        l_ActualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, l_ActualExtent.width));
        l_ActualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, l_ActualExtent.height));

        return l_ActualExtent;
    }
};

int main()
{
    VulkanDemoApp l_App{};
    
    l_App.Run();

    return 0;
}