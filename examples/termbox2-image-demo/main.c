/*
    Unlicense

    Copyright (c) 2025 Mivirl

    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or
    distribute this software, either in source code form or as a compiled
    binary, for any purpose, commercial or non-commercial, and by any
    means.

    In jurisdictions that recognize copyright laws, the author or authors
    of this software dedicate any and all copyright interest in the
    software to the public domain. We make this dedication for the benefit
    of the public at large and to the detriment of our heirs and
    successors. We intend this dedication to be an overt act of
    relinquishment in perpetuity of all present and future rights to this
    software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

    For more information, please refer to <https://unlicense.org/>
*/

#define CLAY_IMPLEMENTATION
#include "../../clay.h"
#include "../../renderers/termbox2/clay_renderer_termbox2.c"

#define TB_IMPL
#include "termbox2.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"


// -------------------------------------------------------------------------------------------------
// -- Data structures

struct img_group {
    clay_tb_image thumbnail;
    clay_tb_image image;
    clay_tb_image image_1;
    clay_tb_image image_2;
    int width;
    int height;
};
typedef struct img_group img_group;


// -------------------------------------------------------------------------------------------------
// -- Internal state

// If the program should exit the main render/interaction loop
bool end_loop = false;

// If the debug tools should be displayed
bool debugMode = false;


// -------------------------------------------------------------------------------------------------
// -- Internal utility functions

img_group img_group_load(const char *filename)
{
    img_group rv;
    rv.thumbnail = Clay_Termbox_Image_Load_File(filename);
    rv.image = Clay_Termbox_Image_Load_File(filename);
    rv.image_1 = Clay_Termbox_Image_Load_File(filename);
    rv.image_2 = Clay_Termbox_Image_Load_File(filename);
    if (NULL == rv.thumbnail.pixel_data
        || NULL == rv.image.pixel_data
        || NULL == rv.image_1.pixel_data
        || NULL == rv.image_2.pixel_data) {
        exit(1);
    }
    rv.width = rv.image.pixel_width;
    rv.height = rv.image.pixel_height;
    return rv;
}
void img_group_free(img_group *img)
{
    Clay_Termbox_Image_Free(&img->thumbnail);
    Clay_Termbox_Image_Free(&img->image);
    Clay_Termbox_Image_Free(&img->image_1);
    Clay_Termbox_Image_Free(&img->image_2);
}


// -------------------------------------------------------------------------------------------------
// -- Clay components

void component_text_pair(const char *key, const char *value)
{
    size_t keylen = strlen(key);
    size_t vallen = strlen(value);
    Clay_String keytext = (Clay_String) {
        .length = keylen,
        .chars = key,
    };
    Clay_String valtext = (Clay_String) {
        .length = vallen,
        .chars = value,
    };
    Clay_TextElementConfig *textconfig =
        CLAY_TEXT_CONFIG({ .textColor = { 0xff, 0xff, 0xff, 0xff } });
    CLAY({
        .layout = {
            .sizing = {
                .width = {
                    .size.minMax = {
                        .min = strlen("Border chars CLAY_TB_IMAGE_MODE_UNICODE_FAST") * Clay_Termbox_Cell_Width(),
                    }
                },
            }
        },
    }) {
        CLAY_TEXT(keytext, textconfig);
        CLAY({ .layout = { .sizing = CLAY_SIZING_GROW(1) } }) { }
        CLAY_TEXT(valtext, textconfig);
    }

}

void component_termbox_settings(void)
{
    CLAY({
            .layout = {
                .sizing = CLAY_SIZING_FIT(),
                .padding = {
                    6 * Clay_Termbox_Cell_Width(),
                    6 * Clay_Termbox_Cell_Width(),
                    2 * Clay_Termbox_Cell_Height(),
                    2 * Clay_Termbox_Cell_Height(),
                }
            },
            .border = {
                .width = CLAY_BORDER_ALL(1),
                .color = { 0x00, 0x00, 0x00, 0xff }
            },
            .backgroundColor = { 0x7f, 0x00, 0x00, 0x7f }
    }) {
        const char *color_mode = NULL;
        switch (clay_tb_color_mode) {
            case TB_OUTPUT_NORMAL: {
                color_mode = "TB_OUTPUT_NORMAL";
                break;
            }
            case TB_OUTPUT_256: {
                color_mode = "TB_OUTPUT_256";
                break;
            }
            case TB_OUTPUT_216: {
                color_mode = "TB_OUTPUT_216";
                break;
            }
            case TB_OUTPUT_GRAYSCALE: {
                color_mode = "TB_OUTPUT_GRAYSCALE";
                break;
            }
            case TB_OUTPUT_TRUECOLOR: {
                color_mode = "TB_OUTPUT_TRUECOLOR";
                break;
            }
            case CLAY_TB_OUTPUT_NOCOLOR: {
                color_mode = "CLAY_TB_OUTPUT_NOCOLOR";
                break;
            }
            default: {
                color_mode = "INVALID";
                break;
            }
        }
        const char *border_mode = NULL;
        switch (clay_tb_border_mode) {
            case CLAY_TB_BORDER_MODE_ROUND: {
                border_mode = "CLAY_TB_BORDER_MODE_ROUND";
                break;
            }
            case CLAY_TB_BORDER_MODE_MINIMUM: {
                border_mode = "CLAY_TB_BORDER_MODE_MINIMUM";
                break;
            }
            default: {
                border_mode = "INVALID";
                break;
            }
        }
        const char *border_chars = NULL;
        switch (clay_tb_border_chars) {
            case CLAY_TB_BORDER_CHARS_ASCII: {
                border_chars = "CLAY_TB_BORDER_CHARS_ASCII";
                break;
            }
            case CLAY_TB_BORDER_CHARS_UNICODE: {
                border_chars = "CLAY_TB_BORDER_CHARS_UNICODE";
                break;
            }
            case CLAY_TB_BORDER_CHARS_BLANK: {
                border_chars = "CLAY_TB_BORDER_CHARS_BLANK";
                break;
            }
            case CLAY_TB_BORDER_CHARS_NONE: {
                border_chars = "CLAY_TB_BORDER_CHARS_NONE";
                break;
            }
            default: {
                border_chars = "INVALID";
                break;
            }
        }
        const char *image_mode = NULL;
        switch (clay_tb_image_mode) {
            case CLAY_TB_IMAGE_MODE_PLACEHOLDER: {
                image_mode = "CLAY_TB_IMAGE_MODE_PLACEHOLDER";
                break;
            }
            case CLAY_TB_IMAGE_MODE_BG: {
                image_mode = "CLAY_TB_IMAGE_MODE_BG";
                break;
            }
            case CLAY_TB_IMAGE_MODE_ASCII_FG: {
                image_mode = "CLAY_TB_IMAGE_MODE_ASCII_FG";
                break;
            }
            case CLAY_TB_IMAGE_MODE_ASCII_FG_FAST: {
                image_mode = "CLAY_TB_IMAGE_MODE_ASCII_FG_FAST";
                break;
            }
            case CLAY_TB_IMAGE_MODE_ASCII: {
                image_mode = "CLAY_TB_IMAGE_MODE_ASCII";
                break;
            }
            case CLAY_TB_IMAGE_MODE_ASCII_FAST: {
                image_mode = "CLAY_TB_IMAGE_MODE_ASCII_FAST";
                break;
            }
            case CLAY_TB_IMAGE_MODE_UNICODE: {
                image_mode = "CLAY_TB_IMAGE_MODE_UNICODE";
                break;
            }
            case CLAY_TB_IMAGE_MODE_UNICODE_FAST: {
                image_mode = "CLAY_TB_IMAGE_MODE_UNICODE_FAST";
                break;
            }
            default: {
                image_mode = "INVALID";
                break;
            }
        }
        const char *transparency = NULL;
        if (clay_tb_transparency) {
            transparency = "true";
        } else {
            transparency = "false";
        }

        CLAY({
            .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM },
        }) {
            component_text_pair("Color mode", color_mode);
            component_text_pair("Border mode", border_mode);
            component_text_pair("Border chars", border_chars);
            component_text_pair("Image mode", image_mode);
            component_text_pair("Transparency", transparency);
        }
    }
}

void component_keybinds(void)
{
    CLAY({
        .layout = {
            .sizing = CLAY_SIZING_FIT(),
            .padding = {
                4 * Clay_Termbox_Cell_Width(),
                4 * Clay_Termbox_Cell_Width(),
                2 * Clay_Termbox_Cell_Height(),
                2 * Clay_Termbox_Cell_Height(),
            }
        },
        .backgroundColor = { 0x00, 0x7f, 0x7f, 0xff }
    }) {
        CLAY_TEXT(
            CLAY_STRING(
                "Termbox2 renderer test\n"
                "\n"
                "Keybinds:\n"
                "  up/down arrows - Change selected image\n"
                "  c/C - Cycle through color modes\n"
                "  b/B - Cycle through border modes\n"
                "  h/H - Cycle through border characters\n"
                "  i/I - Cycle through image modes\n"
                "  t/T - Toggle transparency\n"
                "  d/D - Toggle debug mode\n"
                "  q/Q - Quit\n"
            ),
            CLAY_TEXT_CONFIG({ .textColor = { 0xff, 0xff, 0xff, 0xff }})
        );
    }
}

void component_image(img_group *img_pair)
{
    CLAY({
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW()
            },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .childAlignment = {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER
            },
            .childGap = 1 * Clay_Termbox_Cell_Height()
        },
        .backgroundColor = { 0x24, 0x24, 0x24, 0xff }
    }) {
        CLAY({
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                },
            },
            .image = {
                .imageData = &img_pair->image,
            },
            .aspectRatio = { (float)img_pair->width / img_pair->height }
        }) { }
        component_keybinds();
    }
}

void component_image_small(img_group **img_pairs, int count, int selected_index)
{
    CLAY({
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_PERCENT(0.25),
            },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .childGap = 20,
            .childAlignment = {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER
            },
        },
    }) {
        CLAY({
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.7),
                },
            },
            .image = {
                .imageData = &img_pairs[selected_index]->image_1,
            },
            .aspectRatio = { (float)img_pairs[selected_index]->width / img_pairs[selected_index]->height }
        }) { }
        CLAY({
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                },
            },
            .image = {
                .imageData = &img_pairs[selected_index]->image_2,
            },
            .aspectRatio = { (float)img_pairs[selected_index]->width / img_pairs[selected_index]->height }
        }) { }
        component_termbox_settings();
    }
}

void component_thumbnails(img_group **img_pairs, int count, int selected_index)
{
    CLAY({
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_PERCENT(0.1),
                .height = CLAY_SIZING_GROW()
            },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .childGap = 20
        },
        .backgroundColor = { 0x42, 0x42, 0x42, 0xff }
    }) {
        for (int i = 0; i < count; ++i) {
            Clay_BorderElementConfig border;
            if (i == selected_index) {
                border = (Clay_BorderElementConfig) {
                    .width =CLAY_BORDER_OUTSIDE(10),
                    .color = { 0x00, 0x30, 0xc0, 0x8f }
                };
            } else {
                border = (Clay_BorderElementConfig) {
                    .width = CLAY_BORDER_OUTSIDE(0),
                };
            }
            CLAY({
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_GROW(),
                    },
                },
                .border = border,
                .image = {
                    .imageData = &img_pairs[i]->thumbnail,
                },
                .aspectRatio = { (float)img_pairs[i]->width / img_pairs[i]->height }
            }) { }
        }
    }
}

int selected_thumbnail = 0;
const int thumbnail_count = 5;
Clay_RenderCommandArray CreateLayout(struct img_group **imgs)
{
    Clay_BeginLayout();
    CLAY({
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW()
            },
            .childAlignment = {
                .x = CLAY_ALIGN_X_LEFT,
                .y = CLAY_ALIGN_Y_CENTER
            },
            .childGap = 64
        },
        .backgroundColor = { 0x24, 0x24, 0x24, 0xff }
    }) {
        component_thumbnails(imgs, thumbnail_count, selected_thumbnail);
        component_image_small(imgs, thumbnail_count, selected_thumbnail);
        component_image(imgs[selected_thumbnail]);
    }
    return Clay_EndLayout();
}


// -------------------------------------------------------------------------------------------------
// -- Interactive functions

void handle_clay_errors(Clay_ErrorData errorData)
{
    Clay_Termbox_Close();
    fprintf(stderr, "%s", errorData.errorText.chars);
    exit(1);
}

/**
  Process events received from termbox2 and handle interaction
 */
void handle_termbox_events(void)
{
    // Wait up to 100ms for an event (key/mouse press, terminal resize) before continuing
    // If an event is already available, this doesn't wait. Will not wait due to the previous call
    // to termbox_waitfor_event. Increasing the wait time reduces load without reducing
    // responsiveness (but will of course prevent other code from running on this thread while it's
    // waiting)
    struct tb_event evt;
    int ms_to_wait = 0;
    int err = tb_peek_event(&evt, ms_to_wait);

    switch (err) {
        default:
        case TB_ERR_NO_EVENT: {
            break;
        }
        case TB_ERR_POLL: {
            if (EINTR != tb_last_errno()) {
                Clay_Termbox_Close();
                fprintf(stderr, "Failed to read event from TTY\n");
                exit(1);
            }
            break;
        }
        case TB_OK: {
            switch (evt.type) {
                case TB_EVENT_RESIZE: {
                    Clay_SetLayoutDimensions((Clay_Dimensions) {
                        Clay_Termbox_Width(),
                        Clay_Termbox_Height()
                    });
                    break;
                }
                case TB_EVENT_KEY: {
                    if (TB_KEY_CTRL_C == evt.key) {
                        end_loop = true;
                        break;
                    }
                    if (0 != evt.ch) {
                        switch (evt.ch) {
                            case 'q':
                            case 'Q': {
                                end_loop = true;
                                break;
                            }
                            case 'd':
                            case 'D': {
                                debugMode = !debugMode;
                                Clay_SetDebugModeEnabled(debugMode);
                                break;
                            }
                            case 'c': {
                                int new_mode = clay_tb_color_mode - 1;
                                new_mode = (0 <= new_mode) ? new_mode : TB_OUTPUT_TRUECOLOR;
                                Clay_Termbox_Set_Color_Mode(new_mode);
                                break;
                            }
                            case 'C': {
                                int new_mode = (clay_tb_color_mode + 1) % (TB_OUTPUT_TRUECOLOR + 1);
                                Clay_Termbox_Set_Color_Mode(new_mode);
                                break;
                            }
                            case 'b': {
                                enum border_mode new_mode = clay_tb_border_mode - 1;
                                new_mode = (CLAY_TB_BORDER_MODE_DEFAULT < new_mode)
                                    ? new_mode
                                    : CLAY_TB_BORDER_MODE_MINIMUM;
                                Clay_Termbox_Set_Border_Mode(new_mode);
                                break;
                            }
                            case 'B': {
                                enum border_mode new_mode = (clay_tb_border_mode + 1)
                                    % (CLAY_TB_BORDER_MODE_MINIMUM + 1);
                                new_mode = (CLAY_TB_BORDER_MODE_DEFAULT < new_mode)
                                    ? new_mode
                                    : CLAY_TB_BORDER_MODE_ROUND;
                                Clay_Termbox_Set_Border_Mode(new_mode);
                                break;
                            }
                            case 'h': {
                                enum border_chars new_chars = clay_tb_border_chars - 1;
                                new_chars = (CLAY_TB_BORDER_CHARS_DEFAULT < new_chars)
                                    ? new_chars
                                    : CLAY_TB_BORDER_CHARS_NONE;
                                Clay_Termbox_Set_Border_Chars(new_chars);
                                break;
                            }
                            case 'H': {
                                enum border_chars new_chars
                                    = (clay_tb_border_chars + 1) % (CLAY_TB_BORDER_CHARS_NONE + 1);
                                new_chars = (CLAY_TB_BORDER_CHARS_DEFAULT < new_chars)
                                    ? new_chars
                                    : CLAY_TB_BORDER_CHARS_ASCII;
                                Clay_Termbox_Set_Border_Chars(new_chars);
                                break;
                            }
                            case 'i': {
                                enum image_mode new_mode = clay_tb_image_mode - 1;
                                new_mode = (CLAY_TB_IMAGE_MODE_DEFAULT < new_mode)
                                    ? new_mode
                                    : CLAY_TB_IMAGE_MODE_UNICODE_FAST;
                                Clay_Termbox_Set_Image_Mode(new_mode);
                                break;
                            }
                            case 'I': {
                                enum image_mode new_mode = (clay_tb_image_mode + 1)
                                    % (CLAY_TB_IMAGE_MODE_UNICODE_FAST + 1);
                                new_mode = (CLAY_TB_IMAGE_MODE_DEFAULT < new_mode)
                                    ? new_mode
                                    : CLAY_TB_IMAGE_MODE_PLACEHOLDER;
                                Clay_Termbox_Set_Image_Mode(new_mode);
                                break;
                            }
                            case 't':
                            case 'T': {
                                Clay_Termbox_Set_Transparency(!clay_tb_transparency);
                            }
                        }
                    } else if (0 != evt.key) {
                        switch (evt.key) {
                            case TB_KEY_ARROW_UP: {
                                selected_thumbnail = (selected_thumbnail > 0) ? selected_thumbnail - 1 : 0;
                                break;
                            }
                            case TB_KEY_ARROW_DOWN: {
                                selected_thumbnail = (selected_thumbnail < thumbnail_count - 1) ? selected_thumbnail + 1 : thumbnail_count - 1;
                                break;
                            }
                        }
                    }
                    break;
                }
                case TB_EVENT_MOUSE: {
                    Clay_Vector2 mousePosition = {
                        (float)evt.x * Clay_Termbox_Cell_Width(),
                        (float)evt.y * Clay_Termbox_Cell_Height()
                    };

                    // Mouse release events may not be produced by all terminals, and will
                    // be sent during hover, so can't be used to detect when the mouse has
                    // been released

                    switch (evt.key) {
                        case TB_KEY_MOUSE_LEFT: {
                            Clay_SetPointerState(mousePosition, true);
                            break;
                        }
                        case TB_KEY_MOUSE_RIGHT: {
                            Clay_SetPointerState(mousePosition, false);
                            break;
                        }
                        case TB_KEY_MOUSE_MIDDLE: {
                            Clay_SetPointerState(mousePosition, false);
                            break;
                        }
                        case TB_KEY_MOUSE_RELEASE: {
                            Clay_SetPointerState(mousePosition, false);
                            break;
                        }
                        case TB_KEY_MOUSE_WHEEL_UP: {
                            Clay_Vector2 scrollDelta = { 0, 1 * Clay_Termbox_Cell_Height() };
                            Clay_UpdateScrollContainers(false, scrollDelta, 1);
                            break;
                        }
                        case TB_KEY_MOUSE_WHEEL_DOWN: {
                            Clay_Vector2 scrollDelta = { 0, -1 * Clay_Termbox_Cell_Height() };
                            Clay_UpdateScrollContainers(false, scrollDelta, 1);
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            break;
        }
    }
}

int main(void)
{
    img_group *imgs[thumbnail_count];
    img_group img_shark = img_group_load("resources/512px-Shark_antwerp_zoo.jpeg");
    img_group img_castle = img_group_load("resources/512px-Balmoral_Castle_30_July_2011.jpeg");
    img_group img_dog = img_group_load("resources/512px-German_Shepherd_(aka_Alsatian_and_Alsatian_Wolf_Dog),_Deutscher_Schäferhund_(Folder_(IV)_22.jpeg");
    img_group img_rosa = img_group_load("resources/512px-Rosa_Cubana_2018-09-21_1610.jpeg");
    img_group img_vorderer = img_group_load("resources/512px-Vorderer_Graben_10_Bamberg_20190830_001.jpeg");
    imgs[0] = &img_shark;
    imgs[1] = &img_castle;
    imgs[2] = &img_dog;
    imgs[3] = &img_rosa;
    imgs[4] = &img_vorderer;

    int num_elements = 3 * 8192;
    Clay_SetMaxElementCount(num_elements);

    uint64_t size = Clay_MinMemorySize();
    void *memory = malloc(size);
    if (NULL == memory) { exit(1); }

    Clay_Arena clay_arena = Clay_CreateArenaWithCapacityAndMemory(size, memory);

    Clay_Termbox_Initialize(
        TB_OUTPUT_256, CLAY_TB_BORDER_MODE_DEFAULT, CLAY_TB_BORDER_CHARS_DEFAULT, CLAY_TB_IMAGE_MODE_DEFAULT, false);

    Clay_Initialize(clay_arena, (Clay_Dimensions) { Clay_Termbox_Width(), Clay_Termbox_Height() },
        (Clay_ErrorHandler) { handle_clay_errors, NULL });

    Clay_SetMeasureTextFunction(Clay_Termbox_MeasureText, NULL);

    // Initial render before waiting for events
    Clay_RenderCommandArray commands = CreateLayout(imgs);
    Clay_Termbox_Render(commands);
    tb_present();

    while (!end_loop) {
        // Block until event is available. Optional, but reduces load since this demo is purely
        // synchronous to user input.
        Clay_Termbox_Waitfor_Event();

        handle_termbox_events();

        commands = CreateLayout(imgs);

        Clay_Termbox_Render(commands);
        tb_present();
    }

    Clay_Termbox_Close();
    img_group_free(&img_shark);
    img_group_free(&img_castle);
    img_group_free(&img_dog);
    img_group_free(&img_rosa);
    img_group_free(&img_vorderer);
    free(memory);
    return 0;
}
