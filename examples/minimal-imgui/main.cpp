#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#define SOKOL_IMPL
#define SOKOL_NO_ENTRY
#define SOKOL_GLCORE
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#define SOKOL_IMGUI_IMPL
#include "util/sokol_imgui.h"

#include <stdio.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

sg_pass_action pass_action = {};

static const uint32_t FONT_ID_BODY_24 = 0;
static const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};
static const Clay_Color COLOR_BLUE = (Clay_Color) {111, 173, 162, 255};
static const Clay_Color COLOR_LIGHT = (Clay_Color) {224, 215, 210, 255};

void init();
void frame();
void cleanup();
void input(const sapp_event* event);

static void Label(Clay_String text) {
    CLAY(CLAY_LAYOUT({ .padding = {16, 8} }),
        CLAY_RECTANGLE({ .color = Clay_Hovered() ? COLOR_BLUE : COLOR_ORANGE })
    ) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({
            .textColor = { 255, 255, 255, 255 },
            .fontId = FONT_ID_BODY_24,
            .fontSize = 24,
        }));
    }
}

static Clay_RenderCommandArray CreateLayout() {
    Clay_BeginLayout();
    CLAY(CLAY_ID("MainContent"),
        CLAY_LAYOUT({
            .sizing = {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW(),
            },
            .childAlignment = {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER,
            }
        }),
        CLAY_RECTANGLE({
            .color = COLOR_LIGHT,
        })
    ) {
        Label(CLAY_STRING("Hello, World!"));
    }
    return Clay_EndLayout();
}

Clay_Dimensions measureText(Clay_String *text, Clay_TextElementConfig *config);
void render(Clay_RenderCommandArray renderCommands);

int main(int argc, char** args) {
    //-----------------------------------------------------------------------
    // Setup Clay
    //-----------------------------------------------------------------------
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena) {
        .label = CLAY_STRING("Clay Memory Arena"),
        .capacity = totalMemorySize,
        .memory = (char*)malloc(totalMemorySize),
    };

    Clay_SetMeasureTextFunction(measureText);

    Clay_Initialize(clayMemory, (Clay_Dimensions) { SCREEN_WIDTH, SCREEN_HEIGHT });

    //-----------------------------------------------------------------------
    // Setup Sokol
    //-----------------------------------------------------------------------
    sapp_desc desc = {0};
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup,
    desc.event_cb = input,
    desc.width  = SCREEN_WIDTH,
    desc.height = SCREEN_HEIGHT,
    desc.window_title = "sokol + puredoom",
    desc.icon.sokol_default = true,
    desc.logger.func = slog_func;
    sapp_run(&desc);

    return 0;
}

void init() {
    sg_desc desc = {0};
    desc.environment = sglue_environment();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    pass_action.colors[0] = (sg_color_attachment_action){ .load_action=SG_LOADACTION_CLEAR, .clear_value={0.2f, 0.1f, 0.3f, 1.0f} };

    simgui_desc_t simgui_desc = {0};
    simgui_setup(&simgui_desc);
}

void frame() {
    // const double dt = sapp_frame_duration();

    const int width = sapp_width();
    const int height = sapp_height();
    simgui_new_frame({ width, height, sapp_frame_duration(), sapp_dpi_scale() });

    // imgui
    {
        // ImGui::ShowDemoWindow();
        ImGui::SetNextWindowSize(ImVec2{SCREEN_WIDTH, SCREEN_HEIGHT});
        ImGui::SetNextWindowPos(ImVec2{0, 0});
        ImGui::Begin("Text rendering", NULL, ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoSavedSettings);

        Clay_RenderCommandArray renderCommands = CreateLayout();
        render(renderCommands);

        ImGui::End();
    }

    sg_begin_pass({ .action = pass_action, .swapchain = sglue_swapchain() });
    simgui_render();
    sg_end_pass();
    sg_commit();
}

void cleanup() {
    simgui_shutdown();
    sg_shutdown();
}

void input(const sapp_event* event) {
    simgui_handle_event(event);
}

Clay_Dimensions measureText(Clay_String *text, Clay_TextElementConfig *config)
{
    ImVec2 size = ImGui::CalcTextSize(text->chars, nullptr);
    return (Clay_Dimensions) {
        .width = size.x,
        .height = size.y,
    };
}


void render(Clay_RenderCommandArray renderCommands)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();

    for (uint32_t i = 0; i < renderCommands.length; i++)
    {
        Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        Clay_BoundingBox boundingBox = renderCommand->boundingBox;
        switch (renderCommand->commandType)
        {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleElementConfig *config = renderCommand->config.rectangleElementConfig;
                Clay_Color color = config->color;

                draw_list->AddRectFilled(p+ImVec2(boundingBox.x, boundingBox.y), p+ImVec2(boundingBox.x+boundingBox.width, boundingBox.y+boundingBox.height), ImColor(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f));

                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextElementConfig *config = renderCommand->config.textElementConfig;

                draw_list->AddText(p+ImVec2(boundingBox.x, boundingBox.y), ImColor(config->textColor.r/255.0f, config->textColor.g/255.0f, config->textColor.b/255.0f, config->textColor.a/255.0f), renderCommand->text.chars);

                break;
            }
            default: {
                fprintf(stderr, "Error: unhandled render command: %d\n", renderCommand->commandType);
                #ifdef CLAY_OVERFLOW_TRAP
                raise(SIGTRAP);
                #endif
                exit(1);
            }
        }
    }
}
