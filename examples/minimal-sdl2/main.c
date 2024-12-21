#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


static const uint32_t FONT_ID_BODY_24 = 0;
static const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};
static const Clay_Color COLOR_BLUE = (Clay_Color) {111, 173, 162, 255};
static const Clay_Color COLOR_LIGHT = (Clay_Color) {224, 215, 210, 255};


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


typedef struct
{
    uint32_t fontId;
    TTF_Font *font;
} Font;
static Font fonts[1];


static Clay_Dimensions MeasureText(Clay_String *text, Clay_TextElementConfig *config);
static void Render(SDL_Renderer *renderer, Clay_RenderCommandArray renderCommands);


int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error: could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() < 0) {
        fprintf(stderr, "Error: could not initialize TTF: %s\n", TTF_GetError());
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("resources/Roboto-Regular.ttf", 24);
    if (!font) {
        fprintf(stderr, "Error: could not load font: %s\n", TTF_GetError());
        return 1;
    }
    fonts[FONT_ID_BODY_24] = (Font) {
        .fontId = FONT_ID_BODY_24,
        .font = font,
    };

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer) < 0) {
        fprintf(stderr, "Error: could not create window and renderer: %s", SDL_GetError());
    }

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena) {
        .label = CLAY_STRING("Clay Memory Arena"),
        .capacity = totalMemorySize,
        .memory = (char *)malloc(totalMemorySize),
    };

    Clay_SetMeasureTextFunction(MeasureText);

    int windowWidth = 0;
    int windowHeight = 0;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    Clay_Initialize(clayMemory, (Clay_Dimensions) { (float)windowWidth, (float)windowHeight });

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: goto quit;
            }
        }
        int mouseX = 0;
        int mouseY = 0;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        Clay_Vector2 mousePosition = (Clay_Vector2){ (float)mouseX, (float)mouseY };
        Clay_SetPointerState(mousePosition, mouseState & SDL_BUTTON(1));
        
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        Clay_SetLayoutDimensions((Clay_Dimensions) { (float)windowWidth, (float)windowHeight });

        Clay_RenderCommandArray renderCommands = CreateLayout();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        Render(renderer, renderCommands);

        SDL_RenderPresent(renderer);
    }
quit:

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}


static Clay_Dimensions MeasureText(Clay_String *text, Clay_TextElementConfig *config)
{
    TTF_Font *font = fonts[config->fontId].font;
    char *chars = (char *)calloc(text->length + 1, 1);
    memcpy(chars, text->chars, text->length);
    int width = 0;
    int height = 0;
    if (TTF_SizeUTF8(font, chars, &width, &height) < 0) {
        fprintf(stderr, "Error: could not measure text: %s\n", TTF_GetError());
        #ifdef CLAY_OVERFLOW_TRAP
        raise(SIGTRAP);
        #endif
        exit(1);
    }
    free(chars);
    return (Clay_Dimensions) {
        .width = (float)width,
        .height = (float)height,
    };
}


static void Render(SDL_Renderer *renderer, Clay_RenderCommandArray renderCommands)
{
    for (uint32_t i = 0; i < renderCommands.length; i++)
    {
        Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        Clay_BoundingBox boundingBox = renderCommand->boundingBox;
        switch (renderCommand->commandType)
        {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleElementConfig *config = renderCommand->config.rectangleElementConfig;
                Clay_Color color = config->color;
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_FRect rect = (SDL_FRect) {
                    .x = boundingBox.x,
                    .y = boundingBox.y,
                    .w = boundingBox.width,
                    .h = boundingBox.height,
                };
                SDL_RenderFillRectF(renderer, &rect);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextElementConfig *config = renderCommand->config.textElementConfig;
                Clay_String text = renderCommand->text;
                char *cloned = (char *)calloc(text.length + 1, 1);
                memcpy(cloned, text.chars, text.length);
                TTF_Font* font = fonts[config->fontId].font;
                SDL_Surface *surface = TTF_RenderUTF8_Blended(font, cloned, (SDL_Color) {
                    .r = (Uint8)config->textColor.r,
                    .g = (Uint8)config->textColor.g,
                    .b = (Uint8)config->textColor.b,
                    .a = (Uint8)config->textColor.a,
                });
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

                SDL_Rect destination = (SDL_Rect){
                    .x = (Uint8)boundingBox.x,
                    .y = (Uint8)boundingBox.y,
                    .w = (Uint8)boundingBox.width,
                    .h = (Uint8)boundingBox.height,
                };
                SDL_RenderCopy(renderer, texture, NULL, &destination);

                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                free(cloned);
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
