#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#ifdef CLAY_OVERFLOW_TRAP
#include "signal.h"
#endif

static inline void Console_MoveCursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

bool Clay_PointIsInsideRect(Clay_Vector2 point, Clay_BoundingBox rect) {
    // TODO this function is a copy of Clay__PointIsInsideRect but that one is internal, I don't know if we want
    // TODO to expose Clay__PointIsInsideRect
    return point.x >= rect.x && point.x < rect.x + rect.width && point.y >= rect.y && point.y < rect.y + rect.height;
}

static inline void Console_DrawRectangle(int x0, int y0, int width, int height, Clay_Color color,
                                         Clay_BoundingBox scissorBox) {
    float average = (color.r + color.g + color.b + color.a) / 4 / 255;

    for (int y = y0; y < height + y0; y++) {
        for (int x = x0; x < width + x0; x++) {
            if (!Clay_PointIsInsideRect((Clay_Vector2) {.x = x, .y = y}, scissorBox)) {
                continue;
            }

            Console_MoveCursor(x, y);
            // TODO this should be replaced by a better logarithmic scale if we're doing black and white
            if (average > 0.75) {
                printf("█");
            } else if (average > 0.5) {
                printf("▓");
            } else if (average > 0.25) {
                printf("▒");
            } else {
                printf("░");
            }
        }
    }
}

static inline Clay_Dimensions
Console_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    Clay_Dimensions textSize = {0};
    int columnWidth = *(int *) userData;

    // TODO this function is very wrong, it measures in characters, I have no idea what is the size in pixels

    float maxTextWidth = 0.0f;
    float lineTextWidth = 0;

    float textHeight = 1;

    for (int i = 0; i < text.length; ++i) {
        if (text.chars[i] == '\n') {
            maxTextWidth = maxTextWidth > lineTextWidth ? maxTextWidth : lineTextWidth;
            lineTextWidth = 0;
            textHeight++;
            continue;
        }
        lineTextWidth++;
    }

    maxTextWidth = maxTextWidth > lineTextWidth ? maxTextWidth : lineTextWidth;

    textSize.width = maxTextWidth * columnWidth;
    textSize.height = textHeight * columnWidth;

    return textSize;
}

void Clay_Terminal_Render(Clay_RenderCommandArray renderCommands, int width, int height, int columnWidth) {
    printf("\033[H\033[J"); // Clear

    const Clay_BoundingBox fullWindow = {
            .x = 0,
            .y = 0,
            .width = (float) width,
            .height = (float) height,
    };

    Clay_BoundingBox scissorBox = fullWindow;

    for (int j = 0; j < renderCommands.length; j++) {
        Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, j);
        Clay_BoundingBox boundingBox = (Clay_BoundingBox) {
                .x = (int)((renderCommand->boundingBox.x / columnWidth) + 0.5),
                .y = (int)((renderCommand->boundingBox.y / columnWidth) + 0.5),
                .width = (int)((renderCommand->boundingBox.width / columnWidth) + 0.5),
                .height = (int)((renderCommand->boundingBox.height / columnWidth) + 0.5),
        };
        switch (renderCommand->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextRenderData data = renderCommand->renderData.text;
                Clay_StringSlice text = data.stringContents;
                int y = 0;
                for (int x = 0; x < text.length; x++) {
                    if (text.chars[x] == '\n') {
                        y++;
                        continue;
                    }

                    int cursorX = (int) boundingBox.x + x;
                    int cursorY = (int) boundingBox.y + y;
                    if (cursorY > scissorBox.y + scissorBox.height) {
                        break;
                    }
                    if (!Clay_PointIsInsideRect((Clay_Vector2) {.x = cursorX, .y = cursorY}, scissorBox)) {
                        continue;
                    }

                    Console_MoveCursor(cursorX, cursorY);
                    printf("%c", text.chars[x]);
                }
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                scissorBox = boundingBox;
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                scissorBox = fullWindow;
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleRenderData data = renderCommand->renderData.rectangle;
                Console_DrawRectangle(
                        (int) boundingBox.x,
                        (int) boundingBox.y,
                        (int) boundingBox.width,
                        (int) boundingBox.height,
                        data.backgroundColor,
                        scissorBox);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                Clay_BorderRenderData data = renderCommand->renderData.border;
                // Left border
                if (data.width.left > 0) {
                    Console_DrawRectangle(
                            (int) (boundingBox.x),
                            (int) (boundingBox.y + data.cornerRadius.topLeft),
                            (int) data.width.left,
                            (int) (boundingBox.height - data.cornerRadius.topLeft - data.cornerRadius.bottomLeft),
                            data.color,
                            scissorBox);
                }
                // Right border
                if (data.width.right > 0) {
                    Console_DrawRectangle(
                            (int) (boundingBox.x + boundingBox.width - data.width.right),
                            (int) (boundingBox.y + data.cornerRadius.topRight),
                            (int) data.width.right,
                            (int) (boundingBox.height - data.cornerRadius.topRight - data.cornerRadius.bottomRight),
                            data.color,
                            scissorBox);
                }
                // Top border
                if (data.width.top > 0) {
                    Console_DrawRectangle(
                            (int) (boundingBox.x + data.cornerRadius.topLeft),
                            (int) (boundingBox.y),
                            (int) (boundingBox.width - data.cornerRadius.topLeft - data.cornerRadius.topRight),
                            (int) data.width.top,
                            data.color,
                            scissorBox);
                }
                // Bottom border
                if (data.width.bottom > 0) {
                    Console_DrawRectangle(
                            (int) (boundingBox.x + data.cornerRadius.bottomLeft),
                            (int) (boundingBox.y + boundingBox.height - data.width.bottom),
                            (int) (boundingBox.width - data.cornerRadius.bottomLeft - data.cornerRadius.bottomRight),
                            (int) data.width.bottom,
                            data.color,
                            scissorBox);
                }
                break;
            }
            default: {
                printf("Error: unhandled render command.");
#ifdef CLAY_OVERFLOW_TRAP
                raise(SIGTRAP);
#endif
                exit(1);
            }
        }
    }

    Console_MoveCursor(-1, -1);  // TODO make the user not be able to write
}
