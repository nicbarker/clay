#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define CLAY_IMPLEMENTATION
#define CLAY_RENDERER_GLES3_IMPLEMENTATION

#include <clay.h>

#include "../../renderers/GLES3/clay_renderer_gles3.h"
#include "../shared-layouts/clay-video-demo.c"
#include "../../renderers/GLES3/clay_renderer_gles3_loader_stb.c"

typedef struct VideoCtx
{
    int shouldContinue;
    SDL_Window *sdlWindow;
    SDL_GLContext sdlContext;
    int screenWidth, screenHeight;
} VideoCtx;

VideoCtx g_ctx;

static int initVideo(VideoCtx *ctx, const int initialWidth, const int initialHeight)
{
    SDL_Init(SDL_INIT_VIDEO);

#if defined(__EMSCRIPTEN__)
    // OpenGL ES 3 profile
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    // Apple MacOs will use it own legacy desktop GL instead
    // I know, I lied, I said this was an GLES3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    g_ctx.sdlWindow = SDL_CreateWindow(
        "SDL2 GLES3",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        initialWidth,
        initialHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
    );
    g_ctx.sdlContext = SDL_GL_CreateContext(g_ctx.sdlWindow);

    SDL_ShowWindow(g_ctx.sdlWindow);
    SDL_Delay(1);
    SDL_GL_GetDrawableSize(g_ctx.sdlWindow, &g_ctx.screenWidth, &g_ctx.screenHeight);
    glViewport(0, 0, g_ctx.screenWidth, g_ctx.screenHeight);

    glEnable(GL_BLEND);
    // Enables blending, which allows transparent textures to be rendered properly.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Sets the blending function.
    // - `GL_SRC_ALPHA`: Uses the alpha value of the source (texture or color).
    // - `GL_ONE_MINUS_SRC_ALPHA`: Makes the destination color blend with the background based on alpha.
    // This is commonly used for standard transparency effects.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // Enables depth testing, ensuring that objects closer to the camera are drawn in front of those farther away.
    // This prevents objects from rendering incorrectly based on draw order.

    return 1;
}

void My_ErrorHandler(Clay_ErrorData errorData)
{
    printf("[ClaY ErroR] %s", errorData.errorText.chars);
}

Stb_FontData g_stbFonts[MAX_FONTS]; // Fonts userData
Gles3_Renderer g_gles3;             // The renderer itself

Uint64 NOW = 0;
Uint64 LAST = 0;
double deltaTime = 0;

// is executed before everything
void init()
{
    size_t clayRequiredMemory = Clay_MinMemorySize();
    g_gles3.clayMemory = (Clay_Arena){
        .capacity = clayRequiredMemory,
        .memory = (char *)malloc(clayRequiredMemory),
    };
    Clay_Context *clayCtx = Clay_Initialize(
        g_gles3.clayMemory,
        (Clay_Dimensions){
            .width = (float)g_ctx.screenWidth,
            .height = (float)g_ctx.screenHeight,
        },
        (Clay_ErrorHandler){
            .errorHandlerFunction = My_ErrorHandler,
        });

    // Note that MeasureText has to be set after the Context is set!
    Clay_SetCurrentContext(clayCtx);
    Clay_SetMeasureTextFunction(Stb_MeasureText, &g_stbFonts);
    Gles3_SetRenderTextFunction(&g_gles3, Stb_RenderText, &g_stbFonts);

    Gles3_Initialize(&g_gles3, 4096);

    int atlasW = 1024;
    int atlasH = 1024;
    if (!Stb_LoadFont(
            &g_gles3.fontTextures[0],
            &g_stbFonts[0],
            "resources/Roboto-Regular.ttf",
            24.0f, // bake pixel height
            atlasW,
            atlasH))
        abort();

    Clay_SetDebugModeEnabled(true);
}

void loop()
{

    glClearColor(0.1f, 0.2f, 0.1f, 1.0f);

    Clay_Vector2 scrollDelta = {};
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
        {
            g_ctx.shouldContinue = false;
        }
        case SDL_MOUSEWHEEL:
        {
            scrollDelta.x = event.wheel.x;
            scrollDelta.y = event.wheel.y;
            break;
        }
        }
    }
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();
    deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

    int mouseX = 0;
    int mouseY = 0;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    Clay_Vector2 mousePosition = (Clay_Vector2){(float)mouseX, (float)mouseY};
    Clay_SetPointerState(mousePosition, mouseState & SDL_BUTTON(1));

    Clay_UpdateScrollContainers(
        true,
        (Clay_Vector2){scrollDelta.x, scrollDelta.y},
        deltaTime);

    SDL_GL_GetDrawableSize(g_ctx.sdlWindow, &g_ctx.screenWidth, &g_ctx.screenHeight);
    glViewport(0, 0, g_ctx.screenWidth, g_ctx.screenHeight);
    Clay_SetLayoutDimensions((Clay_Dimensions){(float)g_ctx.screenWidth, (float)g_ctx.screenHeight});

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE); // Clay renderer is simple and never writes to depth buffer

    ClayVideoDemo_Data data = ClayVideoDemo_Initialize();
    Clay_RenderCommandArray cmds = ClayVideoDemo_CreateLayout(&data);

    Gles3_Render(&g_gles3, cmds, g_stbFonts);

    SDL_GL_SwapWindow(g_ctx.sdlWindow);
}

// Just initializes and spins the animation loop
int main()
{
    initVideo(&g_ctx, 1280, 720);
    init();

    g_ctx.shouldContinue = true;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#else
    while (g_ctx.shouldContinue)
    {
        loop();
    }
#endif
}