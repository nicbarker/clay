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
    Clay_SetMeasureTextFunction(sclay_measure_text, NULL);
}

Clay_RenderCommandArray CornerRadiusTest(){
    Clay_BeginLayout();
    Clay_Sizing layoutExpand = {
        .width = CLAY_SIZING_GROW(0),
        .height = CLAY_SIZING_GROW(0)
    };
    CLAY({ .id = CLAY_ID("OuterContainer"),
        .backgroundColor = {43, 41, 51, 255},
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = layoutExpand,
            .padding = {0, 0, 20, 20},
            .childGap = 20
        }
    }) {
        for(int i = 0; i < 6; ++i){
            CLAY({ .id = CLAY_IDI("Row", i),
                .layout = {
                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                    .sizing = layoutExpand,
                    .padding = {20, 20, 0, 0},
                    .childGap = 20
                }
            }) {
                for(int j = 0; j < 6; ++j){
                    CLAY({ .id = CLAY_IDI("Tile", i*6+j),
                        .backgroundColor = {120, 140, 255, 128},
                        .cornerRadius = {(i%3)*15, (j%3)*15, (i/2)*15, (j/2)*15},
                        .border = {
                            .color = {120, 140, 255, 255},
                            .width = {3, 9, 6, 12, 0},
                        },
                        .layout = { .sizing = layoutExpand }
                    });
                }
            }
        }
    }
    return Clay_EndLayout();
}

static void frame() {
    sclay_new_frame();
    Clay_RenderCommandArray renderCommands = CornerRadiusTest();

    sg_begin_pass(&(sg_pass){ .swapchain = sglue_swapchain() });
    sgl_matrix_mode_modelview();
    sgl_load_identity();
    sclay_render(renderCommands, NULL);
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
        .window_title = "Clay - Corner Radius Test",
        .width = 800,
        .height = 600,
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}
