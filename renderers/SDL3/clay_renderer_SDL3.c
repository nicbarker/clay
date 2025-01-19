#include "../../clay.h"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

/* This needs to be global because the "MeasureText" callback doesn't have a
 * user data parameter */
static TTF_Font *gFonts[1];

static void SDL_RenderClayCommands(SDL_Renderer *renderer, Clay_RenderCommandArray *rcommands)
{
    for (size_t i = 0; i < rcommands->length; i++) {
        Clay_RenderCommand *rcmd = Clay_RenderCommandArray_Get(rcommands, i);
        Clay_BoundingBox bounding_box = rcmd->boundingBox;
        SDL_FRect rect = { bounding_box.x, bounding_box.y, bounding_box.width, bounding_box.height };

        switch (rcmd->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleElementConfig *config = rcmd->config.rectangleElementConfig;
                Clay_Color color = config->color;
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(renderer, &rect);
            } break;
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextElementConfig *config = rcmd->config.textElementConfig;
                Clay_String *text = &rcmd->text;
                SDL_Color color = { config->textColor.r, config->textColor.g, config->textColor.b, config->textColor.a };

                TTF_Font *font = gFonts[config->fontId];
                SDL_Surface *surface = TTF_RenderText_Blended(font, text->chars, text->length, color);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_RenderTexture(renderer, texture, NULL, &rect);

                SDL_DestroySurface(surface);
                SDL_DestroyTexture(texture);
            } break;
            default:
                SDL_Log("Unknown render command type: %d", rcmd->commandType);
        }
    }
}
