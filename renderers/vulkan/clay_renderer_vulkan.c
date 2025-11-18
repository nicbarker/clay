// Clay Vulkan renderer sample (C17/C20 friendly)
// This file demonstrates how to traverse Clay_RenderCommandArray and submit Vulkan draw calls.
// The implementation favors clarity over micro-optimizations and documents future improvements.
// Vulkan reference: https://vulkan.lunarg.com/doc/sdk
// ImGui integration notes (for font atlas usage ideas): https://github.com/ocornut/imgui/wiki

#include <stdint.h>
#include <string.h>
#include <vulkan/vulkan.h>
#include "clay.h"

// Forward declarations -------------------------------------------------------
struct ClayVulkanRenderer;
static void ClayVulkan_PushScissor(struct ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer);
static void ClayVulkan_PopScissor(struct ClayVulkanRenderer* renderer, VkCommandBuffer commandBuffer);
static void ClayVulkan_DrawRectangle(struct ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer);
static void ClayVulkan_DrawBorder(struct ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer);
static void ClayVulkan_DrawImage(struct ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer);
static void ClayVulkan_DrawText(struct ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer);
static void ClayVulkan_DrawCustom(struct ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer);

// Resource cache keyed by Clay IDs ------------------------------------------
typedef struct ClayVulkanTexture
{
    uint32_t m_Id;          // Clay image id
    VkImageView m_ImageView;
    VkSampler m_Sampler;
    VkDescriptorSet m_DescriptorSet;
} ClayVulkanTexture;

typedef struct ClayVulkanFont
{
    uint16_t m_FontId;
    uint16_t m_FontSize;
    void* m_FontUserData;   // Pointer to glyph atlas / font metrics owned by the application
} ClayVulkanFont;

typedef struct ClayVulkanResourceCache
{
    ClayVulkanTexture m_Textures[64];
    uint32_t m_TextureCount;
    ClayVulkanFont m_Fonts[32];
    uint32_t m_FontCount;
} ClayVulkanResourceCache;

// Renderer state ------------------------------------------------------------
typedef struct ClayVulkanRenderer
{
    VkDevice m_Device;
    VkRenderPass m_RenderPass; // Compatible with the framebuffer bound by the host application
    VkPipeline m_RectPipeline;
    VkPipeline m_TextPipeline;
    VkPipeline m_ImagePipeline;
    VkPipelineLayout m_PipelineLayout;
    VkCommandBuffer m_ActiveCommandBuffer;
    VkRect2D m_ScissorStack[16];
    uint32_t m_ScissorCount;
    ClayVulkanResourceCache m_ResourceCache;
    // The sample keeps descriptor pools outside; the host passes ready-to-bind descriptor sets.
} ClayVulkanRenderer;

// Utility: fetch or insert cached texture -----------------------------------
static ClayVulkanTexture* ClayVulkan_GetTexture(ClayVulkanResourceCache* cache, uint32_t id)
{
    for (uint32_t it_Index = 0; it_Index < cache->m_TextureCount; ++it_Index)
    {
        ClayVulkanTexture* l_Texture = &cache->m_Textures[it_Index];
        if (l_Texture->m_Id == id)
        {
            return l_Texture;
        }
    }
    if (cache->m_TextureCount >= (uint32_t)(sizeof(cache->m_Textures) / sizeof(cache->m_Textures[0])))
    {
        return NULL; // Out of cache entries; caller can log and skip drawing.
    }
    ClayVulkanTexture* l_Texture = &cache->m_Textures[cache->m_TextureCount++];
    memset(l_Texture, 0, sizeof(*l_Texture));
    l_Texture->m_Id = id;

    return l_Texture;
}

// Utility: fetch or insert cached font --------------------------------------
static ClayVulkanFont* ClayVulkan_GetFont(ClayVulkanResourceCache* cache, uint16_t fontId, uint16_t fontSize)
{
    for (uint32_t it_Index = 0; it_Index < cache->m_FontCount; ++it_Index)
    {
        ClayVulkanFont* l_Font = &cache->m_Fonts[it_Index];
        if (l_Font->m_FontId == fontId && l_Font->m_FontSize == fontSize)
        {
            return l_Font;
        }
    }
    if (cache->m_FontCount >= (uint32_t)(sizeof(cache->m_Fonts) / sizeof(cache->m_Fonts[0])))
    {
        return NULL;
    }
    ClayVulkanFont* l_Font = &cache->m_Fonts[cache->m_FontCount++];
    memset(l_Font, 0, sizeof(*l_Font));
    l_Font->m_FontId = fontId;
    l_Font->m_FontSize = fontSize;

    return l_Font;
}

// Text measurement hook ------------------------------------------------------
// Backs Clay_SetMeasureTextFunction and aligns with Clay_TextRenderData expectations.
static Clay_Dimensions ClayVulkan_MeasureText(Clay_StringSlice text, Clay_TextElementConfig* config, void* userData)
{
    ClayVulkanRenderer* l_Renderer = (ClayVulkanRenderer*)userData;
    ClayVulkanFont* l_Font = ClayVulkan_GetFont(&l_Renderer->m_ResourceCache, config->fontId, config->fontSize);
    // The sample assumes the host supplies pixel metrics via m_FontUserData (e.g., stb_truetype bake results).
    // For demonstration, approximate width using a monospaced assumption and letter spacing.
    const float s_DefaultGlyphWidth = (float)config->fontSize * 0.6f;
    float l_Width = (float)text.length * s_DefaultGlyphWidth + (float)config->letterSpacing * (float)text.length;
    float l_Height = (float)config->lineHeight;
    (void)l_Font; // In a real integration, sample glyph advances from l_Font->m_FontUserData.
    
    // MSVC doesn't support C99 compound literals in C mode; build the return value explicitly for portability.
    Clay_Dimensions l_Dimensions = { 0 };
    l_Dimensions.width = l_Width;
    l_Dimensions.height = l_Height;

    return l_Dimensions;
}

// Scissor management ---------------------------------------------------------
static void ClayVulkan_PushScissor(ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer)
{
    if (renderer->m_ScissorCount >= (uint32_t)(sizeof(renderer->m_ScissorStack) / sizeof(renderer->m_ScissorStack[0])))
    {
        return; // Avoid overflow; future improvement could resize dynamically.
    }
    VkRect2D l_Rect = { 0 };
    l_Rect.offset.x = (int32_t)command->boundingBox.x;
    l_Rect.offset.y = (int32_t)command->boundingBox.y;
    l_Rect.extent.width = (uint32_t)command->boundingBox.width;
    l_Rect.extent.height = (uint32_t)command->boundingBox.height;
    renderer->m_ScissorStack[renderer->m_ScissorCount++] = l_Rect;
    vkCmdSetScissor(commandBuffer, 0, 1, &l_Rect);
}

static void ClayVulkan_PopScissor(ClayVulkanRenderer* renderer, VkCommandBuffer commandBuffer)
{
    if (renderer->m_ScissorCount == 0)
    {
        return;
    }
    renderer->m_ScissorCount--;
    if (renderer->m_ScissorCount == 0)
    {
        // Fall back to a full viewport scissor. The host should have set a default scissor before calling render.
        return;
    }

    VkRect2D l_Rect = renderer->m_ScissorStack[renderer->m_ScissorCount - 1];
    vkCmdSetScissor(commandBuffer, 0, 1, &l_Rect);
}

// Rendering helpers ----------------------------------------------------------
static void ClayVulkan_DrawRectangle(ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer)
{
    (void)renderer;
    // The sample assumes a pipeline using push constants for color + corner radius and a unit quad vertex buffer.
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->m_RectPipeline);
    // Future improvement: instance multiple rectangles in a single vkCmdDrawIndexed to amortize state changes.
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}

static void ClayVulkan_DrawBorder(ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer)
{
    (void)command;
    // Border rendering could expand to 4 rectangles; here we rely on a shader that interprets width + corner radius from push constants.
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->m_RectPipeline);
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}

static void ClayVulkan_DrawImage(ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer)
{
    ClayVulkanTexture* l_Texture = ClayVulkan_GetTexture(&renderer->m_ResourceCache, command->id);
    if (!l_Texture || !l_Texture->m_DescriptorSet || !l_Texture->m_ImageView || !l_Texture->m_Sampler)
    {
        // Ensure descriptor sets are prepared before binding; application is responsible for keeping resources live.
        return;
    }
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->m_ImagePipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->m_PipelineLayout, 0, 1, &l_Texture->m_DescriptorSet, 0, NULL);
    vkCmdDraw(commandBuffer, 6, 1, 0, 0);
}

static void ClayVulkan_DrawText(ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer)
{
    Clay_TextRenderData l_Text = command->renderData.text;
    ClayVulkanFont* l_Font = ClayVulkan_GetFont(&renderer->m_ResourceCache, l_Text.fontId, l_Text.fontSize);
    (void)l_Font;
    // The host is expected to have built glyph vertices into a transient buffer before calling this renderer.
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->m_TextPipeline);
    // Future improvement: bind per-font descriptor sets for font atlas textures instead of assuming a global one.
    vkCmdDraw(commandBuffer, (uint32_t)l_Text.stringContents.length * 4, 1, 0, 0);
}

static void ClayVulkan_DrawCustom(ClayVulkanRenderer* renderer, const Clay_RenderCommand* command, VkCommandBuffer commandBuffer)
{
    (void)renderer;
    (void)command;
    // Custom elements give the host full control. Applications can use command->renderData.custom.customData to pick pipelines.
    // Future improvement: provide callback hooks so applications can supply their own vkCmd* sequence here.
    (void)commandBuffer;
}

// Main entry point -----------------------------------------------------------
void Clay_Vulkan_Render(ClayVulkanRenderer* renderer, Clay_RenderCommandArray renderCommands, VkCommandBuffer commandBuffer)
{
    renderer->m_ActiveCommandBuffer = commandBuffer;
    // Command traversal: the array is already z-sorted; we iterate sequentially and rely on lightweight state changes.
    for (int32_t it_Command = 0; it_Command < renderCommands.length; ++it_Command)
    {
        Clay_RenderCommand* l_Command = Clay_RenderCommandArray_Get(&renderCommands, it_Command);
        switch (l_Command->commandType)
        {
        case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
            ClayVulkan_DrawRectangle(renderer, l_Command, commandBuffer);
            break;
        case CLAY_RENDER_COMMAND_TYPE_BORDER:
            ClayVulkan_DrawBorder(renderer, l_Command, commandBuffer);
            break;
        case CLAY_RENDER_COMMAND_TYPE_TEXT:
            ClayVulkan_DrawText(renderer, l_Command, commandBuffer);
            break;
        case CLAY_RENDER_COMMAND_TYPE_IMAGE:
            ClayVulkan_DrawImage(renderer, l_Command, commandBuffer);
            break;
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START:
            ClayVulkan_PushScissor(renderer, l_Command, commandBuffer);
            break;
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END:
            ClayVulkan_PopScissor(renderer, commandBuffer);
            break;
        case CLAY_RENDER_COMMAND_TYPE_CUSTOM:
            ClayVulkan_DrawCustom(renderer, l_Command, commandBuffer);
            break;
        default:
            break;
        }
    }
}

// Initialization helpers ----------------------------------------------------
void Clay_VulkanRenderer_Init(ClayVulkanRenderer* renderer, VkDevice device, VkRenderPass renderPass, VkPipelineLayout pipelineLayout)
{
    renderer->m_Device = device;
    renderer->m_RenderPass = renderPass;
    renderer->m_PipelineLayout = pipelineLayout;
    renderer->m_RectPipeline = VK_NULL_HANDLE;
    renderer->m_TextPipeline = VK_NULL_HANDLE;
    renderer->m_ImagePipeline = VK_NULL_HANDLE;
    renderer->m_ActiveCommandBuffer = VK_NULL_HANDLE;
    renderer->m_ScissorCount = 0;

    memset(&renderer->m_ResourceCache, 0, sizeof(renderer->m_ResourceCache));
}

void Clay_VulkanRenderer_SetPipelines(ClayVulkanRenderer* renderer, VkPipeline rectPipeline, VkPipeline textPipeline, VkPipeline imagePipeline)
{
    renderer->m_RectPipeline = rectPipeline;
    renderer->m_TextPipeline = textPipeline;
    renderer->m_ImagePipeline = imagePipeline;
}

// Hook into Clay's text measuring API.
void Clay_VulkanRenderer_RegisterTextMeasure(ClayVulkanRenderer* renderer)
{
    Clay_SetMeasureTextFunction(ClayVulkan_MeasureText, renderer);
}

// Example for populating the texture cache with externally created descriptors.
void Clay_VulkanRenderer_RegisterTexture(ClayVulkanRenderer* renderer, uint32_t clayId, VkImageView imageView, VkSampler sampler, VkDescriptorSet descriptorSet)
{
    ClayVulkanTexture* l_Texture = ClayVulkan_GetTexture(&renderer->m_ResourceCache, clayId);
    if (!l_Texture)
    {
        return;
    }

    l_Texture->m_ImageView = imageView;
    l_Texture->m_Sampler = sampler;
    l_Texture->m_DescriptorSet = descriptorSet;
}

// Future improvements -------------------------------------------------------
// - Descriptor recycling: shrink transient allocations by pooling VkDescriptorSet objects per frame.
// - Dynamic rendering: replace render pass dependency with vkCmdBeginRendering for more flexible swapchain formats.
// - Batching: group adjacent rectangles or text quads into large instance draws using secondary command buffers.