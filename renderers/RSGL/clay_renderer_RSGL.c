#include "RSGL.h"
#include "clay.h"

#include <stdio.h>

static Clay_Dimensions RSGL_MeasureText(Clay_String *text, Clay_TextElementConfig *config)
{
    RSGL_area area = RSGL_textArea(text->chars, config->fontSize, text->length);
    return (Clay_Dimensions) {
            .width = (float)area.w,
            .height = (float)area.h,
    };
}

RSGL_rectF currentClippingRectangle;

static void Clay_RSGL_Render(Clay_RenderCommandArray renderCommands)
{
    for (uint32_t i = 0; i < renderCommands.length; i++)
    {
        Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        Clay_BoundingBox boundingBox = renderCommand->boundingBox;
        switch (renderCommand->commandType)
        {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleElementConfig *config = renderCommand->config.rectangleElementConfig;
                RSGL_rectF rect = RSGL_RECTF(
                                boundingBox.x,
                                boundingBox.y,
                                boundingBox.width,
                                boundingBox.height);
                RSGL_color color = RSGL_RGBA(   (u8)(config->color.r), 
                                                (u8)(config->color.g), 
                                                (u8)(config->color.b), 
                                                (u8)(config->color.a));
                RSGL_drawRectF(rect, color);
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
                            boundingBox.height);
                RSGL_drawText_len(text.chars, text.length, destination, color);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                currentClippingRectangle = (RSGL_rectF) {
                            boundingBox.x,
                            boundingBox.y,
                            boundingBox.width,
                            boundingBox.height,
                };
                //printf("Clipping rectangle has not been implemented yet\n");
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                //printf("Clipping rectangle has not been implemented yet\n");
                break;
            }
            default: {
                fprintf(stderr, "Error: unhandled render command: %d\n", renderCommand->commandType);
            }
        }
    }
}