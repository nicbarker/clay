#define CLAY_IMPLEMENTATION
#include "alleg4.h"

#define CLIPPED_OPERATION(BUF, X, Y, W, H, CODE) { \
	int _X0, _Y0, _X1, _Y1; \
	get_clip_rect(BUF, &_X0, &_Y0, &_X1, &_Y1); \
	set_clip_rect(BUF, X, Y, X+W-1, Y+H-1); \
	CODE; \
	set_clip_rect(BUF, _X0, _Y0, _X1, _Y1); \
}

typedef struct alleg4_font_store {
  size_t size;
  FONT *fonts[];
} alleg4_font_store;

static alleg4_font_store *font_store = NULL;

void alleg4_init_fonts(size_t size) {
  font_store = malloc(sizeof(alleg4_font_store) + sizeof(FONT*[size]));
  font_store->size = size;
  for (size_t i = 0; i < size; ++i) {
    font_store->fonts[i] = NULL;
  }
}

void alleg4_set_font(unsigned int font_id, FONT *font_object) {
  font_store->fonts[font_id] = font_object;
}

static inline FONT* get_font(unsigned int font_id) {
  if (font_id >= 0 && font_id < font_store->size) {
    return font_store->fonts[font_id];
  } else {
    return font; /* Default built-in font */
  }
}

static inline void arc_thickness(
  BITMAP *dst,
  int x,
  int y,
  fixed from,
  fixed to,
  int r,
  int color,
  int thickness
) {
  do {
    arc(dst, x, y, from, to, r--, color);
  } while (--thickness);
}

static inline void rectfill_wh(
  BITMAP *dst,
  int x,
  int y,
  int w,
  int h,
  int color
) {
  // rectfill uses stard and end coordinates instead of size, so we'd have to -1 all over the place
  if (w == 1) { vline(dst, x, y, y+h-1, color); }
  else if (h == 1) { hline(dst, x, y, x+w-1, color); }
  else { rectfill(dst, x, y, x+w-1, y+h-1, color); }
}

/* Radiuses array contains corner radiuses in clockwise order starting from top-left */
static inline void roundrectfill(
  BITMAP *dst,
  int x,
  int y,
  int w,
  int h,
  int color,
  int r[]
) {
  int top = CLAY__MAX(r[0],r[1]);
  int bottom = CLAY__MAX(r[2],r[3]);
  int left = CLAY__MAX(r[0],r[3]);
  int right = CLAY__MAX(r[1],r[2]);

  if (r[0]) {
    CLIPPED_OPERATION(dst, x, y, r[0], r[0], {
      circlefill(dst, x+r[0], y+r[0], r[0], color);
    });
  }

  if (r[1]) {
    CLIPPED_OPERATION(dst, x+w-r[1], y, r[1], r[1], {
      circlefill(dst, x+w-1-r[1], y+r[1], r[1], color);
    });
  }

  if (r[2]) {
    CLIPPED_OPERATION(dst, x+w-r[2], y+h-r[2], r[2], r[2], {
      circlefill(dst, x+w-1-r[2], y+h-1-r[2], r[2], color);
    });
  }

  if (r[3]) {
    CLIPPED_OPERATION(dst, x, y+h-r[3], r[3], r[3], {
      circlefill(dst, x+r[3], y+h-1-r[3], r[3], color);
    });
  }

  if (top) {
    rectfill_wh(dst, x+r[0], y, w-r[0]-r[1], top, color);
  }

  if (bottom) {
    rectfill_wh(dst, x+r[3], y+h-bottom, w-r[2]-r[3], bottom, color);
  }

  if (left) {
    rectfill_wh(dst, x, y+top, left, h-top-bottom, color);
  }

  if (right) {
    rectfill_wh(dst, x+w-right, y+top, right, h-top-bottom, color);
  }

  rectfill_wh(dst, x+left, y+top, w-left-right, h-top-bottom, color);
}

void alleg4_render(
  BITMAP *buffer,
  Clay_RenderCommandArray renderCommands
) {
  static int crx0, cry0, crx1, cry1;

  for (uint32_t i = 0; i < renderCommands.length; i++) {
    Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
    Clay_BoundingBox box = renderCommand->boundingBox;

    switch (renderCommand->commandType) {
    case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
        Clay_RectangleRenderData *config = &renderCommand->renderData.rectangle;
        Clay_Color color = config->backgroundColor;

        int radiuses[] = {
          config->cornerRadius.topLeft,
          config->cornerRadius.topRight,
          config->cornerRadius.bottomRight,
          config->cornerRadius.bottomLeft
        };

        if (radiuses[0] + radiuses[1] + radiuses[2] + radiuses[3] > 0) {
          roundrectfill(buffer, box.x, box.y, box.width, box.height, ALLEGCOLOR(config->backgroundColor), radiuses);
        } else {
          rectfill_wh(buffer, box.x, box.y, box.width, box.height, ALLEGCOLOR(config->backgroundColor));
        }

        break;
      }

    case CLAY_RENDER_COMMAND_TYPE_TEXT: {
        Clay_TextRenderData *config = &renderCommand->renderData.text;
        char *slice = (char *)calloc(config->stringContents.length + 1, 1);
        memcpy(slice, config->stringContents.chars, config->stringContents.length);
        FONT *font_object = get_font(config->fontId);
        if (is_color_font(font_object)) {
          textout_ex(buffer, get_font(config->fontId), slice, box.x, box.y, -1, -1);
        } else {
          textout_ex(buffer, get_font(config->fontId), slice, box.x, box.y, ALLEGCOLOR(config->textColor), -1);
        }
        free(slice);
        break;
      }

    case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
        Clay_ImageRenderData *config = &renderCommand->renderData.image;
        Clay_Color tintColor = config->backgroundColor;
        BITMAP *image = (BITMAP *)config->imageData;
        if (tintColor.r + tintColor.g + tintColor.b == 0) {
          draw_sprite(buffer, image, box.x, box.y);
        } else {
          set_trans_blender(tintColor.r, tintColor.g, tintColor.b, 0);
          draw_lit_sprite(buffer, image, box.x, box.y, tintColor.a);
        }
        break;
      }

    case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
        get_clip_rect(buffer, &crx0, &cry0, &crx1, &cry1); /* Save current clip rect coordinates */
        set_clip_rect(buffer, box.x, box.y, box.x + box.width, box.y + box.height);
        break;
      }

    case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
        set_clip_rect(buffer, crx0, cry0, crx1, cry1);
        break;
      }

    case CLAY_RENDER_COMMAND_TYPE_BORDER: {
        Clay_BorderRenderData *config = &renderCommand->renderData.border;

        const float tl = config->cornerRadius.topLeft;
        const float tr = config->cornerRadius.topRight;
        const float bl = config->cornerRadius.bottomLeft;
        const float br = config->cornerRadius.bottomRight;

        if (config->width.left > 0) {
          rectfill_wh(buffer, box.x, box.y + tl, config->width.left, box.height-tl-bl, ALLEGCOLOR(config->color));

          /* Top-left half-arc */
          if (tl > 0) {
            CLIPPED_OPERATION(buffer, box.x, box.y+tl/2, tl, tl/2, {
              arc_thickness(buffer, box.x+tl, box.y+tl, itofix(64), itofix(128), tl, ALLEGCOLOR(config->color), config->width.left);
            });
          }

          /* Bottom-left half-arc */
          if (bl > 0) {
            const int y = box.y+box.height-bl;
            CLIPPED_OPERATION(buffer, box.x, y, bl, bl/2, {
              arc_thickness(buffer, box.x+bl, y-1, itofix(128), itofix(192), bl, ALLEGCOLOR(config->color), config->width.left);
            });
          }
        }

        if (config->width.right > 0) {
          rectfill_wh(buffer, box.x+box.width-config->width.right, box.y+tr, config->width.right, box.height-br-tr, ALLEGCOLOR(config->color));

          /* Top-right half-arc */
          if (tr > 0) {
            const int x = box.x+box.width-tr;
            CLIPPED_OPERATION(buffer, x, box.y+tr/2, tr, tr/2, {
              arc_thickness(buffer, x-1, box.y+tr, itofix(0), itofix(64), tr, ALLEGCOLOR(config->color), config->width.right);
            });
          }

          /* Bottom-right half-arc */
          if (br > 0) {
            const int y = box.y+box.height-br;
            const int x = box.x+box.width-br;
            CLIPPED_OPERATION(buffer, x, y, br, br/2, {
              arc_thickness(buffer, x-1, y-1, itofix(192), itofix(256), br, ALLEGCOLOR(config->color), config->width.right);
            });
          }
        }

        if (config->width.top > 0) {
          rectfill_wh(buffer, box.x + tl, box.y, box.width - tr - tl, config->width.top, ALLEGCOLOR(config->color));

          /* Top-left half-arc */
          if (tl > 0) {
            CLIPPED_OPERATION(buffer, box.x, box.y, tl, tl/2, {
              arc_thickness(buffer, box.x+tl, box.y+tl, itofix(64), itofix(128), tl, ALLEGCOLOR(config->color), config->width.top);
            });
          }

          /* Top-right half-arc */
          if (tr > 0) {
            const int x = box.x+box.width-tr;
            CLIPPED_OPERATION(buffer, x, box.y, tr, tr/2, {
              arc_thickness(buffer, x-1, box.y+tr, itofix(0), itofix(64), tr, ALLEGCOLOR(config->color), config->width.top);
            });
          }
        }

        if (config->width.bottom > 0) {
          rectfill_wh(buffer, box.x+bl, box.y+box.height-config->width.bottom, box.width-br-bl, config->width.bottom, ALLEGCOLOR(config->color));

          /* Bottom-left half-arc */
          if (bl > 0) {
            const int y = box.y+box.height-bl;
            CLIPPED_OPERATION(buffer, box.x, y+bl/2, bl, bl/2, {
              arc_thickness(buffer, box.x+bl, y-1, itofix(128), itofix(192), bl, ALLEGCOLOR(config->color), config->width.bottom);
            });
          }

          /* Bottom-right half-arc */
          if (br > 0) {
            const int y = box.y+box.height-br;
            const int x = box.x+box.width-br;
            CLIPPED_OPERATION(buffer, x, y+br/2, br, br/2, {
              arc_thickness(buffer, x-1, y-1, itofix(192), itofix(256), br, ALLEGCOLOR(config->color), config->width.bottom);
            });
          }
        }

        break;
      }

    case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
        Clay_CustomRenderData *config = &renderCommand->renderData.custom;
        alleg4_custom_element *callback_info = (alleg4_custom_element *)config->customData;
        callback_info->render(buffer, box, callback_info->user_data);
        break;
      }
    }
  }
}

Clay_Dimensions alleg4_measure_text(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
  FONT *font_object = get_font(config->fontId);
  char *slice = (char *)calloc(text.length + 1, 1);
  memcpy(slice, text.chars, text.length);
  const Clay_Dimensions d = (Clay_Dimensions) {
    .width = text_length(font_object, slice),
    .height = text_height(font_object)
  };
  free(slice);
  return d;
}
