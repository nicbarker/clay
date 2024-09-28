#include <iostream>
#define CLAY_IMPLEMENTATION
#include "../../clay.h"

int main(void) {
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_Arena { .label = CLAY_STRING("Clay Memory Arena"), .capacity = totalMemorySize, .memory = (char *)malloc(totalMemorySize) };
    Clay_Initialize(clayMemory, Clay_Dimensions {1024,768});
    return 0;
}
