#include "../../clay.h"
#include "pd_api.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct Rect {
  float x;
  float y;
  float w;
  float h;
};

static size_t utf8_count_codepoints(const char *str, size_t byte_len) {
  size_t count = 0;
  size_t i = 0;
  while (i < byte_len) {
    uint8_t c = (uint8_t)str[i];

    // Skip continuation bytes (10xxxxxx)
    if ((c & 0xC0) != 0x80) {
      count++;
    }

    i++;
  }
  return count;
}

static LCDColor clayColorToLCDColor(Clay_Color color) {
  // Convert the RGB to grayscale using the standard luminance formula
  unsigned char grayscale_value = (unsigned char)(0.299 * color.r + 0.587 * color.g + 0.114 * color.b);

  if (grayscale_value > 128) {
    return kColorWhite;
  }
  return kColorBlack;
}

static LCDBitmapDrawMode clayColorToDrawMode(Clay_Color color) {

  if (color.r == 255 && color.g == 255 && color.b == 255) {
    return kDrawModeFillWhite;
  }
  return kDrawModeCopy;
}

static void Clay_Playdate_Render(PlaydateAPI *pd, Clay_RenderCommandArray renderCommands, LCDFont *fonts) {

  for (uint32_t i = 0; i < renderCommands.length; i++) {
    Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
    Clay_BoundingBox boundingBox = renderCommand->boundingBox;
    switch (renderCommand->commandType) {
    case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
      Clay_RectangleRenderData *config = &renderCommand->renderData.rectangle;
      struct Rect rect = (struct Rect){
        .x = boundingBox.x,
        .y = boundingBox.y,
        .w = boundingBox.width,
        .h = boundingBox.height,
      };
      if (config->cornerRadius.topLeft > 0) {
        pd->graphics->fillRoundRect(
          rect.x, rect.y, rect.w, rect.h,
          config->cornerRadius.topLeft,
          clayColorToLCDColor(config->backgroundColor)
        );
      } else {
        pd->graphics->fillRect(rect.x, rect.y, rect.w, rect.h,
                               clayColorToLCDColor(config->backgroundColor));
      }
      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_TEXT: {
      Clay_TextRenderData *config = &renderCommand->renderData.text;
      // LCDFont *font = fonts[config->fontId];
      // TODO: support loading more than 1 font and use the fonts that clay
      // layout has..
      LCDFont *font = fonts;
      struct Rect destination = (struct Rect){
        .x = boundingBox.x,
        .y = boundingBox.y,
        .w = boundingBox.width,
        .h = boundingBox.height,
      };
      pd->graphics->setDrawMode(clayColorToDrawMode(config->textColor));
      pd->graphics->drawText(
        renderCommand->renderData.text.stringContents.chars,
        utf8_count_codepoints(
          renderCommand->renderData.text.stringContents.chars,
          renderCommand->renderData.text.stringContents.length
        ),
        kUTF8Encoding,
        destination.x,
        destination.y
      );
      pd->graphics->setDrawMode(kDrawModeCopy);
      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
      struct Rect currentClippingRectangle = (struct Rect){
        .x = boundingBox.x,
        .y = boundingBox.y,
        .w = boundingBox.width,
        .h = boundingBox.height,
      };
      pd->graphics->setClipRect(
        currentClippingRectangle.x, currentClippingRectangle.y,
        currentClippingRectangle.w, currentClippingRectangle.h
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
      struct Rect destination = (struct Rect){
        .x = boundingBox.x,
        .y = boundingBox.y,
        .w = boundingBox.width,
        .h = boundingBox.height,
      };
      pd->graphics->drawBitmap(texture, destination.x, destination.y, kBitmapUnflipped);
      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_BORDER: {
      Clay_BorderRenderData *config = &renderCommand->renderData.border;
      struct Rect rect = (struct Rect){
        .x = boundingBox.x,
        .y = boundingBox.y,
        .w = boundingBox.width,
        .h = boundingBox.height,
      };

      // TODO: properly support the different border thickness and radius
      // instead of just using topLeft corner /top thickness as a global setting
      if (config->cornerRadius.topLeft > 0) {
        pd->graphics->drawRoundRect(
          rect.x, rect.y, rect.w, rect.h,
          config->cornerRadius.topLeft,
          config->width.top,
          clayColorToLCDColor(config->color)
        );
      } else {
        pd->graphics->drawRect(
          rect.x, rect.y, rect.w, rect.h,
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
