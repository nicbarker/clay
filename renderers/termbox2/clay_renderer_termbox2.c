/*
    zlib/libpng license

    Copyright (c) 2025 Mivirl

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in a
        product, an acknowledgment in the product documentation would be
        appreciated but is not required.

        2. Altered source versions must be plainly marked as such, and must not
        be misrepresented as being the original software.

        3. This notice may not be removed or altered from any source
        distribution.
*/

#include "../../clay.h"

#define TB_OPT_ATTR_W 32 // Required for truecolor support
#include "termbox2.h"


// -------------------------------------------------------------------------------------------------
// -- Data structures

typedef struct {
    int width, height;
} clay_tb_dimensions;

typedef struct {
    float width, height;
} clay_tb_pixel_dimensions;

typedef struct {
    int x, y;
    int width, height;
} clay_tb_cell_bounding_box;

typedef struct {
    Clay_Color clay;
    uintattr_t termbox;
} color_pair;

enum border_mode {
    CLAY_TB_BORDER_MODE_DEFAULT,
    CLAY_TB_BORDER_MODE_ROUND,
    CLAY_TB_BORDER_MODE_MINIMUM,
};

enum border_chars {
    CLAY_TB_BORDER_CHARS_DEFAULT,
    CLAY_TB_BORDER_CHARS_ASCII,
    CLAY_TB_BORDER_CHARS_UNICODE,
    CLAY_TB_BORDER_CHARS_BLANK,
    CLAY_TB_BORDER_CHARS_NONE,
};

// Truecolor is only enabled if TB_OPT_ATTR_W is set to 32 or 64. The default is 16, so it must be
// defined to reference the constant
#ifndef TB_OUTPUT_TRUECOLOR
#define TB_OUTPUT_TRUECOLOR (TB_OUTPUT_GRAYSCALE + 1)
#endif

// Constant that doesn't collide with termbox2's existing output modes
#define CLAY_TB_OUTPUT_NOCOLOR 0

#if !(defined NDEBUG || defined CLAY_TB_NDEBUG)
#define clay_tb_assert(condition, ...)                                                    \
    if (!(condition)) {                                                                   \
        Clay_Termbox_Close();                                                             \
        fprintf(stderr, "%s %d (%s): Assertion failure: ", __FILE__, __LINE__, __func__); \
        fprintf(stderr, __VA_ARGS__);                                                     \
        fprintf(stderr, "\n");                                                            \
        exit(1);                                                                          \
    }
#else
#define clay_tb_assert(condition, ...)
#endif // NDEBUG || CLAY_TB_NDEBUG


// -------------------------------------------------------------------------------------------------
// -- Public API

/**
  Set the equivalent size for a terminal cell in pixels.

  This size is used to convert Clay's pixel measurements to terminal cells, and
  affects scaling.

  Default dimensions were measured on Debian 12: (9, 21)

  \param width Width of a terminal cell in pixels
  \param height Height of a terminal cell in pixels
*/
void Clay_Termbox_Set_Cell_Pixel_Size(float width, float height);

/**
  Sets the color rendering mode for the terminal

  \param color_mode Termbox output mode as defined in termbox2.h, excluding truecolor
                     - TB_OUTPUT_NORMAL - Use default ANSI colors
                     - TB_OUTPUT_256 - Use 256 terminal colors
                     - TB_OUTPUT_216 - Use 216 terminal colors from 256 color mode
                     - TB_OUTPUT_GRAYSCALE - Use 24 gray colors from 256 color mode
                     - CLAY_TB_OUTPUT_NOCOLOR - Don't use ANSI colors at all
 */
void Clay_Termbox_Set_Color_Mode(int color_mode);

/**
  Sets the method for converting the width of borders to terminal cells

  \param border_mode Method for adjusting border sizes to fit terminal cells
                      - CLAY_TB_BORDER_MODE_DEFAULT       - same as CLAY_TB_BORDER_MODE_MINIMUM
                      - CLAY_TB_BORDER_MODE_ROUND         - borders will be rounded to nearest cell
                                                            size
                      - CLAY_TB_BORDER_MODE_MINIMUM       - borders will have a minimum width of one
                                                            cell
 */
void Clay_Termbox_Set_Border_Mode(enum border_mode border_mode);

/**
  Sets the character style to use for rendering borders

  \param border_chars Characters used for rendering borders
                       - CLAY_TB_BORDER_CHARS_DEFAULT - same as BORDER_UNICODE
                       - CLAY_TB_BORDER_CHARS_ASCII   - Uses ascii characters: '+', '|', '-'
                       - CLAY_TB_BORDER_CHARS_UNICODE - Uses unicode box drawing characters
                       - CLAY_TB_BORDER_CHARS_BLANK   - Draws background colors only
                       - CLAY_TB_BORDER_CHARS_NONE    - Don't draw borders
 */
void Clay_Termbox_Set_Border_Chars(enum border_chars border_chars);

/**
  Enables or disables emulated transparency

  If the color mode is TB_OUTPUT_NORMAL or CLAY_TB_OUTPUT_NOCOLOR, transparency will not be enabled

  \param transparency Transparency value to set
 */
void Clay_Termbox_Set_Transparency(bool transparency);

/**
  Current width of the terminal in pixels
*/
float Clay_Termbox_Width(void);

/**
  Current height of the terminal in pixels
*/
float Clay_Termbox_Height(void);

/**
  Current width of a terminal cell in pixels
*/
float Clay_Termbox_Cell_Width(void);

/**
  Current height of a terminal cell in pixels
*/
float Clay_Termbox_Cell_Height(void);

/**
  Callback function used to measure the dimensions in pixels of a text string

  \param text     Text to measure
  \param config   Ignored
  \param userData Ignored
 */
static inline Clay_Dimensions Clay_Termbox_MeasureText(
    Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);

/**
  Set up configuration, start termbox2, and allocate internal structures.

  Configuration can be overriden by environment variables:
  - CLAY_TB_COLOR_MODE
    - NORMAL
    - 256
    - 216
    - GRAYSCALE
    - TRUECOLOR
    - NOCOLOR
  - CLAY_TB_BORDER_CHARS
    - DEFAULT
    - ASCII
    - UNICODE
    - BLANK
    - NONE
  - CLAY_TB_TRANSPARENCY
    - 1
    - 0
  - CLAY_TB_CELL_PIXELS
    - 10x20

  Must be run before using this renderer.

  \param color_mode   Termbox output mode as defined in termbox2.h, excluding truecolor
  \param border_mode  Method for adjusting border sizes to fit terminal cells
  \param border_chars Characters used for rendering borders
  \param transparency Emulate transparency using background colors
 */
void Clay_Termbox_Initialize(int color_mode, enum border_mode border_mode,
    enum border_chars border_chars, bool transparency);

/**
 Stop termbox2 and release internal structures
 */
void Clay_Termbox_Close(void);

/**
  Render a set of commands to the terminal

  \param commands Array of render commands from Clay's CreateLayout() function
 */
void Clay_Termbox_Render(Clay_RenderCommandArray commands);



// -------------------------------------------------------------------------------------------------
// -- Internal state

// Settings/options
static bool clay_tb_initialized = false;
static int clay_tb_color_mode = TB_OUTPUT_NORMAL;
static bool clay_tb_transparency = false;
static enum border_mode clay_tb_border_mode = CLAY_TB_BORDER_MODE_DEFAULT;
static enum border_chars clay_tb_border_chars = CLAY_TB_BORDER_CHARS_DEFAULT;

// Dimensions of a cell are specified in pixels
// Default dimensions were measured from the default terminal on Debian 12:
//  Terminal:  gnome-terminal
//  Font:      "Monospace Regular"
//  Font size: 11
static clay_tb_pixel_dimensions clay_tb_cell_size = { .width = 9, .height = 21 };

// Scissor mode prevents drawing outside of the specified bounding box
static bool clay_tb_scissor_enabled = false;
clay_tb_cell_bounding_box clay_tb_scissor_box;

// -----------------------------------------------
// -- Color buffer

// Buffer storing background colors from previously drawn items. Used to emulate transparency and
// set the background color for text.
static Clay_Color *clay_tb_color_buffer_clay = NULL;
// Dimensions are specified in cells
static clay_tb_dimensions clay_tb_color_buffer_dimensions = { 0, 0 };
static clay_tb_dimensions clay_tb_color_buffer_max_dimensions = { 0, 0 };


// -------------------------------------------------------------------------------------------------
// -- Internal utility functions

static inline bool clay_tb_valid_color(Clay_Color color)
{
    return (
            0x00 <= color.r && color.r <= 0xff &&
            0x00 <= color.g && color.g <= 0xff &&
            0x00 <= color.b && color.b <= 0xff &&
            0x00 <= color.a && color.a <= 0xff
    );
}

/**
 In 256-color mode, there are 216 colors (excluding default terminal colors and gray colors),
 with 6 different magnitudes for each of r, g, b.

 This function clamps to the nearest intensity (represented by 0-5) that can be output in this mode

 Possible intensities per component: 0x00, 0x5f, 0x87, 0xaf, 0xd7, 0xff

 Examples:
 - 0x20  -> 0
 - 0x2f  -> 1
 - 0x85  -> 2
 - 0xff  -> 5

 \param color 8-bit intensity of one RGB component
*/
static int clay_tb_rgb_intensity_to_index(int color)
{
    clay_tb_assert(0x00 <= color && color <= 0xff, "Invalid intensity (allowed range 0x00-0xff)");
    return (color < 0x2f) ? 0
        : (color < 0x73)  ? 1
            : 2 + ((color - 0x73) / 0x28);
}

/**
  Convert an RGB color from Clay's representation to the nearest representable color in the current
  termbox2 output mode

  \param color Color to convert
 */
static uintattr_t clay_tb_color_convert(Clay_Color color)
{
    clay_tb_assert(clay_tb_valid_color(color), "Invalid Clay color: (%f, %f, %f, %f)", color.r,
        color.g, color.b, color.a);

    uintattr_t tb_color = TB_DEFAULT;

    switch (clay_tb_color_mode) {
        default: {
            clay_tb_assert(false, "Invalid or unimplemented Termbox color output mode (%d)",
                clay_tb_color_mode);
            break;
        }
        case TB_OUTPUT_NORMAL: {
            const int color_lut_count = 16;
            const uintattr_t color_lut[][4] = {
                { TB_BLACK,               0x00, 0x00, 0x00 },
                { TB_RED,                 0xaa, 0x00, 0x00 },
                { TB_GREEN,               0x00, 0xaa, 0x00 },
                { TB_YELLOW,              0xaa, 0x55, 0x00 },
                { TB_BLUE,                0x00, 0x00, 0xaa },
                { TB_MAGENTA,             0xaa, 0x00, 0xaa },
                { TB_CYAN,                0x00, 0xaa, 0xaa },
                { TB_WHITE,               0xaa, 0xaa, 0xaa },
                { TB_BLACK   | TB_BRIGHT, 0x55, 0x55, 0x55 },
                { TB_RED     | TB_BRIGHT, 0xff, 0x55, 0x55 },
                { TB_GREEN   | TB_BRIGHT, 0x55, 0xff, 0x55 },
                { TB_YELLOW  | TB_BRIGHT, 0xff, 0xff, 0x55 },
                { TB_BLUE    | TB_BRIGHT, 0x55, 0x55, 0xff },
                { TB_MAGENTA | TB_BRIGHT, 0xff, 0x55, 0xff },
                { TB_CYAN    | TB_BRIGHT, 0x55, 0xff, 0xff },
                { TB_WHITE   | TB_BRIGHT, 0xff, 0xff, 0xff }
            };

            // Find nearest color on the lookup table
            int color_index = 0;
            float min_distance_squared = 0xff * 0xff * 3;
            for (int i = 0; i < color_lut_count; ++i) {
                float r_distance = color.r - (float)color_lut[i][1];
                float g_distance = color.g - (float)color_lut[i][2];
                float b_distance = color.b - (float)color_lut[i][3];

                float distance_squared =
                    (r_distance * r_distance) +
                    (g_distance * g_distance) +
                    (b_distance * b_distance);

                // Penalize pure black and white to display faded colors more often
                if (TB_BLACK == color_lut[i][0] || TB_WHITE == color_lut[i][0]
                    || (TB_BLACK | TB_BRIGHT) == color_lut[i][0]
                    || (TB_WHITE | TB_BRIGHT) == color_lut[i][0]) {
                    distance_squared *= 2;
                }

                if (distance_squared < min_distance_squared) {
                    min_distance_squared = distance_squared;
                    color_index = i;
                }
            }
            tb_color = color_lut[color_index][0];
            break;
        }
        case TB_OUTPUT_216: {
            int r_index = clay_tb_rgb_intensity_to_index((int)color.r);
            int g_index = clay_tb_rgb_intensity_to_index((int)color.g);
            int b_index = clay_tb_rgb_intensity_to_index((int)color.b);

            tb_color = 0x01 + (36 * r_index) + (6 * g_index) + (b_index);
            break;
        }
        case TB_OUTPUT_256: {
            const int index_lut_count = 6;
            const uintattr_t index_lut[] = { 0x00, 0x5f, 0x87, 0xaf, 0xd7, 0xff };

            int r_index = clay_tb_rgb_intensity_to_index((int)color.r);
            int g_index = clay_tb_rgb_intensity_to_index((int)color.g);
            int b_index = clay_tb_rgb_intensity_to_index((int)color.b);

            int rgb_color = 0x10 + (36 * r_index) + (6 * g_index) + (b_index);

            float rgb_r_distance = color.r - (float)index_lut[r_index];
            float rgb_g_distance = color.g - (float)index_lut[g_index];
            float rgb_b_distance = color.b - (float)index_lut[b_index];

            float rgb_distance_squared =
                (rgb_r_distance * rgb_r_distance) +
                (rgb_g_distance * rgb_g_distance) +
                (rgb_b_distance * rgb_b_distance);

            int avg_color = (int)((color.r + color.g + color.b) / 3);
            int gray_avg_color = (avg_color * 24 / 0x100);
            int gray_color = 0xe8 + gray_avg_color;

            float gray_r_distance = color.r - (float)gray_avg_color;
            float gray_g_distance = color.g - (float)gray_avg_color;
            float gray_b_distance = color.b - (float)gray_avg_color;

            float gray_distance_squared =
                (gray_r_distance * gray_r_distance) +
                (gray_g_distance * gray_g_distance) +
                (gray_b_distance * gray_b_distance);

            tb_color = (rgb_distance_squared < gray_distance_squared) ? rgb_color : gray_color;

            break;
        }
        case TB_OUTPUT_GRAYSCALE: {
            // 24 shades of gray
            float avg_color = ((color.r + color.g + color.b) / 3);
            tb_color = 0x01 + (int)(avg_color * 24 / 0x100);
            break;
        }
        case TB_OUTPUT_TRUECOLOR: {
            clay_tb_assert(32 <= TB_OPT_ATTR_W, "Truecolor requires TB_OPT_ATTR_W to be 32 or 64");
            tb_color = ((uintattr_t)color.r << 4 * 4) + ((uintattr_t)color.g << 2 * 4)
                + ((uintattr_t)color.b);
            if (0x000000 == tb_color) {
                tb_color = TB_HI_BLACK;
            }
            break;
        }
        case CLAY_TB_OUTPUT_NOCOLOR: {
            // Uses default terminal colors
            tb_color = TB_DEFAULT;
            break;
        }
    }
    return tb_color;
}

/**
  Round float to nearest integer value

  Used instead of roundf() so math.h doesn't need to be linked

  \param f Float to round
 */
static inline int clay_tb_roundf(float f)
{
    int i = f;
    return (f - i > 0.5f) ? i + 1 : i;
}

/**
  Snap pixel values from Clay to nearest cell values

  Width/height accounts for offset from x/y, so a box at x=(1.2 * cell_width) and
  width=(1.4 * cell_width) is snapped to x=1 and width=2.

  \param box Bounding box with pixel measurements to convert
 */
static inline clay_tb_cell_bounding_box cell_snap_bounding_box(Clay_BoundingBox box)
{
    return (clay_tb_cell_bounding_box) {
        .x = clay_tb_roundf(box.x / clay_tb_cell_size.width),
        .y = clay_tb_roundf(box.y / clay_tb_cell_size.height),
        .width = clay_tb_roundf((box.x + box.width) / clay_tb_cell_size.width)
            - clay_tb_roundf(box.x / clay_tb_cell_size.width),
        .height = clay_tb_roundf((box.y + box.height) / clay_tb_cell_size.height)
            - clay_tb_roundf(box.y / clay_tb_cell_size.height),
    };
}

/**
  Get stored clay color for a position from the internal color buffer

  \param x X position of cell
  \param y Y position of cell
 */
static inline Clay_Color clay_tb_color_buffer_clay_get(int x, int y)
{
    clay_tb_assert(0 <= x && x < clay_tb_color_buffer_dimensions.width,
        "Cell buffer x position (%d) offscreen (range 0-%d)", x,
        clay_tb_color_buffer_dimensions.width);
    clay_tb_assert(0 <= y && y < clay_tb_color_buffer_dimensions.height,
        "Cell buffer y position (%d) offscreen (range 0-%d)", y,
        clay_tb_color_buffer_dimensions.height);
    return clay_tb_color_buffer_clay[x + (y * clay_tb_color_buffer_dimensions.width)];
}

/**
  Set stored clay color for a position in the internal color buffer

  \param x     X position of cell
  \param y     Y position of cell
  \param color Color to store
 */
static inline void clay_tb_color_buffer_clay_set(int x, int y, Clay_Color color)
{
    clay_tb_assert(0 <= x && x < clay_tb_color_buffer_dimensions.width,
        "Cell buffer x position (%d) offscreen (range 0-%d)", x,
        clay_tb_color_buffer_dimensions.width);
    clay_tb_assert(0 <= y && y < clay_tb_color_buffer_dimensions.height,
        "Cell buffer y position (%d) offscreen (range 0-%d)", y,
        clay_tb_color_buffer_dimensions.height);
    clay_tb_color_buffer_clay[x + (y * clay_tb_color_buffer_dimensions.width)] = color;
}

/**
  Resize internal color buffer to the current terminal size
 */
static void clay_tb_resize_buffer(void)
{
    int current_width = tb_width();
    int current_height = tb_height();

    // Reallocate if the new size is larger than the maximum size of the buffer
    size_t max_size = (size_t)clay_tb_color_buffer_max_dimensions.width
        * clay_tb_color_buffer_max_dimensions.height;
    size_t new_size = (size_t)current_width * current_height;
    if (max_size < new_size) {
        Clay_Color *tmp_clay = tb_realloc(clay_tb_color_buffer_clay, sizeof(Clay_Color) * new_size);
        if (NULL == tmp_clay) {
            clay_tb_assert(false, "Reallocation failure for internal clay color buffer");
        }
        clay_tb_color_buffer_clay = tmp_clay;
        for (size_t i = max_size; i < new_size; ++i) {
            clay_tb_color_buffer_clay[i] = (Clay_Color) { 0 };
        }

        clay_tb_color_buffer_max_dimensions.width = current_width;
        clay_tb_color_buffer_max_dimensions.height = current_height;
    }
    clay_tb_color_buffer_dimensions.width = current_width;
    clay_tb_color_buffer_dimensions.height = current_height;
}

/**
  Calculate color at a given position after emulating transparency.

  This isn't true transparency, just the background colors changing to emulate it

  \param color Pair of termbox and clay color representations to overlay with the background color
  \param x     X position of cell
  \param y     Y position of cell
 */
static inline clay_tb_color_pair clay_tb_get_transparency_color(
    int x, int y, clay_tb_color_pair color)
{
    if (!clay_tb_transparency) {
        return color;
    }

    Clay_Color color_bg = clay_tb_color_buffer_clay_get(x, y);
    Clay_Color new_color = {
        .r = color_bg.r + (color.clay.a / 255) * (color.clay.r - color_bg.r),
        .g = color_bg.g + (color.clay.a / 255) * (color.clay.g - color_bg.g),
        .b = color_bg.b + (color.clay.a / 255) * (color.clay.b - color_bg.b),
        .a = 255
    };

    return (clay_tb_color_pair) {
        .clay = new_color,
        .termbox = clay_tb_color_convert(new_color)
    };
}

/**
  Draw a character cell at a position on screen.

  Accounts for scissor mode and stores the cell to the internal color buffer for transparency and
  text backgrounds.

  \param x     X position of cell
  \param y     Y position of cell
  \param ch    Utf32 representation of character to draw
  \param tb_fg Foreground color in termbox representation
  \param tb_bg Background color in termbox representation
  \param fg    Foreground color in clay representation
  \param bg    Background color in clay representation
 */
static int clay_tb_set_cell(
    int x, int y, uint32_t ch, uintattr_t tb_fg, uintattr_t tb_bg, Clay_Color bg)
{
    clay_tb_assert(0 <= x && x < tb_width(), "Cell buffer x position (%d) offscreen (range 0-%d)",
        x, tb_width());
    clay_tb_assert(0 <= y && y < tb_height(), "Cell buffer y position (%d) offscreen (range 0-%d)",
        y, tb_height());

    if (!clay_tb_scissor_enabled
        || (clay_tb_scissor_enabled
            && (clay_tb_scissor_box.x <= x
                && x < clay_tb_scissor_box.x + clay_tb_scissor_box.width)
            && (clay_tb_scissor_box.y <= y
                && y < clay_tb_scissor_box.y + clay_tb_scissor_box.height))) {
        int codepoint_width = tb_wcwidth(ch);
        if (-1 == codepoint_width) {
            // Nonprintable character, use REPLACEMENT CHARACTER (U+FFFD)
            ch = U'\ufffd';
            codepoint_width = tb_wcwidth(ch);
        }

        int err;
        int max_x = CLAY__MIN(x + codepoint_width, tb_width());
        for (int i = x; i < max_x; ++i) {
            clay_tb_color_buffer_clay_set(i, y, bg);
            err = tb_set_cell(i, y, ch, tb_fg, tb_bg);
            if (TB_OK != err) {
                break;
            }
        }

        return err;
    }
    return -1;
}


// -------------------------------------------------------------------------------------------------
// -- Public API implementation

void Clay_Termbox_Set_Cell_Pixel_Size(float width, float height)
{
    clay_tb_assert(0 <= width, "Cell pixel width must be > 0");
    clay_tb_assert(0 <= height, "Cell pixel height must be > 0");
    clay_tb_cell_size = (clay_tb_pixel_dimensions) { .width = width, .height = height };
}

void Clay_Termbox_Set_Color_Mode(int color_mode)
{
    clay_tb_assert(clay_tb_initialized, "Clay_Termbox_Initialize must be run first");
    clay_tb_assert(CLAY_TB_OUTPUT_NOCOLOR <= color_mode && color_mode <= TB_OUTPUT_TRUECOLOR,
        "Color mode invalid (%d)", color_mode);

    if (CLAY_TB_OUTPUT_NOCOLOR == color_mode) {
        tb_set_output_mode(TB_OUTPUT_NORMAL);
    } else {
        tb_set_output_mode(color_mode);
    }

    // Force complete re-render to ensure all colors are redrawn
    tb_invalidate();

    clay_tb_color_mode = color_mode;

    // Re-set transparency value. It will be toggled off if the new output mode doesn't support it
    Clay_Termbox_Set_Transparency(clay_tb_transparency);
}

void Clay_Termbox_Set_Border_Mode(enum border_mode border_mode)
{
    clay_tb_assert(CLAY_TB_BORDER_MODE_DEFAULT <= border_mode
            && border_mode <= CLAY_TB_BORDER_MODE_MINIMUM,
        "Border mode invalid (%d)", border_mode);
    if (CLAY_TB_BORDER_MODE_DEFAULT == border_mode) {
        clay_tb_border_mode = CLAY_TB_BORDER_MODE_MINIMUM;
    } else {
        clay_tb_border_mode = border_mode;
    }
}

void Clay_Termbox_Set_Border_Chars(enum border_chars border_chars)
{
    clay_tb_assert(
        CLAY_TB_BORDER_CHARS_DEFAULT <= border_chars && border_chars <= CLAY_TB_BORDER_CHARS_NONE,
        "Border mode invalid (%d)", border_chars);
    if (CLAY_TB_BORDER_CHARS_DEFAULT == border_chars) {
        clay_tb_border_chars = CLAY_TB_BORDER_CHARS_UNICODE;
    } else {
        clay_tb_border_chars = border_chars;
    }
}

void Clay_Termbox_Set_Transparency(bool transparency)
{
    clay_tb_transparency = transparency;
    if (TB_OUTPUT_NORMAL == clay_tb_color_mode || CLAY_TB_OUTPUT_NOCOLOR == clay_tb_color_mode) {
        clay_tb_transparency = false;
    }
}

float Clay_Termbox_Width(void)
{
    clay_tb_assert(clay_tb_initialized, "Clay_Termbox_Initialize must be run first");

    return (float)tb_width() * clay_tb_cell_size.width;
}

float Clay_Termbox_Height(void)
{
    clay_tb_assert(clay_tb_initialized, "Clay_Termbox_Initialize must be run first");

    return (float)tb_height() * clay_tb_cell_size.height;
}

float Clay_Termbox_Cell_Width(void)
{
    return clay_tb_cell_size.width;
}

float Clay_Termbox_Cell_Height(void)
{
    return clay_tb_cell_size.height;
}

static inline Clay_Dimensions Clay_Termbox_MeasureText(
    Clay_StringSlice text, Clay_TextElementConfig *config, void *userData)
{
    clay_tb_assert(clay_tb_initialized, "Clay_Termbox_Initialize must be run first");

    int width = 0;
    int height = 1;

    // Convert to utf32 so termbox2's internal wcwidth function can get the printed width of each
    // codepoint
    for (int32_t i = 0; i < text.length;) {
        uint32_t ch;
        int codepoint_bytes = tb_utf8_char_to_unicode(&ch, text.chars + i);
        if (0 > codepoint_bytes) {
            clay_tb_assert(false, "Invalid utf8");
        }
        i += codepoint_bytes;

        int codepoint_width = tb_wcwidth(ch);
        if (-1 == codepoint_width) {
            // Nonprintable character, use width of REPLACEMENT CHARACTER (U+FFFD)
            codepoint_width = tb_wcwidth(0xfffd);
        }
        width += codepoint_width;
    }
    return (Clay_Dimensions) {
        (float)width * clay_tb_cell_size.width,
        (float)height * clay_tb_cell_size.height
    };
}

void Clay_Termbox_Initialize(
    int color_mode, enum border_mode border_mode, enum border_chars border_chars, bool transparency)
{
    int new_color_mode = color_mode;
    int new_border_mode = border_mode;
    int new_border_chars = border_chars;
    int new_transparency = transparency;
    clay_tb_pixel_dimensions new_pixel_size = clay_tb_cell_size;

    // Check for environment variables that override settings

    const char *env_color_mode = getenv("CLAY_TB_COLOR_MODE");
    if (NULL != env_color_mode) {
        if (0 == strcmp("NORMAL", env_color_mode)) {
            new_color_mode = TB_OUTPUT_NORMAL;
        } else if (0 == strcmp("256", env_color_mode)) {
            new_color_mode = TB_OUTPUT_256;
        } else if (0 == strcmp("216", env_color_mode)) {
            new_color_mode = TB_OUTPUT_216;
        } else if (0 == strcmp("GRAYSCALE", env_color_mode)) {
            new_color_mode = TB_OUTPUT_GRAYSCALE;
        } else if (0 == strcmp("TRUECOLOR", env_color_mode)) {
            new_color_mode = TB_OUTPUT_TRUECOLOR;
        } else if (0 == strcmp("NOCOLOR", env_color_mode)) {
            new_color_mode = CLAY_TB_OUTPUT_NOCOLOR;
        }
    }

    const char *env_border_chars = getenv("CLAY_TB_BORDER_CHARS");
    if (NULL != env_border_chars) {
        if (0 == strcmp("DEFAULT", env_border_chars)) {
            new_border_chars = CLAY_TB_BORDER_CHARS_DEFAULT;
        } else if (0 == strcmp("ASCII", env_border_chars)) {
            new_border_chars = CLAY_TB_BORDER_CHARS_ASCII;
        } else if (0 == strcmp("UNICODE", env_border_chars)) {
            new_border_chars = CLAY_TB_BORDER_CHARS_UNICODE;
        } else if (0 == strcmp("BLANK", env_border_chars)) {
            new_border_chars = CLAY_TB_BORDER_CHARS_BLANK;
        } else if (0 == strcmp("NONE", env_border_chars)) {
            new_border_chars = CLAY_TB_BORDER_CHARS_NONE;
        }
    }

    const char *env_transparency = getenv("CLAY_TB_TRANSPARENCY");
    if (NULL != env_transparency) {
        if (0 == strcmp("1", env_transparency)) {
            new_transparency = true;
        } else if (0 == strcmp("0", env_transparency)) {
            new_transparency = false;
        }
    }

    const char *env_cell_pixels = getenv("CLAY_TB_CELL_PIXELS");
    if (NULL != env_cell_pixels) {
        const char *str_width = env_cell_pixels;
        const char *str_height = strstr(env_cell_pixels, "x") + 1;
        if (NULL + 1 != str_height) {
            bool missing_value = false;

            errno = 0;
            float cell_width = strtof(str_width, NULL);
            if (0 != errno || 0 > cell_width) {
                missing_value = true;
            }
            float cell_height = strtof(str_height, NULL);
            if (0 != errno || 0 >= cell_height) {
                missing_value = true;
            }

            if (!missing_value) {
                new_pixel_size = (clay_tb_pixel_dimensions) { cell_width, cell_height };
            }
        }
    }

    // NO_COLOR indicates that ANSI colors shouldn't be used: https://no-color.org/
    const char *env_nocolor = getenv("NO_COLOR");
    if (NULL != env_nocolor && '\0' != env_nocolor[0]) {
        new_color_mode = CLAY_TB_OUTPUT_NOCOLOR;
    }

    tb_init();
    tb_set_input_mode(TB_INPUT_MOUSE);

    // Enable mouse hover support
    // - see https://github.com/termbox/termbox2/issues/71#issuecomment-2179581609
    // - 1003 "Any-event tracking" mode
    // - 1006 SGR extended coordinates (already enabled with TB_INPUT_MOUSE)
    tb_sendf("\x1b[?%d;%dh", 1003, 1006);

    clay_tb_initialized = true;

    Clay_Termbox_Set_Color_Mode(new_color_mode);
    Clay_Termbox_Set_Border_Mode(new_border_mode);
    Clay_Termbox_Set_Border_Chars(new_border_chars);
    Clay_Termbox_Set_Transparency(new_transparency);
    Clay_Termbox_Set_Cell_Pixel_Size(new_pixel_size.width, new_pixel_size.height);

    size_t size = (size_t)tb_width() * tb_height();
    clay_tb_color_buffer_clay = tb_malloc(sizeof(Clay_Color) * size);
    for (int i = 0; i < size; ++i) {
        clay_tb_color_buffer_clay[i] = (Clay_Color) { 0, 0, 0, 0 };
    }
}

void Clay_Termbox_Close(void)
{
    if (clay_tb_initialized) {
        // Disable mouse hover support
        tb_sendf("\x1b[?%d;%dl", 1003, 1006);

        tb_free(clay_tb_color_buffer_clay);
        tb_shutdown();
        clay_tb_initialized = false;
    }
}

void Clay_Termbox_Render(Clay_RenderCommandArray commands)
{
    clay_tb_assert(clay_tb_initialized, "Clay_Termbox_Initialize must be run first");

    clay_tb_resize_buffer();
    for (int32_t i = 0; i < commands.length; ++i) {
        const Clay_RenderCommand *command = Clay_RenderCommandArray_Get(&commands, i);
        const clay_tb_cell_bounding_box cell_box = cell_snap_bounding_box(command->boundingBox);

        int box_begin_x = CLAY__MAX(cell_box.x, 0);
        int box_end_x = CLAY__MIN(cell_box.x + cell_box.width, tb_width());
        int box_begin_y = CLAY__MAX(cell_box.y, 0);
        int box_end_y = CLAY__MIN(cell_box.y + cell_box.height, tb_height());

        if (box_end_x < 0 || box_end_y < 0 || tb_width() < box_begin_x
            || tb_height() < box_begin_y) {
            continue;
        }

        switch (command->commandType) {
            default: {
                clay_tb_assert(false, "Unhandled command: %d\n", command->commandType);
            }
            case CLAY_RENDER_COMMAND_TYPE_NONE: {
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleRenderData render_data = command->renderData.rectangle;
                Clay_Color color_fg = { 0, 0, 0, 0 };
                Clay_Color color_bg = render_data.backgroundColor;
                uintattr_t color_tb_fg = TB_DEFAULT;
                uintattr_t color_tb_bg = clay_tb_color_convert(color_bg);

                for (int y = box_begin_y; y < box_end_y; ++y) {
                    for (int x = box_begin_x; x < box_end_x; ++x) {
                        clay_tb_color_pair color_bg_new = clay_tb_get_transparency_color(
                            x, y, (clay_tb_color_pair) { color_bg, color_tb_bg });
                        clay_tb_set_cell(
                            x, y, ' ', color_tb_fg, color_bg_new.termbox, color_bg_new.clay);
                    }
                }
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                if (CLAY_TB_BORDER_CHARS_NONE == clay_tb_border_chars) {
                    break;
                }
                Clay_BorderRenderData render_data = command->renderData.border;
                Clay_Color color_fg = { 0, 0, 0, 1 };
                Clay_Color color_bg = render_data.color;
                uintattr_t color_tb_fg = TB_DEFAULT;
                uintattr_t color_tb_bg = clay_tb_color_convert(color_bg);

                int border_skip_begin_x = box_begin_x;
                int border_skip_end_x = box_end_x;
                int border_skip_begin_y = box_begin_y;
                int border_skip_end_y = box_end_y;

                switch (clay_tb_border_mode) {
                    default: {
                        clay_tb_assert(false, "Invalid or unimplemented border mode (%d)",
                            clay_tb_border_mode);
                        break;
                    }
                    case CLAY_TB_BORDER_MODE_MINIMUM: {
                        // Borders will be at least one cell wide if width is nonzero
                        // and the bounding box is large enough to not be all borders
                        if (0 < cell_box.width) {
                            if (0 < render_data.width.left) {
                                border_skip_begin_x = box_begin_x
                                    + (int)CLAY__MAX(
                                        1, (render_data.width.left / clay_tb_cell_size.width));
                            }
                            if (0 < render_data.width.right) {
                                border_skip_end_x = box_end_x
                                    - (int)CLAY__MAX(
                                        1, (render_data.width.right / clay_tb_cell_size.width));
                            }
                        }
                        if (0 < cell_box.height) {
                            if (0 < render_data.width.top) {
                                border_skip_begin_y = box_begin_y
                                    + (int)CLAY__MAX(
                                        1, (render_data.width.top / clay_tb_cell_size.width));
                            }
                            if (0 < render_data.width.bottom) {
                                border_skip_end_y = box_end_y
                                    - (int)CLAY__MAX(
                                        1, (render_data.width.bottom / clay_tb_cell_size.width));
                            }
                        }
                        break;
                    }
                    case CLAY_TB_BORDER_MODE_ROUND: {
                        int halfwidth = clay_tb_roundf(clay_tb_cell_size.width / 2);
                        int halfheight = clay_tb_roundf(clay_tb_cell_size.height / 2);

                        if (halfwidth < render_data.width.left) {
                            border_skip_begin_x = box_begin_x
                                + (int)CLAY__MAX(
                                    1, (render_data.width.left / clay_tb_cell_size.width));
                        }
                        if (halfwidth < render_data.width.right) {
                            border_skip_end_x = box_end_x
                                - (int)CLAY__MAX(
                                    1, (render_data.width.right / clay_tb_cell_size.width));
                        }
                        if (halfheight < render_data.width.top) {
                            border_skip_begin_y = box_begin_y
                                + (int)CLAY__MAX(
                                    1, (render_data.width.top / clay_tb_cell_size.width));
                        }
                        if (halfheight < render_data.width.bottom) {
                            border_skip_end_y = box_end_y
                                - (int)CLAY__MAX(
                                    1, (render_data.width.bottom / clay_tb_cell_size.width));
                        }
                        break;
                    }
                }

                // Draw border, skipping over the center of the bounding box
                for (int y = box_begin_y; y < box_end_y; ++y) {
                    for (int x = box_begin_x; x < box_end_x; ++x) {
                        if ((border_skip_begin_x <= x && x < border_skip_end_x)
                            && (border_skip_begin_y <= y && y < border_skip_end_y)) {
                            x = border_skip_end_x - 1;
                            continue;
                        }

                        uint32_t ch;
                        switch (clay_tb_border_chars) {
                            default: {
                                clay_tb_assert(false,
                                    "Invalid or unimplemented border character mode (%d)",
                                    clay_tb_border_chars);
                            }
                            case CLAY_TB_BORDER_CHARS_UNICODE: {
                                if ((x < border_skip_begin_x)
                                    && (y < border_skip_begin_y)) { // Top left
                                    ch = U'\u250c';
                                } else if ((x >= border_skip_end_x)
                                    && (y < border_skip_begin_y)) { // Top right
                                    ch = U'\u2510';
                                } else if ((x < border_skip_begin_x)
                                    && (y >= border_skip_end_y)) { // Bottom left
                                    ch = U'\u2514';
                                } else if ((x >= border_skip_end_x)
                                    && (y >= border_skip_end_y)) { // Bottom right
                                    ch = U'\u2518';
                                } else if (x < border_skip_begin_x || x >= border_skip_end_x) {
                                    ch = U'\u2502';
                                } else if (y < border_skip_begin_y || y >= border_skip_end_y) {
                                    ch = U'\u2500';
                                }
                                break;
                            }
                            case CLAY_TB_BORDER_CHARS_DEFAULT:
                            case CLAY_TB_BORDER_CHARS_ASCII: {
                                if ((x < border_skip_begin_x || x >= border_skip_end_x)
                                    && (y < border_skip_begin_y || y >= border_skip_end_y)) {
                                    ch = '+';
                                } else if (x < border_skip_begin_x || x >= border_skip_end_x) {
                                    ch = '|';
                                } else if (y < border_skip_begin_y || y >= border_skip_end_y) {
                                    ch = '-';
                                }
                                break;
                            }
                            case CLAY_TB_BORDER_CHARS_BLANK: {
                                ch = ' ';
                                break;
                            }
                        }

                        clay_tb_color_pair color_bg_new = clay_tb_get_transparency_color(
                            x, y, (clay_tb_color_pair) { color_bg, color_tb_bg });
                        clay_tb_set_cell(
                            x, y, ch, color_tb_fg, color_bg_new.termbox, color_bg_new.clay);
                    }
                }
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextRenderData render_data = command->renderData.text;
                Clay_Color color_fg = render_data.textColor;
                uintattr_t color_tb_fg = clay_tb_color_convert(color_fg);

                Clay_StringSlice *text = &render_data.stringContents;
                int32_t i = 0;
                for (int y = box_begin_y; y < box_end_y; ++y) {
                    for (int x = box_begin_x; x < box_end_x;) {
                        uint32_t ch = ' ';
                        if (i < text->length) {
                            int codepoint_length = tb_utf8_char_to_unicode(&ch, text->chars + i);
                            if (0 > codepoint_length) {
                                clay_tb_assert(false, "Invalid utf8");
                            }
                            i += codepoint_length;

                            uintattr_t color_tb_bg = (clay_tb_transparency)
                                ? TB_DEFAULT
                                : clay_tb_color_convert(clay_tb_color_buffer_clay_get(x, y));
                            Clay_Color color_bg = { 0 };
                            clay_tb_color_pair color_bg_new = clay_tb_get_transparency_color(
                                x, y, (clay_tb_color_pair) { color_bg, color_tb_bg });
                            clay_tb_set_cell(
                                x, y, ch, color_tb_fg, color_bg_new.termbox, color_bg_new.clay);
                        }

                        int codepoint_width = tb_wcwidth(ch);
                        if (-1 == codepoint_width) {
                            // Nonprintable character, use REPLACEMENT CHARACTER (U+FFFD)
                            ch = U'\ufffd';
                            codepoint_width = tb_wcwidth(ch);
                        }

                        x += codepoint_width;
                    }
                }
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
                Clay_ImageRenderData render_data = command->renderData.image;
                Clay_Color color_fg = { 0, 0, 0, 0 };
                Clay_Color color_bg = render_data.backgroundColor;
                uintattr_t color_tb_fg = clay_tb_color_convert(color_fg);
                uintattr_t color_tb_bg;

                // Only set background to the provided color if it's non-default
                bool color_specified
                    = !(color_bg.r == 0 && color_bg.g == 0 && color_bg.b == 0 && color_bg.a == 0);
                if (color_specified) {
                    color_tb_bg = clay_tb_color_convert(color_bg);
                }

                const char *placeholder_text = "[Image]";

                int i = 0;
                unsigned long len = strlen(placeholder_text);
                for (int y = box_begin_y; y < box_end_y; ++y) {
                    float percent_y = (float)(y - box_begin_y) / (float)cell_box.height;

                    for (int x = box_begin_x; x < box_end_x; ++x) {
                        char ch = ' ';
                        if (i < len) {
                            ch = placeholder_text[i++];
                        }

                        if (!color_specified) {
                            // Use a placeholder pattern for the image
                            float percent_x = (float)(cell_box.width - (x - box_begin_x))
                                / (float)cell_box.width;
                            if (percent_x > percent_y) {
                                color_bg = (Clay_Color) { 0x94, 0xb4, 0xff, 0xff };
                                color_tb_bg = clay_tb_color_convert(color_bg);
                            } else {
                                color_bg = (Clay_Color) { 0x3f, 0xcc, 0x45, 0xff };
                                color_tb_bg = clay_tb_color_convert(color_bg);
                            }
                        }
                        clay_tb_set_cell(x, y, ch, color_tb_fg, color_tb_bg, color_bg);
                    }
                }
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                clay_tb_scissor_box = (clay_tb_cell_bounding_box) {
                    .x = box_begin_x,
                    .y = box_begin_y,
                    .width = box_end_x - box_begin_x,
                    .height = box_end_y - box_begin_y,
                };
                clay_tb_scissor_enabled = true;
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                clay_tb_scissor_enabled = false;
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
                break;
            }
        }
    }
}
