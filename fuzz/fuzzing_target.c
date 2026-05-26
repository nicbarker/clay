#include "clay.h"
#include <stdint.h>
#include <stddef.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size < sizeof(Clay_String)) return 0;

    Clay_String testString = { .length = size, .chars = (const char *)data };

    Clay_Dimensions dimensions = MeasureText(&testString, NULL);

    // Call other critical functions
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(1024, (void*)data);
    Clay_Initialize(arena, (Clay_Dimensions){1024, 768});
    Clay_SetPointerState((Clay_Vector2){0, 0}, false);
    Clay_BeginLayout();
    Clay_EndLayout();

    // Handle pointer state changes
    Clay_SetPointerState((Clay_Vector2){1, 1}, true);
    Clay_SetPointerState((Clay_Vector2){2, 2}, false);

    return 0;
}
