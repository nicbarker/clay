#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#include <stdio.h>

#include "../../renderers/SDL3/clay_renderer_SDL3.c"

static const Uint32 FONT_ID = 0;

static const Clay_Color COLOR_ORANGE    = (Clay_Color) {225, 138, 50, 255};
static const Clay_Color COLOR_BLUE      = (Clay_Color) {111, 173, 162, 255};
static const Clay_Color COLOR_LIGHT     = (Clay_Color) {224, 215, 210, 255};

typedef struct app_state {
    SDL_Window *window;
    SDL_Renderer *renderer;
} AppState;

static inline Clay_Dimensions SDL_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData)
{
    TTF_Font *font = gFonts[config->fontId];
    int width, height;

    if (!TTF_GetStringSize(font, text.chars, text.length, &width, &height)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to measure text: %s", SDL_GetError());
    }

    return (Clay_Dimensions) { (float) width, (float) height };
}

static void Label(const Clay_String text, const int cornerRadius)
{
    CLAY(CLAY_LAYOUT({ .padding = {8, 8} }),
        CLAY_RECTANGLE({
            .color = Clay_Hovered() ? COLOR_BLUE : COLOR_ORANGE,
            .cornerRadius = cornerRadius,
        })) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({
           .textColor = { 255, 255, 255, 255 },
           .fontId = FONT_ID,
           .fontSize = 24,
        }));
   }
}

static void LabelBorder(const Clay_String text, const int cornerRadius, const int thickness)
{
    CLAY(
        CLAY_LAYOUT({
            .padding = {16, 16, 8, 8 } }),
            CLAY_BORDER_OUTSIDE_RADIUS(
                thickness,
                COLOR_BLUE,
                cornerRadius)
    ){
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({
           .textColor = { 255, 255, 255, 255 },
           .fontId = FONT_ID,
           .fontSize = 24,
        }));
    }
}

static Clay_RenderCommandArray Clay_CreateLayout()
{
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
            },
            .childGap = 10,
            .padding = { 10, 10 },
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
        }),
        CLAY_BORDER({
            .left = { 20, COLOR_BLUE },
            .right = { 20, COLOR_BLUE },
            .bottom = { 20, COLOR_BLUE }
        }),
        CLAY_RECTANGLE({
            .color = COLOR_LIGHT,
        })
    ) {
        Label(CLAY_STRING("Rounded - Button 1"), 10);
        Label(CLAY_STRING("Straight - Button 2") , 0);
        Label(CLAY_STRING("Rounded+ - Button 3") , 20);
        LabelBorder(CLAY_STRING("Border - Button 4"), 0, 5);
        LabelBorder(CLAY_STRING("RoundedBorder - Button 5"), 10, 5);
        LabelBorder(CLAY_STRING("RoundedBorder - Button 6"), 40, 15);
    }
    return Clay_EndLayout();
}

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    if (!TTF_Init()) {
        return SDL_APP_FAILURE;
    }

    AppState *state = SDL_calloc(1, sizeof(AppState));
    if (!state) {
        return SDL_APP_FAILURE;
    }
    *appstate = state;

    if (!SDL_CreateWindowAndRenderer("Clay Demo", 640, 480, 0, &state->window, &state->renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetWindowResizable(state->window, true);

    TTF_Font *font = TTF_OpenFont("resources/Roboto-Regular.ttf", 24);
    if (!font) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    gFonts[FONT_ID] = font;

    /* Initialize Clay */
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena) {
        .memory = SDL_malloc(totalMemorySize),
        .capacity = totalMemorySize
    };

    int width, height;
    SDL_GetWindowSize(state->window, &width, &height);
    Clay_Initialize(clayMemory, (Clay_Dimensions) { (float) width, (float) height }, (Clay_ErrorHandler) { HandleClayErrors });
    Clay_SetMeasureTextFunction(SDL_MeasureText, 0);

    *appstate = state;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    SDL_AppResult ret_val = SDL_APP_CONTINUE;

    switch (event->type) {
        case SDL_EVENT_QUIT:
            ret_val = SDL_APP_SUCCESS;
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            Clay_SetLayoutDimensions((Clay_Dimensions) { (float) event->window.data1, (float) event->window.data2 });
            break;
        case SDL_EVENT_MOUSE_MOTION:
            Clay_SetPointerState((Clay_Vector2) { event->motion.x, event->motion.y },
                                 event->motion.state & SDL_BUTTON_LEFT);
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            Clay_UpdateScrollContainers(true, (Clay_Vector2) { event->motion.xrel, event->motion.yrel }, 0.01f);
            break;
        default:
            break;
    };

    return ret_val;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *state = appstate;

    Clay_RenderCommandArray render_commands = Clay_CreateLayout();

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    SDL_RenderClayCommands(state->renderer, &render_commands);

    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    (void) result;

    if (result != SDL_APP_SUCCESS) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Application failed to run");
    }

    AppState *state = appstate;

    if (state) {
        if (state->renderer)
            SDL_DestroyRenderer(state->renderer);

        if (state->window)
            SDL_DestroyWindow(state->window);

        SDL_free(state);
    }
    TTF_Quit();
}
