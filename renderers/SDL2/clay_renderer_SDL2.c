#include "../../clay.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>

#define CLAY_COLOR_TO_SDL_COLOR_ARGS(color) color.r, color.g, color.b, color.a

typedef struct
{
    uint32_t fontId;
    TTF_Font *font;
} SDL2_Font;


static Clay_Dimensions SDL2_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData)
{
    SDL2_Font *fonts = (SDL2_Font*)userData;

    TTF_Font *font = fonts[config->fontId].font;
    char *chars = (char *)calloc(text.length + 1, 1);
    memcpy(chars, text.chars, text.length);
    int width = 0;
    int height = 0;
    if (TTF_SizeUTF8(font, chars, &width, &height) < 0) {
        fprintf(stderr, "Error: could not measure text: %s\n", TTF_GetError());
        exit(1);
    }
    free(chars);
    return (Clay_Dimensions) {
            .width = (float)width,
            .height = (float)height,
    };
}

SDL_Rect currentClippingRectangle;

static void Clay_SDL2_Render(SDL_Renderer *renderer, Clay_RenderCommandArray renderCommands, SDL2_Font *fonts)
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
                Clay_StringSlice text = renderCommand->text;
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
                        .x = boundingBox.x,
                        .y = boundingBox.y,
                        .w = boundingBox.width,
                        .h = boundingBox.height,
                };
                SDL_RenderCopy(renderer, texture, NULL, &destination);

                SDL_DestroyTexture(texture);
                SDL_FreeSurface(surface);
                free(cloned);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                currentClippingRectangle = (SDL_Rect) {
                        .x = boundingBox.x,
                        .y = boundingBox.y,
                        .w = boundingBox.width,
                        .h = boundingBox.height,
                };
                SDL_RenderSetClipRect(renderer, &currentClippingRectangle);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                SDL_RenderSetClipRect(renderer, NULL);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
                SDL_Surface *image = (SDL_Surface *)renderCommand->config.imageElementConfig->imageData;

                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);

                SDL_Rect destination = (SDL_Rect){
                    .x = boundingBox.x,
                    .y = boundingBox.y,
                    .w = boundingBox.width,
                    .h = boundingBox.height,
                };

                SDL_RenderCopy(renderer, texture, NULL, &destination);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                Clay_BorderElementConfig *config = renderCommand->config.borderElementConfig;

                if (config->left.width > 0) {
                    SDL_SetRenderDrawColor(renderer, CLAY_COLOR_TO_SDL_COLOR_ARGS(config->left.color));
                    SDL_RenderFillRectF(renderer, &(SDL_FRect){ boundingBox.x, boundingBox.y + config->cornerRadius.topLeft, config->left.width, boundingBox.height - config->cornerRadius.topLeft - config->cornerRadius.bottomLeft });
                }

                if (config->right.width > 0) {
                    SDL_SetRenderDrawColor(renderer, CLAY_COLOR_TO_SDL_COLOR_ARGS(config->right.color));
                    SDL_RenderFillRectF(renderer, &(SDL_FRect){ boundingBox.x + boundingBox.width - config->right.width, boundingBox.y + config->cornerRadius.topRight, config->right.width, boundingBox.height - config->cornerRadius.topRight - config->cornerRadius.bottomRight });
                }

                if (config->right.width > 0) {
                    SDL_SetRenderDrawColor(renderer, CLAY_COLOR_TO_SDL_COLOR_ARGS(config->right.color));
                    SDL_RenderFillRectF(renderer, &(SDL_FRect){ boundingBox.x + boundingBox.width - config->right.width, boundingBox.y + config->cornerRadius.topRight, config->right.width, boundingBox.height - config->cornerRadius.topRight - config->cornerRadius.bottomRight });
                }

                if (config->top.width > 0) {
                    SDL_SetRenderDrawColor(renderer, CLAY_COLOR_TO_SDL_COLOR_ARGS(config->right.color));
                    SDL_RenderFillRectF(renderer, &(SDL_FRect){ boundingBox.x + config->cornerRadius.topLeft, boundingBox.y, boundingBox.width - config->cornerRadius.topLeft - config->cornerRadius.topRight, config->top.width });
                }

                if (config->bottom.width > 0) {
                    SDL_SetRenderDrawColor(renderer, CLAY_COLOR_TO_SDL_COLOR_ARGS(config->bottom.color));
                    SDL_RenderFillRectF(renderer, &(SDL_FRect){ boundingBox.x + config->cornerRadius.bottomLeft, boundingBox.y + boundingBox.height - config->bottom.width, boundingBox.width - config->cornerRadius.bottomLeft - config->cornerRadius.bottomRight, config->bottom.width });
                }

                break;
            }
            default: {
                fprintf(stderr, "Error: unhandled render command: %d\n", renderCommand->commandType);
                exit(1);
            }
        }
    }
}
