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
// -- Internal state

// If the program should exit the main render/interaction loop
bool end_loop = false;

// If the debug tools should be displayed
bool debugMode = false;


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
        .floating = {
            .attachTo = CLAY_ATTACH_TO_PARENT,
            .zIndex = 1,
            .attachPoints = { CLAY_ATTACH_POINT_CENTER_CENTER, CLAY_ATTACH_POINT_CENTER_TOP },
            .offset = { 0, 0 }
        },
    }) {
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
}

void component_color_palette(void)
{
    CLAY({
        .layout = {
            .childGap = 16,
            .padding = {
                2 * Clay_Termbox_Cell_Width(),
                2 * Clay_Termbox_Cell_Width(),
                2 * Clay_Termbox_Cell_Height(),
                2 * Clay_Termbox_Cell_Height(),
            }
        },
        .border = {
            .width = CLAY_BORDER_OUTSIDE(2),
            .color = { 0x00, 0x00, 0x00, 0xff }
        },
        .backgroundColor = { 0x7f, 0x7f, 0x7f, 0xff }
    }) {
        for (int type = 0; type < 2; ++type) {
            CLAY({
                .layout ={
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .sizing = CLAY_SIZING_FIT(),
                    .childGap = Clay_Termbox_Cell_Height()
                },
            }) {
                for (float ri = 0; ri < 4; ri += 1) {
                    CLAY({
                        .layout ={
                            .sizing = CLAY_SIZING_FIT(),
                            .childGap = Clay_Termbox_Cell_Width()
                        },
                    }) {
                        for (float r = ri * 0x44; r < (ri + 1) * 0x44; r += 0x22) {
                            CLAY({
                                .layout ={
                                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                    .sizing = CLAY_SIZING_FIT(),
                                },
                            }) {
                                for (float g = 0; g < 0xff; g += 0x22) {
                                    CLAY({
                                        .layout ={
                                            .sizing = CLAY_SIZING_FIT(),
                                        },
                                    }) {
                                        for (float b = 0; b < 0xff; b += 0x22) {
                                            Clay_Color color = { r, g, b, 0x7f };

                                            Clay_LayoutConfig layout = (Clay_LayoutConfig) {
                                                .sizing = {
                                                    .width = CLAY_SIZING_FIXED(2 * Clay_Termbox_Cell_Width()),
                                                    .height = CLAY_SIZING_FIXED(1 * Clay_Termbox_Cell_Height())
                                                }
                                            };
                                            if (0 == type) {
                                                CLAY({
                                                    .layout = layout,
                                                    .backgroundColor = color
                                                }) {}
                                            } else if (1 == type) {
                                                CLAY({
                                                    .layout = layout,
                                                }) {
                                                    CLAY_TEXT(CLAY_STRING("#"), CLAY_TEXT_CONFIG({ .textColor = color }));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void component_unicode_text(void)
{
    CLAY({
        .layout = {
            .sizing = CLAY_SIZING_FIT(),
            .padding = {
                2 * Clay_Termbox_Cell_Width(),
                2 * Clay_Termbox_Cell_Width(),
                2 * Clay_Termbox_Cell_Height(),
                2 * Clay_Termbox_Cell_Height(),
            }
        },
        .backgroundColor = { 0xcc, 0xbb, 0xaa, 0xff },
        .border = {
            // This border should still be displayed in CLAY_TB_BORDER_MODE_ROUND mode
            .width = {
                0.75 * Clay_Termbox_Cell_Width(),
                0.75 * Clay_Termbox_Cell_Width(),
                0.75 * Clay_Termbox_Cell_Height(),
                0.75 * Clay_Termbox_Cell_Height(),
            },
            .color = { 0x33, 0x33, 0x33, 0xff },
        },
    }) {
        CLAY_TEXT(
            CLAY_STRING("Non-ascii character tests:\n"
                "\n"
                "(from https://www.w3.org/2001/06/utf-8-test/UTF-8-demo.html)\n"
                " Mathematics and Sciences:\n"
                "  ∮ E⋅da = Q,  n → ∞, ∑ f(i) = ∏ g(i), ∀x∈ℝ: ⌈x⌉ = −⌊−x⌋, α ∧ ¬β = ¬(¬α ∨ β),\n"
                "  ℕ ⊆ ℕ₀ ⊂ ℤ ⊂ ℚ ⊂ ℝ ⊂ ℂ, ⊥ < a ≠ b ≡ c ≤ d ≪ ⊤ ⇒ (A ⇔ B),\n"
                "  2H₂ + O₂ ⇌ 2H₂O, R = 4.7 kΩ, ⌀ 200 mm\n"
                "\n"
                " Compact font selection example text:\n"
                "  ABCDEFGHIJKLMNOPQRSTUVWXYZ /0123456789\n"
                "  abcdefghijklmnopqrstuvwxyz £©µÀÆÖÞßéöÿ\n"
                "  –—‘“”„†•…‰™œŠŸž€ ΑΒΓΔΩαβγδω АБВГДабвгд\n"
                "  ∀∂∈ℝ∧∪≡∞ ↑↗↨↻⇣ ┐┼╔╘░►☺♀ ﬁ�⑀₂ἠḂӥẄɐː⍎אԱა\n"
                "\n"
                "(from https://blog.denisbider.com/2015/09/when-monospace-fonts-arent-unicode.html):\n"
                " aeioucsz\n"
                " áéíóúčšž\n"
                " 台北1234\n"
                " ＱＲＳ12\n"
                " ｱｲｳ1234\n"
                "\n"
                "(from https://stackoverflow.com/a/1644280)\n"
                " ٩(-̮̮̃-̃)۶ ٩(●̮̮̃•̃)۶ ٩(͡๏̯͡๏)۶ ٩(-̮̮̃•̃)."
            ),
            CLAY_TEXT_CONFIG({ .textColor = { 0x11, 0x11, 0x11, 0xff } })
        );
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

void component_image(clay_tb_image *image, int width)
{
    CLAY({
        .layout = {
            .sizing = {
                .width = (0 == width) ? CLAY_SIZING_GROW() : CLAY_SIZING_FIXED(width),
            },
        },
        .image = {
            .imageData = image,
        },
        .aspectRatio = { 512.0 / 406.0 }
    }) { }
}

void component_mouse_data(void)
{
    CLAY({
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(),
            },
        },
    }) {
        Clay_Context* context = Clay_GetCurrentContext();
        Clay_Vector2 mouse_position = context->pointerInfo.position;

        Clay_LayoutConfig layout = (Clay_LayoutConfig) {
            .sizing = {
                .width = CLAY_SIZING_FIXED(2 * Clay_Termbox_Cell_Width()),
                .height = CLAY_SIZING_FIXED(1 * Clay_Termbox_Cell_Height())
            }
        };

        float v = 255 * mouse_position.x / Clay_Termbox_Width();
        v = (0 > v) ? 0 : v;
        v = (255 < v) ? 255 : v;
        Clay_Color color = (Clay_Color) { v, v, v, 0xff };

        CLAY({
            .layout = layout,
            .backgroundColor = color
        }) {}

        v = 255 * mouse_position.y / Clay_Termbox_Height();
        v = (0 > v) ? 0 : v;
        v = (255 < v) ? 255 : v;
        color = (Clay_Color) { v, v, v, 0xff };


        CLAY({
            .layout = layout,
            .backgroundColor = color
        }) {}

    }
}

void component_bordered_text(void)
{
    CLAY({
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = {
                .width = CLAY_SIZING_FIT(450),
                .height = CLAY_SIZING_FIT(),
            },
            .padding = CLAY_PADDING_ALL(32)
        },
        .backgroundColor = { 0x24, 0x55, 0x34, 0xff },
    }) {
        CLAY({
            .border = { .width = { 1, 1, 1, 1, 1 }, .color = { 0xaa, 0x00, 0x00, 0xff } },
        }) {
            CLAY_TEXT(
                CLAY_STRING("Test"), CLAY_TEXT_CONFIG({ .textColor = { 0xff, 0xff, 0xff, 0xff } }));
        }
        CLAY({
            .border = { .width = { 1, 1, 1, 1, 1 }, .color = { 0x00, 0xaa, 0x00, 0xff } },
        }) {
            CLAY_TEXT(CLAY_STRING("of the border width"),
                CLAY_TEXT_CONFIG({ .textColor = { 0xff, 0xff, 0xff, 0xff } }));
        }
        CLAY({
            .border = { .width = { 1, 1, 1, 1, 1 }, .color = { 0x00, 0x00, 0xaa, 0xff } },
        }) {
            CLAY_TEXT(CLAY_STRING("and overlap for multiple lines\nof text"),
                CLAY_TEXT_CONFIG({ .textColor = { 0xff, 0xff, 0xff, 0xff } }));
        }
        CLAY({
            .border = { .width = { 1, 1, 1, 1, 1 }, .color = { 0x00, 0x00, 0xaa, 0xff } },
        }) {
            CLAY_TEXT(CLAY_STRING("this text\nis long enough\nto display all\n borders\naround it"),
                CLAY_TEXT_CONFIG({ .textColor = { 0xff, 0xff, 0xff, 0xff } }));
        }
    }
}

Clay_RenderCommandArray CreateLayout(clay_tb_image *image1, clay_tb_image *image2)
{
    Clay_BeginLayout();
    CLAY({
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW()
            },
            .childAlignment = {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER
            },
            .childGap = 64
        },
        .backgroundColor = { 0x24, 0x24, 0x24, 0xff }
    }) {
        CLAY({
            .layout = {
                .childAlignment = {
                    .x = CLAY_ALIGN_X_RIGHT,
                },
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = CLAY_SIZING_FIT(),
            },
        }) {
            component_keybinds();
            component_unicode_text();
        }
        CLAY({
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childGap = 32,
                .sizing = CLAY_SIZING_FIT(),
            },
        }) {
            component_termbox_settings();
            component_image(image1, 150);
            component_image(image2, 0);
            component_mouse_data();
            component_bordered_text();
        }

        component_color_palette();
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
    clay_tb_image shark_image1 = Clay_Termbox_Image_Load_File("resources/512px-Shark_antwerp_zoo.jpeg");
    clay_tb_image shark_image2 = Clay_Termbox_Image_Load_File("resources/512px-Shark_antwerp_zoo.jpeg");
    if (NULL == shark_image1.pixel_data) { exit(1); }
    if (NULL == shark_image2.pixel_data) { exit(1); }

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
    Clay_RenderCommandArray commands = CreateLayout(&shark_image1, &shark_image2);
    Clay_Termbox_Render(commands);
    tb_present();

    while (!end_loop) {
        // Block until event is available. Optional, but reduces load since this demo is purely
        // synchronous to user input.
        Clay_Termbox_Waitfor_Event();

        handle_termbox_events();

        commands = CreateLayout(&shark_image1, &shark_image2);

        tb_clear();
        Clay_Termbox_Render(commands);
        tb_present();
    }

    Clay_Termbox_Close();
    Clay_Termbox_Image_Free(&shark_image1);
    Clay_Termbox_Image_Free(&shark_image2);
    free(memory);
    return 0;
}
