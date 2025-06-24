#define CLAY_IMPLEMENTATION
#include "../../clay.h"
#include "../../renderers/raylib/clay_renderer_raylib.c"

const uint32_t FONT_ID_BODY_24 = 0;
const uint32_t FONT_ID_BODY_16 = 1;
#define COLOR_ORANGE (Clay_Color) {225, 138, 50, 255}
#define COLOR_BLUE (Clay_Color) {111, 173, 162, 255}

Texture2D profilePicture;
#define RAYLIB_VECTOR2_TO_CLAY_VECTOR2(vector) (Clay_Vector2) { .x = vector.x, .y = vector.y }

Clay_String profileText = CLAY_STRING_CONST("Profile Page one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen");
Clay_TextElementConfig headerTextConfig = { .fontId = 1, .letterSpacing = 5, .fontSize = 16, .textColor = {0,0,0,255} };

void HandleHeaderButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        // Do some click handling
    }
}

Clay_ElementDeclaration HeaderButtonStyle(bool hovered) {
    return (Clay_ElementDeclaration) {
        .layout = {.padding = {16, 16, 8, 8}},
        .backgroundColor = hovered ? COLOR_ORANGE : COLOR_BLUE,
    };
}

// Examples of re-usable "Components"
void RenderHeaderButton(Clay_String text) {
    CLAY(HeaderButtonStyle(Clay_Hovered())) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG(headerTextConfig));
    }
}

Clay_LayoutConfig dropdownTextItemLayout = { .padding = {8, 8, 4, 4} };
Clay_TextElementConfig dropdownTextElementConfig = { .fontSize = 24, .textColor = {255,255,255,255} };

void RenderDropdownTextItem(int index) {
    CLAY({ .layout = dropdownTextItemLayout, .backgroundColor = {180, 180, 180, 255} }) {
        CLAY_TEXT(CLAY_STRING("I'm a text field in a scroll container."), &dropdownTextElementConfig);
    }
}

typedef struct {
    int id;
    Clay_Color color;
} SortableBox;

SortableBox colors[30] = {};

#define GG { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() }

#define cWHITE { 255, 255, 255, 255 }

typedef struct {
    int value;
} Test;

typedef struct {
    void* memory;
    uintptr_t offset;
} Arena;

Arena frameArena = {};

Clay_String* FrameAllocateString(Clay_String string) {
    Clay_String *allocated = (Clay_String *)(frameArena.memory + frameArena.offset);
    *allocated = string;
    frameArena.offset += sizeof(Clay_String);
    return allocated;
}

bool moveTransition(Clay_TransitionData *current, Clay_TransitionData *target, float deltaTime) {
    current->boundingBox.x = (current->boundingBox.x + target->boundingBox.x) / 2;
    return true;
}

Clay_RenderCommandArray CreateLayout(void) {
    frameArena.offset = 0;
    Clay_BeginLayout();
    CLAY({ .id = CLAY_ID("OuterContainer"), .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) }, .padding = { 16, 16, 16, 16 }, .childGap = 12 }, .backgroundColor = cWHITE }) {
        CLAY({ .layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(60) }, .cornerRadius = { 12, 12, 12, 12 }, .backgroundColor = {174, 143, 204, 255 } }) {}
        for (int i = 0; i < 5; i++) {
            CLAY({ .layout.childGap = 12, .layout.sizing = GG }) {
                for (int j = 0; j < 6; j++) {
                    Clay_Color boxColor = colors[i * 5 + j].color;
                    Clay_Color darker = { boxColor.r * 0.9, boxColor.g * 0.9, boxColor.b * 0.9, 255 };
                    Clay_Color darkerStill = { boxColor.r * 0.8, boxColor.g * 0.8, boxColor.b * 0.8, 255 };
                    CLAY({
                        .id = CLAY_IDI_LOCAL("box", colors[i * 5 + j].id),
                        .layout.sizing = GG,
                        .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                        .backgroundColor = boxColor,
                        .cornerRadius = {12, 12, 12, 12},
                        .border = { darker, CLAY_BORDER_OUTSIDE(3) },
                        .transitions = {
                            .onMove = moveTransition,
                            .deltaTime = GetFrameTime()
                        }
                    }) {
                        char *idString = (char *)(frameArena.memory + frameArena.offset);
                        snprintf(idString, 3, "%02d", colors[i * 5 + j].id);
                        frameArena.offset += 2;
                        CLAY_TEXT(((Clay_String) { .length = 2, .chars = idString }), CLAY_TEXT_CONFIG({
                            .fontSize = 32,
                            .textColor = {154, 123, 184, 255 }
                        }));
                    }
                }
            }
        }
    }
    return Clay_EndLayout();
}

typedef struct
{
    Clay_Vector2 clickOrigin;
    Clay_Vector2 positionOrigin;
    bool mouseDown;
} ScrollbarData;

ScrollbarData scrollbarData = {0};

bool debugEnabled = false;

void UpdateDrawFrame(Font* fonts)
{
    Vector2 mouseWheelDelta = GetMouseWheelMoveV();
    float mouseWheelX = mouseWheelDelta.x;
    float mouseWheelY = mouseWheelDelta.y;

    if (IsKeyPressed(KEY_D)) {
        debugEnabled = !debugEnabled;
        Clay_SetDebugModeEnabled(debugEnabled);
    }
    //----------------------------------------------------------------------------------
    // Handle scroll containers
    Clay_Vector2 mousePosition = RAYLIB_VECTOR2_TO_CLAY_VECTOR2(GetMousePosition());
    Clay_SetPointerState(mousePosition, IsMouseButtonDown(0) && !scrollbarData.mouseDown);
    Clay_SetLayoutDimensions((Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() });
    if (!IsMouseButtonDown(0)) {
        scrollbarData.mouseDown = false;
    }

    if (IsMouseButtonDown(0) && !scrollbarData.mouseDown && Clay_PointerOver(Clay__HashString(CLAY_STRING("ScrollBar"), 0, 0))) {
        Clay_ScrollContainerData scrollContainerData = Clay_GetScrollContainerData(Clay__HashString(CLAY_STRING("MainContent"), 0, 0));
        scrollbarData.clickOrigin = mousePosition;
        scrollbarData.positionOrigin = *scrollContainerData.scrollPosition;
        scrollbarData.mouseDown = true;
    } else if (scrollbarData.mouseDown) {
        Clay_ScrollContainerData scrollContainerData = Clay_GetScrollContainerData(Clay__HashString(CLAY_STRING("MainContent"), 0, 0));
        if (scrollContainerData.contentDimensions.height > 0) {
            Clay_Vector2 ratio = (Clay_Vector2) {
                scrollContainerData.contentDimensions.width / scrollContainerData.scrollContainerDimensions.width,
                scrollContainerData.contentDimensions.height / scrollContainerData.scrollContainerDimensions.height,
            };
            if (scrollContainerData.config.vertical) {
                scrollContainerData.scrollPosition->y = scrollbarData.positionOrigin.y + (scrollbarData.clickOrigin.y - mousePosition.y) * ratio.y;
            }
            if (scrollContainerData.config.horizontal) {
                scrollContainerData.scrollPosition->x = scrollbarData.positionOrigin.x + (scrollbarData.clickOrigin.x - mousePosition.x) * ratio.x;
            }
        }
    }

    Clay_UpdateScrollContainers(true, (Clay_Vector2) {mouseWheelX, mouseWheelY}, GetFrameTime());
    // Generate the auto layout for rendering
    double currentTime = GetTime();
    Clay_RenderCommandArray renderCommands = CreateLayout();
    printf("layout time: %f microseconds\n", (GetTime() - currentTime) * 1000 * 1000);
    // RENDERING ---------------------------------
//    currentTime = GetTime();
    BeginDrawing();
    ClearBackground(BLACK);
    Clay_Raylib_Render(renderCommands, fonts);
    EndDrawing();
//    printf("render time: %f ms\n", (GetTime() - currentTime) * 1000);

    //----------------------------------------------------------------------------------
}

bool reinitializeClay = false;

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
    if (errorData.errorType == CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED) {
        reinitializeClay = true;
        Clay_SetMaxElementCount(Clay_GetMaxElementCount() * 2);
    } else if (errorData.errorType == CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED) {
        reinitializeClay = true;
        Clay_SetMaxMeasureTextCacheWordCount(Clay_GetMaxMeasureTextCacheWordCount() * 2);
    }
}

int main(void) {
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(clayMemory, (Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() }, (Clay_ErrorHandler) { HandleClayErrors, 0 });
    Clay_Raylib_Initialize(1024, 768, "Clay - Raylib Renderer Example", FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    profilePicture = LoadTexture("resources/profile-picture.png");

    Font fonts[2];
    fonts[FONT_ID_BODY_24] = LoadFontEx("resources/Roboto-Regular.ttf", 48, 0, 400);
	SetTextureFilter(fonts[FONT_ID_BODY_24].texture, TEXTURE_FILTER_BILINEAR);
    fonts[FONT_ID_BODY_16] = LoadFontEx("resources/Roboto-Regular.ttf", 32, 0, 400);
    SetTextureFilter(fonts[FONT_ID_BODY_16].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    frameArena = (Arena) {.memory = malloc(1024) };

    for (int i = 0; i < 30; i++) {
        colors[i] = (SortableBox) {
            .id = i,
            .color = { 255 - i, 255 - i * 4, 255 - i * 2, 255 }
        };
    }

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (reinitializeClay) {
            Clay_SetMaxElementCount(8192);
            totalMemorySize = Clay_MinMemorySize();
            clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
            Clay_Initialize(clayMemory, (Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() }, (Clay_ErrorHandler) { HandleClayErrors, 0 });
            reinitializeClay = false;
        }
        UpdateDrawFrame(fonts);
    }
    Clay_Raylib_Close();
    return 0;
}
