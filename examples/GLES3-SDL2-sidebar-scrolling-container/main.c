#include <SDL.h>
#include <stdio.h>

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
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
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

uint64_t g_drawCallsDuringLastFrame = 0;

double g_timeAccumulator = 0.0;
double g_avgFrameMs = 0.0;
double g_fps = 0.0;
int g_frameCount = 0;

char g_fpsText[128];
size_t g_fpsTextLen = 0;

static double g_wallTimeAccumulator = 0.0;

char g_glInfoText[512];
size_t g_glInfoTextLen;

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
    if (!Stb_LoadFont(
            &g_gles3.fontTextures[1],
            &g_stbFonts[1],
            "resources/RobotoMono-Medium.ttf",
            24.0f, // bake pixel height
            atlasW,
            atlasH))
        abort();

    if (!Stb_LoadImage(
            &g_gles3.imageTextures[0],
            "resources/profile-picture.png"))
        abort();

    if (!Stb_LoadImage(
            &g_gles3.imageTextures[1],
            "resources/millbank.jpeg"))
        abort();

    Clay_SetDebugModeEnabled(true);

    const GLubyte *glVersion = glGetString(GL_VERSION);
    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *renderer = glGetString(GL_RENDERER);

    g_glInfoTextLen = (size_t)snprintf(
        g_glInfoText,
        sizeof(g_glInfoText),
        "OpenGL Version : %s\n"
        "GLSL Version   : %s\n"
        "Vendor         : %s\n"
        "Renderer       : %s",
        glVersion ? (const char *)glVersion : "unknown",
        glslVersion ? (const char *)glslVersion : "unknown",
        vendor ? (const char *)vendor : "unknown",
        renderer ? (const char *)renderer : "unknown");
}

Gles3_ImageConfig g_profilePicture = (Gles3_ImageConfig){
    .textureToUse = 0,
    .u0 = 0.0f,
    .v0 = 0.0f,
    .u1 = 1.0f,
    .v1 = 1.0f,
};

Gles3_ImageConfig g_window1 = (Gles3_ImageConfig){
    .textureToUse = 1,
    .u0 = 0.0f,
    .v0 = 0.35f,
    .u1 = 0.18f,
    .v1 = 0.75f,
};
Gles3_ImageConfig g_window2 = (Gles3_ImageConfig){
    .textureToUse = 1,
    .u0 = 0.25f,
    .v0 = 0.35f,
    .u1 = 0.47f,
    .v1 = 0.75f,
};
Gles3_ImageConfig g_window3 = (Gles3_ImageConfig){
    .textureToUse = 1,
    .u0 = 0.52f,
    .v0 = 0.35f,
    .u1 = 0.76f,
    .v1 = 0.75f,
};
Gles3_ImageConfig g_window4 = (Gles3_ImageConfig){
    .textureToUse = 1,
    .u0 = 0.82f,
    .v0 = 0.35f,
    .u1 = 1.0f,
    .v1 = 0.75f,
};

Clay_LayoutConfig dropdownTextItemLayout = {.padding = {8, 8, 4, 4}};
Clay_TextElementConfig dropdownTextElementConfig = {.fontSize = 24, .textColor = {55, 55, 55, 255}};
void RenderDropdownTextItem(int index)
{
    CLAY_AUTO_ID({.layout = dropdownTextItemLayout, .backgroundColor = {220, 220, 220, 255}})
    {
        CLAY_TEXT(CLAY_STRING("I'm a text field in a scroll container."), &dropdownTextElementConfig);
    }
}

const uint32_t FONT_ID_BODY_24 = 1;

Clay_String profileText = CLAY_STRING_CONST("Profile Page one two three four five six seven eight nine ten eleven twelve thirteen fourteen fifteen");

void RenderHeaderButton1(Clay_String text)
{
    CLAY_AUTO_ID(
        {.layout = {
             .padding = {16, 16, 8, 8}},
         .backgroundColor = {140, 140, 140, 255},
         .border = {.width = CLAY_BORDER_OUTSIDE(14), .color = {180, 80, 80, 255}},
         .cornerRadius = CLAY_CORNER_RADIUS(5)})
    {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({.fontId = FONT_ID_BODY_16, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
    }
}
void RenderHeaderButton2(Clay_String text)
{
    CLAY_AUTO_ID(
        {.layout = {
             .padding = {16, 16, 8, 8}},
         .backgroundColor = {140, 140, 140, 255},
         // .border = { .width = CLAY_BORDER_OUTSIDE(4), .color = {180, 80, 80, 255} },
         .cornerRadius = CLAY_CORNER_RADIUS(5)})
    {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({.fontId = FONT_ID_BODY_16, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
    }
}
void RenderHeaderButton3(Clay_String text)
{
    CLAY_AUTO_ID(
        {.layout = {
             .padding = {16, 16, 8, 8}},
         .backgroundColor = {140, 140, 140, 255},
         .border = {.width = CLAY_BORDER_OUTSIDE(14), .color = {180, 80, 80, 255}},
         .cornerRadius = CLAY_CORNER_RADIUS(0)})
    {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({.fontId = FONT_ID_BODY_16, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
    }
}
void RenderHeaderButton4(Clay_String text)
{
    CLAY_AUTO_ID(
        {.layout = {
             .padding = {16, 16, 8, 8}},
         .backgroundColor = {140, 140, 140, 255},
         .border = {.width = CLAY_BORDER_OUTSIDE(4), .color = {180, 80, 80, 255}},
         .cornerRadius = CLAY_CORNER_RADIUS(5)})
    {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({.fontId = FONT_ID_BODY_16, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
    }
}
Clay_RenderCommandArray CreateLayout(void)
{
    Clay_BeginLayout();
    CLAY(CLAY_ID("OuterContainer"),
         {.layout = {.sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0)}, .padding = {16, 16, 16, 16}, .childGap = 16}, .backgroundColor = {200, 200, 200, 255}})
    {
        CLAY(CLAY_ID("SideBar"),
             {.layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW(0)}, .padding = {16, 16, 16, 16}, .childGap = 16}, .backgroundColor = {150, 150, 255, 255}})
        {
            CLAY(CLAY_ID("ProfilePictureOuter"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(0)}, .padding = {8, 8, 8, 8}, .childGap = 8, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}}, .backgroundColor = {130, 130, 255, 255}})
            {
                CLAY(CLAY_ID("ProfilePicture"),
                     {
                         .layout = {.sizing = {.width = CLAY_SIZING_FIXED(60), .height = CLAY_SIZING_FIXED(60)}},
                         .image = {.imageData = &g_profilePicture},
                         .cornerRadius = {30, 30, 30, 30},
                     })
                {
                }
                CLAY_TEXT(profileText, CLAY_TEXT_CONFIG({.fontSize = 24, .textColor = {0, 0, 0, 255}, .textAlignment = CLAY_TEXT_ALIGN_RIGHT}));
            }
            CLAY(CLAY_ID("SidebarBlob1"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50)}}, .backgroundColor = {110, 110, 255, 255}}) {}
            CLAY(CLAY_ID("SidebarBlob2"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50)}}, .backgroundColor = {110, 110, 255, 255}}) {}
            CLAY(CLAY_ID("SidebarBlob3"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50)}}, .backgroundColor = {110, 110, 255, 255}}) {}
            CLAY(CLAY_ID("SidebarBlob4"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50)}}, .backgroundColor = {110, 110, 255, 255}}) {}
        }
        CLAY(CLAY_ID("RightPanel"), {.layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0)}, .childGap = 16}})
        {
            CLAY_AUTO_ID({.layout = {.sizing = {.width = CLAY_SIZING_GROW(0)}, .childAlignment = {.x = CLAY_ALIGN_X_RIGHT}, .padding = {8, 8, 8, 8}, .childGap = 18}, .backgroundColor = {180, 180, 180, 255}})
            {
                RenderHeaderButton1(CLAY_STRING("Header Item 1"));
                RenderHeaderButton2(CLAY_STRING("Header Item 2"));
                RenderHeaderButton3(CLAY_STRING("Header Item 3"));
                RenderHeaderButton4(CLAY_STRING("Header Item 4"));
            }
            CLAY(
                CLAY_ID("MainContent"),
                {
                    .layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM, .padding = {16, 16, 16, 16}, .childGap = 16, .sizing = {.width = CLAY_SIZING_GROW(0)}},
                    .backgroundColor = {200, 200, 255, 255},
                    .clip = {.vertical = true, .childOffset = Clay_GetScrollOffset()},
                })
            {
                CLAY(
                    CLAY_ID("FloatingContainer"),
                    {
                        .layout = {.sizing = {.width = CLAY_SIZING_PERCENT(0.5), .height = CLAY_SIZING_FIXED(300)}, .padding = {16, 16, 16, 16}},
                        .backgroundColor = {140, 80, 200, 200},
                        .floating = {.attachTo = CLAY_ATTACH_TO_PARENT, .zIndex = 1, .attachPoints = {CLAY_ATTACH_POINT_CENTER_TOP, CLAY_ATTACH_POINT_CENTER_TOP}, .offset = {0, 0}},
                        .border = {.width = CLAY_BORDER_OUTSIDE(4), .color = {80, 80, 80, 255}},
                        .cornerRadius = {30, 3, 3, 30},
                    })
                {
                    CLAY_TEXT(
                        CLAY_STRING("I'm an inline floating container."), CLAY_TEXT_CONFIG({.fontSize = 24, .textColor = {255, 255, 255, 255}}));
                }

                CLAY_TEXT(
                    CLAY_STRING("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt."),
                    CLAY_TEXT_CONFIG({.fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = {0, 0, 0, 255}}));

                CLAY_TEXT(
                    CLAY_STRING("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt."),
                    CLAY_TEXT_CONFIG({.fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = {0, 0, 0, 255}}));

                CLAY_TEXT(CLAY_STRING("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt."),
                          CLAY_TEXT_CONFIG({.fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = {0, 0, 0, 255}}));

                CLAY(CLAY_ID("Photos2"), {.layout = {.childGap = 16, .padding = {16, 16, 16, 16}}, .backgroundColor = {180, 180, 220, (float)(Clay_Hovered() ? 120 : 255)}})
                {
                    CLAY(CLAY_ID("Picture4"), {.layout = {.sizing = {.width = CLAY_SIZING_FIXED(100), .height = CLAY_SIZING_FIXED(120)}}, .image = {.imageData = &g_window1}}) {}
                    CLAY(CLAY_ID("Picture5"), {.layout = {.sizing = {.width = CLAY_SIZING_FIXED(100), .height = CLAY_SIZING_FIXED(120)}}, .image = {.imageData = &g_window2}}) {}
                    CLAY(CLAY_ID("Picture6"), {.layout = {.sizing = {.width = CLAY_SIZING_FIXED(100), .height = CLAY_SIZING_FIXED(120)}}, .image = {.imageData = &g_window3}}) {}
                    CLAY(CLAY_ID("Picture6.5"), {.layout = {.sizing = {.width = CLAY_SIZING_FIXED(100), .height = CLAY_SIZING_FIXED(120)}}, .image = {.imageData = &g_window4}}) {}
                }

                Clay_String cs = {.isStaticallyAllocated = false, .length = g_glInfoTextLen, .chars = g_glInfoText};
                Clay_TextElementConfig glInfoElementConfig = {.fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = {255, 255, 255, 255}};
                CLAY_TEXT(cs, &glInfoElementConfig);

                CLAY_TEXT(
                    CLAY_STRING("Faucibus purus in massa tempor nec. Nec ullamcorper sit amet risus nullam eget felis eget nunc. Diam vulputate ut pharetra sit amet aliquam id diam. Lacus suspendisse faucibus interdum posuere lorem. A diam sollicitudin tempor id. Amet massa vitae tortor condimentum lacinia. Aliquet nibh praesent tristique magna."),
                    CLAY_TEXT_CONFIG({.fontSize = 24, .lineHeight = 60, .textColor = {0, 0, 0, 255}, .textAlignment = CLAY_TEXT_ALIGN_CENTER}));

                CLAY_TEXT(CLAY_STRING("Suspendisse in est ante in nibh. Amet venenatis urna cursus eget nunc scelerisque viverra. Elementum sagittis vitae et leo duis ut diam quam nulla. Enim nulla aliquet porttitor lacus. Pellentesque habitant morbi tristique senectus et. Facilisi nullam vehicula ipsum a arcu cursus vitae.\nSem fringilla ut morbi tincidunt. Euismod quis viverra nibh cras pulvinar mattis nunc sed. Velit sed ullamcorper morbi tincidunt ornare massa. Varius quam quisque id diam vel quam. Nulla pellentesque dignissim enim sit amet venenatis. Enim lobortis scelerisque fermentum dui faucibus in. Pretium viverra suspendisse potenti nullam ac tortor vitae. Lectus vestibulum mattis ullamcorper velit sed. Eget mauris pharetra et ultrices neque ornare aenean euismod elementum. Habitant morbi tristique senectus et. Integer vitae justo eget magna fermentum iaculis eu. Semper quis lectus nulla at volutpat diam. Enim praesent elementum facilisis leo. Massa vitae tortor condimentum lacinia quis vel."),
                          CLAY_TEXT_CONFIG({.fontSize = 24, .textColor = {0, 0, 0, 255}}));

                CLAY(CLAY_ID("Photos"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(0)}, .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}, .childGap = 16, .padding = {16, 16, 16, 16}}, .backgroundColor = {180, 180, 220, 255}})
                {
                    CLAY(CLAY_ID("Picture2"), {.layout = {.sizing = {.width = CLAY_SIZING_FIXED(120)}}, .aspectRatio = 1, .image = {.imageData = &g_profilePicture}}) {}
                    CLAY(CLAY_ID("Picture1"), {.layout = {.childAlignment = {.x = CLAY_ALIGN_X_CENTER}, .layoutDirection = CLAY_TOP_TO_BOTTOM, .padding = {8, 8, 8, 8}}, .backgroundColor = {170, 170, 220, 255}})
                    {
                        CLAY(CLAY_ID("ProfilePicture2"), {.layout = {.sizing = {.width = CLAY_SIZING_FIXED(60), .height = CLAY_SIZING_FIXED(60)}}, .image = {.imageData = &g_profilePicture}}) {}
                        CLAY_TEXT(CLAY_STRING("Image caption below"), CLAY_TEXT_CONFIG({.fontSize = 24, .textColor = {0, 0, 0, 255}}));
                    }
                    CLAY(CLAY_ID("Picture3"), {.layout = {.sizing = {.width = CLAY_SIZING_FIXED(120)}}, .aspectRatio = 1, .image = {.imageData = &g_profilePicture}}) {}
                }

                CLAY_TEXT(
                    CLAY_STRING("Amet cursus sit amet dictum sit amet justo donec. Et malesuada fames ac turpis egestas maecenas. A lacus vestibulum sed arcu non odio euismod lacinia. Gravida neque convallis a cras. Dui nunc mattis enim ut tellus elementum sagittis vitae et. Orci sagittis eu volutpat odio facilisis mauris. Neque gravida in fermentum et sollicitudin ac orci. Ultrices dui sapien eget mi proin sed libero. Euismod quis viverra nibh cras pulvinar mattis. Diam volutpat commodo sed egestas egestas. In fermentum posuere urna nec tincidunt praesent semper. Integer eget aliquet nibh praesent tristique magna.\nId cursus metus aliquam eleifend mi in. Sed pulvinar proin gravida hendrerit lectus a. Etiam tempor orci eu lobortis elementum nibh tellus. Nullam vehicula ipsum a arcu cursus vitae. Elit scelerisque mauris pellentesque pulvinar pellentesque habitant morbi tristique senectus. Condimentum lacinia quis vel eros donec ac odio. Mattis pellentesque id nibh tortor id aliquet lectus. Turpis egestas integer eget aliquet nibh praesent tristique. Porttitor massa id neque aliquam vestibulum morbi. Mauris commodo quis imperdiet massa tincidunt nunc pulvinar sapien et. Nunc scelerisque viverra mauris in aliquam sem fringilla. Suspendisse ultrices gravida dictum fusce ut placerat orci nulla.\nLacus laoreet non curabitur gravida arcu ac tortor dignissim. Urna nec tincidunt praesent semper feugiat nibh sed pulvinar. Tristique senectus et netus et malesuada fames ac. Nunc aliquet bibendum enim facilisis gravida. Egestas maecenas pharetra convallis posuere morbi leo urna molestie. Sapien nec sagittis aliquam malesuada bibendum arcu vitae elementum curabitur. Ac turpis egestas maecenas pharetra convallis posuere morbi leo urna. Viverra vitae congue eu consequat. Aliquet enim tortor at auctor urna. Ornare massa eget egestas purus viverra accumsan in nisl nisi. Elit pellentesque habitant morbi tristique senectus et netus et malesuada.\nSuspendisse ultrices gravida dictum fusce ut placerat orci nulla pellentesque. Lobortis feugiat vivamus at augue eget arcu. Vitae justo eget magna fermentum iaculis eu. Gravida rutrum quisque non tellus orci. Ipsum faucibus vitae aliquet nec. Nullam non nisi est sit amet. Nunc consequat interdum varius sit amet mattis vulputate enim. Sem fringilla ut morbi tincidunt augue interdum. Vitae purus faucibus ornare suspendisse. Massa tincidunt nunc pulvinar sapien et. Fringilla ut morbi tincidunt augue interdum velit euismod in. Donec massa sapien faucibus et. Est placerat in egestas erat imperdiet. Gravida rutrum quisque non tellus. Morbi non arcu risus quis varius quam quisque id diam. Habitant morbi tristique senectus et netus et malesuada fames ac. Eget lorem dolor sed viverra.\nOrnare massa eget egestas purus viverra. Varius vel pharetra vel turpis nunc eget lorem. Consectetur purus ut faucibus pulvinar elementum. Placerat in egestas erat imperdiet sed euismod nisi. Interdum velit euismod in pellentesque massa placerat duis ultricies lacus. Aliquam nulla facilisi cras fermentum odio eu. Est pellentesque elit ullamcorper dignissim cras tincidunt. Nunc sed id semper risus in hendrerit gravida rutrum. A pellentesque sit amet porttitor eget dolor morbi. Pellentesque habitant morbi tristique senectus et netus et malesuada fames. Nisl nunc mi ipsum faucibus vitae aliquet nec ullamcorper. Sed id semper risus in hendrerit gravida. Tincidunt praesent semper feugiat nibh. Aliquet lectus proin nibh nisl condimentum id venenatis a. Enim sit amet venenatis urna cursus eget. In egestas erat imperdiet sed euismod nisi porta lorem mollis. Lacinia quis vel eros donec ac odio tempor orci. Donec pretium vulputate sapien nec sagittis aliquam malesuada bibendum arcu. Erat pellentesque adipiscing commodo elit at.\nEgestas sed sed risus pretium quam vulputate. Vitae congue mauris rhoncus aenean vel elit scelerisque mauris pellentesque. Aliquam malesuada bibendum arcu vitae elementum. Congue mauris rhoncus aenean vel elit scelerisque mauris. Pellentesque dignissim enim sit amet venenatis urna cursus. Et malesuada fames ac turpis egestas sed tempus urna. Vel fringilla est ullamcorper eget nulla facilisi etiam dignissim. Nibh cras pulvinar mattis nunc sed blandit libero. Fringilla est ullamcorper eget nulla facilisi etiam dignissim. Aenean euismod elementum nisi quis eleifend quam adipiscing vitae proin. Mauris pharetra et ultrices neque ornare aenean euismod elementum. Ornare quam viverra orci sagittis eu. Odio ut sem nulla pharetra diam sit amet nisl suscipit. Ornare lectus sit amet est. Ullamcorper sit amet risus nullam eget. Tincidunt lobortis feugiat vivamus at augue eget arcu dictum.\nUrna nec tincidunt praesent semper feugiat nibh. Ut venenatis tellus in metus vulputate eu scelerisque felis. Cursus risus at ultrices mi tempus. In pellentesque massa placerat duis ultricies lacus sed turpis. Platea dictumst quisque sagittis purus. Cras adipiscing enim eu turpis egestas. Egestas sed tempus urna et pharetra pharetra. Netus et malesuada fames ac turpis egestas integer eget aliquet. Ac turpis egestas sed tempus. Sed lectus vestibulum mattis ullamcorper velit sed. Ante metus dictum at tempor commodo ullamcorper a. Augue neque gravida in fermentum et sollicitudin ac. Praesent semper feugiat nibh sed pulvinar proin gravida. Metus aliquam eleifend mi in nulla posuere sollicitudin aliquam ultrices. Neque gravida in fermentum et sollicitudin ac orci phasellus egestas.\nRidiculus mus mauris vitae ultricies. Morbi quis commodo odio aenean. Duis ultricies lacus sed turpis. Non pulvinar neque laoreet suspendisse interdum consectetur. Scelerisque eleifend donec pretium vulputate sapien nec sagittis aliquam. Volutpat est velit egestas dui id ornare arcu odio ut. Viverra tellus in hac habitasse platea dictumst vestibulum rhoncus est. Vestibulum lectus mauris ultrices eros. Sed blandit libero volutpat sed cras ornare. Id leo in vitae turpis massa sed elementum tempus. Gravida dictum fusce ut placerat orci nulla pellentesque. Pretium quam vulputate dignissim suspendisse in. Nisl suscipit adipiscing bibendum est ultricies integer quis auctor. Risus viverra adipiscing at in tellus. Turpis nunc eget lorem dolor sed viverra ipsum. Senectus et netus et malesuada fames ac. Habitasse platea dictumst vestibulum rhoncus est. Nunc sed id semper risus in hendrerit gravida. Felis eget velit aliquet sagittis id. Eget felis eget nunc lobortis.\nMaecenas pharetra convallis posuere morbi leo. Maecenas volutpat blandit aliquam etiam. A condimentum vitae sapien pellentesque habitant morbi tristique senectus et. Pulvinar mattis nunc sed blandit libero volutpat sed. Feugiat in ante metus dictum at tempor commodo ullamcorper. Vel pharetra vel turpis nunc eget lorem dolor. Est placerat in egestas erat imperdiet sed euismod. Quisque non tellus orci ac auctor augue mauris augue. Placerat vestibulum lectus mauris ultrices eros in cursus turpis. Enim nunc faucibus a pellentesque sit. Adipiscing vitae proin sagittis nisl. Iaculis at erat pellentesque adipiscing commodo elit at imperdiet. Aliquam sem fringilla ut morbi.\nArcu odio ut sem nulla pharetra diam sit amet nisl. Non diam phasellus vestibulum lorem sed. At erat pellentesque adipiscing commodo elit at. Lacus luctus accumsan tortor posuere ac ut consequat. Et malesuada fames ac turpis egestas integer. Tristique magna sit amet purus. A condimentum vitae sapien pellentesque habitant. Quis varius quam quisque id diam vel quam. Est ullamcorper eget nulla facilisi etiam dignissim diam quis. Augue interdum velit euismod in pellentesque massa. Elit scelerisque mauris pellentesque pulvinar pellentesque habitant. Vulputate eu scelerisque felis imperdiet. Nibh tellus molestie nunc non blandit massa. Velit euismod in pellentesque massa placerat. Sed cras ornare arcu dui. Ut sem viverra aliquet eget sit. Eu lobortis elementum nibh tellus molestie nunc non. Blandit libero volutpat sed cras ornare arcu dui vivamus.\nSit amet aliquam id diam maecenas. Amet risus nullam eget felis eget nunc lobortis mattis aliquam. Magna sit amet purus gravida. Egestas purus viverra accumsan in nisl nisi. Leo duis ut diam quam. Ante metus dictum at tempor commodo ullamcorper. Ac turpis egestas integer eget. Fames ac turpis egestas integer eget aliquet nibh. Sem integer vitae justo eget magna fermentum. Semper auctor neque vitae tempus quam pellentesque nec nam aliquam. Vestibulum mattis ullamcorper velit sed. Consectetur adipiscing elit duis tristique sollicitudin nibh. Massa id neque aliquam vestibulum morbi blandit cursus risus.\nCursus sit amet dictum sit amet justo donec enim diam. Egestas erat imperdiet sed euismod. Nullam vehicula ipsum a arcu cursus vitae congue mauris. Habitasse platea dictumst vestibulum rhoncus est pellentesque elit. Duis ultricies lacus sed turpis tincidunt id aliquet risus feugiat. Faucibus ornare suspendisse sed nisi lacus sed viverra. Pretium fusce id velit ut tortor pretium viverra. Fermentum odio eu feugiat pretium nibh ipsum consequat nisl vel. Senectus et netus et malesuada. Tellus pellentesque eu tincidunt tortor aliquam. Aenean sed adipiscing diam donec adipiscing tristique risus nec feugiat. Quis vel eros donec ac odio. Id interdum velit laoreet id donec ultrices tincidunt.\nMassa id neque aliquam vestibulum morbi blandit cursus risus at. Enim tortor at auctor urna nunc id cursus metus. Lorem ipsum dolor sit amet consectetur. At quis risus sed vulputate odio. Facilisis mauris sit amet massa vitae tortor condimentum lacinia quis. Et malesuada fames ac turpis egestas maecenas. Bibendum arcu vitae elementum curabitur vitae nunc sed velit dignissim. Viverra orci sagittis eu volutpat odio facilisis mauris. Adipiscing bibendum est ultricies integer quis auctor elit sed. Neque viverra justo nec ultrices dui sapien. Elementum nibh tellus molestie nunc non blandit massa enim. Euismod elementum nisi quis eleifend quam adipiscing vitae proin sagittis. Faucibus ornare suspendisse sed nisi. Quis viverra nibh cras pulvinar mattis nunc sed blandit. Tristique senectus et netus et. Magnis dis parturient montes nascetur ridiculus mus.\nDolor magna eget est lorem ipsum dolor. Nibh sit amet commodo nulla. Donec pretium vulputate sapien nec sagittis aliquam malesuada. Cras adipiscing enim eu turpis egestas pretium. Cras ornare arcu dui vivamus arcu felis bibendum ut tristique. Mus mauris vitae ultricies leo integer. In nulla posuere sollicitudin aliquam ultrices sagittis orci. Quis hendrerit dolor magna eget. Nisl tincidunt eget nullam non. Vitae congue eu consequat ac felis donec et odio. Vivamus at augue eget arcu dictum varius duis at. Ornare quam viverra orci sagittis.\nErat nam at lectus urna duis convallis. Massa placerat duis ultricies lacus sed turpis tincidunt id aliquet. Est ullamcorper eget nulla facilisi etiam dignissim diam. Arcu vitae elementum curabitur vitae nunc sed velit dignissim sodales. Tortor vitae purus faucibus ornare suspendisse sed nisi lacus. Neque viverra justo nec ultrices dui sapien eget mi proin. Viverra accumsan in nisl nisi scelerisque eu ultrices. Consequat interdum varius sit amet mattis. In aliquam sem fringilla ut morbi. Eget arcu dictum varius duis at. Nulla aliquet porttitor lacus luctus accumsan tortor posuere. Arcu bibendum at varius vel pharetra vel turpis. Hac habitasse platea dictumst quisque sagittis purus sit amet. Sapien eget mi proin sed libero enim sed. Quam elementum pulvinar etiam non quam lacus suspendisse faucibus interdum. Semper viverra nam libero justo. Fusce ut placerat orci nulla pellentesque dignissim enim sit amet. Et malesuada fames ac turpis egestas maecenas pharetra convallis posuere.\nTurpis egestas sed tempus urna et pharetra pharetra massa. Gravida in fermentum et sollicitudin ac orci phasellus. Ornare suspendisse sed nisi lacus sed viverra tellus in. Fames ac turpis egestas maecenas pharetra convallis posuere. Mi proin sed libero enim sed faucibus turpis. Sit amet mauris commodo quis imperdiet massa tincidunt nunc. Ut etiam sit amet nisl purus in mollis nunc. Habitasse platea dictumst quisque sagittis purus sit amet volutpat consequat. Eget aliquet nibh praesent tristique magna. Sit amet est placerat in egestas erat. Commodo sed egestas egestas fringilla. Enim nulla aliquet porttitor lacus luctus accumsan tortor posuere ac. Et molestie ac feugiat sed lectus vestibulum mattis ullamcorper. Dignissim convallis aenean et tortor at risus viverra. Morbi blandit cursus risus at ultrices mi. Ac turpis egestas integer eget aliquet nibh praesent tristique magna.\nVolutpat sed cras ornare arcu dui. Egestas erat imperdiet sed euismod nisi porta lorem mollis aliquam. Viverra justo nec ultrices dui sapien. Amet risus nullam eget felis eget nunc lobortis. Metus aliquam eleifend mi in. Ut eu sem integer vitae. Auctor elit sed vulputate mi sit amet. Nisl nisi scelerisque eu ultrices. Dictum fusce ut placerat orci nulla. Pellentesque habitant morbi tristique senectus et. Auctor elit sed vulputate mi sit. Tincidunt arcu non sodales neque. Mi in nulla posuere sollicitudin aliquam. Morbi non arcu risus quis varius quam quisque id diam. Cras adipiscing enim eu turpis egestas pretium aenean pharetra magna. At auctor urna nunc id cursus metus aliquam. Mauris a diam maecenas sed enim ut sem viverra. Nunc scelerisque viverra mauris in. In iaculis nunc sed augue lacus viverra vitae congue eu. Volutpat blandit aliquam etiam erat velit scelerisque in dictum non."),
                    CLAY_TEXT_CONFIG({.fontSize = 24, .textColor = {0, 0, 0, 255}}));
            }

            CLAY_AUTO_ID({.layout = {.sizing = {.width = CLAY_SIZING_GROW(0)}, .padding = {8, 8, 8, 8}, .childGap = 8}, .backgroundColor = {180, 180, 180, 255}})
            {
                char drawCallsText[200];
                int drawCallsTextLen = snprintf(drawCallsText, sizeof(drawCallsText),
                                                "Last frame got: %llu draw calls\n", g_drawCallsDuringLastFrame);
                if (drawCallsTextLen < 0)
                    drawCallsTextLen = 0;
                else if ((size_t)drawCallsTextLen >= sizeof(drawCallsText))
                    drawCallsTextLen = (int)sizeof(drawCallsText) - 1;
                Clay_String cs = {.isStaticallyAllocated = false, .length = (size_t)drawCallsTextLen, .chars = drawCallsText};
                Clay_TextElementConfig drawCallsElementConfig = {.fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = {255, 255, 255, 255}};
                CLAY_TEXT(cs, &drawCallsElementConfig);
            }
            CLAY_AUTO_ID({.layout = {.sizing = {.width = CLAY_SIZING_GROW(0)}, .padding = {8, 8, 8, 8}, .childGap = 8}, .backgroundColor = {180, 180, 180, 255}})
            {
                Clay_String cs = {.isStaticallyAllocated = false, .length = g_fpsTextLen, .chars = g_fpsText};
                Clay_TextElementConfig fpsElementConfig = {.fontId = FONT_ID_BODY_24, .fontSize = 24, .textColor = {255, 255, 255, 255}};
                CLAY_TEXT(cs, &fpsElementConfig);
            }
        }

        CLAY(
            CLAY_ID("Blob4Floating2"),
            {
                .floating = {.attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID, .zIndex = 1, .parentId = Clay_GetElementId(CLAY_STRING("SidebarBlob4")).id},
                .backgroundColor = {40, 80, 200, 200},
                .border = {.width = {0, 10, 0, 10}, .color = {0, 0, 0, 80}},
                .cornerRadius = CLAY_CORNER_RADIUS(18),
                .layout = {
                    .padding = {10, 10, 10, 10},
                },
            })
        {
            CLAY(CLAY_ID("ScrollContainer"), {.layout = {.sizing = {.height = CLAY_SIZING_FIXED(200)}, .childGap = 2}, .clip = {.vertical = true, .childOffset = Clay_GetScrollOffset()}})
            {
                CLAY(CLAY_ID("FloatingContainer2"), {.layout.sizing.height = CLAY_SIZING_GROW(), .floating = {.attachTo = CLAY_ATTACH_TO_PARENT, .zIndex = 1}})
                {
                    CLAY(CLAY_ID("FloatingContainerInner"),
                         {
                             .layout = {.sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW()}, .padding = {16, 16, 16, 16}},
                             .backgroundColor = {140, 80, 200, 200},
                             .border = {.width = CLAY_BORDER_OUTSIDE(4), .color = {80, 80, 80, 255}},
                             .cornerRadius = {30, 3, 3, 30},
                         })
                    {
                        CLAY_TEXT(CLAY_STRING("I'm an inline floating container."), CLAY_TEXT_CONFIG({.fontSize = 24, .textColor = {255, 255, 0, 255}}));
                    }
                }
                CLAY(CLAY_ID("ScrollContainerInner"), {.layout = {.layoutDirection = CLAY_TOP_TO_BOTTOM}, .backgroundColor = {160, 160, 160, 255}})
                {
                    for (int i = 0; i < 100; i++)
                    {
                        RenderDropdownTextItem(i);
                    }
                }
            }
        }
        Clay_ScrollContainerData scrollData = Clay_GetScrollContainerData(Clay_GetElementId(CLAY_STRING("MainContent")));
        if (scrollData.found)
        {
            CLAY(CLAY_ID("ScrollBar"),
                 {.floating = {
                      .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID,
                      .offset = {.y = -(scrollData.scrollPosition->y / scrollData.contentDimensions.height) * scrollData.scrollContainerDimensions.height},
                      .zIndex = 1,
                      .parentId = Clay_GetElementId(CLAY_STRING("MainContent")).id,
                      .attachPoints = {.element = CLAY_ATTACH_POINT_RIGHT_TOP, .parent = CLAY_ATTACH_POINT_RIGHT_TOP}}})
            {
                CLAY(
                    CLAY_ID("ScrollBarButton"),
                    {.layout = {
                         .sizing = {CLAY_SIZING_FIXED(12), CLAY_SIZING_FIXED((scrollData.scrollContainerDimensions.height / scrollData.contentDimensions.height) * scrollData.scrollContainerDimensions.height)}},
                     .backgroundColor = Clay_PointerOver(Clay_GetElementId(CLAY_STRING("ScrollBar"))) ? (Clay_Color){100, 100, 140, 150} : (Clay_Color){120, 120, 160, 150},
                     .cornerRadius = CLAY_CORNER_RADIUS(6)}) {}
            }
        }
    }
    return Clay_EndLayout();
}

void loop()
{
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();
    deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

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

    Clay_RenderCommandArray cmds = CreateLayout();
    uint64_t drawCalls1 = g_gles3.totalDrawCallsToOpenGl;
    Gles3_Render(&g_gles3, cmds, g_stbFonts);
    uint64_t drawCalls2 = g_gles3.totalDrawCallsToOpenGl;
    g_drawCallsDuringLastFrame = drawCalls2 - drawCalls1;

    // update FPS counter
    Uint64 NOW2 = SDL_GetPerformanceCounter();

    /* FPS based on simulation delta */
    g_timeAccumulator += deltaTime;
    g_frameCount++;

    /* wall-clock frame time */
    double frameSeconds =
        (double)(NOW2 - NOW) /
        (double)SDL_GetPerformanceFrequency();

    g_wallTimeAccumulator += frameSeconds;

    /* update text ONLY every 5 seconds */
    double measureInterval = 3000.0;
    double measuresPerSecond = 1000.0 / measureInterval;
    if (g_timeAccumulator >= measureInterval)
    {
        g_fps = (g_frameCount / g_timeAccumulator) * 1000.0;

        g_avgFrameMs = (g_wallTimeAccumulator / g_frameCount) * 1000.0;

        g_fpsTextLen = (size_t)snprintf(
            (char *)g_fpsText,
            sizeof(g_fpsText),
            "FPS: %.3f | Avg frame: %.3f ms",
            g_fps,
            g_avgFrameMs);

        g_timeAccumulator = 0.0;
        g_wallTimeAccumulator = 0.0;
        g_frameCount = 0;
    }

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
