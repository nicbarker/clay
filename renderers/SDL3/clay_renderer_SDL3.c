#include "../../clay.h"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <math.h> //needed to perform the rounded corners rendering

/* This needs to be global because the "MeasureText" callback doesn't have a
 * user data parameter */
static TTF_Font *gFonts[1];

//all rendering is performed  by a single SDL call, avoiding multiple RenderRect + plumbing choice for circles.
static void SDL_RenderRoundedRect(SDL_Renderer *renderer, const SDL_FRect rect, const float cornerRadius, const Clay_Color _color) {
    /* Even in 4K this is enough for smooth curves (low radius or rect size coupled with
     * no AA or low resolution might make it appear as jagged curves) */
    const int NUM_CIRCLE_SEGMENTS = 16;

    const SDL_FColor color = { _color.r/255, _color.g/255, _color.b/255, _color.a/255 };

    int indexCount = 0, vertexCount = 0;

    const float minRadius = fminf(rect.w, rect.h) / 2.0f;
    const float clampedRadius = fminf(cornerRadius, minRadius);

    int totalVertices = 4 + (4 * (NUM_CIRCLE_SEGMENTS * 2)) + 2*4;
    int totalIndices = 6 + (4 * (NUM_CIRCLE_SEGMENTS * 3)) + 6*4;

    SDL_Vertex vertices[totalVertices];
    int indices[totalIndices];

    //define center rectangle
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x + clampedRadius, rect.y + clampedRadius}, color, {0, 0} }; //0 center TL
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x + rect.w - clampedRadius, rect.y + clampedRadius}, color, {1, 0} }; //1 center TR
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x + rect.w - clampedRadius, rect.y + rect.h - clampedRadius}, color, {1, 1} }; //2 center BR
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x + clampedRadius, rect.y + rect.h - clampedRadius}, color, {0, 1} }; //3 center BL

    indices[indexCount++] = 0;
    indices[indexCount++] = 1;
    indices[indexCount++] = 3;
    indices[indexCount++] = 1;
    indices[indexCount++] = 2;
    indices[indexCount++] = 3;

    //define rounded corners as triangle fans
    const float step = M_PI_2 / NUM_CIRCLE_SEGMENTS;
    for (int i = 0; i < NUM_CIRCLE_SEGMENTS; i++)
    {
        const float angle1 = (float)i * step;
        const float angle2 = ((float)i + 1.0f) * step;

        for (int j = 0; j < 4; j++) {  // Iterate over four corners
            float cx, cy, signX, signY;

            switch (j) {
                case 0: cx = rect.x + clampedRadius; cy = rect.y + clampedRadius; signX = -1; signY = -1; break; // Top-left
                case 1: cx = rect.x + rect.w - clampedRadius; cy = rect.y + clampedRadius; signX = 1; signY = -1; break; // Top-right
                case 2: cx = rect.x + rect.w - clampedRadius; cy = rect.y + rect.h - clampedRadius; signX = 1; signY = 1; break; // Bottom-right
                case 3: cx = rect.x + clampedRadius; cy = rect.y + rect.h - clampedRadius; signX = -1; signY = 1; break; // Bottom-left
                default: SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Invalid corner index"); return;
            }

            vertices[vertexCount++] = (SDL_Vertex){ {cx + cosf(angle1) * clampedRadius * signX, cy + sinf(angle1) * clampedRadius * signY}, color, {0, 0} };
            vertices[vertexCount++] = (SDL_Vertex){ {cx + cosf(angle2) * clampedRadius * signX, cy + sinf(angle2) * clampedRadius * signY}, color, {0, 0} };

            indices[indexCount++] = j;  // Connect to corresponding central rectangle vertex
            indices[indexCount++] = vertexCount - 2;
            indices[indexCount++] = vertexCount - 1;
        }
    }

    //Define edge rectangles
    // Top edge
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x + clampedRadius, rect.y}, color, {0, 0} }; //TL
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x + rect.w - clampedRadius, rect.y}, color, {1, 0} }; //TR

    indices[indexCount++] = 0;
    indices[indexCount++] = vertexCount - 2; //TL
    indices[indexCount++] = vertexCount - 1; //TR
    indices[indexCount++] = 1;
    indices[indexCount++] = 0;
    indices[indexCount++] = vertexCount - 1; //TR
    // Right edge
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x + rect.w, rect.y + clampedRadius}, color, {1, 0} }; //RT
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x + rect.w, rect.y + rect.h - clampedRadius}, color, {1, 1} }; //RB

    indices[indexCount++] = 1;
    indices[indexCount++] = vertexCount - 2; //RT
    indices[indexCount++] = vertexCount - 1; //RB
    indices[indexCount++] = 2;
    indices[indexCount++] = 1;
    indices[indexCount++] = vertexCount - 1; //RB
    // Bottom edge
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x + rect.w - clampedRadius, rect.y + rect.h}, color, {1, 1} }; //BR
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x + clampedRadius, rect.y + rect.h}, color, {0, 1} }; //BL

    indices[indexCount++] = 2;
    indices[indexCount++] = vertexCount - 2; //BR
    indices[indexCount++] = vertexCount - 1; //BL
    indices[indexCount++] = 3;
    indices[indexCount++] = 2;
    indices[indexCount++] = vertexCount - 1; //BL
    // Left edge
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x, rect.y + rect.h - clampedRadius}, color, {0, 1} }; //LB
    vertices[vertexCount++] = (SDL_Vertex){ {rect.x, rect.y + clampedRadius}, color, {0, 0} }; //LT

    indices[indexCount++] = 3;
    indices[indexCount++] = vertexCount - 2; //LB
    indices[indexCount++] = vertexCount - 1; //LT
    indices[indexCount++] = 0;
    indices[indexCount++] = 3;
    indices[indexCount++] = vertexCount - 1; //LT

    // Render everything
    SDL_RenderGeometry(renderer, NULL, vertices, vertexCount, indices, indexCount);
}


static void SDL_RenderClayCommands(SDL_Renderer *renderer, Clay_RenderCommandArray *rcommands)
{
    for (size_t i = 0; i < rcommands->length; i++) {
        Clay_RenderCommand *rcmd = Clay_RenderCommandArray_Get(rcommands, i);
        const Clay_BoundingBox bounding_box = rcmd->boundingBox;
        const SDL_FRect rect = { bounding_box.x, bounding_box.y, bounding_box.width, bounding_box.height };

        switch (rcmd->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                const Clay_RectangleElementConfig *config = rcmd->config.rectangleElementConfig;
                const Clay_Color color = config->color;

                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                if (config->cornerRadius.topLeft > 0) {
                    //const float radius = (config->cornerRadius.topLeft * 2) / (float)((bounding_box.width > bounding_box.height) ? bounding_box.height : bounding_box.width);
                    //SDL_RenderRoundedRect(renderer, rect, radius, color);
                    SDL_RenderRoundedRect(renderer, rect, config->cornerRadius.topLeft, color);
                } else {
                    SDL_RenderFillRect(renderer, &rect);
                }
            } break;
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                const Clay_TextElementConfig *config = rcmd->config.textElementConfig;
                const Clay_String *text = &rcmd->text;
                const SDL_Color color = { config->textColor.r, config->textColor.g, config->textColor.b, config->textColor.a };

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
