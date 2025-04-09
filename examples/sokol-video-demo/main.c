#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#include "util/sokol_gl.h"
#include "fontstash.h"
#include "util/sokol_fontstash.h"
#define SOKOL_CLAY_IMPL
#include "../../renderers/sokol/sokol_clay.h"

#include "../shared-layouts/clay-video-demo.c"

static ClayVideoDemo_Data demoData;
static sclay_font_t fonts[1];

static void init() {
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });
    sgl_setup(&(sgl_desc_t){
        .logger.func = slog_func,
    });
    sclay_setup();
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(clayMemory, (Clay_Dimensions){ (float)sapp_width(), (float)sapp_height() }, (Clay_ErrorHandler){0});
    fonts[FONT_ID_BODY_16] = sclay_add_font("resources/Roboto-Regular.ttf");
    Clay_SetMeasureTextFunction(sclay_measure_text, &fonts);
    demoData = ClayVideoDemo_Initialize();
}

static void frame() {
    sclay_new_frame();
    Clay_RenderCommandArray renderCommands = ClayVideoDemo_CreateLayout(&demoData);

    sg_begin_pass(&(sg_pass){ .swapchain = sglue_swapchain() });
    sgl_matrix_mode_modelview();
    sgl_load_identity();
    sclay_render(renderCommands, fonts);
    sgl_draw();
    sg_end_pass();
    sg_commit();
}

static void event(const sapp_event *ev) {
    if(ev->type == SAPP_EVENTTYPE_KEY_DOWN && ev->key_code == SAPP_KEYCODE_D){
        Clay_SetDebugModeEnabled(true);
    } else {
        sclay_handle_event(ev);
    }
}

static void cleanup() {
    sclay_shutdown();
    sgl_shutdown();
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char **argv) {
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .event_cb = event,
        .cleanup_cb = cleanup,
        .window_title = "Clay - Sokol Renderer Example",
        .width = 800,
        .height = 600,
        .high_dpi = true,
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}
