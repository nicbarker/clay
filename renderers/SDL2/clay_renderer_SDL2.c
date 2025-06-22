#include "../../clay.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159
#endif

#define CLAY_COLOR_TO_SDL_COLOR_ARGS(color) color.r, color.g, color.b, color.a

typedef struct
{
    uint32_t fontId;
    TTF_Font *font;
} SDL2_Font;


static Clay_Dimensions SDL2_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData)
{
    SDL2_Font *fonts = (SDL2_Font*)userData;

    TTF_Font *font = fonts[config->fontId].font;
    TTF_SetFontSize(font, config->fontSize);
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

/* Global for convenience. Even in 4K this is enough for smooth curves (low radius or rect size coupled with
 * no AA or low resolution might make it appear as jagged curves) */
static int NUM_CIRCLE_SEGMENTS = 16;

//all rendering is performed by a single SDL call, avoiding multiple RenderRect + plumbing choice for circles.
static void SDL_RenderFillRoundedRect(SDL_Renderer* renderer, const SDL_FRect rect, const float cornerRadius, const Clay_Color _color) {
    const SDL_Color color = (SDL_Color) {
            .r = (Uint8)_color.r,
            .g = (Uint8)_color.g,
            .b = (Uint8)_color.b,
            .a = (Uint8)_color.a,
    };

    int indexCount = 0, vertexCount = 0;

    const float maxRadius = SDL_min(rect.w, rect.h) / 2.0f;
    const float clampedRadius = SDL_min(cornerRadius, maxRadius);

    const int numCircleSegments = SDL_max(NUM_CIRCLE_SEGMENTS, (int)clampedRadius * 0.5f);

    SDL_Vertex vertices[512];
    int indices[512];

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
    const float step = (M_PI / 2) / numCircleSegments;
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

//all rendering is performed by a single SDL call, using twi sets of arcing triangles, inner and outer, that fit together; along with two tringles to fill the end gaps.
static void SDL_RenderCornerBorder(SDL_Renderer *renderer, Clay_BoundingBox* boundingBox, Clay_BorderRenderData* config, int cornerIndex, Clay_Color _color){
    /////////////////////////////////
    //The arc is constructed of outer triangles and inner triangles (if needed).
    //First three vertices are first outer triangle's vertices
    //Each two vertices after that are the inner-middle and second-outer vertex of 
    //each outer triangle after the first, because there first-outer vertex is equal to the
    //second-outer vertex of the previous triangle. Indices set accordingly.
    //The final two vertices are the missing vertices for the first and last inner triangles (if needed)
    //Everything is in clockwise order (CW).
    /////////////////////////////////
    const SDL_Color color = (SDL_Color) {
        .r = (Uint8)_color.r,
        .g = (Uint8)_color.g,
        .b = (Uint8)_color.b,
        .a = (Uint8)_color.a,
    };

    float centerX, centerY, outerRadius, clampedRadius, startAngle, borderWidth;
    const float maxRadius = SDL_min(boundingBox->width, boundingBox->height) / 2.0f;
    
    SDL_Vertex vertices[512];
    int indices[512];
    int indexCount = 0, vertexCount = 0;

    switch (cornerIndex) {
        case(0):
            startAngle = M_PI; 
            outerRadius = SDL_min(config->cornerRadius.topLeft, maxRadius);
            centerX = boundingBox->x + outerRadius; 
            centerY = boundingBox->y + outerRadius; 
            borderWidth = config->width.top;
        break;
        case(1):
            startAngle = 3*M_PI/2;
            outerRadius = SDL_min(config->cornerRadius.topRight, maxRadius);
            centerX = boundingBox->x + boundingBox->width - outerRadius; 
            centerY = boundingBox->y + outerRadius; 
            borderWidth = config->width.top;
            break;
        case(2):
            startAngle = 0;
            outerRadius = SDL_min(config->cornerRadius.bottomRight, maxRadius);
            centerX = boundingBox->x + boundingBox->width - outerRadius; 
            centerY = boundingBox->y + boundingBox->height - outerRadius; 
            borderWidth = config->width.bottom;
            break;
        case(3):
            startAngle = M_PI/2;
            outerRadius = SDL_min(config->cornerRadius.bottomLeft, maxRadius);
            centerX = boundingBox->x + outerRadius; 
            centerY = boundingBox->y + boundingBox->height - outerRadius; 
            borderWidth = config->width.bottom;
            break;
        default: break;
    }
    
    const float innerRadius = outerRadius - borderWidth;
    const int minNumOuterTriangles = NUM_CIRCLE_SEGMENTS;
    const int numOuterTriangles = SDL_max(minNumOuterTriangles, ceilf(outerRadius * 0.5f));
    const float angleStep = M_PI / (2.0*(float)numOuterTriangles);

    //outer triangles, in CW order
    for (int i = 0; i < numOuterTriangles; i++) { 
        float angle1 =  startAngle + i*angleStep; //first-outer vertex angle
        float angle2 =  startAngle + ((float)i + 0.5) * angleStep; //inner-middle vertex angle
        float angle3 =  startAngle + (i+1)*angleStep; // second-outer vertex angle

        if( i == 0){ //first outer triangle
            vertices[vertexCount++] = (SDL_Vertex){ {centerX + SDL_cosf(angle1) * outerRadius, centerY + SDL_sinf(angle1) * outerRadius}, color, {0, 0} }; //vertex index = 0
        }
        indices[indexCount++] = vertexCount - 1; //will be second-outer vertex of last outer triangle if not first outer triangle.

        vertices[vertexCount++] = (innerRadius > 0)?
            (SDL_Vertex){ {centerX + SDL_cosf(angle2) * (innerRadius), centerY + SDL_sinf(angle2) * (innerRadius)}, color, {0, 0}}:
            (SDL_Vertex){ {centerX, centerY }, color, {0, 0}};
        indices[indexCount++] = vertexCount - 1;

        vertices[vertexCount++] = (SDL_Vertex){ {centerX + SDL_cosf(angle3) * outerRadius, centerY + SDL_sinf(angle3) * outerRadius}, color, {0, 0} };
        indices[indexCount++] = vertexCount - 1;
    }

    if(innerRadius > 0){
        // inner triangles in CW order (except the first and last)
        for (int i = 0; i < numOuterTriangles - 1; i++){ //skip the last outer triangle
            if(i==0){ //first outer triangle -> second inner triangle
                indices[indexCount++] = 1; //inner-middle vertex of first outer triangle
                indices[indexCount++] = 2; //second-outer vertex of first outer triangle
                indices[indexCount++] = 3; //innder-middle vertex of second-outer triangle
            }else{
                int baseIndex = 3; //skip first outer triangle
                indices[indexCount++] = baseIndex + (i-1)*2; // inner-middle vertex of current outer triangle
                indices[indexCount++] = baseIndex + (i-1)*2 + 1; // second-outer vertex of current outer triangle
                indices[indexCount++] = baseIndex + (i-1)*2 + 2; // inner-middle vertex of next outer triangle
            }
        }

        float endAngle = startAngle + M_PI/2.0;

        //last inner triangle
        indices[indexCount++] = vertexCount - 2; //inner-middle vertex of last outer triangle
        indices[indexCount++] = vertexCount - 1; //second-outer vertex of last outer triangle
        vertices[vertexCount++] = (SDL_Vertex){ {centerX + SDL_cosf(endAngle) * innerRadius, centerY + SDL_sinf(endAngle) * innerRadius}, color, {0, 0} }; //missing vertex
        indices[indexCount++] = vertexCount - 1; 
        
        // //first inner triangle
        indices[indexCount++] = 0; //first-outer vertex of first outer triangle
        indices[indexCount++] = 1; //inner-middle vertex of first outer triangle
        vertices[vertexCount++] = (SDL_Vertex){ {centerX + SDL_cosf(startAngle) * innerRadius, centerY + SDL_sinf(startAngle) * innerRadius}, color, {0, 0} }; //missing vertex
        indices[indexCount++] = vertexCount - 1; 
    }

    SDL_RenderGeometry(renderer, NULL, vertices, vertexCount, indices, indexCount);
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
                Clay_RectangleRenderData *config = &renderCommand->renderData.rectangle;
                Clay_Color color = config->backgroundColor;
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_FRect rect = (SDL_FRect) {
                        .x = boundingBox.x,
                        .y = boundingBox.y,
                        .w = boundingBox.width,
                        .h = boundingBox.height,
                };
                if (config->cornerRadius.topLeft > 0) {
                    SDL_RenderFillRoundedRect(renderer, rect, config->cornerRadius.topLeft, color);
                }
                else {
                    SDL_RenderFillRectF(renderer, &rect);
                }
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextRenderData *config = &renderCommand->renderData.text;
                char *cloned = (char *)calloc(config->stringContents.length + 1, 1);
                memcpy(cloned, config->stringContents.chars, config->stringContents.length);
                TTF_Font* font = fonts[config->fontId].font;
                TTF_SetFontSize(font, config->fontSize);
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
                Clay_ImageRenderData *config = &renderCommand->renderData.image;

                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, config->imageData);

                SDL_Rect destination = (SDL_Rect){
                    .x = boundingBox.x,
                    .y = boundingBox.y,
                    .w = boundingBox.width,
                    .h = boundingBox.height,
                };

                SDL_RenderCopy(renderer, texture, NULL, &destination);

                SDL_DestroyTexture(texture);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                Clay_BorderRenderData *config = &renderCommand->renderData.border;
                SDL_SetRenderDrawColor(renderer, CLAY_COLOR_TO_SDL_COLOR_ARGS(config->color));

                if(boundingBox.width > 0 & boundingBox.height > 0){
                    const float maxRadius = SDL_min(boundingBox.width, boundingBox.height) / 2.0f;

                    if (config->width.left > 0) {
                        const float clampedRadiusTop = SDL_min((float)config->cornerRadius.topLeft, maxRadius);
                        const float clampedRadiusBottom = SDL_min((float)config->cornerRadius.bottomLeft, maxRadius);
                        SDL_FRect rect = { 
                            boundingBox.x, 
                            boundingBox.y + clampedRadiusTop, 
                            (float)config->width.left, 
                            (float)boundingBox.height - clampedRadiusTop - clampedRadiusBottom
                        };
                        SDL_RenderFillRectF(renderer, &rect);
                    }
    
                    if (config->width.right > 0) {
                        const float clampedRadiusTop = SDL_min((float)config->cornerRadius.topRight, maxRadius);
                        const float clampedRadiusBottom = SDL_min((float)config->cornerRadius.bottomRight, maxRadius);
                        SDL_FRect rect = { 
                            boundingBox.x + boundingBox.width - config->width.right,
                            boundingBox.y + clampedRadiusTop,
                            (float)config->width.right,
                            (float)boundingBox.height - clampedRadiusTop - clampedRadiusBottom
                        };
                        SDL_RenderFillRectF(renderer, &rect);
                    }
    
                    if (config->width.top > 0) {
                        const float clampedRadiusLeft = SDL_min((float)config->cornerRadius.topLeft, maxRadius);
                        const float clampedRadiusRight = SDL_min((float)config->cornerRadius.topRight, maxRadius);
                        SDL_FRect rect = { 
                            boundingBox.x + clampedRadiusLeft, 
                            boundingBox.y, 
                            boundingBox.width - clampedRadiusLeft - clampedRadiusRight, 
                            (float)config->width.top };
                        SDL_RenderFillRectF(renderer, &rect);
                    }
    
                    if (config->width.bottom > 0) {
                        const float clampedRadiusLeft = SDL_min((float)config->cornerRadius.bottomLeft, maxRadius);
                        const float clampedRadiusRight = SDL_min((float)config->cornerRadius.bottomRight, maxRadius);
                        SDL_FRect rect = { 
                            boundingBox.x + clampedRadiusLeft, 
                            boundingBox.y + boundingBox.height - config->width.bottom, 
                            boundingBox.width - clampedRadiusLeft - clampedRadiusRight, 
                            (float)config->width.bottom 
                        };
                        SDL_RenderFillRectF(renderer, &rect);
                    }
    
                    //corner index: 0->3 topLeft -> CW -> bottonLeft
                    if (config->width.top > 0 & config->cornerRadius.topLeft > 0) {
                        SDL_RenderCornerBorder(renderer, &boundingBox, config, 0, config->color);
                    }

                    if (config->width.top > 0 & config->cornerRadius.topRight> 0) {
                        SDL_RenderCornerBorder(renderer, &boundingBox, config, 1, config->color);
                    }

                    if (config->width.bottom > 0 & config->cornerRadius.bottomRight > 0) {
                        SDL_RenderCornerBorder(renderer, &boundingBox, config, 2, config->color);
                    }

                    if (config->width.bottom > 0 & config->cornerRadius.bottomLeft > 0) {
                        SDL_RenderCornerBorder(renderer, &boundingBox, config, 3, config->color);
                    }
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
