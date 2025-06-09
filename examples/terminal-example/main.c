// Must be defined in one file, _before_ #include "clay.h"
#define CLAY_IMPLEMENTATION

#include <unistd.h>
#include "../../clay.h"
#include "../../renderers/terminal/clay_renderer_terminal_ansi.c"
#include "../shared-layouts/clay-video-demo.c"

const Clay_Color COLOR_LIGHT = (Clay_Color) {224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color) {168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}

int main() {
    const int width = 145;
    const int height = 41;
    int columnWidth = 16;

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(arena,
                    (Clay_Dimensions) {.width = (float) width * columnWidth, .height = (float) height * columnWidth},
                    (Clay_ErrorHandler) {HandleClayErrors});
    // Tell clay how to measure text
    Clay_SetMeasureTextFunction(Console_MeasureText, &columnWidth);
    ClayVideoDemo_Data demoData = ClayVideoDemo_Initialize();

    while (true) {
        Clay_RenderCommandArray renderCommands = ClayVideoDemo_CreateLayout(&demoData);

        Clay_Terminal_Render(renderCommands, width, height, columnWidth);

        fflush(stdout);
        sleep(1);
    }
}