#include "pd_api.h"
#include "../../clay.h"

// Playdate drawText function expects the number of codepoints to draw, not byte length
static size_t Clay_Playdate_CountUtf8Codepoints(const char *str, size_t byteLen) {
    size_t count = 0;
    size_t i = 0;
    while (i < byteLen) {
        uint8_t c = (uint8_t)str[i];
        if ((c & 0xC0) != 0x80) {
            count++;
        }
        i++;
    }
    return count;
}

// As the playdate can only display black and white, we need to resolve Clay_color to either black or white
// for both color and draw mode.
static LCDColor clayColorToLCDColor(Clay_Color color) {
    if (color.r > 0 || color.g > 0 || color.b > 0) {
        return kColorWhite;
    }
    return kColorBlack;
}

static LCDBitmapDrawMode clayColorToDrawMode(Clay_Color color) {
    if (color.r > 0 || color.g > 0 || color.b > 0) {
        return kDrawModeFillWhite;
    }
    return kDrawModeCopy;
}

static float clampCornerRadius(float yAxisSize, float radius) {
    if (radius < 1.0f) {
        return 0.0f;
    }
    if (radius > yAxisSize / 2) {
        return yAxisSize / 2;
    }
    // Trying to draw a 2x2 ellipse seems to result in just a dot, so if
    // there is a corner radius at minimum it must be 2
    return CLAY__MAX(2, radius);
}

static void Clay_Playdate_Render(PlaydateAPI *pd, Clay_RenderCommandArray renderCommands, LCDFont **fonts) {
    for (uint32_t i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        Clay_BoundingBox boundingBox = renderCommand->boundingBox;

        switch (renderCommand->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleRenderData *config = &renderCommand->renderData.rectangle;

                float radiusTl = clampCornerRadius(boundingBox.height, config->cornerRadius.topLeft);
                float radiusTr = clampCornerRadius(boundingBox.height, config->cornerRadius.topRight);
                float radiusBl = clampCornerRadius(boundingBox.height, config->cornerRadius.bottomLeft);
                float radiusBr = clampCornerRadius(boundingBox.height, config->cornerRadius.bottomRight);

                pd->graphics->fillEllipse(
                    boundingBox.x, boundingBox.y,
                    radiusTl * 2, radiusTl * 2,
                    -90.0f, 0.0f,
                    clayColorToLCDColor(config->backgroundColor)
                );

                pd->graphics->fillEllipse(
                    boundingBox.x + boundingBox.width - radiusTr * 2, boundingBox.y,
                    radiusTr * 2, radiusTr * 2,
                    0.0f, 90.0f,
                    clayColorToLCDColor(config->backgroundColor)
                );

                pd->graphics->fillEllipse(
                    boundingBox.x + boundingBox.width - radiusBr * 2,
                    boundingBox.y + boundingBox.height - radiusBr * 2,
                    radiusBr * 2, radiusBr * 2,
                    90.0f, 180.0f,
                    clayColorToLCDColor(config->backgroundColor)
                );

                pd->graphics->fillEllipse(
                    boundingBox.x,
                    boundingBox.y + boundingBox.height - radiusBl * 2,
                    radiusBl * 2, radiusBl * 2,
                    180.0f, 270.0f,
                    clayColorToLCDColor(config->backgroundColor)
                );

                // Top chunk
                pd->graphics->fillRect(
                    boundingBox.x + radiusTl, boundingBox.y,
                    boundingBox.width - radiusTl - radiusTr,
                    CLAY__MAX(radiusTl, radiusTr),
                    clayColorToLCDColor(config->backgroundColor)
                );

                // bottom chunk
                int bottomChunkHeight = CLAY__MAX(radiusBl, radiusBr);
                pd->graphics->fillRect(
                    boundingBox.x + radiusBl, boundingBox.y + boundingBox.height - bottomChunkHeight,
                    boundingBox.width - radiusBl - radiusBr,
                    bottomChunkHeight,
                    clayColorToLCDColor(config->backgroundColor)
                );

                // Middle chunk
                int middleChunkHeight = boundingBox.height - CLAY__MIN(radiusBr, radiusBl) - CLAY__MIN(radiusTr, radiusTl);
                pd->graphics->fillRect(
                    boundingBox.x + CLAY__MIN(radiusTl, radiusBl), boundingBox.y + CLAY__MIN(radiusTr, radiusTl),
                    boundingBox.width - radiusBl - radiusBr,
                    middleChunkHeight,
                    clayColorToLCDColor(config->backgroundColor)
                );

                // Left chunk
                int leftChunkHeight = boundingBox.height - radiusTl - radiusBl;
                int leftChunkWidth = CLAY__MAX(radiusTl, radiusBl);
                pd->graphics->fillRect(
                    boundingBox.x, boundingBox.y + radiusTl,
                    leftChunkWidth,
                    leftChunkHeight,
                    clayColorToLCDColor(config->backgroundColor)
                );

                // Right chunk
                int rightChunkHeight = boundingBox.height - radiusTr - radiusBr;
                int rightChunkWidth = CLAY__MAX(radiusTr, radiusBr);
                pd->graphics->fillRect(
                    boundingBox.x + boundingBox.width - rightChunkWidth, boundingBox.y + radiusTr,
                    rightChunkWidth,
                    rightChunkHeight,
                    clayColorToLCDColor(config->backgroundColor)
                );
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextRenderData *config = &renderCommand->renderData.text;
                LCDFont *font = fonts[config->fontId];
                pd->graphics->setFont(font);
                pd->graphics->setDrawMode(clayColorToDrawMode(config->textColor));
                pd->graphics->drawText(
                    renderCommand->renderData.text.stringContents.chars,
                    Clay_Playdate_CountUtf8Codepoints(
                        renderCommand->renderData.text.stringContents.chars,
                        renderCommand->renderData.text.stringContents.length
                    ),
                    kUTF8Encoding,
                    boundingBox.x,
                    boundingBox.y
                );
                pd->graphics->setDrawMode(kDrawModeCopy);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                pd->graphics->setClipRect(
                    boundingBox.x,boundingBox.y,
                    boundingBox.width, boundingBox.height
                );
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                pd->graphics->clearClipRect();
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
                Clay_ImageRenderData *config = &renderCommand->renderData.image;
                LCDBitmap *texture = config->imageData;
                int texWidth;
                int texHeight;
                pd->graphics->getBitmapData(texture, &texWidth, &texHeight, NULL, NULL, NULL);
                if (texWidth != boundingBox.width || texHeight != boundingBox.height) {
                    pd->graphics->drawScaledBitmap(
                        texture,
                        boundingBox.x, boundingBox.y,
                        boundingBox.width / texWidth,
                        boundingBox.height / texHeight
                    );
                } else {
                    pd->graphics->drawBitmap(texture, boundingBox.x, boundingBox.y, kBitmapUnflipped);
                }
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                Clay_BorderRenderData *config = &renderCommand->renderData.border;

                float radiusTl = clampCornerRadius(boundingBox.height, config->cornerRadius.topLeft);
                float radiusTr = clampCornerRadius(boundingBox.height, config->cornerRadius.topRight);
                float radiusBl = clampCornerRadius(boundingBox.height, config->cornerRadius.bottomLeft);
                float radiusBr = clampCornerRadius(boundingBox.height, config->cornerRadius.bottomRight);

                if (config->width.top > 0) {
                    pd->graphics->drawEllipse(
                        boundingBox.x, boundingBox.y,
                        radiusTl * 2, radiusTl * 2,
                        config->width.top,
                        -90.0f, 0.0f,
                        clayColorToLCDColor(config->color)
                    );

                    pd->graphics->drawLine(
                        boundingBox.x + radiusTl, boundingBox.y,
                        boundingBox.x + boundingBox.width - radiusTr - config->width.right, boundingBox.y,
                        config->width.top,
                        clayColorToLCDColor(config->color)
                    );

                    pd->graphics->drawEllipse(
                        boundingBox.x + boundingBox.width - radiusTr * 2, boundingBox.y,
                        radiusTr * 2, radiusTr * 2,
                        config->width.top,
                        0.0f, 90.0f,
                        clayColorToLCDColor(config->color)
                    );
                }

                if (config->width.right > 0 && radiusTr + radiusBr <= boundingBox.height) {
                    pd->graphics->drawLine(
                        boundingBox.x + boundingBox.width - config->width.right,
                        boundingBox.y + radiusTr,
                        boundingBox.x + boundingBox.width - config->width.right,
                        boundingBox.y + boundingBox.height - radiusBr - config->width.bottom,
                        config->width.right,
                        clayColorToLCDColor(config->color)
                    );
                }

                if (config->width.bottom > 0) {
                    pd->graphics->drawEllipse(
                        boundingBox.x + boundingBox.width - radiusBr * 2,
                        boundingBox.y + boundingBox.height - radiusBr * 2,
                        radiusBr * 2, radiusBr * 2,
                        config->width.bottom,
                        90.0f, 180.0f,
                        clayColorToLCDColor(config->color)
                    );

                    pd->graphics->drawLine(
                        boundingBox.x + boundingBox.width - radiusBr - config->width.right,
                        boundingBox.y + boundingBox.height - config->width.bottom,
                        boundingBox.x + radiusBl,
                        boundingBox.y + boundingBox.height - config->width.bottom,
                        config->width.bottom,
                        clayColorToLCDColor(config->color)
                    );

                    pd->graphics->drawEllipse(
                        boundingBox.x,
                        boundingBox.y + boundingBox.height - radiusBl * 2,
                        radiusBl * 2, radiusBl * 2,
                        config->width.bottom,
                        180.0f, 270.0f,
                        clayColorToLCDColor(config->color)
                    );
                }

                if (config->width.left > 0 && radiusBl + radiusTl < boundingBox.height) {
                    pd->graphics->drawLine(
                        boundingBox.x, boundingBox.y + boundingBox.height - radiusBl - config->width.bottom,
                        boundingBox.x, boundingBox.y + radiusTl,
                        config->width.left,
                        clayColorToLCDColor(config->color)
                    );
                }
                break;
            }
            default: {
                pd->system->logToConsole("Error: unhandled render command: %d\n", renderCommand->commandType);
                return;
            }
        }
    }
}
