#include "pd_api.h"
#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#include "../../renderers/playdate/clay_renderer_playdate.c"
#include "clay-video-demo-playdate.c"

static int update(void *userdata);

#define NUM_FONTS 2
const char *fontsToLoad[NUM_FONTS] = {
    "/System/Fonts/Asheville-Sans-14-Bold.pft",
    "/System/Fonts/Roobert-10-Bold.pft"
};

void HandleClayErrors(Clay_ErrorData errorData) {}

struct TextUserData {
    LCDFont *font[NUM_FONTS];
    PlaydateAPI *pd;
};

static struct TextUserData textUserData = { .font = { NULL }, .pd = NULL };

static Clay_Dimensions PlayDate_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    struct TextUserData *textUserData = userData;
    int width = textUserData->pd->graphics->getTextWidth(
        textUserData->font[config->fontId],
        text.chars,
        Clay_Playdate_CountUtf8Codepoints(text.chars, text.length),
        kUTF8Encoding,
        0
    );
    int height = textUserData->pd->graphics->getFontHeight(textUserData->font[config->fontId]);
    return (Clay_Dimensions){
        .width = (float)width,
        .height = (float)height,
    };
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t eventArg) {
    if (event == kEventInit) {
        const char *err;
        for (int i = 0; i < NUM_FONTS; ++i) {

            textUserData.font[i] = pd->graphics->loadFont(fontsToLoad[i], &err);
            if (textUserData.font[i] == NULL) {
                pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontsToLoad[i], err);
            }
        }

        textUserData.pd = pd;
        pd->system->setUpdateCallback(update, pd);

        uint64_t totalMemorySize = Clay_MinMemorySize();
        Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(
            totalMemorySize,
            pd->system->realloc(NULL, totalMemorySize)
        );
        Clay_Initialize(
            clayMemory,
            (Clay_Dimensions){
                (float)pd->display->getWidth(),
                (float)pd->display->getHeight()
            },
            (Clay_ErrorHandler){HandleClayErrors}
        );
        Clay_SetMeasureTextFunction(PlayDate_MeasureText, &textUserData);
        ClayVideoDemoPlaydate_Initialize(pd);
    }

    return 0;
}

int selectedDocumentIndex = 0;
#define WRAP_RANGE(x, N) ((((x) % (N)) + (N)) % (N))

static int update(void *userdata) {
    PlaydateAPI *pd = userdata;
    PDButtons pushedButtons;
    pd->system->getButtonState(NULL, &pushedButtons, NULL);

    if (pushedButtons & kButtonDown) {
        selectedDocumentIndex = WRAP_RANGE(selectedDocumentIndex + 1, MAX_DOCUMENTS);
    } else if (pushedButtons & kButtonUp) {
        selectedDocumentIndex = WRAP_RANGE(selectedDocumentIndex - 1, MAX_DOCUMENTS);
    }

    pd->graphics->clear(kColorWhite);

    // A bit hacky, setting the cursor on to the document view so it can be
    // scrolled..
    Clay_SetPointerState(
        (Clay_Vector2){
            .x = pd->display->getWidth() / 2.0f,
            .y = pd->display->getHeight() / 2.0f
        },
        false
    );

    float crankDelta = pd->system->getCrankChange();
    Clay_UpdateScrollContainers(
        false,
        (Clay_Vector2){ 0, -crankDelta * 0.25f },
        pd->system->getElapsedTime()
    );

    Clay_RenderCommandArray renderCommands = ClayVideoDemoPlaydate_CreateLayout(selectedDocumentIndex);
    Clay_Playdate_Render(pd, renderCommands, textUserData.font);

    return 1;
}
