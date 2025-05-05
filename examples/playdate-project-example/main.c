
#include "pd_api.h"
#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#include "../../renderers/playdate/clay_renderer_playdate.c"
#include "clay-video-demo-playdate.c"

static int update(void *userdata);
const char *fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont *font = NULL;

void HandleClayErrors(Clay_ErrorData errorData) {}

struct TextUserData {
  LCDFont *font;
  PlaydateAPI *pd;
};

static struct TextUserData testUserData = {.font = NULL, .pd = NULL};

static Clay_Dimensions PlayDate_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
  // TODO: playdate needs to load fonts at the given size, so need to do that before we can use different font sizes!
  struct TextUserData *textUserData = userData;
  int width = textUserData->pd->graphics->getTextWidth(
    textUserData->font,
    text.chars,
    utf8_count_codepoints(text.chars, text.length),
    kUTF8Encoding,
    0
  );
  int height = textUserData->pd->graphics->getFontHeight(textUserData->font);
  return (Clay_Dimensions){
      .width = (float)width,
      .height = (float)height,
  };
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t eventArg)
{
  if (event == kEventInit) {
    const char *err;
    font = pd->graphics->loadFont(fontpath, &err);

    if (font == NULL) {
      pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err);
    }

    testUserData.pd = pd;
    testUserData.font = font;
    pd->system->setUpdateCallback(update, pd);

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, pd->system->realloc(NULL, totalMemorySize));
    Clay_Initialize(
      clayMemory,
      (Clay_Dimensions){(float)pd->display->getWidth(), (float)pd->display->getHeight()},
      (Clay_ErrorHandler){HandleClayErrors}
    );
    Clay_SetMeasureTextFunction(PlayDate_MeasureText, &testUserData);
    ClayVideoDemo_Initialize();
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
  pd->graphics->setFont(font);

  // A bit hacky, setting the cursor on to the document view so it can be
  // scrolled..
  Clay_SetPointerState((Clay_Vector2){
    .x = pd->display->getWidth() / 2.0f,
    .y = pd->display->getHeight() / 2.0f
  }, false);
  float crankDelta = pd->system->getCrankChange();
  Clay_UpdateScrollContainers(
    false,
    (Clay_Vector2){0, -crankDelta * 0.25f},
    pd->system->getElapsedTime()
  );
  Clay_RenderCommandArray renderCommands = ClayVideoDemo_CreateLayout(selectedDocumentIndex);

  Clay_Playdate_Render(pd, renderCommands, font);

  return 1;
}
