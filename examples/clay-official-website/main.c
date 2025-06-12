#define CLAY_IMPLEMENTATION
#include "../../clay.h"

double windowWidth = 1024, windowHeight = 768;
float modelPageOneZRotation = 0;
uint32_t ACTIVE_RENDERER_INDEX = 0;

const uint32_t FONT_ID_BODY_16 = 0;
const uint32_t FONT_ID_TITLE_56 = 1;
const uint32_t FONT_ID_BODY_24 = 2;
const uint32_t FONT_ID_BODY_36 = 3;
const uint32_t FONT_ID_TITLE_36 = 4;
const uint32_t FONT_ID_MONOSPACE_24 = 5;

const Clay_Color COLOR_LIGHT = (Clay_Color) {244, 235, 230, 255};
const Clay_Color COLOR_LIGHT_HOVER = (Clay_Color) {224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color) {168, 66, 28, 255};
const Clay_Color COLOR_RED_HOVER = (Clay_Color) {148, 46, 8, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};
const Clay_Color COLOR_BLUE = (Clay_Color) {111, 173, 162, 255};

// Colors for top stripe
const Clay_Color COLOR_TOP_BORDER_1 = (Clay_Color) {168, 66, 28, 255};
const Clay_Color COLOR_TOP_BORDER_2 = (Clay_Color) {223, 110, 44, 255};
const Clay_Color COLOR_TOP_BORDER_3 = (Clay_Color) {225, 138, 50, 255};
const Clay_Color COLOR_TOP_BORDER_4 = (Clay_Color) {236, 189, 80, 255};
const Clay_Color COLOR_TOP_BORDER_5 = (Clay_Color) {240, 213, 137, 255};

const Clay_Color COLOR_BLOB_BORDER_1 = (Clay_Color) {168, 66, 28, 255};
const Clay_Color COLOR_BLOB_BORDER_2 = (Clay_Color) {203, 100, 44, 255};
const Clay_Color COLOR_BLOB_BORDER_3 = (Clay_Color) {225, 138, 50, 255};
const Clay_Color COLOR_BLOB_BORDER_4 = (Clay_Color) {236, 159, 70, 255};
const Clay_Color COLOR_BLOB_BORDER_5 = (Clay_Color) {240, 189, 100, 255};

#define RAYLIB_VECTOR2_TO_CLAY_VECTOR2(vector) (Clay_Vector2) { .x = (vector).x, .y = (vector).y }

Clay_TextElementConfig headerTextConfig = (Clay_TextElementConfig) { .fontId = 2, .fontSize = 24, .textColor = {61, 26, 5, 255} };
Clay_TextElementConfig blobTextConfig = (Clay_TextElementConfig) { .fontId = 2, .fontSize = 30, .textColor = {244, 235, 230, 255} };

typedef struct {
    void* memory;
    uintptr_t offset;
} Arena;

Arena frameArena = {};

typedef struct d {
    Clay_String link;
    bool cursorPointer;
    bool disablePointerEvents;
} CustomHTMLData;

CustomHTMLData* FrameAllocateCustomData(CustomHTMLData data) {
    CustomHTMLData *customData = (CustomHTMLData *)(frameArena.memory + frameArena.offset);
    *customData = data;
    frameArena.offset += sizeof(CustomHTMLData);
    return customData;
}

Clay_String* FrameAllocateString(Clay_String string) {
    Clay_String *allocated = (Clay_String *)(frameArena.memory + frameArena.offset);
    *allocated = string;
    frameArena.offset += sizeof(Clay_String);
    return allocated;
}

void LandingPageBlob(int index, int fontSize, Clay_Color color, Clay_String text, Clay_String imageURL) {
    CLAY({ .id = CLAY_IDI("HeroBlob", index), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 480) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER} }, .border = { .color = color, .width = { 2, 2, 2, 2 }}, .cornerRadius = CLAY_CORNER_RADIUS(10) }) {
        CLAY({ .id = CLAY_IDI("CheckImage", index), .layout = { .sizing = { CLAY_SIZING_FIXED(32) } }, .aspectRatio = { 1 }, .image = { .imageData = FrameAllocateString(imageURL) } }) {}
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({ .fontSize = fontSize, .fontId = FONT_ID_BODY_24, .textColor = color }));
    }
}

void LandingPageDesktop() {
    CLAY({ .id = CLAY_ID("LandingPage1Desktop"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIT(.min = windowHeight - 70) }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}, .padding = { 50, 50 } } }) {
        CLAY({ .id = CLAY_ID("LandingPage1"), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}, .padding = CLAY_PADDING_ALL(32), .childGap = 32 }, .border = { .width = { .left = 2, .right = 2 }, .color = COLOR_RED } }) {
            CLAY({ .id = CLAY_ID("LeftText"), .layout = { .sizing = { .width = CLAY_SIZING_PERCENT(0.55f) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 8 } }) {
                CLAY_TEXT(CLAY_STRING("Clay is a flex-box style UI auto layout library in C, with declarative syntax and microsecond performance."), CLAY_TEXT_CONFIG({ .fontSize = 56, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_RED }));
                CLAY({ .id = CLAY_ID("LandingPageSpacer"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(32) } } }) {}
                CLAY_TEXT(CLAY_STRING("Clay is laying out this webpage right now!"), CLAY_TEXT_CONFIG({ .fontSize = 36, .fontId = FONT_ID_TITLE_36, .textColor = COLOR_ORANGE }));
            }
            CLAY({ .id = CLAY_ID("HeroImageOuter"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_PERCENT(0.45f) }, .childAlignment = { CLAY_ALIGN_X_CENTER }, .childGap = 16 } }) {
                LandingPageBlob(1, 32, COLOR_BLOB_BORDER_5, CLAY_STRING("High performance"), CLAY_STRING("/clay/images/check_5.png"));
                LandingPageBlob(2, 32, COLOR_BLOB_BORDER_4, CLAY_STRING("Flexbox-style responsive layout"), CLAY_STRING("/clay/images/check_4.png"));
                LandingPageBlob(3, 32, COLOR_BLOB_BORDER_3, CLAY_STRING("Declarative syntax"), CLAY_STRING("/clay/images/check_3.png"));
                LandingPageBlob(4, 32, COLOR_BLOB_BORDER_2, CLAY_STRING("Single .h file for C/C++"), CLAY_STRING("/clay/images/check_2.png"));
                LandingPageBlob(5, 32, COLOR_BLOB_BORDER_1, CLAY_STRING("Compile to 15kb .wasm"), CLAY_STRING("/clay/images/check_1.png"));
            }
        }
    }
}

void LandingPageMobile() {
    CLAY({ .id = CLAY_ID("LandingPage1Mobile"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIT(.min = windowHeight - 70) }, .childAlignment = {CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}, .padding = { 16, 16, 32, 32 }, .childGap = 32 } }) {
        CLAY({ .id = CLAY_ID("LeftText"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 8 } }) {
            CLAY_TEXT(CLAY_STRING("Clay is a flex-box style UI auto layout library in C, with declarative syntax and microsecond performance."), CLAY_TEXT_CONFIG({ .fontSize = 48, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_RED }));
            CLAY({ .id = CLAY_ID("LandingPageSpacer"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(32) } } }) {}
            CLAY_TEXT(CLAY_STRING("Clay is laying out this webpage right now!"), CLAY_TEXT_CONFIG({ .fontSize = 32, .fontId = FONT_ID_TITLE_36, .textColor = COLOR_ORANGE }));
        }
        CLAY({ .id = CLAY_ID("HeroImageOuter"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_GROW(0) }, .childAlignment = { CLAY_ALIGN_X_CENTER }, .childGap = 16 } }) {
            LandingPageBlob(1, 28, COLOR_BLOB_BORDER_5, CLAY_STRING("High performance"), CLAY_STRING("/clay/images/check_5.png"));
            LandingPageBlob(2, 28, COLOR_BLOB_BORDER_4, CLAY_STRING("Flexbox-style responsive layout"), CLAY_STRING("/clay/images/check_4.png"));
            LandingPageBlob(3, 28, COLOR_BLOB_BORDER_3, CLAY_STRING("Declarative syntax"), CLAY_STRING("/clay/images/check_3.png"));
            LandingPageBlob(4, 28, COLOR_BLOB_BORDER_2, CLAY_STRING("Single .h file for C/C++"), CLAY_STRING("/clay/images/check_2.png"));
            LandingPageBlob(5, 28, COLOR_BLOB_BORDER_1, CLAY_STRING("Compile to 15kb .wasm"), CLAY_STRING("/clay/images/check_1.png"));
        }
    }
}

void FeatureBlocksDesktop() {
    CLAY({ .id = CLAY_ID("FeatureBlocksOuter"), .layout = { .sizing = { CLAY_SIZING_GROW(0) } } }) {
        CLAY({ .id = CLAY_ID("FeatureBlocksInner"), .layout = { .sizing = { CLAY_SIZING_GROW(0) }, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } }, .border = { .width = { .betweenChildren = 2 }, .color = COLOR_RED } }) {
            Clay_TextElementConfig *textConfig = CLAY_TEXT_CONFIG({ .fontSize = 24, .fontId = FONT_ID_BODY_24, .textColor = COLOR_RED });
            CLAY({ .id = CLAY_ID("HFileBoxOuter"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { CLAY_SIZING_PERCENT(0.5f) }, .childAlignment = {0, CLAY_ALIGN_Y_CENTER}, .padding = {50, 50, 32, 32}, .childGap = 8 } }) {
                CLAY({ .id = CLAY_ID("HFileIncludeOuter"), .layout = { .padding = {8, 4} }, .backgroundColor = COLOR_RED, .cornerRadius = CLAY_CORNER_RADIUS(8) }) {
                    CLAY_TEXT(CLAY_STRING("#include clay.h"), CLAY_TEXT_CONFIG({ .fontSize = 24, .fontId = FONT_ID_BODY_24, .textColor = COLOR_LIGHT }));
                }
                CLAY_TEXT(CLAY_STRING("~2000 lines of C99."), textConfig);
                CLAY_TEXT(CLAY_STRING("Zero dependencies, including no C standard library."), textConfig);
            }
            CLAY({ .id = CLAY_ID("BringYourOwnRendererOuter"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { CLAY_SIZING_PERCENT(0.5f) }, .childAlignment = {0, CLAY_ALIGN_Y_CENTER}, .padding = {50, 50, 32, 32}, .childGap = 8 } }) {
                CLAY_TEXT(CLAY_STRING("Renderer agnostic."), CLAY_TEXT_CONFIG({ .fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = COLOR_ORANGE }));
                CLAY_TEXT(CLAY_STRING("Layout with clay, then render with Raylib, WebGL Canvas or even as HTML."), textConfig);
                CLAY_TEXT(CLAY_STRING("Flexible output for easy compositing in your custom engine or environment."), textConfig);
            }
        }
    }
}

void FeatureBlocksMobile() {
    CLAY({ .id = CLAY_ID("FeatureBlocksInner"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { CLAY_SIZING_GROW(0) } }, .border = { .width = { .betweenChildren = 2 }, .color = COLOR_RED } }) {
        Clay_TextElementConfig *textConfig = CLAY_TEXT_CONFIG({ .fontSize = 24, .fontId = FONT_ID_BODY_24, .textColor = COLOR_RED });
        CLAY({ .id = CLAY_ID("HFileBoxOuter"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { CLAY_SIZING_GROW(0) }, .childAlignment = {0, CLAY_ALIGN_Y_CENTER}, .padding = {16, 16, 32, 32}, .childGap = 8 } }) {
            CLAY({ .id = CLAY_ID("HFileIncludeOuter"), .layout = { .padding = {8, 4} }, .backgroundColor = COLOR_RED, .cornerRadius = CLAY_CORNER_RADIUS(8) }) {
                CLAY_TEXT(CLAY_STRING("#include clay.h"), CLAY_TEXT_CONFIG({ .fontSize = 24, .fontId = FONT_ID_BODY_24, .textColor = COLOR_LIGHT }));
            }
            CLAY_TEXT(CLAY_STRING("~2000 lines of C99."), textConfig);
            CLAY_TEXT(CLAY_STRING("Zero dependencies, including no C standard library."), textConfig);
        }
        CLAY({ .id = CLAY_ID("BringYourOwnRendererOuter"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { CLAY_SIZING_GROW(0) }, .childAlignment = {0, CLAY_ALIGN_Y_CENTER}, .padding = {16, 16, 32, 32}, .childGap = 8 } }) {
            CLAY_TEXT(CLAY_STRING("Renderer agnostic."), CLAY_TEXT_CONFIG({ .fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = COLOR_ORANGE }));
            CLAY_TEXT(CLAY_STRING("Layout with clay, then render with Raylib, WebGL Canvas or even as HTML."), textConfig);
            CLAY_TEXT(CLAY_STRING("Flexible output for easy compositing in your custom engine or environment."), textConfig);
        }
    }
}

void DeclarativeSyntaxPageDesktop() {
    CLAY({ .id = CLAY_ID("SyntaxPageDesktop"), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(.min = windowHeight - 50) }, .childAlignment = {0, CLAY_ALIGN_Y_CENTER}, .padding = { 50, 50 } } }) {
        CLAY({ .id = CLAY_ID("SyntaxPage"), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) }, .childAlignment = { 0, CLAY_ALIGN_Y_CENTER }, .padding = CLAY_PADDING_ALL(32), .childGap = 32 }, .border = { .width = { .left = 2, .right = 2 }, .color = COLOR_RED }}) {
            CLAY({ .id = CLAY_ID("SyntaxPageLeftText"), .layout = { .sizing = { CLAY_SIZING_PERCENT(0.5) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 8 } }) {
                CLAY_TEXT(CLAY_STRING("Declarative Syntax"), CLAY_TEXT_CONFIG({ .fontSize = 52, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_RED }));
                CLAY({ .id = CLAY_ID("SyntaxSpacer"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 16) } } }) {}
                CLAY_TEXT(CLAY_STRING("Flexible and readable declarative syntax with nested UI element hierarchies."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
                CLAY_TEXT(CLAY_STRING("Mix elements with standard C code like loops, conditionals and functions."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
                CLAY_TEXT(CLAY_STRING("Create your own library of re-usable components from UI primitives like text, images and rectangles."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
            }
            CLAY({ .id = CLAY_ID("SyntaxPageRightImage"), .layout = { .sizing = { CLAY_SIZING_PERCENT(0.50) }, .childAlignment = {.x = CLAY_ALIGN_X_CENTER} } }) {
                CLAY({ .id = CLAY_ID("SyntaxPageRightImageInner"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 568) } }, .aspectRatio = { 1136.0 / 1194.0 }, .image = { .imageData = FrameAllocateString(CLAY_STRING("/clay/images/declarative.png")) } }) {}
            }
        }
    }
}

void DeclarativeSyntaxPageMobile() {
    CLAY({ .id = CLAY_ID("SyntaxPageDesktop"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(.min = windowHeight - 50) }, .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}, .padding = {16, 16, 32, 32}, .childGap = 16 } }) {
        CLAY({ .id = CLAY_ID("SyntaxPageLeftText"), .layout = { .sizing = { CLAY_SIZING_GROW(0) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 8 } }) {
            CLAY_TEXT(CLAY_STRING("Declarative Syntax"), CLAY_TEXT_CONFIG({ .fontSize = 48, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_RED }));
            CLAY({ .id = CLAY_ID("SyntaxSpacer"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 16) } } }) {}
            CLAY_TEXT(CLAY_STRING("Flexible and readable declarative syntax with nested UI element hierarchies."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
            CLAY_TEXT(CLAY_STRING("Mix elements with standard C code like loops, conditionals and functions."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
            CLAY_TEXT(CLAY_STRING("Create your own library of re-usable components from UI primitives like text, images and rectangles."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
        }
        CLAY({ .id = CLAY_ID("SyntaxPageRightImage"), .layout = { .sizing = { CLAY_SIZING_GROW(0) }, .childAlignment = {.x = CLAY_ALIGN_X_CENTER} } }) {
            CLAY({ .id = CLAY_ID("SyntaxPageRightImageInner"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 568) } }, .aspectRatio = { 1136.0 / 1194.0 }, .image = { .imageData = FrameAllocateString(CLAY_STRING("/clay/images/declarative.png")) } }) {}
        }
    }
}

Clay_Color ColorLerp(Clay_Color a, Clay_Color b, float amount) {
    return (Clay_Color) {
        .r = a.r + (b.r - a.r) * amount,
        .g = a.g + (b.g - a.g) * amount,
        .b = a.b + (b.b - a.b) * amount,
        .a = a.a + (b.a - a.a) * amount,
    };
}

Clay_String LOREM_IPSUM_TEXT = CLAY_STRING("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");

void HighPerformancePageDesktop(float lerpValue) {
    CLAY({ .id = CLAY_ID("PerformanceOuter"), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(.min = windowHeight - 50) }, .childAlignment = {0, CLAY_ALIGN_Y_CENTER}, .padding = {82, 82, 32, 32}, .childGap = 64 }, .backgroundColor = COLOR_RED }) {
        CLAY({ .id = CLAY_ID("PerformanceLeftText"), .layout = { .sizing = { CLAY_SIZING_PERCENT(0.5) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 8 } }) {
            CLAY_TEXT(CLAY_STRING("High Performance"), CLAY_TEXT_CONFIG({ .fontSize = 52, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_LIGHT }));
            CLAY({ .id = CLAY_ID("PerformanceSpacer"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 16) }} }) {}
            CLAY_TEXT(CLAY_STRING("Fast enough to recompute your entire UI every frame."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_LIGHT }));
            CLAY_TEXT(CLAY_STRING("Small memory footprint (3.5mb default) with static allocation & reuse. No malloc / free."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_LIGHT }));
            CLAY_TEXT(CLAY_STRING("Simplify animations and reactive UI design by avoiding the standard performance hacks."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_LIGHT }));
        }
        CLAY({ .id = CLAY_ID("PerformanceRightImageOuter"), .layout = { .sizing = { CLAY_SIZING_PERCENT(0.50) }, .childAlignment = {CLAY_ALIGN_X_CENTER} } }) {
            CLAY({ .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(400) } }, .border = { .width = {2, 2, 2, 2}, .color = COLOR_LIGHT } }) {
                CLAY({ .id = CLAY_ID("AnimationDemoContainerLeft"), .layout = { .sizing = { CLAY_SIZING_PERCENT(0.3f + 0.4f * lerpValue), CLAY_SIZING_GROW(0) }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}, .padding = CLAY_PADDING_ALL(32) }, .backgroundColor = ColorLerp(COLOR_RED, COLOR_ORANGE, lerpValue) }) {
                    CLAY_TEXT(LOREM_IPSUM_TEXT, CLAY_TEXT_CONFIG({ .fontSize = 24, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_LIGHT }));
                }
                CLAY({ .id = CLAY_ID("AnimationDemoContainerRight"), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER},  .padding = CLAY_PADDING_ALL(32) }, .backgroundColor = ColorLerp(COLOR_ORANGE, COLOR_RED, lerpValue) }) {
                    CLAY_TEXT(LOREM_IPSUM_TEXT, CLAY_TEXT_CONFIG({ .fontSize = 24, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_LIGHT }));
                }
            }
        }
    }
}

void HighPerformancePageMobile(float lerpValue) {
    CLAY({ .id = CLAY_ID("PerformanceOuter"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(.min = windowHeight - 50) }, .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}, .padding = {16, 16, 32, 32}, .childGap = 32 }, .backgroundColor = COLOR_RED }) {
        CLAY({ .id = CLAY_ID("PerformanceLeftText"), .layout = { .sizing = { CLAY_SIZING_GROW(0) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 8 } }) {
            CLAY_TEXT(CLAY_STRING("High Performance"), CLAY_TEXT_CONFIG({ .fontSize = 48, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_LIGHT }));
            CLAY({ .id = CLAY_ID("PerformanceSpacer"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 16) }} }) {}
            CLAY_TEXT(CLAY_STRING("Fast enough to recompute your entire UI every frame."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_LIGHT }));
            CLAY_TEXT(CLAY_STRING("Small memory footprint (3.5mb default) with static allocation & reuse. No malloc / free."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_LIGHT }));
            CLAY_TEXT(CLAY_STRING("Simplify animations and reactive UI design by avoiding the standard performance hacks."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_LIGHT }));
        }
        CLAY({ .id = CLAY_ID("PerformanceRightImageOuter"), .layout = { .sizing = { CLAY_SIZING_GROW(0) }, .childAlignment = {CLAY_ALIGN_X_CENTER} } }) {
            CLAY({ .id = CLAY_ID(""), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(400) } }, .border = { .width = { 2, 2, 2, 2 }, .color = COLOR_LIGHT }}) {
                CLAY({ .id = CLAY_ID("AnimationDemoContainerLeft"), .layout = { .sizing = { CLAY_SIZING_PERCENT(0.35f + 0.3f * lerpValue), CLAY_SIZING_GROW(0) }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}, .padding = CLAY_PADDING_ALL(16) }, .backgroundColor = ColorLerp(COLOR_RED, COLOR_ORANGE, lerpValue) }) {
                    CLAY_TEXT(LOREM_IPSUM_TEXT, CLAY_TEXT_CONFIG({ .fontSize = 24, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_LIGHT }));
                }
                CLAY({ .id = CLAY_ID("AnimationDemoContainerRight"), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) }, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}, .padding = CLAY_PADDING_ALL(16) }, .backgroundColor = ColorLerp(COLOR_ORANGE, COLOR_RED, lerpValue) }) {
                    CLAY_TEXT(LOREM_IPSUM_TEXT, CLAY_TEXT_CONFIG({ .fontSize = 24, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_LIGHT }));
                }
            }
        }
    }
}

void HandleRendererButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerInfo, intptr_t userData) {
    if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        ACTIVE_RENDERER_INDEX = (uint32_t)userData;
        Clay_SetCullingEnabled(ACTIVE_RENDERER_INDEX == 1);
        Clay_SetExternalScrollHandlingEnabled(ACTIVE_RENDERER_INDEX == 0);
    }
}

void RendererButtonActive(Clay_String text) {
    CLAY({
        .layout = { .sizing = {CLAY_SIZING_FIXED(300) }, .padding = CLAY_PADDING_ALL(16) },
        .backgroundColor = Clay_Hovered() ? COLOR_RED_HOVER : COLOR_RED,
        .cornerRadius = CLAY_CORNER_RADIUS(10),
        .userData = FrameAllocateCustomData((CustomHTMLData) { .disablePointerEvents = true, .cursorPointer = true })
    }) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_LIGHT }));
    }
}

void RendererButtonInactive(Clay_String text, size_t rendererIndex) {
    CLAY({
        .layout = { .sizing = {CLAY_SIZING_FIXED(300)}, .padding = CLAY_PADDING_ALL(16) },
        .border = { .width = {2, 2, 2, 2}, .color = COLOR_RED },
        .backgroundColor = Clay_Hovered() ? COLOR_LIGHT_HOVER : COLOR_LIGHT,
        .cornerRadius = CLAY_CORNER_RADIUS(10),
        .userData = FrameAllocateCustomData((CustomHTMLData) { .disablePointerEvents = true, .cursorPointer = true })
    }) {
        Clay_OnHover(HandleRendererButtonInteraction, rendererIndex);
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
    }
}

void RendererPageDesktop() {
    CLAY({ .id = CLAY_ID("RendererPageDesktop"), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(.min = windowHeight - 50) }, .childAlignment = {0, CLAY_ALIGN_Y_CENTER}, .padding = { 50, 50 } } }) {
        CLAY({ .id = CLAY_ID("RendererPage"), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) }, .childAlignment = { 0, CLAY_ALIGN_Y_CENTER }, .padding = CLAY_PADDING_ALL(32), .childGap = 32 }, .border = { .width = { .left = 2, .right = 2 }, .color = COLOR_RED } }) {
            CLAY({ .id = CLAY_ID("RendererLeftText"), .layout = { .sizing = { CLAY_SIZING_PERCENT(0.5) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 8 } }) {
                CLAY_TEXT(CLAY_STRING("Renderer & Platform Agnostic"), CLAY_TEXT_CONFIG({ .fontSize = 52, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_RED }));
                CLAY({ .id = CLAY_ID("RendererSpacerLeft"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 16) }} }) {}
                CLAY_TEXT(CLAY_STRING("Clay outputs a sorted array of primitive render commands, such as RECTANGLE, TEXT or IMAGE."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
                CLAY_TEXT(CLAY_STRING("Write your own renderer in a few hundred lines of code, or use the provided examples for Raylib, WebGL canvas and more."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
                CLAY_TEXT(CLAY_STRING("There's even an HTML renderer - you're looking at it right now!"), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
            }
            CLAY({ .id = CLAY_ID("RendererRightText"), .layout = { .sizing = { CLAY_SIZING_PERCENT(0.5) }, .childAlignment = {CLAY_ALIGN_X_CENTER}, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 16 } }) {
                CLAY_TEXT(CLAY_STRING("Try changing renderer!"), CLAY_TEXT_CONFIG({ .fontSize = 36, .fontId = FONT_ID_BODY_36, .textColor = COLOR_ORANGE }));
                CLAY({ .id = CLAY_ID("RendererSpacerRight"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 32) } } }) {}
                if (ACTIVE_RENDERER_INDEX == 0) {
                    RendererButtonActive(CLAY_STRING("HTML Renderer"));
                    RendererButtonInactive(CLAY_STRING("Canvas Renderer"), 1);
                } else {
                    RendererButtonInactive(CLAY_STRING("HTML Renderer"), 0);
                    RendererButtonActive(CLAY_STRING("Canvas Renderer"));
                }
            }
        }
    }
}

void RendererPageMobile() {
    CLAY({ .id = CLAY_ID("RendererMobile"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(.min = windowHeight - 50) }, .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}, .padding = { 16, 16, 32, 32}, .childGap = 32 }, .backgroundColor = COLOR_LIGHT }) {
        CLAY({ .id = CLAY_ID("RendererLeftText"), .layout = { .sizing = { CLAY_SIZING_GROW(0) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 8 } }) {
            CLAY_TEXT(CLAY_STRING("Renderer & Platform Agnostic"), CLAY_TEXT_CONFIG({ .fontSize = 48, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_RED }));
            CLAY({ .id = CLAY_ID("RendererSpacerLeft"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 16) }} }) {}
            CLAY_TEXT(CLAY_STRING("Clay outputs a sorted array of primitive render commands, such as RECTANGLE, TEXT or IMAGE."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
            CLAY_TEXT(CLAY_STRING("Write your own renderer in a few hundred lines of code, or use the provided examples for Raylib, WebGL canvas and more."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
            CLAY_TEXT(CLAY_STRING("There's even an HTML renderer - you're looking at it right now!"), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_RED }));
        }
        CLAY({ .id = CLAY_ID("RendererRightText"), .layout = { .sizing = { CLAY_SIZING_GROW(0) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 16 } }) {
            CLAY_TEXT(CLAY_STRING("Try changing renderer!"), CLAY_TEXT_CONFIG({ .fontSize = 36, .fontId = FONT_ID_BODY_36, .textColor = COLOR_ORANGE }));
            CLAY({ .id = CLAY_ID("RendererSpacerRight"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 32) }} }) {}
            if (ACTIVE_RENDERER_INDEX == 0) {
                RendererButtonActive(CLAY_STRING("HTML Renderer"));
                RendererButtonInactive(CLAY_STRING("Canvas Renderer"), 1);
            } else {
                RendererButtonInactive(CLAY_STRING("HTML Renderer"), 0);
                RendererButtonActive(CLAY_STRING("Canvas Renderer"));
            }
        }
    }
}

void DebuggerPageDesktop() {
    CLAY({ .id = CLAY_ID("DebuggerDesktop"), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT(.min = windowHeight - 50) }, .childAlignment = {0, CLAY_ALIGN_Y_CENTER}, .padding = { 82, 82, 32, 32 }, .childGap = 64 }, .backgroundColor = COLOR_RED }) {
        CLAY({ .id = CLAY_ID("DebuggerLeftText"), .layout = { .sizing = { CLAY_SIZING_PERCENT(0.5) }, .layoutDirection = CLAY_TOP_TO_BOTTOM, .childGap = 8 } }) {
            CLAY_TEXT(CLAY_STRING("Integrated Debug Tools"), CLAY_TEXT_CONFIG({ .fontSize = 52, .fontId = FONT_ID_TITLE_56, .textColor = COLOR_LIGHT }));
            CLAY({ .id = CLAY_ID("DebuggerSpacer"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 16) }} }) {}
            CLAY_TEXT(CLAY_STRING("Clay includes built in \"Chrome Inspector\"-style debug tooling."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_LIGHT }));
            CLAY_TEXT(CLAY_STRING("View your layout hierarchy and config in real time."), CLAY_TEXT_CONFIG({ .fontSize = 28, .fontId = FONT_ID_BODY_36, .textColor = COLOR_LIGHT }));
            CLAY({ .id = CLAY_ID("DebuggerPageSpacer"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(32) } } }) {}
            CLAY_TEXT(CLAY_STRING("Press the \"d\" key to try it out now!"), CLAY_TEXT_CONFIG({ .fontSize = 32, .fontId = FONT_ID_TITLE_36, .textColor = COLOR_ORANGE }));
        }
        CLAY({ .id = CLAY_ID("DebuggerRightImageOuter"), .layout = { .sizing = { CLAY_SIZING_PERCENT(0.50) }, .childAlignment = {CLAY_ALIGN_X_CENTER} } }) {
            CLAY({ .id = CLAY_ID("DebuggerPageRightImageInner"), .layout = { .sizing = { CLAY_SIZING_GROW(.max = 558) } }, .aspectRatio = { 1620.0 / 1474.0 }, .image = {.imageData = FrameAllocateString(CLAY_STRING("/clay/images/debugger.png")) } }) {}
        }
    }
}

typedef struct
{
    Clay_Vector2 clickOrigin;
    Clay_Vector2 positionOrigin;
    bool mouseDown;
} ScrollbarData;

ScrollbarData scrollbarData = (ScrollbarData) {};
float animationLerpValue = -1.0f;

Clay_RenderCommandArray CreateLayout(bool mobileScreen, float lerpValue) {
    Clay_BeginLayout();
    CLAY({ .id = CLAY_ID("OuterContainer"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) } }, .backgroundColor = COLOR_LIGHT }) {
        CLAY({ .id = CLAY_ID("Header"), .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(50) }, .childAlignment = { 0, CLAY_ALIGN_Y_CENTER }, .childGap = 16, .padding = { 32, 32 } } }) {
            CLAY_TEXT(CLAY_STRING("Clay"), &headerTextConfig);
            CLAY({ .id = CLAY_ID("Spacer"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) } } }) {}
            if (!mobileScreen) {
                CLAY({ .id = CLAY_ID("LinkExamplesOuter"), .layout = { .padding = {8, 8} } }) {
                    CLAY_TEXT(CLAY_STRING("Examples"), CLAY_TEXT_CONFIG({
                        .userData = FrameAllocateCustomData((CustomHTMLData) {
                            .link = CLAY_STRING("https://github.com/nicbarker/clay/tree/main/examples")
                        }),
                        .fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = {61, 26, 5, 255} }));
                }
                CLAY({ .id = CLAY_ID("LinkDocsOuter"), .layout = { .padding = {8, 8} } }) {
                    CLAY_TEXT(CLAY_STRING("Docs"), CLAY_TEXT_CONFIG({
                        .userData = FrameAllocateCustomData((CustomHTMLData) { .link = CLAY_STRING("https://github.com/nicbarker/clay/blob/main/README.md") }),
                        .fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = {61, 26, 5, 255} })
                    );
                }
            }
            CLAY({
                .layout = { .padding = {16, 16, 6, 6} },
                .backgroundColor = Clay_Hovered() ? COLOR_LIGHT_HOVER : COLOR_LIGHT,
                .border = { .width = {2, 2, 2, 2}, .color = COLOR_RED },
                .cornerRadius = CLAY_CORNER_RADIUS(10),
                .userData = FrameAllocateCustomData((CustomHTMLData) { .link = CLAY_STRING("https://discord.gg/b4FTWkxdvT") }),
            }) {
                CLAY_TEXT(CLAY_STRING("Discord"), CLAY_TEXT_CONFIG({
                    .userData = FrameAllocateCustomData((CustomHTMLData) { .disablePointerEvents = true }),
                    .fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = {61, 26, 5, 255} }));
            }
            CLAY({
                .layout = { .padding = {16, 16, 6, 6} },
                .backgroundColor = Clay_Hovered() ? COLOR_LIGHT_HOVER : COLOR_LIGHT,
                .border = { .width = {2, 2, 2, 2}, .color = COLOR_RED },
                .cornerRadius = CLAY_CORNER_RADIUS(10),
                .userData = FrameAllocateCustomData((CustomHTMLData) { .link = CLAY_STRING("https://github.com/nicbarker/clay") }),
            }) {
                CLAY_TEXT(CLAY_STRING("Github"), CLAY_TEXT_CONFIG({
                    .userData = FrameAllocateCustomData((CustomHTMLData) { .disablePointerEvents = true }),
                    .fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = {61, 26, 5, 255} }));
            }
        }
        Clay_LayoutConfig topBorderConfig = (Clay_LayoutConfig) { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(4) }};
        CLAY({ .id = CLAY_ID("TopBorder1"), .layout = topBorderConfig, .backgroundColor = COLOR_TOP_BORDER_5 }) {}
        CLAY({ .id = CLAY_ID("TopBorder2"), .layout = topBorderConfig, .backgroundColor = COLOR_TOP_BORDER_4 }) {}
        CLAY({ .id = CLAY_ID("TopBorder3"), .layout = topBorderConfig, .backgroundColor = COLOR_TOP_BORDER_3 }) {}
        CLAY({ .id = CLAY_ID("TopBorder4"), .layout = topBorderConfig, .backgroundColor = COLOR_TOP_BORDER_2 }) {}
        CLAY({ .id = CLAY_ID("TopBorder5"), .layout = topBorderConfig, .backgroundColor = COLOR_TOP_BORDER_1 }) {}
        CLAY({ .id = CLAY_ID("OuterScrollContainer"),
            .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0) }, .layoutDirection = CLAY_TOP_TO_BOTTOM },
            .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() },
            .border = { .width = { .betweenChildren = 2 }, .color = COLOR_RED }
        }) {
            if (mobileScreen) {
                LandingPageMobile();
                FeatureBlocksMobile();
                DeclarativeSyntaxPageMobile();
                HighPerformancePageMobile(lerpValue);
                RendererPageMobile();
            } else {
                LandingPageDesktop();
                FeatureBlocksDesktop();
                DeclarativeSyntaxPageDesktop();
                HighPerformancePageDesktop(lerpValue);
                RendererPageDesktop();
                DebuggerPageDesktop();
            }
        }
    }

    if (!mobileScreen && ACTIVE_RENDERER_INDEX == 1) {
        Clay_ScrollContainerData scrollData = Clay_GetScrollContainerData(Clay_GetElementId(CLAY_STRING("OuterScrollContainer")));
        Clay_Color scrollbarColor = (Clay_Color){225, 138, 50, 120};
        if (scrollbarData.mouseDown) {
            scrollbarColor = (Clay_Color){225, 138, 50, 200};
        } else if (Clay_PointerOver(Clay_GetElementId(CLAY_STRING("ScrollBar")))) {
            scrollbarColor = (Clay_Color){225, 138, 50, 160};
        }
        float scrollHeight = scrollData.scrollContainerDimensions.height - 12;
        CLAY({
            .id = CLAY_ID("ScrollBar"),
            .floating = { .offset = { .x = -6, .y = -(scrollData.scrollPosition->y / scrollData.contentDimensions.height) * scrollHeight + 6}, .zIndex = 1, .parentId = Clay_GetElementId(CLAY_STRING("OuterScrollContainer")).id, .attachPoints = {.element = CLAY_ATTACH_POINT_RIGHT_TOP, .parent = CLAY_ATTACH_POINT_RIGHT_TOP }, .attachTo = CLAY_ATTACH_TO_PARENT },
            .layout = { .sizing = {CLAY_SIZING_FIXED(10), CLAY_SIZING_FIXED((scrollHeight / scrollData.contentDimensions.height) * scrollHeight)} },
            .backgroundColor = scrollbarColor,
            .cornerRadius = CLAY_CORNER_RADIUS(5)
        }) {}
    }
    return Clay_EndLayout();
}

bool debugModeEnabled = false;

CLAY_WASM_EXPORT("SetScratchMemory") void SetScratchMemory(void * memory) {
    frameArena.memory = memory;
}

CLAY_WASM_EXPORT("UpdateDrawFrame") Clay_RenderCommandArray UpdateDrawFrame(float width, float height, float mouseWheelX, float mouseWheelY, float mousePositionX, float mousePositionY, bool isTouchDown, bool isMouseDown, bool arrowKeyDownPressedThisFrame, bool arrowKeyUpPressedThisFrame, bool dKeyPressedThisFrame, float deltaTime) {
    frameArena.offset = 0;
    windowWidth = width;
    windowHeight = height;
    Clay_SetLayoutDimensions((Clay_Dimensions) { width, height });
    Clay_ScrollContainerData scrollContainerData = Clay_GetScrollContainerData(Clay_GetElementId(CLAY_STRING("OuterScrollContainer")));
    Clay_LayoutElementHashMapItem *perfPage = Clay__GetHashMapItem(Clay_GetElementId(CLAY_STRING("PerformanceOuter")).id);
    // NaN propagation can cause pain here
    float perfPageYOffset = perfPage->boundingBox.y + scrollContainerData.scrollPosition->y;
    if (deltaTime == deltaTime && (ACTIVE_RENDERER_INDEX == 1 || (perfPageYOffset < height && perfPageYOffset + perfPage->boundingBox.height > 0))) {
        animationLerpValue += deltaTime;
        if (animationLerpValue > 1) {
            animationLerpValue -= 2;
        }
    }

    if (dKeyPressedThisFrame) {
        debugModeEnabled = !debugModeEnabled;
        Clay_SetDebugModeEnabled(debugModeEnabled);
    }
    Clay_SetCullingEnabled(ACTIVE_RENDERER_INDEX == 1);
    Clay_SetExternalScrollHandlingEnabled(ACTIVE_RENDERER_INDEX == 0);

    Clay__debugViewHighlightColor = (Clay_Color) {105,210,231, 120};

    Clay_SetPointerState((Clay_Vector2) {mousePositionX, mousePositionY}, isMouseDown || isTouchDown);

    if (!isMouseDown) {
        scrollbarData.mouseDown = false;
    }

    if (isMouseDown && !scrollbarData.mouseDown && Clay_PointerOver(Clay_GetElementId(CLAY_STRING("ScrollBar")))) {
        scrollbarData.clickOrigin = (Clay_Vector2) { mousePositionX, mousePositionY };
        scrollbarData.positionOrigin = *scrollContainerData.scrollPosition;
        scrollbarData.mouseDown = true;
    } else if (scrollbarData.mouseDown) {
        if (scrollContainerData.contentDimensions.height > 0) {
            Clay_Vector2 ratio = (Clay_Vector2) {
                scrollContainerData.contentDimensions.width / scrollContainerData.scrollContainerDimensions.width,
                scrollContainerData.contentDimensions.height / scrollContainerData.scrollContainerDimensions.height,
            };
            if (scrollContainerData.config.vertical) {
                scrollContainerData.scrollPosition->y = scrollbarData.positionOrigin.y + (scrollbarData.clickOrigin.y - mousePositionY) * ratio.y;
            }
            if (scrollContainerData.config.horizontal) {
                scrollContainerData.scrollPosition->x = scrollbarData.positionOrigin.x + (scrollbarData.clickOrigin.x - mousePositionX) * ratio.x;
            }
        }
    }

    if (arrowKeyDownPressedThisFrame) {
        if (scrollContainerData.contentDimensions.height > 0) {
            scrollContainerData.scrollPosition->y = scrollContainerData.scrollPosition->y - 50;
        }
    } else if (arrowKeyUpPressedThisFrame) {
        if (scrollContainerData.contentDimensions.height > 0) {
            scrollContainerData.scrollPosition->y = scrollContainerData.scrollPosition->y + 50;
        }
    }

    Clay_UpdateScrollContainers(isTouchDown, (Clay_Vector2) {mouseWheelX, mouseWheelY}, deltaTime);
    bool isMobileScreen = windowWidth < 750;
    if (debugModeEnabled) {
        isMobileScreen = windowWidth < 950;
    }
    return CreateLayout(isMobileScreen, animationLerpValue < 0 ? (animationLerpValue + 1) : (1 - animationLerpValue));
    //----------------------------------------------------------------------------------
}

// Dummy main() to please cmake - TODO get wasm working with cmake on this example
int main() {
    return 0;
}
