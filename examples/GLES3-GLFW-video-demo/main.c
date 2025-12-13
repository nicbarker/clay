#include <stdio.h>

#include <GLFW/glfw3.h>

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
    GLFWwindow *glfwWindow;
    int screenWidth, screenHeight;
} VideoCtx;

VideoCtx g_ctx;

static int initVideo(VideoCtx *ctx, const int initialWidth, const int initialHeight)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to init GLFW\n");
        return 0;
    }

    glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4); // enable multisampling
    
    // NO solution for high DPI yet
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);

    g_ctx.glfwWindow = glfwCreateWindow(initialWidth, initialHeight, "GLES3 GLFW Video Demo", NULL, NULL);

    if (g_ctx.glfwWindow == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(g_ctx.glfwWindow);

    glfwGetWindowSize(g_ctx.glfwWindow, &g_ctx.screenWidth, &g_ctx.screenHeight);
    glViewport(0, 0, g_ctx.screenWidth, g_ctx.screenHeight);
    printf("Frame buffer size %dx%d\n", g_ctx.screenWidth, g_ctx.screenHeight);


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

static Clay_Vector2 g_scrollDelta = {0.0f, 0.0f};
static double g_lastTime = 0.0;
static double g_deltaTime = 0.0;
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_scrollDelta.x += (float)xoffset;
    g_scrollDelta.y += (float)yoffset;
}

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
    // This example uses stb loader, but you can inject your custom loader
    // to load Images and Fonts if you don't want to use STB library
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
    glfwSetScrollCallback(g_ctx.glfwWindow, scroll_callback);
}


void loop()
{
    Clay_Vector2 scrollDelta = {0.0f, 0.0f};

    glfwPollEvents();

    /* Quit handling */
    if (glfwWindowShouldClose(g_ctx.glfwWindow))
    {
        g_ctx.shouldContinue = false;
    }

    /* Consume scroll delta (accumulated via callback) */
    scrollDelta = g_scrollDelta;
    g_scrollDelta.x = 0.0f;
    g_scrollDelta.y = 0.0f;

    /* Delta time (milliseconds, like your SDL version) */
    double now = glfwGetTime(); // seconds
    g_deltaTime = (now - g_lastTime) * 1000.0;
    g_lastTime = now;

    double mouseX = 0.0;
    double mouseY = 0.0;
    glfwGetCursorPos(g_ctx.glfwWindow, &mouseX, &mouseY);

    Clay_Vector2 mousePosition = {
        (float)mouseX,
        (float)mouseY
    };

    int mousePressed =
        glfwGetMouseButton(g_ctx.glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    Clay_SetPointerState(mousePosition, mousePressed);

    Clay_UpdateScrollContainers(
        true,
        (Clay_Vector2){scrollDelta.x, scrollDelta.y},
        g_deltaTime);

    glfwGetWindowSize(g_ctx.glfwWindow, &g_ctx.screenWidth, &g_ctx.screenHeight);
    glViewport(0, 0, g_ctx.screenWidth, g_ctx.screenHeight);

    Clay_SetLayoutDimensions((Clay_Dimensions){(float)g_ctx.screenWidth, (float)g_ctx.screenHeight});
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE); // Clay renderer is simple and never writes to depth buffer
    glClearColor(0.1f, 0.2f, 0.1f, 1.0f);

    ClayVideoDemo_Data data = ClayVideoDemo_Initialize();
    Clay_RenderCommandArray cmds = ClayVideoDemo_CreateLayout(&data);

    Gles3_Render(&g_gles3, cmds, g_stbFonts);

    glfwSwapBuffers(g_ctx.glfwWindow);

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