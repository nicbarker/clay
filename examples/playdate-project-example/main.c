
#include "pd_api.h"
#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#include "../../renderers/playdate/clay_renderer_playdate.c"
#include "../shared-layouts/clay-video-demo.c"

static int update(void *userdata);
const char *fontpath = "/System/Fonts/Asheville-Sans-14-Bold.pft";
LCDFont *font = NULL;

void HandleClayErrors(Clay_ErrorData errorData) {}

struct TextUserData {
  LCDFont *font;
  PlaydateAPI *pd;
};

static struct TextUserData gTextUserData = {.font = NULL, .pd = NULL};
static ClayVideoDemo_Data demoData;

static Clay_Dimensions PlayDate_MeasureText(Clay_StringSlice text,
                                            Clay_TextElementConfig *config,
                                            void *userData) {
  // TODO: playdate needs to load fonts at the given size, so need to do that
  // before we can use different font sizes!
  struct TextUserData *textUserData = userData;
  int width = textUserData->pd->graphics->getTextWidth(
      textUserData->font, text.chars,
      utf8_count_codepoints(text.chars, text.length), kUTF8Encoding, 0);
  int height = textUserData->pd->graphics->getFontHeight(textUserData->font);
  return (Clay_Dimensions){
      .width = (float)width,
      .height = (float)height,
  };
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
  (void)arg; // arg is currently only used for event = kEventKeyPressed

  if (event == kEventInit) {
    const char *err;
    font = pd->graphics->loadFont(fontpath, &err);

    if (font == NULL)
      pd->system->error("%s:%i Couldn't load font %s: %s", __FILE__, __LINE__,
                        fontpath, err);

    gTextUserData.pd = pd;
    gTextUserData.font = font;

    // Note: If you set an update callback in the kEventInit handler, the system
    // assumes the game is pure C and doesn't run any Lua code in the game
    pd->system->setUpdateCallback(update, pd);

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(
        totalMemorySize, pd->system->realloc(NULL, totalMemorySize));
    Clay_Initialize(clayMemory,
                    (Clay_Dimensions){(float)pd->display->getWidth(),
                                      (float)pd->display->getHeight()},
                    (Clay_ErrorHandler){HandleClayErrors});
    Clay_SetMeasureTextFunction(PlayDate_MeasureText, &gTextUserData);
    demoData = ClayVideoDemo_Initialize(pd);
  }

  return 0;
}

#define TEXT_WIDTH 86
#define TEXT_HEIGHT 16

int x = (400 - TEXT_WIDTH) / 2;
int y = (240 - TEXT_HEIGHT) / 2;
int dx = 1;
int dy = 2;

static int update(void *userdata) {
  PlaydateAPI *pd = userdata;

  pd->graphics->clear(kColorWhite);
  pd->graphics->setFont(font);

  Clay_SetPointerState((Clay_Vector2){.x = pd->display->getWidth() / 2.0f,
                                      .y = pd->display->getHeight() / 2.0f},
                       false);
  float crankDelta = pd->system->getCrankChange();
  Clay_UpdateScrollContainers(true, (Clay_Vector2){0, crankDelta * 0.25f},
                              pd->system->getElapsedTime());
  Clay_RenderCommandArray renderCommands =
      ClayVideoDemo_CreateLayout(&demoData);

  Clay_Playdate_Render(pd, renderCommands, font);

  pd->system->drawFPS(0, 0);

  return 1;
}
