#include "../../clay.h"
#include <nanovg.h>

#include <stdlib.h>
#include <stdio.h>

static Clay_Dimensions Clay_NanoVG_MeasureText(Clay_StringSlice text, Clay_TextElementConfig* config, uintptr_t user)
{
    NVGcontext* nvg = (NVGcontext*)user;
    nvgFontFaceId(nvg, config->fontId);
    nvgFontSize(nvg, config->fontSize);
    nvgTextLineHeight(nvg, config->lineHeight);
    nvgTextLetterSpacing(nvg, config->letterSpacing);
    nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    float bounds[4];
    nvgTextBounds(nvg, 0, 0, text.chars, text.chars + text.length, bounds);
    return (Clay_Dimensions) {
        .width = bounds[2],
        .height = bounds[3],
    };
}

static void Clay_NanoVG_Render(NVGcontext *nvg, Clay_RenderCommandArray commands)
{
    for (int i = 0; i < commands.length; i++) {
        Clay_RenderCommand* command = Clay_RenderCommandArray_Get(&commands, i);
        Clay_BoundingBox box = command->boundingBox;
        switch (command->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleElementConfig* config = command->config.rectangleElementConfig;
                Clay_Color color = config->color;
                nvgBeginPath(nvg);
                nvgFillColor(nvg, nvgRGBA(color.r, color.g, color.b, color.a));
                nvgRoundedRect(nvg, box.x, box.y, box.width, box.height, config->cornerRadius.topLeft);
                nvgFill(nvg);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextElementConfig* config = command->config.textElementConfig;
                Clay_String text = command->text;
                Clay_Color color = config->textColor;
                nvgFontFaceId(nvg, config->fontId);
                nvgFontSize(nvg, config->fontSize);
                nvgTextLineHeight(nvg, config->lineHeight);
                nvgTextLetterSpacing(nvg, config->letterSpacing);
                nvgFillColor(nvg, nvgRGBA(color.r, color.g, color.b, color.a));
                nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
                nvgText(nvg, box.x, box.y, text.chars, text.chars + text.length);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                nvgScissor(nvg, box.x, box.y, box.width, box.height);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                nvgResetScissor(nvg);
                break;
            }
            default: {
                fprintf(stderr, "Error: unhandled render command: %d\n", command->commandType);
                exit(1);
            }
        }
    }
}
