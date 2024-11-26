#include "../../clay.h"

// NOTE: This file only exists to make sure that clay works when included in multiple translation units.

void SatisfyCompiler() {
    CLAY(CLAY_ID("SatisfyCompiler"), CLAY_LAYOUT({})) {
      CLAY_TEXT(CLAY_STRING("Test"), CLAY_TEXT_CONFIG({ .fontId = 0, .fontSize = 24 }));
    }
}