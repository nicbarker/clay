#include "../../clay.h"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

/* This needs to be global because the "MeasureText" callback doesn't have a
 * user data parameter */
static TTF_Font *gFonts[1];
/* Global for convenience. Even in 4K this is enough for smooth curves (low radius or rect size coupled with
 * no AA or low resolution might make it appear as jagged curves) */
static int NUM_CIRCLE_SEGMENTS = 16;

//all rendering is performed by a single SDL call, avoiding multiple RenderRect + plumbing choice for circles.
static void SDL_RenderFillRoundedRect(SDL_Renderer *renderer, const SDL_FRect rect, const float cornerRadius, const Clay_Color _color) {
    const SDL_FColor color = { _color.r/255, _color.g/255, _color.b/255, _color.a/255 };

    int indexCount = 0, vertexCount = 0;

    const float minRadius = SDL_min(rect.w, rect.h) / 2.0f;
    const float clampedRadius = SDL_min(cornerRadius, minRadius);

    const int numCircleSegments = SDL_max(NUM_CIRCLE_SEGMENTS, (int) clampedRadius * 0.5f);

    int totalVertices = 4 + (4 * (numCircleSegments * 2)) + 2*4;
    int totalIndices = 6 + (4 * (numCircleSegments * 3)) + 6*4;

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
    const float step = (SDL_PI_F/2) / numCircleSegments;
    for (int i = 0; i < numCircleSegments; i++) {
        const float angle1 = (float)i * step;
        const float angle2 = ((float)i + 1.0f) * step;

        for (int j = 0; j < 4; j++) {  // Iterate over four corners
            float cx, cy, signX, signY;

            switch (j) {
                case 0: cx = rect.x + clampedRadius; cy = rect.y + clampedRadius; signX = -1; signY = -1; break; // Top-left
                case 1: cx = rect.x + rect.w - clampedRadius; cy = rect.y + clampedRadius; signX = 1; signY = -1; break; // Top-right
                case 2: cx = rect.x + rect.w - clampedRadius; cy = rect.y + rect.h - clampedRadius; signX = 1; signY = 1; break; // Bottom-right
                case 3: cx = rect.x + clampedRadius; cy = rect.y + rect.h - clampedRadius; signX = -1; signY = 1; break; // Bottom-left
                default: return;
            }

            vertices[vertexCount++] = (SDL_Vertex){ {cx + SDL_cosf(angle1) * clampedRadius * signX, cy + SDL_sinf(angle1) * clampedRadius * signY}, color, {0, 0} };
            vertices[vertexCount++] = (SDL_Vertex){ {cx + SDL_cosf(angle2) * clampedRadius * signX, cy + SDL_sinf(angle2) * clampedRadius * signY}, color, {0, 0} };

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

static void SDL_RenderArc(SDL_Renderer *renderer, const SDL_FPoint center, const float radius, const float startAngle, const float endAngle, const float thickness, const Clay_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    const float radStart = startAngle * (SDL_PI_F / 180.0f);
    const float radEnd = endAngle * (SDL_PI_F / 180.0f);

    const int numCircleSegments = SDL_max(NUM_CIRCLE_SEGMENTS, (int)(radius * 1.5f)); //increase circle segments for larger circles, 1.5 is arbitrary.

    const float angleStep = (radEnd - radStart) / (float)numCircleSegments;
    const float thicknessStep = 0.4f; //arbitrary value to avoid overlapping lines. Changing THICKNESS_STEP or numCircleSegments might cause artifacts.

    for (float t = thicknessStep; t < thickness - thicknessStep; t += thicknessStep) {
        SDL_FPoint points[numCircleSegments + 1];
        const float clampedRadius = SDL_max(radius - t, 1.0f);

        for (int i = 0; i <= numCircleSegments; i++) {
            const float angle = radStart + i * angleStep;
            points[i] = (SDL_FPoint){
                    SDL_roundf(center.x + SDL_cosf(angle) * clampedRadius),
                    SDL_roundf(center.y + SDL_sinf(angle) * clampedRadius) };
        }
        SDL_RenderLines(renderer, points, numCircleSegments + 1);
    }
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
                    SDL_RenderFillRoundedRect(renderer, rect, config->cornerRadius.topLeft, color);
                } else {
                    SDL_RenderFillRect(renderer, &rect);
                }
            } break;
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                const Clay_TextElementConfig *config = rcmd->config.textElementConfig;
                const Clay_StringSlice *text = &rcmd->text;
                const SDL_Color color = { config->textColor.r, config->textColor.g, config->textColor.b, config->textColor.a };

                TTF_Font *font = gFonts[config->fontId];
                SDL_Surface *surface = TTF_RenderText_Blended(font, text->chars, text->length, color);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_RenderTexture(renderer, texture, NULL, &rect);

                SDL_DestroySurface(surface);
                SDL_DestroyTexture(texture);
            } break;
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                const Clay_BorderElementConfig *config = rcmd->config.borderElementConfig;

                const float minRadius = SDL_min(rect.w, rect.h) / 2.0f;
                const Clay_CornerRadius clampedRadii = {
                    .topLeft = SDL_min(config->cornerRadius.topLeft, minRadius),
                    .topRight = SDL_min(config->cornerRadius.topRight, minRadius),
                    .bottomLeft = SDL_min(config->cornerRadius.bottomLeft, minRadius),
                    .bottomRight = SDL_min(config->cornerRadius.bottomRight, minRadius)
                };
                //edges
                SDL_SetRenderDrawColor(renderer, config->left.color.r, config->left.color.g, config->left.color.b, config->left.color.a);
                if (config->left.width > 0) {
                    const float starting_y = rect.y + clampedRadii.topLeft;
                    const float length = rect.h - clampedRadii.topLeft - clampedRadii.bottomLeft;
                    SDL_FRect line = { rect.x, starting_y, config->left.width, length };
                    SDL_RenderFillRect(renderer, &line);
                }
                if (config->right.width > 0) {
                    const float starting_x = rect.x + rect.w - (float)config->right.width;
                    const float starting_y = rect.y + clampedRadii.topRight;
                    const float length = rect.h - clampedRadii.topRight - clampedRadii.bottomRight;
                    SDL_FRect line = { starting_x, starting_y, config->right.width, length };
                    SDL_RenderFillRect(renderer, &line);
                }
                if (config->top.width > 0) {
                    const float starting_x = rect.x + clampedRadii.topLeft;
                    const float length = rect.w - clampedRadii.topLeft - clampedRadii.topRight;
                    SDL_FRect line = { starting_x, rect.y, length, config->top.width };
                    SDL_RenderFillRect(renderer, &line);
                }
                if (config->bottom.width > 0) {
                    const float starting_x = rect.x + clampedRadii.bottomLeft;
                    const float starting_y = rect.y + rect.h - (float)config->bottom.width;
                    const float length = rect.w - clampedRadii.bottomLeft - clampedRadii.bottomRight;
                    SDL_FRect line = { starting_x, starting_y, length, config->bottom.width };
                    SDL_SetRenderDrawColor(renderer, config->bottom.color.r, config->bottom.color.g, config->bottom.color.b, config->bottom.color.a);
                    SDL_RenderFillRect(renderer, &line);
                }
                //corners
                if (config->cornerRadius.topLeft > 0) {
                    const float centerX = rect.x + clampedRadii.topLeft -1;
                    const float centerY = rect.y + clampedRadii.topLeft;
                    SDL_RenderArc(renderer, (SDL_FPoint){centerX, centerY}, clampedRadii.topLeft,
                        180.0f, 270.0f, config->top.width, config->top.color);
                }
                if (config->cornerRadius.topRight > 0) {
                    const float centerX = rect.x + rect.w - clampedRadii.topRight -1;
                    const float centerY = rect.y + clampedRadii.topRight;
                    SDL_RenderArc(renderer, (SDL_FPoint){centerX, centerY}, clampedRadii.topRight,
                        270.0f, 360.0f, config->top.width, config->top.color);
                }
                if (config->cornerRadius.bottomLeft > 0) {
                    const float centerX = rect.x + clampedRadii.bottomLeft -1;
                    const float centerY = rect.y + rect.h - clampedRadii.bottomLeft -1;
                    SDL_RenderArc(renderer, (SDL_FPoint){centerX, centerY}, clampedRadii.bottomLeft,
                        90.0f, 180.0f, config->bottom.width, config->bottom.color);
                }
                if (config->cornerRadius.bottomRight > 0) {
                    const float centerX = rect.x + rect.w - clampedRadii.bottomRight -1; //TODO: why need to -1 in all calculations???
                    const float centerY = rect.y + rect.h - clampedRadii.bottomRight -1;
                    SDL_RenderArc(renderer, (SDL_FPoint){centerX, centerY}, clampedRadii.bottomRight,
                        0.0f, 90.0f, config->bottom.width, config->bottom.color);
                }

            } break;
            default:
                SDL_Log("Unknown render command type: %d", rcmd->commandType);
        }
    }
}
