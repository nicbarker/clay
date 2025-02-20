#ifndef CLAY_ALLEG4_HEADER
#define CLAY_ALLEG4_HEADER

#include "clay.h"
#include <allegro.h>

#define ALLEGCOLOR(COLOR) makecol(COLOR.r, COLOR.g, COLOR.b)

typedef struct {
  void (*render)(BITMAP *buffer, Clay_BoundingBox box, void *user_data);
  void *user_data;
} alleg4_custom_element;

void alleg4_init_fonts(size_t);
void alleg4_set_font(unsigned int, FONT *);

void alleg4_render(
	BITMAP *buffer,
	Clay_RenderCommandArray renderCommands
);

Clay_Dimensions alleg4_measure_text(
	Clay_StringSlice text,
	Clay_TextElementConfig *config,
	void *userData
);

#endif
