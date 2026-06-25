/**
 * Simple example based off the Clay README
 */
#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#include "../../renderers/tigr/clay_renderer_tigr.h"

const Clay_Color COLOR_LIGHT = (Clay_Color) {224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color) {168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    printf("%s\n", errorData.errorText.chars);
    switch(errorData.errorType) {
        // etc
    }
}

// Layout config is just a struct that can be declared statically, or inline
Clay_ElementDeclaration sidebarItemConfig = (Clay_ElementDeclaration) {
    .layout = {
        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50) }
    },
    .backgroundColor = COLOR_ORANGE
};

// Re-useable components are just normal functions
void SidebarItemComponent(char* my_str, int number) {
    snprintf(my_str, 8, "Box #%02d", number);
    Clay_String box_txt = (Clay_String){.isStaticallyAllocated = false, .length = 8, .chars = my_str};
    CLAY_AUTO_ID(sidebarItemConfig) {
        CLAY_TEXT(box_txt, { .fontSize = 8, .textColor = {255, 255, 255, 255} });
    }
}

int main() {
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));

    int screenWidth  = 1000;
    int screenHeight = 600;

    int mousePositionX, mousePositionY, isMouseDown;
    float mouseWheelX, mouseWheelY;
    Clay_Initialize(arena, (Clay_Dimensions) { screenWidth, screenHeight }, (Clay_ErrorHandler) { HandleClayErrors });
    Clay_SetMeasureTextFunction(Clay_Tigr_MeasureText, NULL);

    Tigr* win = tigrWindow(screenWidth, screenHeight, "Clay & Tigr", TIGR_AUTO);

    /* 20 8 byte strings */
    char* my_str = calloc(20, 8);
    if(my_str == NULL) {
        printf("Failed to allocate memoery\n");
        return -1;
    }

    uint64_t frame_cnt = 0;
    while(!tigrClosed(win) && !tigrKeyDown(win, TK_ESCAPE)) {

        int deltaTime = tigrTime(); // tigrTime return the time since it was last called
        tigrMouse(win, &mousePositionX, &mousePositionY, &isMouseDown);
        tigrScrollWheel(win, &mouseWheelX, &mouseWheelY);

        screenWidth = win->w;
        screenHeight = win->h;
        Clay_SetLayoutDimensions((Clay_Dimensions) { screenWidth, screenHeight });
        Clay_SetPointerState((Clay_Vector2) { mousePositionX, mousePositionY }, isMouseDown);
        Clay_UpdateScrollContainers(true, (Clay_Vector2) { mouseWheelX, mouseWheelY }, deltaTime);

        Clay_BeginLayout();

        CLAY(CLAY_ID("OuterContainer"), {
                .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                .padding = CLAY_PADDING_ALL(16), .childGap = 16 },
                .backgroundColor = {41, 41, 61,255} }) {

            CLAY(CLAY_ID("SideBarContainer"), {
                .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .sizing = {CLAY_SIZING_FIXED(300), CLAY_SIZING_GROW(0)},
                            .padding = CLAY_PADDING_ALL(16),
                            .childGap = 16},
                .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() },
                .backgroundColor = COLOR_LIGHT }) {

                CLAY(CLAY_ID("ProfilePictureOuter"), {
                        .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) },
                                    .padding = CLAY_PADDING_ALL(16),
                                    .childGap = 16,
                                    .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } },
                                    .backgroundColor = COLOR_RED }) {

                    CLAY(CLAY_ID("ProfilePicture"), {
                            .layout = { .sizing = { .width = CLAY_SIZING_FIXED(70), .height = CLAY_SIZING_FIXED(86) }},
                            .image = { .imageData = "./resources/leroy.png" } }) {}


                    CLAY_TEXT(CLAY_STRING("Clay & Tigr - UI Library"), { .fontSize = 16, .textColor = {255, 255, 255, 255} });
                }

                for (int i = 0; i < 20; i++) {
                    SidebarItemComponent(my_str + (8 * i), i);
                }

            }

            CLAY(CLAY_ID("MainContent"), {
                    .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) } },
                    .backgroundColor = COLOR_LIGHT }) {}
        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout(deltaTime);
        Clay_Tigr_Render(renderCommands, win);

        tigrUpdate(win);
    }

    free(my_str);
    tigrFree(win);
}
