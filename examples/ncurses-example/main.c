/**
 * @file main.c
 * @author Seintian
 * @date 2025-12-28
 * @brief Ncurses Example Application for Clay.
 * 
 * Demonstrates how to use Clay with the Ncurses renderer to create a terminal-based UI.
 * Features include:
 * - A responsive layout with a sidebar and main content area.
 * - Scrollable content (feed).
 * - "Floating" modal windows (Help).
 * - Keyboard user input handling.
 * - Custom widgets (ProgressBar).
 */

#define CLAY_IMPLEMENTATION
#include "../../clay.h"
#include "../../renderers/ncurses/clay_renderer_ncurses.c"
#include <time.h> // for nanosleep

// -------------------------------------------------------------------------------------------------
// -- Constants & Configuration
// -------------------------------------------------------------------------------------------------

/** @brief Scroll speed per key press. */
#define DEFAULT_SCROLL_SENSITIVITY 3.0f

/** @brief Accent color: Green. */
#define COLOR_ACCENT_GREEN (Clay_Color){0, 200, 0, 255}

/** @brief Accent color: Orange. */
#define COLOR_ACCENT_ORANGE (Clay_Color){200, 150, 0, 255}

/** @brief Accent color: Red. */
#define COLOR_ACCENT_RED   (Clay_Color){255, 100, 100, 255}

/** @brief Accent color: Blue. */
#define COLOR_ACCENT_BLUE  (Clay_Color){100, 100, 255, 255}

/** @brief Standard text color: White. */
#define COLOR_TEXT_WHITE   (Clay_Color){255, 255, 255, 255}

/** @brief Dimmed text color: Grey. */
#define COLOR_TEXT_DIM     (Clay_Color){150, 150, 150, 255}

/** @brief Background color for panels. */
#define COLOR_PANEL_BG     (Clay_Color){20, 20, 20, 255}

/** @brief Border color for panels. */
#define COLOR_PANEL_BORDER (Clay_Color){100, 100, 100, 255}

// -------------------------------------------------------------------------------------------------
// -- Application State
// -------------------------------------------------------------------------------------------------

/**
 * @brief Global application state.
 * Stores all mutable state required for the UI logic.
 */
typedef struct {
    bool isSidebarVisible;      /**< Toggles the visibility of the sidebar. */
    bool isHelpModalVisible;    /**< Toggles the help overlay. */
    bool isQuitting;            /**< Flag to exit the main loop. */
    float scrollDelta;          /**< Accumulated scroll amount for the current frame. */
} AppState;

/** @brief Static instance of application state. */
static AppState _appState = { 
    .isSidebarVisible = true, 
    .isHelpModalVisible = false,
    .isQuitting = false, 
    .scrollDelta = 0.0f
};

// -------------------------------------------------------------------------------------------------
// -- Callback Handlers
// -------------------------------------------------------------------------------------------------

void HandleHelpToggleClick(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        _appState.isHelpModalVisible = !_appState.isHelpModalVisible;
    }
}

// -------------------------------------------------------------------------------------------------
// -- Input Processing
// -------------------------------------------------------------------------------------------------

/**
 * @brief Processes input for the current frame.
 * Updates _appState directly based on key presses.
 * Uses ncurses input processing (non-blocking if timeout is set).
 */
void App_ProcessInput() {
    _appState.scrollDelta = 0.0f;

    int key;
    while ((key = Clay_Ncurses_ProcessInput(stdscr)) != ERR) {
        if (key == CLAY_NCURSES_KEY_MOUSE_CLICK) {
            // Stop processing input this frame to ensure Clay sees the "Pressed" state
            // before a subsequent "Released" event might overwrite it.
            break; 
        }
        switch (key) {
            case 'q':
            case 'Q':
                _appState.isQuitting = true;
                break;
            case 's':
            case 'S':
                // Toggle between two states
                _appState.isSidebarVisible = !_appState.isSidebarVisible;
                break;
            case 'h':
            case 'H':
                _appState.isHelpModalVisible = !_appState.isHelpModalVisible;
                break;
            case KEY_UP:
            case CLAY_NCURSES_KEY_SCROLL_UP:
                _appState.scrollDelta += DEFAULT_SCROLL_SENSITIVITY;
                break;
            case KEY_DOWN:
            case CLAY_NCURSES_KEY_SCROLL_DOWN:
                _appState.scrollDelta -= DEFAULT_SCROLL_SENSITIVITY;
                break;
        }
    }
}

// -------------------------------------------------------------------------------------------------
// -- UI Components
// -------------------------------------------------------------------------------------------------

/**
 * @brief Renders a progress bar widget.
 * @param label The text label displayed above the bar.
 * @param percentage The fill percentage (0.0 to 1.0).
 * @param color The color of the filled portion.
 */
void UI_ProgressBar(Clay_String label, float percentage, Clay_Color color) {
    CLAY(CLAY_ID_LOCAL("ProgressBar"), {
        .layout = { 
            .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0) }, 
            .layoutDirection = CLAY_TOP_TO_BOTTOM, 
            .childGap = CLAY_NCURSES_CELL_HEIGHT 
        }
    }) {
        CLAY(CLAY_ID_LOCAL("Label"), {
            .layout = { 
                .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0) }, 
                .layoutDirection = CLAY_LEFT_TO_RIGHT, 
                .childGap = CLAY_NCURSES_CELL_HEIGHT, 
                .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} 
            }
        }) {
            CLAY_TEXT(label, CLAY_TEXT_CONFIG({ .textColor = {200, 200, 200, 255}, .fontSize = 16 }));
        }

        CLAY(CLAY_ID_LOCAL("Track"), {
            .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_HEIGHT) } },
            .backgroundColor = {40, 40, 40, 255},
            .cornerRadius = {1} 
        }) {
            CLAY(CLAY_ID_LOCAL("Fill"), {
                .layout = { .sizing = { CLAY_SIZING_PERCENT(percentage), CLAY_SIZING_GROW() } },
                .backgroundColor = color,
                .cornerRadius = {1}
            }) {}
        }
    }
}

/**
 * @brief Renders the Server Status widget containing CPU and Memory usage bars.
 */
void UI_ServerStatusWidget() {
    CLAY(CLAY_ID("ServerStatusWidget"), {
        .layout = { 
            .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0) },
            .padding = {16, 16, 16, 16},
            .childGap = 16,
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .backgroundColor = {25, 25, 25, 255},
        .border = { .color = {60, 60, 60, 255}, .width = {2, 2, 2, 2} }
    }) {
        CLAY_TEXT(CLAY_STRING("SERVER STATUS"), CLAY_TEXT_CONFIG({ .textColor = COLOR_TEXT_WHITE }));
        UI_ProgressBar(CLAY_STRING("CPU"), 0.45f, COLOR_ACCENT_GREEN);
        UI_ProgressBar(CLAY_STRING("Mem"), 0.82f, COLOR_ACCENT_ORANGE);
    }
}

/**
 * @brief Renders a single item used in the sidebar.
 * @param label Text to display.
 * @param textColor Color of the text.
 */
void UI_SidebarItem(Clay_String label, Clay_Color textColor) {
    CLAY(CLAY_ID_LOCAL("SidebarItem"), {
        .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_HEIGHT * 2) } },
        .backgroundColor = Clay_Hovered() ? (Clay_Color){60, 60, 60, 255} : COLOR_PANEL_BG
    }) {
        CLAY_TEXT(label, CLAY_TEXT_CONFIG({ .textColor = textColor }));
    }
}

/**
 * @brief Renders the application Sidebar.
 * conditionally rendered based on _appState.isSidebarVisible.
 */
void UI_Sidebar() {
    if (!_appState.isSidebarVisible) return;

    CLAY(CLAY_ID("Sidebar"), {
        .layout = { 
            .sizing = { CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_WIDTH * 30), CLAY_SIZING_GROW() },
            .padding = CLAY_PADDING_ALL(CLAY_NCURSES_CELL_HEIGHT),
            .childGap = CLAY_NCURSES_CELL_HEIGHT,
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .backgroundColor = COLOR_PANEL_BG,
        .border = { .color = COLOR_PANEL_BORDER, .width = { .right = 2 } }
    }) {
        CLAY_TEXT(CLAY_STRING("SIDEBAR"), CLAY_TEXT_CONFIG({ .textColor = {255, 255, 0, 255} }));

        UI_ServerStatusWidget();

        UI_SidebarItem(CLAY_STRING(" > Item 1 🌍"), (Clay_Color){0, 255, 255, 255});
        UI_SidebarItem(CLAY_STRING(" > Item 2 🌐"), COLOR_TEXT_WHITE);

        CLAY(CLAY_ID("HelpToggleButton"), {
            .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_HEIGHT * 2) }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} },
            .backgroundColor = Clay_Hovered() ? (Clay_Color){0, 100, 0, 255} : COLOR_PANEL_BG,
            .cornerRadius = {1}
        }) {
            Clay_Ncurses_OnClick(HandleHelpToggleClick, NULL);
            CLAY_TEXT(CLAY_STRING(" > Toggle Help"), CLAY_TEXT_CONFIG({ .textColor = COLOR_TEXT_WHITE }));
        }

        // Mixed Style Items
        CLAY(CLAY_ID("SidebarItemMixed1"), {
            .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_HEIGHT * 3) }, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } },
            .backgroundColor = COLOR_PANEL_BG,
            .cornerRadius = { .topLeft = 1 },
            .border = { .color = COLOR_ACCENT_RED, .width = {2, 2, 2, 2} }
        }) {
            CLAY_TEXT(CLAY_STRING(" > TL BOLD"), CLAY_TEXT_CONFIG({ .textColor = COLOR_ACCENT_RED, .fontId = CLAY_NCURSES_FONT_BOLD }));
        }

        CLAY(CLAY_ID("SidebarItemMixed2"), {
            .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_HEIGHT * 3) }, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } },
            .backgroundColor = COLOR_PANEL_BG,
            .cornerRadius = { .topLeft = 1, .bottomRight = 1 },
            .border = { .color = {100, 255, 100, 255}, .width = {2, 2, 2, 2} }
        }) {
            CLAY_TEXT(CLAY_STRING(" > Diag Under"), CLAY_TEXT_CONFIG({ .textColor = {100, 255, 100, 255}, .fontId = CLAY_NCURSES_FONT_UNDERLINE }));
        }

        CLAY(CLAY_ID("SidebarItemMixed3"), {
            .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_HEIGHT * 3) }, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } },
            .backgroundColor = COLOR_PANEL_BG,
            .cornerRadius = { .topLeft = 1, .topRight = 1 },
            .border = { .color = COLOR_ACCENT_BLUE, .width = {2, 2, 2, 2} }
        }) {
            CLAY_TEXT(CLAY_STRING(" > Top Bold Und"), CLAY_TEXT_CONFIG({ .textColor = COLOR_ACCENT_BLUE, .fontId = CLAY_NCURSES_FONT_BOLD | CLAY_NCURSES_FONT_UNDERLINE }));
        }
    }
}

// Data for "Realistic" Content

/** @brief Sample names for feed posts. */
const char* NAMES[] = { "Alice", "Bob", "Charlie", "Diana", "Ethan", "Fiona", "George", "Hannah" };

/** @brief Sample titles for feed posts. */
const char* TITLES[] = { "Just released a new library!", "Thoughts on C programming?", "Check out this cool algorithm", "Why I love Ncurses", "Clay UI is pretty flexible", "Debugging segfaults all day...", "Coffee break time ☕", "Anyone going to the conf?" };

/** @brief Sample body text for feed posts. */
const char* LOREM[] = { "Lorem ipsum dolor sit amet, consectetur adipiscing elit.", "Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.", "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris.", "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum.", "Excepteur sint occaecat cupidatat non proident, sunt in culpa." };

/**
 * @brief Renders a single social media feed post.
 * @param index The index of the post (used to generate deterministic content from sample data).
 */
void UI_FeedPost(int index) {
    CLAY(CLAY_IDI("FeedPost", index), {
        .layout = { 
            .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0) },
            .padding = CLAY_PADDING_ALL(CLAY_NCURSES_CELL_HEIGHT),
            .childGap = CLAY_NCURSES_CELL_HEIGHT,
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .backgroundColor = COLOR_PANEL_BG,
        .cornerRadius = {1},
        .border = { .color = {80, 80, 80, 255}, .width = {2, 2, 2, 2} }
    }) {
        // Header: Avatar + Name + Time
        CLAY(CLAY_IDI("PostHeader", index), {
            .layout = { 
                .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0) },
                .childGap = CLAY_NCURSES_CELL_WIDTH * 2,
                .childAlignment = { .y = CLAY_ALIGN_Y_TOP },
                .layoutDirection = CLAY_LEFT_TO_RIGHT
            }
        }) {
            CLAY(CLAY_IDI("Avatar", index), {
                .layout = { .sizing = { CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_WIDTH * 4), CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_HEIGHT * 2) } },
                .backgroundColor = { (index * 50) % 255, (index * 80) % 255, (index * 30) % 255, 255 },
                .cornerRadius = {1}
            }) {}

            CLAY(CLAY_IDI("AuthorInfo", index), {
                .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 0 }
            }) {
                Clay_String name = { .length = strlen(NAMES[index % 8]), .chars = NAMES[index % 8] };
                Clay_String title = { .length = strlen(TITLES[index % 8]), .chars = TITLES[index % 8] };
                CLAY_TEXT(name, CLAY_TEXT_CONFIG({ .textColor = COLOR_TEXT_WHITE }));
                CLAY_TEXT(title, CLAY_TEXT_CONFIG({ .textColor = COLOR_TEXT_DIM }));
            }
        }

        // Body
        CLAY(CLAY_IDI("PostBody", index), {
            .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0) }, .padding = { .top = CLAY_NCURSES_CELL_HEIGHT, .bottom = CLAY_NCURSES_CELL_HEIGHT } }
        }) {
            Clay_String lorem = { .length = strlen(LOREM[index % 5]), .chars = LOREM[index % 5] };
            CLAY_TEXT(lorem, CLAY_TEXT_CONFIG({ .textColor = {200, 200, 200, 255} }));
        }

        // Actions
        CLAY(CLAY_IDI("PostActions", index), {
            .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0) }, .childGap = CLAY_NCURSES_CELL_HEIGHT, .layoutDirection = CLAY_LEFT_TO_RIGHT }
        }) {
            CLAY_TEXT(CLAY_STRING("[ Like ]"), CLAY_TEXT_CONFIG({ .textColor = {0, 255, 0, 255} }));
            CLAY_TEXT(CLAY_STRING("[ Comment ]"), CLAY_TEXT_CONFIG({ .textColor = {0, 100, 255, 255} }));
            CLAY_TEXT(CLAY_STRING("[ Share ]"), CLAY_TEXT_CONFIG({ .textColor = {255, 0, 0, 255} }));
        }
    }
}

/**
 * @brief Renders the main content area with the scrollable feed.
 */
void UI_MainContent() {
    CLAY(CLAY_ID("ContentArea"), {
        .layout = { 
            .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
            .padding = CLAY_PADDING_ALL(CLAY_NCURSES_CELL_HEIGHT),
            .childGap = CLAY_NCURSES_CELL_HEIGHT,
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .backgroundColor = COLOR_PANEL_BG
    }) {
        // Sticky Header
        CLAY(CLAY_ID("StickyHeader"), {
            .layout = { 
                .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_HEIGHT * 3) },
                .padding = { .left = CLAY_NCURSES_CELL_WIDTH * 2, .right=CLAY_NCURSES_CELL_WIDTH * 2 },
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER }
            },
            .backgroundColor = COLOR_PANEL_BG,
            .border = { .color = {0, 100, 255, 255}, .width = { .bottom = 1 } }
        }) {
            CLAY_TEXT(CLAY_STRING("Clay Social Feed"), CLAY_TEXT_CONFIG({ .textColor = COLOR_TEXT_WHITE }));
        }

        // Scrollable Viewport
        CLAY(CLAY_ID("Viewport"), {
            .layout = { 
                .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() }, 
                .padding = { .top = 8, .bottom = 8 }
            },
            .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() },
            .backgroundColor = COLOR_PANEL_BG
        }) {
            CLAY(CLAY_ID("FeedList"), {
                .layout = { 
                    .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0) },
                    .childGap = 16, 
                    .layoutDirection = CLAY_TOP_TO_BOTTOM 
                }
            }) {
                // Determine if we need to initialize scroll position
                Clay_ElementData item0 = Clay_GetElementData(CLAY_IDI("FeedPost", 0));

                for (int i = 0; i < 50; ++i) { 
                    UI_FeedPost(i);
                }

                CLAY_TEXT(CLAY_STRING("--- End of Feed ---"), CLAY_TEXT_CONFIG({ .textColor = {140, 140, 140, 255} }));
            }
        }

        CLAY_TEXT(CLAY_STRING("Controls: ARROW UP/DOWN to Scroll | Q to Quit | S to Toggle Sidebar"), CLAY_TEXT_CONFIG({ .textColor = {120, 120, 120, 255} }));
    }
}

/**
 * @brief Renders the Help modal overlay.
 */
void UI_HelpModal() {
    if (!_appState.isHelpModalVisible) return;

    CLAY(CLAY_ID("HelpModalOverlay"), {
        .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() }, .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER} },
        .floating = { .zIndex = 100, .attachTo = CLAY_ATTACH_TO_ROOT, .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_CAPTURE },
        .backgroundColor = {0, 0, 0, 150}
    }) {
        CLAY(CLAY_ID("HelpModalWindow"), {
            .layout = { 
                .sizing = { CLAY_SIZING_FIXED(CLAY_NCURSES_CELL_WIDTH * 60), CLAY_SIZING_FIT(0) },
                .padding = CLAY_PADDING_ALL(CLAY_NCURSES_CELL_HEIGHT),
                .childGap = CLAY_NCURSES_CELL_WIDTH,
                .layoutDirection = CLAY_TOP_TO_BOTTOM
            },
            .backgroundColor = {30, 30, 30, 255},
            .cornerRadius = {1},
            .border = { .color = COLOR_TEXT_WHITE, .width = {2, 2, 2, 2} }
        }) {
            CLAY_TEXT(CLAY_STRING("Ncurses Example Help"), CLAY_TEXT_CONFIG({ .textColor = COLOR_TEXT_WHITE }));

            CLAY(CLAY_ID("HelpLine1"), { .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0)} } }) {
                CLAY_TEXT(CLAY_STRING("Keys:"), CLAY_TEXT_CONFIG({ .textColor = {200, 200, 0, 255} }));
            }
            CLAY_TEXT(CLAY_STRING("- ARROW KEYS: Scroll Feed"), CLAY_TEXT_CONFIG({ .textColor = {200, 200, 200, 255} }));
            CLAY_TEXT(CLAY_STRING("- S: Toggle Sidebar"), CLAY_TEXT_CONFIG({ .textColor = {200, 200, 200, 255} }));
            CLAY_TEXT(CLAY_STRING("- H: Toggle This Help"), CLAY_TEXT_CONFIG({ .textColor = {200, 200, 200, 255} }));
            CLAY_TEXT(CLAY_STRING("- Q: Quit Application"), CLAY_TEXT_CONFIG({ .textColor = {200, 200, 200, 255} }));

            CLAY(CLAY_ID("HelpCloseTip"), { .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_FIT(0)}, .padding = {.top = 16} } }) {
                CLAY_TEXT(CLAY_STRING("Press 'H' to close."), CLAY_TEXT_CONFIG({ .textColor = {100, 100, 100, 255} }));
            }
        }
    }
}

/**
 * @brief Renders the root layout of the application.
 */
void UI_RootLayout() {
    CLAY(CLAY_ID("Root"), {
        .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() }, .layoutDirection = CLAY_LEFT_TO_RIGHT },
    }) {
        UI_Sidebar();
        UI_MainContent();
        UI_HelpModal();
    }
}

// -------------------------------------------------------------------------------------------------
// -- Main Loop
// -------------------------------------------------------------------------------------------------

/**
 * @brief Application Entry Point.
 * Initializes Memory, Clay, Ncurses, and runs the main event loop.
 */
int main() {
    uint32_t minMemory = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(minMemory, malloc(minMemory));

    Clay_Initialize(arena, (Clay_Dimensions){0,0}, (Clay_ErrorHandler){NULL});
    Clay_SetMeasureTextFunction(Clay_Ncurses_MeasureText, NULL);

    // Initialize Ncurses Renderer
    Clay_Ncurses_Initialize();

    // Set non-blocking input for game loop
    timeout(0); 

    while(!_appState.isQuitting) {
        App_ProcessInput();

        Clay_Dimensions dims = Clay_Ncurses_GetLayoutDimensions();
        Clay_SetLayoutDimensions(dims);

        // Handle Scroll Logic
        Clay_ElementId viewportId = CLAY_ID("Viewport");
        Clay_ElementData viewportData = Clay_GetElementData(viewportId);

        if (viewportData.found) {
            Clay_UpdateScrollContainers(true, (Clay_Vector2){0, _appState.scrollDelta}, 0.016f);
        }

        Clay_BeginLayout();
        UI_RootLayout();
        Clay_RenderCommandArray commands = Clay_EndLayout();

        Clay_Ncurses_Render(commands);

        // 60 FPS Target (approx)
        struct timespec ts = { .tv_sec = 0, .tv_nsec = 16000 * 1000 };
        nanosleep(&ts, NULL); 
    }

    Clay_Ncurses_Terminate();
    free(arena.memory);
    return 0;
}
