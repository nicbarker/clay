// Must be defined in one file, _before_ #include "clay.h"
#define CLAY_IMPLEMENTATION

#include <unistd.h>
#include "../../clay.h"
#include "../../renderers/terminal/clay_renderer_terminal.c"

const Clay_Color COLOR_LIGHT = (Clay_Color) {224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color) {168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};

// An example function to begin the "root" of your layout tree
Clay_RenderCommandArray CreateLayout() {
	Clay_BeginLayout();

	CLAY(CLAY_ID("OuterContainer"),
	     CLAY_LAYOUT({.layoutDirection = CLAY_LEFT_TO_RIGHT, .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}, }),
	     CLAY_RECTANGLE({ .color = {0,0,0,255} })) {
		CLAY(CLAY_ID("SideBar"),
		     CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_PERCENT(
			     0.5), .height = CLAY_SIZING_PERCENT(1)}}),
		     CLAY_RECTANGLE({.color = (Clay_Color) {255, 255, 255, 255}})
		) {
		}
		CLAY(CLAY_ID("OtherSideBar"),
		     CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_PERCENT(
			     0.5), .height = CLAY_SIZING_PERCENT(1)}}),
		     CLAY_RECTANGLE({ .color = {0,0, 0, 255 }})
		) {
			// TODO font size is wrong, only one is allowed, but I don't know which it is
			CLAY_TEXT(CLAY_STRING("0123456789 0123456 78901 234567 89012 34567 8901234567890 123456789"),
			          CLAY_TEXT_CONFIG({ .fontId = 0, .fontSize = 24, .textColor = {255,255,255,255} }));
		}
	}

	return Clay_EndLayout();
}

int main() {
	const int width = 80;
	const int height = 24;

	uint64_t totalMemorySize = Clay_MinMemorySize();
	Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
	Clay_Initialize(arena, (Clay_Dimensions) { .width = (float) width, .height = (float) height }); // TODO this is wrong, but I have no idea what the actual size of the terminal is in pixels
	// Tell clay how to measure text
	Clay_SetMeasureTextFunction(Console_MeasureText);

	while(true) {
		Clay_RenderCommandArray layout = CreateLayout();

		Clay_Console_Render(layout, width, height);

		fflush(stdout);
		sleep(1);
	}
}