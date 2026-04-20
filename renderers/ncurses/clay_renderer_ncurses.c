/**
 * @file clay_renderer_ncurses.c
 * @author Seintian
 * @date 2025-12-29
 * @brief Ncurses renderer implementation for the Clay UI library.
 * 
 * This file provides a backend for rendering Clay UI layouts using the Ncurses library.
 * It handles terminal initialization, color management using standard ANSI or 256-color modes,
 * text measurement (assuming monospace cells), and primitive rendering (rectangles, text, borders).
 */

#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include <ncurses.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include "../../clay.h"

#define CLAY_NCURSES_FONT_BOLD 1
#define CLAY_NCURSES_FONT_UNDERLINE 2

#define CLAY_NCURSES_KEY_SCROLL_UP 123456
#define CLAY_NCURSES_KEY_SCROLL_DOWN 123457
#define CLAY_NCURSES_KEY_MOUSE_CLICK 123458

// -------------------------------------------------------------------------------------------------
// -- Internal State & Constants
// -------------------------------------------------------------------------------------------------

/**
 * @brief Assumed width of a single terminal cell in logical units.
 * Used to convert Clay's floating point layout coordinates to integer terminal grid coordinates.
 */
#define CLAY_NCURSES_CELL_WIDTH 8.0f

/**
 * @brief Assumed height of a single terminal cell in logical units.
 * Used to convert Clay's floating point layout coordinates to integer terminal grid coordinates.
 */
#define CLAY_NCURSES_CELL_HEIGHT 16.0f

/** @brief Current screen width in characters. */
static int _screenWidth = 0;

/** @brief Current screen height in characters. */
static int _screenHeight = 0;

/** @brief Flag indicating if the ncurses subsystem has been successfully initialized. */
static bool _isInitialized = false;

// Scissor / Clipping State

/** @brief Maximum depth of the scissor/clipping stack. */
#define MAX_SCISSOR_STACK_DEPTH 16

/** @brief Stack of clipping rectangles current active. */
static Clay_BoundingBox _scissorStack[MAX_SCISSOR_STACK_DEPTH];

/** @brief Current index into the scissor stack. */
static int _scissorStackIndex = 0;

// Color State

/** @brief Maximum number of color pairs to cache. */
#define MAX_COLOR_PAIRS_CACHE 1024

/**
 * @brief Cache entry for an Ncurses color pair.
 * Mapes a generic foreground/background color combination to an Ncurses pair ID.
 */
static struct {
    short fg;       /**< Foreground color index. */
    short bg;       /**< Background color index. */
    int pairId;     /**< Assigned Ncurses pair ID. */
} _colorPairCache[MAX_COLOR_PAIRS_CACHE];

/** @brief Current number of cached color pairs. */
static int _colorPairCacheSize = 0;

// -------------------------------------------------------------------------------------------------
// -- Forward Declarations & Internal Helpers
// -------------------------------------------------------------------------------------------------

/**
 * @brief Converts a Clay_Color to the nearest Ncurses color index.
 * @param color The Clay RGBA color.
 * @return The corresponding Ncurses color index (0-255).
 */
static short Clay_Ncurses_GetColorId(Clay_Color color);

/**
 * @brief Retrieves or creates a color pair for the given foreground and background.
 * @param fg Foreground color index.
 * @param bg Background color index.
 * @return The Ncurses pair ID representing this combination.
 */
static int Clay_Ncurses_GetColorPair(short fg, short bg);

/**
 * @brief Measures the visual width of a string in terminal cells.
 * Handles multibyte characters (UTF-8) correctly.
 * @param text The string slice to measure.
 * @return The width in cells (columns).
 */
static int Clay_Ncurses_MeasureStringWidth(Clay_StringSlice text);

/**
 * @brief Calculates the intersection of a requested rectangle with the current scissor clip.
 * 
 * @param x Requested X position (cells).
 * @param y Requested Y position (cells).
 * @param w Requested Width (cells).
 * @param h Requested Height (cells).
 * @param[out] outX Resulting visible X position.
 * @param[out] outY Resulting visible Y position.
 * @param[out] outW Resulting visible Width.
 * @param[out] outH Resulting visible Height.
 * @return true If the rectangle is at least partially visible.
 * @return false If the rectangle is completely clipped (invisible).
 */
static bool Clay_Ncurses_GetVisibleRect(int x, int y, int w, int h, int *outX, int *outY, int *outW, int *outH) {
    Clay_BoundingBox clip = _scissorStack[_scissorStackIndex];

    // Convert clip to cell coords
    int cx = (int)(clip.x / CLAY_NCURSES_CELL_WIDTH);
    int cy = (int)(clip.y / CLAY_NCURSES_CELL_HEIGHT);
    int cw = (int)(clip.width / CLAY_NCURSES_CELL_WIDTH);
    int ch = (int)(clip.height / CLAY_NCURSES_CELL_HEIGHT);

    // Intersect 
    int ix = (x > cx) ? x : cx;
    int iy = (y > cy) ? y : cy;
    int right = (x + w < cx + cw) ? (x + w) : (cx + cw);
    int bottom = (y + h < cy + ch) ? (y + h) : (cy + ch);

    int iw = right - ix;
    int ih = bottom - iy;

    if (iw <= 0 || ih <= 0) return false;

    *outX = ix;
    *outY = iy;
    *outW = iw;
    *outH = ih;
    return true;
}

/**
 * @brief Gets the background color index of the character currently at the specified coordinates.
 * Used for transparent rendering over existing content.
 * @param x Screen X coordinate.
 * @param y Screen Y coordinate.
 * @return The background color index.
 */
static short Clay_Ncurses_GetBackgroundAt(int x, int y) {
    chtype ch = mvinch(y, x);
    int pair = PAIR_NUMBER(ch);
    short fg, bg;
    pair_content(pair, &fg, &bg);
    return bg;
}

/**
 * @brief Initializes the system locale for UTF-8 support.
 * Attempts to set LC_ALL to empty (system default), "C.UTF-8", or "en_US.UTF-8" in that order.
 */
static void Clay_Ncurses_InitLocale(void) {
    char *locale = setlocale(LC_ALL, "");
    if (!locale || strcmp(locale, "C") == 0 || strcmp(locale, "POSIX") == 0) {
        locale = setlocale(LC_ALL, "C.UTF-8");
        if (!locale) {
            locale = setlocale(LC_ALL, "en_US.UTF-8");
        }
    }
}

// -------------------------------------------------------------------------------------------------
// -- Atomic Render Functions
// -------------------------------------------------------------------------------------------------

/**
 * @brief Renders a solid color rectangle.
 * @param command The render command containing the rectangle data (bounds, color).
 */
static void Clay_Ncurses_RenderRectangle(Clay_RenderCommand *command) {
    Clay_BoundingBox box = command->boundingBox;
    int x = (int)(box.x / CLAY_NCURSES_CELL_WIDTH);
    int y = (int)(box.y / CLAY_NCURSES_CELL_HEIGHT);
    int w = (int)(box.width / CLAY_NCURSES_CELL_WIDTH);
    int h = (int)(box.height / CLAY_NCURSES_CELL_HEIGHT);

    int dx, dy, dw, dh;
    if (!Clay_Ncurses_GetVisibleRect(x, y, w, h, &dx, &dy, &dw, &dh)) return;

    short fg = Clay_Ncurses_GetColorId(command->renderData.rectangle.backgroundColor);
    short bg = fg;
    int pair = Clay_Ncurses_GetColorPair(fg, bg);

    chtype targetChar = ' ' | COLOR_PAIR(pair);

    // Optimization: Don't redraw if character is already correct (reduces flicker/bandwidth)
    for (int row = dy; row < dy + dh; row++) {
        for (int col = dx; col < dx + dw; col++) {
            chtype current = mvinch(row, col);
            if ((current & (A_CHARTEXT | A_COLOR)) != (targetChar & (A_CHARTEXT | A_COLOR))) {
                mvaddch(row, col, targetChar);
            }
        }
    }
}

/**
 * @brief Renders a text string.
 * Uses multibyte to wide char conversion for correct UTF-8 rendering.
 * @param command The render command containing the text data.
 */
static void Clay_Ncurses_RenderText(Clay_RenderCommand *command) {
    Clay_BoundingBox box = command->boundingBox;
    int x = (int)(box.x / CLAY_NCURSES_CELL_WIDTH);
    int y = (int)(box.y / CLAY_NCURSES_CELL_HEIGHT);
    Clay_StringSlice text = command->renderData.text.stringContents;

    int textWidth = Clay_Ncurses_MeasureStringWidth(text);

    int dx, dy, dw, dh;
    // Text height is always 1 cell
    if (!Clay_Ncurses_GetVisibleRect(x, y, textWidth, 1, &dx, &dy, &dw, &dh)) return;

    short fg = Clay_Ncurses_GetColorId(command->renderData.text.textColor);
    short bg = Clay_Ncurses_GetBackgroundAt(dx, dy);
    int pair = Clay_Ncurses_GetColorPair(fg, bg);

    attron(COLOR_PAIR(pair));
    attron(COLOR_PAIR(pair));
    if (command->renderData.text.fontId & CLAY_NCURSES_FONT_BOLD) attron(A_BOLD);
    if (command->renderData.text.fontId & CLAY_NCURSES_FONT_UNDERLINE) attron(A_UNDERLINE);

    // Complex multibyte string handling
    // We render to a temporary buffer first to handle wide characters
    int maxLen = text.length + 1;
    wchar_t *wbuf = (wchar_t *)malloc(maxLen * sizeof(wchar_t));
    if (!wbuf) {
        attroff(COLOR_PAIR(pair));
        return;
    }

    char *tempC = (char *)malloc(text.length + 1);
    memcpy(tempC, text.chars, text.length);
    tempC[text.length] = '\0';

    int wlen = mbstowcs(wbuf, tempC, maxLen);
    free(tempC);

    if (wlen != -1) {
        int skipCols = dx - x;
        int takeCols = dw;
        int currentCols = 0;
        int printStart = -1;
        int printLen = 0;

        // Find start index based on columns skipped
        for (int k = 0; k < wlen; k++) {
            int cw = wcwidth(wbuf[k]);
            if (cw < 0) cw = 0;

            if (currentCols >= skipCols && currentCols < skipCols + takeCols) {
                if (printStart == -1) printStart = k;
                printLen++;
            }
            currentCols += cw;
            if (currentCols >= skipCols + takeCols) break;
        }

        if (printStart != -1) {
            mvaddnwstr(dy, dx, wbuf + printStart, printLen);
        }
    }

    free(wbuf);
    if (command->renderData.text.fontId & CLAY_NCURSES_FONT_BOLD) attroff(A_BOLD);
    if (command->renderData.text.fontId & CLAY_NCURSES_FONT_UNDERLINE) attroff(A_UNDERLINE);
    attroff(COLOR_PAIR(pair));
}

/**
 * @brief Renders a border around a rectangle.
 * Supports rounded corners using ACS_ CORNER characters.
 * @param command The render command containing the border data.
 */
static void Clay_Ncurses_RenderBorder(Clay_RenderCommand *command) {
    Clay_BoundingBox box = command->boundingBox;
    int x = (int)(box.x / CLAY_NCURSES_CELL_WIDTH);
    int y = (int)(box.y / CLAY_NCURSES_CELL_HEIGHT);
    int w = (int)(box.width / CLAY_NCURSES_CELL_WIDTH);
    int h = (int)(box.height / CLAY_NCURSES_CELL_HEIGHT);

    int dx, dy, dw, dh;
    if (!Clay_Ncurses_GetVisibleRect(x, y, w, h, &dx, &dy, &dw, &dh)) return;

    short color = Clay_Ncurses_GetColorId(command->renderData.border.color);
    short bg = Clay_Ncurses_GetBackgroundAt(dx, dy);
    int pair = Clay_Ncurses_GetColorPair(color, bg);

    attron(COLOR_PAIR(pair));

    // Draw Top
    if (y >= dy && y < dy + dh) {
        int startX = (x > dx) ? x : dx;
        int endX = (x + w < dx + dw) ? (x + w) : (dx + dw);
        if (x < startX) startX++; // Adjust for corner if clipped
        if (x + w - 1 > endX) endX--; // Adjust for corner

        // Simplification: Check intersection with range for horizontal lines
        int h_sx = x + 1;
        int h_ex = x + w - 1;

        int draw_sx = (h_sx > dx) ? h_sx : dx;
        int draw_ex = (h_ex < dx + dw) ? h_ex : dx + dw;

        if (draw_ex > draw_sx) {
            mvwhline(stdscr, y, draw_sx, ACS_HLINE, draw_ex - draw_sx);
        }
    }

    // Draw Bottom
    if (y + h - 1 >= dy && y + h - 1 < dy + dh) {
        int h_sx = x + 1;
        int h_ex = x + w - 1;
        int draw_sx = (h_sx > dx) ? h_sx : dx;
        int draw_ex = (h_ex < dx + dw) ? h_ex : dx + dw;

        if (draw_ex > draw_sx) {
            mvwhline(stdscr, y + h - 1, draw_sx, ACS_HLINE, draw_ex - draw_sx);
        }
    }

    // Draw Left
    if (x >= dx && x < dx + dw) {
        int v_sy = y + 1;
        int v_ey = y + h - 1;
        int draw_sy = (v_sy > dy) ? v_sy : dy;
        int draw_ey = (v_ey < dy + dh) ? v_ey : dy + dh;

        if (draw_ey > draw_sy) {
            mvwvline(stdscr, draw_sy, x, ACS_VLINE, draw_ey - draw_sy);
        }
    }

    // Draw Right
    if (x + w - 1 >= dx && x + w - 1 < dx + dw) {
        int v_sy = y + 1;
        int v_ey = y + h - 1;
        int draw_sy = (v_sy > dy) ? v_sy : dy;
        int draw_ey = (v_ey < dy + dh) ? v_ey : dy + dh;

        if (draw_ey > draw_sy) {
            mvwvline(stdscr, draw_sy, x + w - 1, ACS_VLINE, draw_ey - draw_sy);
        }
    }

    // Corners
    bool drawTop = (y >= dy && y < dy + dh);
    bool drawBottom = (y + h - 1 >= dy && y + h - 1 < dy + dh);
    bool drawLeft = (x >= dx && x < dx + dw);
    bool drawRight = (x + w - 1 >= dx && x + w - 1 < dx + dw);

    if (drawTop && drawLeft) {
        mvaddch(y, x, ACS_ULCORNER);
    }
    if (drawTop && drawRight) {
        mvaddch(y, x + w - 1, ACS_URCORNER);
    }
    if (drawBottom && drawLeft) {
        mvaddch(y + h - 1, x, ACS_LLCORNER);
    }
    if (drawBottom && drawRight) {
        mvaddch(y + h - 1, x + w - 1, ACS_LRCORNER);
    }

    attroff(COLOR_PAIR(pair));
}

/**
 * @brief Pushes a new clipping rectangle onto the scissor stack.
 * The new clip is intersected with the current top of the stack.
 * @param boundingBox The new clipping region.
 */
static void Clay_Ncurses_PushScissor(Clay_BoundingBox boundingBox) {
    if (_scissorStackIndex >= MAX_SCISSOR_STACK_DEPTH - 1) return;

    Clay_BoundingBox current = _scissorStack[_scissorStackIndex];
    Clay_BoundingBox next = boundingBox;

    float nX = (next.x > current.x) ? next.x : current.x;
    float nY = (next.y > current.y) ? next.y : current.y;
    float nR = ((next.x + next.width) < (current.x + current.width)) ? (next.x + next.width) : (current.x + current.width);
    float nB = ((next.y + next.height) < (current.y + current.height)) ? (next.y + next.height) : (current.y + current.height);

    _scissorStackIndex++;
    _scissorStack[_scissorStackIndex] = (Clay_BoundingBox){ nX, nY, nR - nX, nB - nY };
}

/**
 * @brief Pops the current clipping rectangle from the stack.
 */
static void Clay_Ncurses_PopScissor() {
    if (_scissorStackIndex > 0) {
        _scissorStackIndex--;
    }
}

// -------------------------------------------------------------------------------------------------
// -- Public API Implementation
// -------------------------------------------------------------------------------------------------

/**
 * @brief Initializes the Ncurses library and internal renderer state.
 * Sets up locale, screen, keyboard input, and color support.
 */
void Clay_Ncurses_Initialize() {
    if (_isInitialized) return;

    Clay_Ncurses_InitLocale();
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0);

    // Force xterm mouse tracking (Any Event) to ensure we get position updates
    // even when buttons are not pressed. This fixes hover detection.
    puts("\033[?1003h");

    start_color();
    use_default_colors();

    getmaxyx(stdscr, _screenHeight, _screenWidth);

    _scissorStack[0] = (Clay_BoundingBox){0, 0, (float)_screenWidth * CLAY_NCURSES_CELL_WIDTH, (float)_screenHeight * CLAY_NCURSES_CELL_HEIGHT};
    _scissorStackIndex = 0;

    _isInitialized = true;
}

/**
 * @brief Terminates the Ncurses library and cleans up.
 * Returns the terminal to its normal state.
 */
void Clay_Ncurses_Terminate() {
    if (_isInitialized) {
        // Restore mouse tracking state
        puts("\033[?1003l");

        clear();
        refresh();
        endwin();

        SCREEN *s = set_term(NULL);
        if (s) {
            delscreen(s);
        }

        _isInitialized = false;
    }
}

/**
 * @brief Returns the layout dimensions of the current Ncurses screen.
 * @return The dimensions in Clay logical units.
 */
Clay_Dimensions Clay_Ncurses_GetLayoutDimensions() {
    return (Clay_Dimensions) {
        .width = (float)_screenWidth * CLAY_NCURSES_CELL_WIDTH,
        .height = (float)_screenHeight * CLAY_NCURSES_CELL_HEIGHT
    };
}

/**
 * @brief Measures text for layout purposes.
 * @param text The text to measure.
 * @param config Text configuration (unused in this fixed-w renderer).
 * @param userData Custom user data (unused).
 * @return The dimensions of the text in Clay logical units.
 */
Clay_Dimensions Clay_Ncurses_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    (void)config;
    (void)userData;

    int width = Clay_Ncurses_MeasureStringWidth(text);
    return (Clay_Dimensions) {
        .width = (float)width * CLAY_NCURSES_CELL_WIDTH,
        .height = CLAY_NCURSES_CELL_HEIGHT
    };
}

/**
 * @brief Main rendering entry point. Processes the Clay RenderCommandBuffer and draws to the terminal.
 * @param renderCommands The array of commands produced by Clay_EndLayout().
 */
void Clay_Ncurses_Render(Clay_RenderCommandArray renderCommands) {
    if (!_isInitialized) return;

    // Update screen dimensions if terminal successfully resized
    int newW, newH;
    getmaxyx(stdscr, newH, newW);
    if (newW != _screenWidth || newH != _screenHeight) {
        _screenWidth = newW;
        _screenHeight = newH;
    }

    // Reset Scissor Stack for new frame
    _scissorStack[0] = (Clay_BoundingBox){0, 0, (float)_screenWidth * CLAY_NCURSES_CELL_WIDTH, (float)_screenHeight * CLAY_NCURSES_CELL_HEIGHT};
    _scissorStackIndex = 0;

    for (int i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommand *command = Clay_RenderCommandArray_Get(&renderCommands, i);

        switch (command->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
                Clay_Ncurses_RenderRectangle(command);
                break;
            case CLAY_RENDER_COMMAND_TYPE_TEXT:
                Clay_Ncurses_RenderText(command);
                break;
            case CLAY_RENDER_COMMAND_TYPE_BORDER:
                Clay_Ncurses_RenderBorder(command);
                break;
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START:
                Clay_Ncurses_PushScissor(command->boundingBox);
                break;
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END:
                Clay_Ncurses_PopScissor();
                break;
            case CLAY_RENDER_COMMAND_TYPE_IMAGE:
            case CLAY_RENDER_COMMAND_TYPE_CUSTOM:
            default: 
                break;
        }
    }

    refresh();
}

// -------------------------------------------------------------------------------------------------
// -- Internal Logic: Color & Measure
// -------------------------------------------------------------------------------------------------

/**
 * @brief Approximates a true color (RGB) to the nearest available Ncurses color index.
 * Supports 256-color mode (6x6x6 cube) and 8-color fallback.
 * @param color The requested RGB color.
 * @return The approximate Ncurses color index.
 */
static short Clay_Ncurses_MatchColor(Clay_Color color) {
    // 256 Color Mode
    if (COLORS >= 256) {
        int r = (int)((color.r / 255.0f) * 5.0f);
        int g = (int)((color.g / 255.0f) * 5.0f);
        int b = (int)((color.b / 255.0f) * 5.0f);
        return (short)(16 + (36 * r) + (6 * g) + b);
    }

    // 8 Color Fallback
    int r = color.r > 128;
    int g = color.g > 128;
    int b = color.b > 128;

    if (r && g && b) return COLOR_WHITE;
    if (!r && !g && !b) return COLOR_BLACK;
    if (r && g) return COLOR_YELLOW;
    if (r && b) return COLOR_MAGENTA;
    if (g && b) return COLOR_CYAN;
    if (r) return COLOR_RED;
    if (g) return COLOR_GREEN;
    if (b) return COLOR_BLUE;
    return COLOR_WHITE;
}

static short Clay_Ncurses_GetColorId(Clay_Color color) {
    return Clay_Ncurses_MatchColor(color);
}

static int Clay_Ncurses_GetColorPair(short fg, short bg) {
    for (int i = 0; i < _colorPairCacheSize; i++) {
        if (_colorPairCache[i].fg == fg && _colorPairCache[i].bg == bg) {
            return _colorPairCache[i].pairId;
        }
    }

    if (_colorPairCacheSize >= MAX_COLOR_PAIRS_CACHE) {
        return 0; // Cache full, fallback to default
    }

    int newId = _colorPairCacheSize + 1;
    init_pair(newId, fg, bg);

    _colorPairCache[_colorPairCacheSize].fg = fg;
    _colorPairCache[_colorPairCacheSize].bg = bg;
    _colorPairCache[_colorPairCacheSize].pairId = newId;
    _colorPairCacheSize++;

    return newId;
}

static int Clay_Ncurses_MeasureStringWidth(Clay_StringSlice text) {
    int width = 0;
    const char *ptr = text.chars;
    int len = text.length;

    mbtowc(NULL, NULL, 0); // Reset state

    while (len > 0) {
        wchar_t wc;
        int bytes = mbtowc(&wc, ptr, len);
        if (bytes <= 0) {
            ptr++;
            len--;
            continue; 
        }

        int w = wcwidth(wc);
        if (w > 0) width += w;
        ptr += bytes;
        len -= bytes;
    }

    return width;
}

/**
 * @brief Handles Ncurses input and updates Clay's internal pointer state.
 * Use this instead of standard getch() in your main loop to enable mouse interaction.
 * 
 * @param window The Ncurses window to read input from (e.g. stdscr).
 * @return The key code pressed, or ERR if no input.
 */
int Clay_Ncurses_ProcessInput(WINDOW *window) {
    int key = wgetch(window);

    // Handle Mouse
    if (key == KEY_MOUSE) {
        MEVENT event;
        if (getmouse(&event) == OK) {
            // Convert Cell Coordinates -> Clay Logical Coordinates
            Clay_Vector2 mousePos = { 
                (float)event.x * CLAY_NCURSES_CELL_WIDTH, 
                (float)event.y * CLAY_NCURSES_CELL_HEIGHT 
            };

            // Persistent state to handle drag/move events where button state might be absent in the event mask
            static bool _isMouseDown = false;
            bool shouldReturnClick = false;

            if (event.bstate & (BUTTON1_PRESSED | BUTTON1_DOUBLE_CLICKED | BUTTON1_TRIPLE_CLICKED)) {
                _isMouseDown = true;
                shouldReturnClick = true;
            }
            else if (event.bstate & BUTTON1_RELEASED) {
                _isMouseDown = false;
            }

            // Update Clay State with the final determined state for this event
            Clay_SetPointerState(mousePos, _isMouseDown);

            if (shouldReturnClick) {
                return CLAY_NCURSES_KEY_MOUSE_CLICK;
            }

            // Handle Scroll Wheel
            #ifdef BUTTON4_PRESSED
                if (event.bstate & BUTTON4_PRESSED) {
                    return CLAY_NCURSES_KEY_SCROLL_UP;
                }
            #endif
            #ifdef BUTTON5_PRESSED
                if (event.bstate & BUTTON5_PRESSED) {
                    return CLAY_NCURSES_KEY_SCROLL_DOWN;
                }
            #endif
        }
    }

    return key;
}

/**
 * @brief Helper to attach an OnClick listener to the current element.
 * Registers a hover callback. The user's function must check `pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME`.
 * 
 * @param onClickFunc Function pointer to call.
 * @param userData User data passed to the callback.
 */
void Clay_Ncurses_OnClick(
    void (*onClickFunc)(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData), 
    void *userData
) {
    if (onClickFunc) {
        Clay_OnHover(onClickFunc, userData);
    }
}
