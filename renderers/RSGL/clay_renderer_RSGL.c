#include "RSGL.h"
#include "clay.h"

#include <stdio.h>

#define RFONT_FONT_SCALE 1

#define CLAY_COLOR_TO_RSGL_COLOR(color) \
        RSGL_RGBA((unsigned char)roundf(color.r), (unsigned char)roundf(color.g), (unsigned char)roundf(color.b), (unsigned char)roundf(color.a))

static Clay_Dimensions RSGL_MeasureText(Clay_String *text, Clay_TextElementConfig *config)
{
    RSGL_area area = RSGL_textArea(text->chars, config->fontSize / RFONT_FONT_SCALE, text->length);
    return (Clay_Dimensions) {
            .width = (float)area.w,
            .height = (float)area.h,
    };
}

static void Clay_RSGL_Render(Clay_RenderCommandArray renderCommands)
{
    for (uint32_t i = 0; i < renderCommands.length; i++)
    {
        Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        RSGL_rectF boundingBox = RSGL_RECTF(renderCommand->boundingBox.x, 
                                        renderCommand->boundingBox.y, 
                                        renderCommand->boundingBox.width,
                                        renderCommand->boundingBox.height);

        switch (renderCommand->commandType)
        {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleElementConfig *config = renderCommand->config.rectangleElementConfig;
                RSGL_drawRectF(boundingBox, CLAY_COLOR_TO_RSGL_COLOR(config->color));
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextElementConfig *config = renderCommand->config.textElementConfig;
                Clay_String text = renderCommand->text;
                RSGL_setFont(config->fontId);
                
                RSGL_color color = RSGL_RGBA(   (u8)(config->textColor.r), 
                                                (u8)(config->textColor.g), 
                                                (u8)(config->textColor.b), 
                                                (u8)(config->textColor.a));

                RSGL_circle destination = RSGL_CIRCLE(
                                boundingBox.x,
                                boundingBox.y,
                                config->fontSize / RFONT_FONT_SCALE);
                RSGL_drawText_pro(text.chars, text.length, config->letterSpacing, destination, color);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_IMAGE:
                Clay_ImageElementConfig* config = renderCommand->config.imageElementConfig;
                RSGL_setTexture((RSGL_texture)config->imageData);
                RSGL_drawRectF(boundingBox, RSGL_RGBA(255, 255, 255, 255));
                break;
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                Clay_BorderElementConfig *config = renderCommand->config.borderElementConfig;
                if (config->left.width > 0) {
                    RSGL_drawRectF(RSGL_RECTF(boundingBox.x, 
                                                boundingBox.y + config->cornerRadius.topLeft, 
                                                config->left.width, 
                                                boundingBox.h - config->cornerRadius.topLeft - config->cornerRadius.bottomLeft), 
                                                CLAY_COLOR_TO_RSGL_COLOR(config->left.color));
                }
                if (config->right.width > 0) {                    
                    RSGL_drawRectF(RSGL_RECTF(boundingBox.x + boundingBox.w - config->right.width, 
                                                boundingBox.y + config->cornerRadius.topRight, 
                                                config->right.width, 
                                                boundingBox.h - config->cornerRadius.topRight - config->cornerRadius.bottomRight),
                                                CLAY_COLOR_TO_RSGL_COLOR(config->right.color));
                }
                if (config->top.width > 0) {RSGL_drawRectF(RSGL_RECTF(boundingBox.x + config->cornerRadius.topLeft, boundingBox.y, 
                                                boundingBox.w - config->cornerRadius.topLeft - config->cornerRadius.topRight,  
                                                config->top.width), 
                                                CLAY_COLOR_TO_RSGL_COLOR(config->top.color));
                }
                if (config->bottom.width > 0) {RSGL_drawRectF(RSGL_RECTF(boundingBox.x + config->cornerRadius.bottomLeft, \
                                                boundingBox.y + boundingBox.h - config->bottom.width, 
                                                boundingBox.w - config->cornerRadius.bottomLeft - config->cornerRadius.bottomRight, 
                                                config->bottom.width),
                                                CLAY_COLOR_TO_RSGL_COLOR(config->bottom.color));
                }
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                RSGL_renderScissorStart(boundingBox);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                RSGL_renderScissorEnd();
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_CUSTOM:
                printf("Custom render commands have not been implemented yet\n");
                break;
            default: {
                fprintf(stderr, "Error: unhandled render command: %d\n", renderCommand->commandType);
            }
        }
    }
}