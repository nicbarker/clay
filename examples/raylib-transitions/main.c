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
    CLAY_AUTO_ID(HeaderButtonStyle(Clay_Hovered())) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG(headerTextConfig));
    }
}

Clay_LayoutConfig dropdownTextItemLayout = { .padding = {8, 8, 4, 4} };
Clay_TextElementConfig dropdownTextElementConfig = { .fontSize = 24, .textColor = {255,255,255,255} };

void RenderDropdownTextItem(int index) {
    CLAY_AUTO_ID({ .layout = dropdownTextItemLayout, .backgroundColor = {180, 180, 180, 255} }) {
        CLAY_TEXT(CLAY_STRING("I'm a text field in a scroll container."), dropdownTextElementConfig);
    }
}

typedef struct {
    int id;
    Clay_Color color;
    const char* stringId;
} SortableBox;

int maxCount = 30;
int cellCount = 30;
char* charData;
SortableBox colors[100] = {};

bool blueColor = 0;

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

bool Clay_EaseOut(Clay_TransitionCallbackArguments arguments) {
    float ratio = arguments.elapsedTime / arguments.duration;
    if (ratio < 1) {
        float lerpAmount = (1 - powf(1 - ratio, 3.0f));
        bool allProperties = arguments.properties == CLAY_TRANSITION_PROPERTY_ALL;
        if (allProperties || arguments.properties & CLAY_TRANSITION_PROPERTY_X) {
            arguments.current->boundingBox.x = Lerp(arguments.initial.boundingBox.x, arguments.target.boundingBox.x, lerpAmount);
        } else {
            arguments.current->boundingBox.x = arguments.target.boundingBox.x;
        }
        if (allProperties || arguments.properties & CLAY_TRANSITION_PROPERTY_Y) {
            arguments.current->boundingBox.y = Lerp(arguments.initial.boundingBox.y, arguments.target.boundingBox.y, lerpAmount);
        } else {
            arguments.current->boundingBox.y = arguments.target.boundingBox.y;
        }
        if (allProperties || arguments.properties & CLAY_TRANSITION_PROPERTY_WIDTH) {
            arguments.current->boundingBox.width = Lerp(arguments.initial.boundingBox.width, arguments.target.boundingBox.width, lerpAmount);
        } else {
            arguments.current->boundingBox.width = arguments.target.boundingBox.width;
        }
        if (allProperties || arguments.properties & CLAY_TRANSITION_PROPERTY_HEIGHT) {
            arguments.current->boundingBox.height = Lerp(arguments.initial.boundingBox.height, arguments.target.boundingBox.height, lerpAmount);
        } else {
            arguments.current->boundingBox.height = arguments.target.boundingBox.height;
        }
        if (allProperties || arguments.properties & CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR) {
            arguments.current->backgroundColor = (Clay_Color) {
                .r = Lerp(arguments.initial.backgroundColor.r, arguments.target.backgroundColor.r, lerpAmount),
                .g = Lerp(arguments.initial.backgroundColor.g, arguments.target.backgroundColor.g, lerpAmount),
                .b = Lerp(arguments.initial.backgroundColor.b, arguments.target.backgroundColor.b, lerpAmount),
                .a = Lerp(arguments.initial.backgroundColor.a, arguments.target.backgroundColor.a, lerpAmount),
            };
        } else {
            arguments.current->backgroundColor = arguments.target.backgroundColor;
        }
        if (allProperties || arguments.properties & CLAY_TRANSITION_PROPERTY_OVERLAY_COLOR) {
            arguments.current->overlayColor = (Clay_Color) {
                .r = Lerp(arguments.initial.overlayColor.r, arguments.target.overlayColor.r, lerpAmount),
                .g = Lerp(arguments.initial.overlayColor.g, arguments.target.overlayColor.g, lerpAmount),
                .b = Lerp(arguments.initial.overlayColor.b, arguments.target.overlayColor.b, lerpAmount),
                .a = Lerp(arguments.initial.overlayColor.a, arguments.target.overlayColor.a, lerpAmount),
            };
        } else {
            arguments.current->overlayColor = arguments.target.overlayColor;
        }
        return false;
    } else {
        arguments.current->boundingBox = arguments.target.boundingBox;
        arguments.current->backgroundColor = arguments.target.backgroundColor;
        arguments.current->overlayColor = arguments.target.overlayColor;
        return true;
    }
}

Clay_TransitionData EnterExitSlideUp(Clay_TransitionData initialState) {
    Clay_TransitionData targetState = initialState;
    targetState.boundingBox.y += 20;
    targetState.overlayColor = (Clay_Color) { 255, 255, 255, 255 };
    return targetState;
}
// Swaps two elements in an array
void swap(SortableBox *a, SortableBox *b) {
    SortableBox temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(SortableBox *array, size_t n) {
    if (n <= 1) return;

    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        swap(&array[i], &array[j]);
    }
}

void add(SortableBox *array, int32_t length, int32_t index, SortableBox toAdd) {
    for (int i = length; i > index; --i) {
        array[i] = array[i - 1];
    }
    array[index] = toAdd;
}

void HandleRandomiseButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        shuffle(colors, cellCount);
    }
}

void HandlePinkButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        for (int i = 0; i < cellCount; i++) {
            int index = colors[i].id;
            colors[i] = (SortableBox) {
                .id = index,
                .color = { 255 - index, 255 - index * 4, 255 - index * 2, 255 },
                .stringId = colors[i].stringId
            };
        }
    }
}

void HandleNewButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        int32_t randomIndex = rand() % (cellCount + 1);
        int32_t newId = maxCount;
        snprintf(&charData[newId * 3], 3, "%02d", newId);

        add(colors, maxCount, randomIndex, (SortableBox) {
            .id = newId,
            .color = { 255 - newId, 255 - newId * 4, 255 - newId * 2, 255 },
            .stringId = &charData[newId * 3]
        });

        cellCount++;
        maxCount++;
    }
}

void HandleBlueButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        for (int i = 0; i < cellCount; i++) {
            int index = colors[i].id;
            colors[i] = (SortableBox) {
                .id = index,
                .color = { 255 - index * 4, 255 - index * 2, 255 - index, 255 },
                .stringId = colors[i].stringId
            };
        }
    }
}

void HandleCellButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, void *userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        for (int i = (uintptr_t)userData; i < cellCount; i++) {
            colors[i] = colors[i + 1];
        }
        cellCount = CLAY__MAX(cellCount - 1, 0);
    }
}

Clay_RenderCommandArray CreateLayout(void) {
    frameArena.offset = 0;
    Clay_BeginLayout();
    CLAY(CLAY_ID("OuterContainer"), { .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) }, .padding = { 16, 16, 16, 16 }, .childGap = 12 }, .backgroundColor = cWHITE }) {
        CLAY_AUTO_ID({ .layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(60) }, .layout.padding.left = 16, .layout.childGap = 16, .layout.childAlignment.y = CLAY_ALIGN_Y_CENTER, .cornerRadius = { 12, 12, 12, 12 }, .backgroundColor = {174, 143, 204, 255 } }) {
            CLAY(CLAY_ID("ShuffleButton"), {
                .backgroundColor = Clay_Hovered() ? (Clay_Color){ 154, 123, 184, 255 } : (Clay_Color){ },
                .layout.padding = { 16, 16, 8, 8 },
                .cornerRadius = CLAY_CORNER_RADIUS(6),
                .border = { .color = cWHITE, .width = CLAY_BORDER_OUTSIDE(2) },
            }) {
                Clay_OnHover(HandleRandomiseButtonInteraction, 0);
                CLAY_TEXT(CLAY_STRING("Randomise"), CLAY_TEXT_CONFIG({ .fontSize = 20, .textColor = cWHITE }));
            }
            CLAY(CLAY_ID("bluebutton"), {
                .backgroundColor = Clay_Hovered() ? (Clay_Color){ 154, 123, 184, 255 } : (Clay_Color){ },
                .layout.padding = { 16, 16, 8, 8 },
                .cornerRadius = CLAY_CORNER_RADIUS(6),
                .border = { .color = cWHITE, .width = CLAY_BORDER_OUTSIDE(2) },
            }) {
                Clay_OnHover(HandleBlueButtonInteraction, 0);
                CLAY_TEXT(CLAY_STRING("Blue"), CLAY_TEXT_CONFIG({ .fontSize = 20, .textColor = cWHITE }));
            }
            CLAY(CLAY_ID("PinkButton"), {
                .backgroundColor = Clay_Hovered() ? (Clay_Color){ 154, 123, 184, 255 } : (Clay_Color){ },
                .layout.padding = { 16, 16, 8, 8 },
                .cornerRadius = CLAY_CORNER_RADIUS(6),
                .border = { .color = cWHITE, .width = CLAY_BORDER_OUTSIDE(2) },
            }) {
                Clay_OnHover(HandlePinkButtonInteraction, 0);
                CLAY_TEXT(CLAY_STRING("Pink"), CLAY_TEXT_CONFIG({ .fontSize = 20, .textColor = cWHITE }));
            }
            CLAY(CLAY_ID("AddButton"), {
                .backgroundColor = Clay_Hovered() ? (Clay_Color){ 154, 123, 184, 255 } : (Clay_Color){ },
                .layout.padding = { 16, 16, 8, 8 },
                .cornerRadius = CLAY_CORNER_RADIUS(6),
                .border = { .color = cWHITE, .width = CLAY_BORDER_OUTSIDE(2) },
            }) {
                Clay_OnHover(HandleNewButtonInteraction, 0);
                CLAY_TEXT(CLAY_STRING("Add Box"), CLAY_TEXT_CONFIG({ .fontSize = 20, .textColor = cWHITE }));
            }
        }
        for (int i = 0; i < 5; i++) {
            CLAY(CLAY_IDI("row", i), { .layout.childGap = 12, .layout.sizing = GG }) {
                for (int j = 0; j < 6; j++) {
                    int index = i * 6 + j;
                    if (index >= cellCount) {
                        break;
                    }
                    Clay_Color boxColor = colors[index].color;
                    Clay_Color darker = { boxColor.r * 0.9, boxColor.g * 0.9, boxColor.b * 0.9, 255 };
                    CLAY(CLAY_IDI("box", colors[index].id), {
                        .layout.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
                        .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
                        .backgroundColor = boxColor,
                        .overlayColor = Clay_Hovered() ? (Clay_Color) { 80, 80, 80, 80 } : (Clay_Color) { 255, 255, 255, 0 },
                        .cornerRadius = {12, 12, 12, 12},
                        .border = { darker, CLAY_BORDER_OUTSIDE(3) },
                        .transition = {
                            .handler = Clay_EaseOut,
                            .duration = 0.5,
                            .properties = CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR
                                    | CLAY_TRANSITION_PROPERTY_OVERLAY_COLOR
                                    | CLAY_TRANSITION_PROPERTY_BOUNDING_BOX,
                            .enter = { .setInitialState = EnterExitSlideUp },
                            .exit = { .setFinalState = EnterExitSlideUp },
                        }
                    }) {
                        Clay_OnHover(HandleCellButtonInteraction, (void*)index);
                        CLAY_TEXT(((Clay_String) { .length = 2, .chars = colors[index].stringId, .isStaticallyAllocated = true }), CLAY_TEXT_CONFIG({
                            .fontSize = 32,
                            .textColor = colors[index].id > 29 ? (Clay_Color) {255, 255, 255, 255} : (Clay_Color) {154, 123, 184, 255 }
                        }));
                    }
                }
            }
        }
    }
    return Clay_EndLayout(GetFrameTime());
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

    if (IsMouseButtonDown(0) && !scrollbarData.mouseDown && Clay_PointerOver(Clay__HashString(CLAY_STRING("ScrollBar"), 0))) {
        Clay_ScrollContainerData scrollContainerData = Clay_GetScrollContainerData(Clay__HashString(CLAY_STRING("MainContent"), 0));
        scrollbarData.clickOrigin = mousePosition;
        scrollbarData.positionOrigin = *scrollContainerData.scrollPosition;
        scrollbarData.mouseDown = true;
    } else if (scrollbarData.mouseDown) {
        Clay_ScrollContainerData scrollContainerData = Clay_GetScrollContainerData(Clay__HashString(CLAY_STRING("MainContent"), 0));
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
    printf("%s\n", errorData.errorText.chars);
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
    Clay_Context *context = Clay_Initialize(clayMemory, (Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() }, (Clay_ErrorHandler) { HandleClayErrors, 0 });
    Clay_Raylib_Initialize(1024, 768, "Clay - Raylib Renderer Example", FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    profilePicture = LoadTexture("resources/profile-picture.png");

    Font fonts[2];
    fonts[FONT_ID_BODY_24] = LoadFontEx("resources/Roboto-Regular.ttf", 48, 0, 400);
	SetTextureFilter(fonts[FONT_ID_BODY_24].texture, TEXTURE_FILTER_BILINEAR);
    fonts[FONT_ID_BODY_16] = LoadFontEx("resources/Roboto-Regular.ttf", 32, 0, 400);
    SetTextureFilter(fonts[FONT_ID_BODY_16].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    frameArena = (Arena) {.memory = malloc(1024) };

    charData = (char*)malloc(100 * 3);

    for (int i = 0; i < cellCount; i++) {
        snprintf(&charData[i * 3], 3, "%02d", i);
        colors[i] = (SortableBox) {
            .id = i,
            .color = { 255 - i, 255 - i * 4, 255 - i * 2, 255 },
            .stringId = &charData[i * 3],
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
