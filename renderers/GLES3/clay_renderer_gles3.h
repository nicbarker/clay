#ifndef CLAY_RENDERER_GLES3_H
#define CLAY_RENDERER_GLES3_H

// There may be custom header customizations, very client specific
// let client indicate that they manage headers by setting GLSL_VERSION
#ifndef GLSL_VERSION
#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#define GLSL_VERSION "#version 300 es"
#else
// Only apple computers now sorry
// That means it is not really GLES3 but desktop OpenGL 3
// Luckily, it is compatible with GLES3
#include <OpenGL/gl3.h>
#define GLSL_VERSION "#version 330 core"
#endif
#endif

#define MAX_IMAGES 4
#define MAX_FONTS 4

/*
 * Instanced rendering for Rects/Images/Borders
 * will use this data
 * Note, it needs to be padded to 4 floats
 * Draws:
 * - One rectangular with possibly rounded corner
 * - And possibly with a hole inside (with rounded edges too, if corners are rounded)
 * - It could also draw a picture with alsoe rounded corner
 */
typedef struct RectInstance
{
    float x, y, w, h;         // 4 Draw where on screen
    float u0, v0, u1, v1;     // 4 Atlas region
    float r, g, b, a;         // 4 Color
    float radiusTL, radiusTR; // 2 Corner rounding
    float radiusBL, radiusBR; // 2
    float borderL, borderR;   // 2 Border widths
    float borderT, borderB;   // 2
    float texToUse;           // 1 Texture atlas to take an image from (1-4)
    float pad[3];             // 3
} RectInstance;

/*
 * Struct for glyph instanced rendering
 * Each glyph consists of 6 vertexes (to make 2 triangle of a quad)
 */
typedef struct GlyphVtx
{
    float x, y;         // To draw Where
    float u, v;         // To draw What
    float r, g, b, a;   // Text color
    float atlasTexUnit; // Shader will have all samples loaded but this will point which to use
    float pad[3];       // 3
} GlyphVtx;

typedef struct Gles3_GlyphVtxArray
{
    GlyphVtx *instData;
    int capacity;
    int count;
} Gles3_GlyphVtxArray;

typedef struct Gles3_QuadInstanceArray
{
    RectInstance *instData; // packed per-instance floats
    int capacity;           // how many instances it can hold
    int count;              // how many instances does it actually hold
} Gles3_QuadInstanceArray;

typedef struct Gles3_ImageConfig
{
    int textureToUse;
    float u0, v0;
    float u1, v1;
} Gles3_ImageConfig;

#ifndef CLAY_RENDERER_GLES3_IMPLEMENTATION
typedef struct Gles3_Renderer Gles3_Renderer;
#endif

#ifdef CLAY_RENDERER_GLES3_IMPLEMENTATION

#include <math.h>
#include <clay.h>
#include <stdlib.h>

#include "clay_renderer_gles3.h"

enum
{
    ATTR_QUAD_POS = 0,
    ATTR_QUAD_RECT = 1,
    ATTR_QUAD_COLOR = 2,
    ATTR_QUAD_UV = 3,
    ATTR_QUAD_RAD = 4,
    ATTR_QUAD_BORDER = 5,
    ATTR_QUAD_TEX = 6,
};

enum
{
    ATTR_GLYPH_POS = 0,
    ATTR_GLYPH_UV = 1,
    ATTR_GLYPH_COLOR = 2,
    ATTR_GLYPH_TEX = 3,
};

/*
 * rendering
 */

const char *GLES3_QUAD_VERTEX_SHADER =
    GLSL_VERSION
    "\n"
    "precision mediump float;\n"
    "layout(location = 0) in vec2 aPos;        // unit quad (0..1)\n"
    "layout(location = 1) in vec4 aRect;       // x,y,w,h (pixels)\n"
    "layout(location = 3) in vec4 aUV;         // u0,v0,u1,v1\n"
    "layout(location = 2) in vec4 aColor;      // rgba\n"
    "layout(location = 4) in vec4 aCornerRadii;\n"
    "layout(location = 5) in vec4 aBorderWidths;\n"
    "layout(location = 6) in float aTexSlot;\n"
    "uniform vec2 uScreen; // screen size in pixels\n"
    "out vec2 vPos;\n"
    "out vec4 vRect;\n"
    "out vec4 vColor;\n"
    "out vec2 vUV;\n"
    "out vec4 vCornerRadii;\n"
    "out vec4 vBorderWidths;\n"
    "out float vTexSlot;\n"
    "void main() {\n"
    "    vec2 pos = vec2(aPos.x * aRect.z + aRect.x, aPos.y * aRect.w + aRect.y);\n"
    "    vec2 ndc = pos / uScreen * 2.0 - 1.0; // ndc.y increases up; pos y increases down (we will inve\n"
    "    ndc.y = -ndc.y;\n"
    "    gl_Position = vec4(ndc, 0.0, 1.0);\n"
    "    vPos = aPos;\n"
    "    vRect = aRect;\n"
    "    vColor = aColor;\n"
    "    vUV = mix(aUV.xy, aUV.zw, aPos);\n"
    "    vCornerRadii = aCornerRadii;\n"
    "    vBorderWidths = aBorderWidths;\n"
    "    vTexSlot = aTexSlot;\n"
    "}\n";

const char *GLES3_QUAD_FRAGMENT_SHADER =
    GLSL_VERSION
    "\n"
    "precision mediump float;\n"
    "in vec2 vPos;\n"
    "in vec4 vRect;\n"
    "in vec4 vColor;\n"
    "in vec2 vUV;\n"
    "in vec4 vCornerRadii;\n"
    "in vec4 vBorderWidths;\n"
    "in float vTexSlot;\n"
    "uniform sampler2D uTex0;\n"
    "uniform sampler2D uTex1;\n"
    "uniform sampler2D uTex2;\n"
    "uniform sampler2D uTex3;\n"
    "out vec4 frag;\n"
    "void main() {\n"
    "    // Pixel coordinates in pixel space\n"
    "    vec2 pix = vRect.xy + vPos * vRect.zw;\n"
    "    float x0 = vRect.x;\n"
    "    float y0 = vRect.y;\n"
    "    float w  = vRect.z;\n"
    "    float h  = vRect.w;\n"
    "    // Local position inside the rectangle (0..w, 0..h)\n"
    "    vec2 local = pix - vec2(x0, y0);\n"
    "    // Original corner radii\n"
    "    float tl = vCornerRadii.x;\n"
    "    float tr = vCornerRadii.y;\n"
    "    float bl = vCornerRadii.z;\n"
    "    float br = vCornerRadii.w;\n"
    "    // Border thicknesses\n"
    "    float L = vBorderWidths.x;\n"
    "    float R = vBorderWidths.y;\n"
    "    float T = vBorderWidths.z;\n"
    "    float B = vBorderWidths.w;\n"
    "    bool CLAY_BORDERS_ARE_INSET = true; // it is true\n"
    "    bool isBorder = (L > 0.0 || R > 0.0 || T > 0.0 || B > 0.0);\n"
    "    float outerAlpha = 1.0;\n"
    "    // If it is not a border but rect or image, then it only has outer border what is provided\n"
    "    // Otherwise it increases the outter border, but the provided borde is the ineer border\n"
    "    // I think is better not increase rounding radius when that radius is smaller than border thickness\n"
    "    float outter_tl;\n"
    "    float outter_tr;\n"
    "    float outter_bl;\n"
    "    float outter_br;\n"
    "    if (CLAY_BORDERS_ARE_INSET) {\n"
    "        // Actural behaviour\n"
    "        outter_tl = tl;\n"
    "        outter_tr = tr;\n"
    "        outter_bl = bl;\n"
    "        outter_br = br;\n"
    "        tl = (tl > min(T, L)) ? tl - min(T, L) : tl;\n"
    "        tr = (tr > min(T, R)) ? tr - min(T, R) : tr;\n"
    "        bl = (bl > min(B, L)) ? bl - min(B, L) : bl;\n"
    "        br = (br > min(B, R)) ? br - min(B, R) : br;\n"
    "    } else {\n"
    "        // Hypothetical behaviour\n"
    "        outter_tl = (tl > min(T, L)) ? tl + min(T, L) : tl;\n"
    "        outter_tr = (tr > min(T, R)) ? tr + min(T, R) : tr;\n"
    "        outter_bl = (bl > min(B, L)) ? bl + min(B, L) : bl;\n"
    "        outter_br = (br > min(B, R)) ? br + min(B, R) : br;\n"
    "    }\n"
    "    if (outter_tl > 0.0 && local.x < outter_tl && local.y < outter_tl)\n"
    "        outerAlpha = step(length(local - vec2(outter_tl, outter_tl)), outter_tl);\n"
    "    if (outter_tr > 0.0 && local.x > w - outter_tr && local.y < outter_tr)\n"
    "        outerAlpha *= step(length(local - vec2(w - outter_tr, outter_tr)), outter_tr);\n"
    "    if (outter_bl > 0.0 && local.x < outter_bl && local.y > h - outter_bl)\n"
    "        outerAlpha *= step(length(local - vec2(outter_bl, h - outter_bl)), outter_bl);\n"
    "    if (outter_br > 0.0 && local.x > w - outter_br && local.y > h - outter_br)\n"
    "        outerAlpha *= step(length(local - vec2(w - outter_br, h - outter_br)), outter_br);\n"
    "    if (outerAlpha < 0.5)\n"
    "        discard;\n"
    "    // -------- Border logic --------\n"
    "    if (isBorder) {\n"
    "        float iw = w - L - R;\n"
    "        float ih = h - T - B;\n"
    "        vec2 innerLocal = local - vec2(L, T);\n"
    "        // Check if pixel is inside inner rounded rect\n"
    "        bool insideInner = true;\n"
    "        if (tl > 0.0 && innerLocal.x < tl && innerLocal.y < tl)\n"
    "            insideInner = (length(innerLocal - vec2(tl, tl)) <= tl);\n"
    "        if (tr > 0.0 && innerLocal.x > iw - tr && innerLocal.y < tr)\n"
    "            insideInner = insideInner && (length(innerLocal - vec2(iw - tr, tr)) <= tr);\n"
    "        // Bottom-left\n"
    "        if (bl> 0.0 && innerLocal.x < bl && innerLocal.y > ih - bl) \n"
    "            insideInner = insideInner && (length(innerLocal - vec2(bl, ih - bl)) <= bl);\n"
    "        // Bottom-right\n"
    "        if (br > 0.0 && innerLocal.x > iw - br && innerLocal.y > ih - br)\n"
    "            insideInner = insideInner && (length(innerLocal - vec2(iw - br, ih - br)) <= br);\n"
    "        // Discard pixels inside inner rounded rect\n"
    "        if (insideInner && innerLocal.x >= 0.0 && innerLocal.x <= iw && innerLocal.y >= 0.0 && innerLocal.y <= ih)\n"
    "            discard;\n"
    "        frag = vColor;\n"
    "        return;\n"
    "    }\n"
    "    // -------- Non-border rectangle or image --------\n"
    "    if (vTexSlot < 0.0) {\n"
    "        frag = vColor;\n"
    "    } else {\n"
    "        int slot = int(vTexSlot + 0.5);\n"
    "        if (slot == 0) frag = texture(uTex0, vUV);\n"
    "        if (slot == 1) frag = texture(uTex1, vUV);\n"
    "        if (slot == 2) frag = texture(uTex2, vUV);\n"
    "        if (slot == 3) frag = texture(uTex3, vUV);\n"
    "    }\n"
    "}\n";

const char *GLES3_TEXT_VERTEX_SHADER =
    GLSL_VERSION
    "\n"
    "precision mediump float;\n"
    "layout(location = 0) in vec2 aPos;\n"
    "layout(location = 1) in vec2 aUV;\n"
    "layout(location = 2) in vec4 aColor;\n"
    "layout(location = 3) in float aTexSlot;\n"
    "uniform vec2 uScreen;\n"
    "out vec2 vUV;\n"
    "out vec4 vColor;\n"
    "out float vTexSlot;\n"
    "void main() {\n"
    "    vec2 ndc = (aPos / uScreen) * 2.0 - 1.0;\n"
    "    gl_Position = vec4(ndc * vec2(1.0, -1.0), 0.0, 1.0);\n"
    "    vUV = aUV;\n"
    "    vColor = aColor;\n"
    "    vTexSlot = aTexSlot;\n"
    "}\n";

const char *GLES3_TEXT_FRAGMENT_SHADER =
    GLSL_VERSION
    "\n"
    "precision mediump float;\n"
    "in vec2 vUV;\n"
    "in vec4 vColor;\n"
    "in float vTexSlot;\n"
    "uniform sampler2D uTex0;\n"
    "uniform sampler2D uTex1;\n"
    "uniform sampler2D uTex2;\n"
    "uniform sampler2D uTex3;\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "    int slot = int(vTexSlot + 0.5);\n"
    "    float coverage;\n"
    "    if (slot == 0) coverage = texture(uTex0, vUV).r;\n"
    "    if (slot == 1) coverage = texture(uTex1, vUV).r;\n"
    "    if (slot == 2) coverage = texture(uTex2, vUV).r;\n"
    "    if (slot == 3) coverage = texture(uTex3, vUV).r;\n"
    "    fragColor = vec4(vColor.rgb, vColor.a * coverage);\n"
    "} \n";

/**
 * This renderer accumulates all quads and glyphs of every draw coommand
 * in their array, and flushes them in just 2 instanced draw calls to OpenGL
 */
typedef struct Gles3_Renderer
{
    Clay_Arena clayMemory;

    // It is super important keep track on the performance of this renderer:
    uint64_t totalDrawCallsToOpenGl;

    float screenWidth;
    float screenHeight;

    /* Quads rendering */
    GLuint quadVAO;
    GLuint quadVBO;
    GLuint quadInstanceVBO;
    GLuint quadShaderId;
    GLuint imageTextures[MAX_IMAGES];
    Gles3_QuadInstanceArray quadInstanceArray; // Each instance is one quad

    /* Fonts rendering */
    GLuint textVAO;
    GLuint textVBO;
    GLuint textShader;
    GLuint fontTextures[MAX_FONTS];
    Gles3_GlyphVtxArray glyphVtxArray; // Instance data: every vertex is an element,
                                       // 6 elements per each instance

    // Text renderer is delegated to external function, which is supposed
    // to add glyph data based on passed render text command
    void (*renderTextFunction)(
        Clay_RenderCommand *cmd,    // Will be always of CLAY_RENDER_COMMAND_TYPE_TEXT
        Gles3_GlyphVtxArray *accum, // 6 vertices need to be added to this array
        void *userData              // Fonts pallete
    );
} Gles3_Renderer;

static GLuint Gles3__CompileShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        printf("ERROR::SHADER::COMPILATION_FAILED\n");
        printf("SHADER SOURCE:\n%s\n", source);
        printf("SHADER TYPE: ");
        if (type == GL_VERTEX_SHADER)
            printf("Vertex Shader");
        else if (type == GL_FRAGMENT_SHADER)
            printf("Fragment Shader");
        else
            printf("Unknown");
        printf("\nSHADER COMPILATION ERROR:\n%s\n", infoLog);
        abort();
    }
    return shader;
}

GLuint Gles3__CreateShaderProgram(
    const char *vertexShaderSource,
    const char *fragmentShaderSource)
{
    GLuint vertexShader =
        Gles3__CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader =
        Gles3__CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void Gles3_Initialize(Gles3_Renderer *renderer, int maxInstances)
{
    renderer->totalDrawCallsToOpenGl = 0;
    // compile shader
    renderer->quadShaderId = Gles3__CreateShaderProgram(
        GLES3_QUAD_VERTEX_SHADER, GLES3_QUAD_FRAGMENT_SHADER);

    glUseProgram(renderer->quadShaderId);
    glUniform1i(glGetUniformLocation(renderer->quadShaderId, "uTex0"), 0);
    glUniform1i(glGetUniformLocation(renderer->quadShaderId, "uTex1"), 1);
    glUniform1i(glGetUniformLocation(renderer->quadShaderId, "uTex2"), 2);
    glUniform1i(glGetUniformLocation(renderer->quadShaderId, "uTex3"), 3);

    // create unit quad VBO (0..1)
    const float quadVerts[8] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};
    glGenVertexArrays(1, &renderer->quadVAO);
    glBindVertexArray(renderer->quadVAO);

    glGenBuffers(1, &renderer->quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    // attribute 0: aPos (vec2), per-vertex
    glEnableVertexAttribArray(ATTR_QUAD_POS);
    glVertexAttribPointer(ATTR_QUAD_POS, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glVertexAttribDivisor(ATTR_QUAD_POS, 0);

    // create instance buffer big enough
    Gles3_QuadInstanceArray *quads = &renderer->quadInstanceArray;
    quads->capacity = maxInstances;
    quads->instData =
        (RectInstance *)malloc(sizeof(RectInstance) * quads->capacity);
    quads->count = 0;

    glGenBuffers(1, &renderer->quadInstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->quadInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(RectInstance) * quads->capacity,
                 NULL,
                 GL_DYNAMIC_DRAW);

    // set up instance attributes
    GLsizei stride = sizeof(RectInstance);

    glEnableVertexAttribArray(ATTR_QUAD_RECT);
    glVertexAttribPointer(ATTR_QUAD_RECT, 4, GL_FLOAT, GL_FALSE,
                          stride, (void *)offsetof(RectInstance, x));
    glVertexAttribDivisor(ATTR_QUAD_RECT, 1);

    glEnableVertexAttribArray(ATTR_QUAD_COLOR);
    glVertexAttribPointer(ATTR_QUAD_COLOR, 4, GL_FLOAT, GL_FALSE,
                          stride, (void *)offsetof(RectInstance, r));
    glVertexAttribDivisor(ATTR_QUAD_COLOR, 1);

    glEnableVertexAttribArray(ATTR_QUAD_UV);
    glVertexAttribPointer(ATTR_QUAD_UV, 4, GL_FLOAT, GL_FALSE,
                          stride, (void *)offsetof(RectInstance, u0));
    glVertexAttribDivisor(ATTR_QUAD_UV, 1);

    glEnableVertexAttribArray(ATTR_QUAD_RAD);
    glVertexAttribPointer(ATTR_QUAD_RAD, 4, GL_FLOAT, GL_FALSE,
                          stride, (void *)offsetof(RectInstance, radiusTL));
    glVertexAttribDivisor(ATTR_QUAD_RAD, 1);

    glEnableVertexAttribArray(ATTR_QUAD_BORDER);
    glVertexAttribPointer(ATTR_QUAD_BORDER, 4, GL_FLOAT, GL_FALSE,
                          stride, (void *)offsetof(RectInstance, borderL));
    glVertexAttribDivisor(ATTR_QUAD_BORDER, 1);

    glEnableVertexAttribArray(ATTR_QUAD_TEX);
    glVertexAttribPointer(ATTR_QUAD_TEX, 1, GL_FLOAT, GL_FALSE,
                          stride, (void *)offsetof(RectInstance, texToUse));
    glVertexAttribDivisor(ATTR_QUAD_TEX, 1);

    glBindVertexArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Ok now we will initialize text!
    Gles3_GlyphVtxArray *gVerts = &renderer->glyphVtxArray;

    // configure capacity
    gVerts->capacity = maxInstances;
    gVerts->count = 0;

    // allocate CPU-side vertex buffer: 6 vertices per glyph
    gVerts->instData = (GlyphVtx *)malloc(sizeof(GlyphVtx) * 6 * gVerts->capacity);
    if (!gVerts->instData)
    {
        fprintf(stderr, "Failed to allocate glyph_vertices\n");
        gVerts->capacity = 0;
    }

    // create VAO/VBO for text rendering
    glGenVertexArrays(1, &renderer->textVAO);
    glBindVertexArray(renderer->textVAO);

    glGenBuffers(1, &renderer->textVBO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->textVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GlyphVtx) * 6 * gVerts->capacity,
                 NULL,
                 GL_DYNAMIC_DRAW);

    GLsizei gv_stride = sizeof(GlyphVtx);

    glEnableVertexAttribArray(ATTR_GLYPH_POS);
    glVertexAttribPointer(ATTR_GLYPH_POS, 2, GL_FLOAT, GL_FALSE, gv_stride, (void *)(offsetof(GlyphVtx, x)));

    glEnableVertexAttribArray(ATTR_GLYPH_UV);
    glVertexAttribPointer(ATTR_GLYPH_UV, 2, GL_FLOAT, GL_FALSE, gv_stride, (void *)(offsetof(GlyphVtx, u)));

    glEnableVertexAttribArray(ATTR_GLYPH_COLOR);
    glVertexAttribPointer(ATTR_GLYPH_COLOR, 4, GL_FLOAT, GL_FALSE, gv_stride, (void *)(offsetof(GlyphVtx, r)));

    glEnableVertexAttribArray(ATTR_GLYPH_TEX);
    glVertexAttribPointer(ATTR_GLYPH_TEX, 1, GL_FLOAT, GL_FALSE, gv_stride, (void *)(offsetof(GlyphVtx, atlasTexUnit)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    renderer->textShader = Gles3__CreateShaderProgram(
        GLES3_TEXT_VERTEX_SHADER, GLES3_TEXT_FRAGMENT_SHADER);
    glUseProgram(renderer->textShader);

    // Link sampler uniforms in the text shader to the correct texture units.
    // Each uniform tells the shader which unit to read from.
    glUniform1i(glGetUniformLocation(renderer->textShader, "uTex0"), 0);
    glUniform1i(glGetUniformLocation(renderer->textShader, "uTex1"), 1);
    glUniform1i(glGetUniformLocation(renderer->textShader, "uTex2"), 2);
    glUniform1i(glGetUniformLocation(renderer->textShader, "uTex3"), 3);
}

void Gles3_SetRenderTextFunction(
    Gles3_Renderer *renderer,
    void (*renderTextFunction)(
        Clay_RenderCommand *cmd, Gles3_GlyphVtxArray *accum, void *userData),
    void *userData)
{
    renderer->renderTextFunction = renderTextFunction;
}

void Gles3_Render(
    Gles3_Renderer *renderer,
    Clay_RenderCommandArray cmds,
    void *userData // eg. fonts
)
{
    Clay_Dimensions layoutDimensions = Clay_GetCurrentContext()->layoutDimensions;
    renderer->screenWidth = layoutDimensions.width;
    renderer->screenHeight = layoutDimensions.height;

    Gles3_QuadInstanceArray *quads = &renderer->quadInstanceArray;
    Gles3_GlyphVtxArray *gVerts = &renderer->glyphVtxArray;

    gVerts->count = 0;

    for (int i = 0; i < cmds.length; i++)
    {
        Clay_RenderCommand *cmd = Clay_RenderCommandArray_Get(&cmds, i);
        Clay_BoundingBox boundingBox = (Clay_BoundingBox){
            .x = roundf(cmd->boundingBox.x),
            .y = roundf(cmd->boundingBox.y),
            .width = roundf(cmd->boundingBox.width),
            .height = roundf(cmd->boundingBox.height),
        };

        bool scissorChanged = false;
        switch (cmd->commandType)
        {
        case CLAY_RENDER_COMMAND_TYPE_TEXT:
        {
            renderer->renderTextFunction(
                cmd,
                &renderer->glyphVtxArray,
                userData);
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
        case CLAY_RENDER_COMMAND_TYPE_IMAGE:
        {
            Clay_RectangleRenderData *config = &cmd->renderData.rectangle;
            Clay_Color c = config->backgroundColor;

            // Convert to float 0..1
            float rf = c.r / 255.0f;
            float gf = c.g / 255.0f;
            float bf = c.b / 255.0f;
            float af = c.a / 255.0f;

            bool isImage = cmd->commandType == CLAY_RENDER_COMMAND_TYPE_IMAGE;

            // Ensure we don't overflow the capacity
            if (quads->count >= quads->capacity)
            {
                printf("Clay renderer: instance overflow!\n");
                break;
            }

            int idx = quads->count;
            RectInstance *dst = &quads->instData[idx];
            dst->x = boundingBox.x;
            dst->y = boundingBox.y;
            dst->w = boundingBox.width;
            dst->h = boundingBox.height;

            if (isImage)
            {
                Gles3_ImageConfig *imgConf = (Gles3_ImageConfig *)cmd->renderData.image.imageData;
                dst->u0 = imgConf->u0;
                dst->v0 = imgConf->v0;
                dst->u1 = imgConf->u1;
                dst->v1 = imgConf->v1;
                dst->texToUse = (float)imgConf->textureToUse;
            }
            else
            {
                dst->u0 = dst->v0 = 0.0f;
                dst->u1 = dst->v1 = 1.0f;
                dst->texToUse = -1.0f; // This means no image, use albedo color
            }

            // colour
            dst->r = rf;
            dst->g = gf;
            dst->b = bf;
            dst->a = af;

            // corner radii
            Clay_CornerRadius r = config->cornerRadius;
            dst->radiusTL = r.topLeft;
            dst->radiusTR = r.topRight;
            dst->radiusBL = r.bottomLeft;
            dst->radiusBR = r.bottomRight;

            dst->borderT = 0.0f;
            dst->borderR = 0.0f;
            dst->borderB = 0.0f;
            dst->borderL = 0.0f;

            quads->count++;
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START:
        {
            scissorChanged = true;
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END:
        {
            scissorChanged = true;
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_BORDER:
        {
            Clay_BorderRenderData *br = &cmd->renderData.border;

            float rf = br->color.r / 255.0f;
            float gf = br->color.g / 255.0f;
            float bf = br->color.b / 255.0f;
            float af = br->color.a / 255.0f;

            float x = boundingBox.x;
            float y = boundingBox.y;
            float w = boundingBox.width;
            float h = boundingBox.height;

            float top = br->width.top;
            float bottom = br->width.bottom;
            float left = br->width.left;
            float right = br->width.right;

            int idx = quads->count;
            RectInstance *dst = &quads->instData[idx];

            dst->x = x - left;
            dst->y = y - top;
            dst->w = w + right;
            dst->h = h + bottom;

            dst->borderB = bottom;
            dst->borderL = left;
            dst->borderT = top;
            dst->borderR = right;

            // Clay borders are inset, but adding support to outset borders
            // Is as easy as this + some minor changes in shader too
            bool CLAY_BORDERS_ARE_INSET = true;
            if (CLAY_BORDERS_ARE_INSET)
            {
                // Normal behaviour
                dst->x = x;
                dst->y = y;
                dst->w = w;
                dst->h = h;
            }
            else
            {
                // Hypotethical behaviour, if the borders were outside
                dst->x = x - left;
                dst->y = y - top;
                dst->w = w + left + right;
                dst->h = h + top + bottom;
            }

            dst->u0 = 0.0f;
            dst->v0 = 0.0f;
            dst->u1 = 1.0f;
            dst->v1 = 1.0f;

            dst->r = rf;
            dst->g = gf;
            dst->b = bf;
            dst->a = af;

            dst->radiusTL = br->cornerRadius.topLeft;
            dst->radiusTR = br->cornerRadius.topRight;
            dst->radiusBR = br->cornerRadius.bottomRight;
            dst->radiusBL = br->cornerRadius.bottomLeft;

            dst->texToUse = -1.0f;

            quads->count++;
            break;
        }

        case CLAY_RENDER_COMMAND_TYPE_CUSTOM:
        {
            // printf("Unhandled clay cmd: custom\n");
            break;
        }
        default:
        {
            printf("Error: unhandled render command\n");
            exit(1);
        }
        }

        // Flush draw calls if scissors about to change in this iteration
        if (i == cmds.length - 1 || scissorChanged)
        {
            scissorChanged = false;
            // Render Recatangles and Images
            if (quads->count > 0)
            {
                glUseProgram(renderer->quadShaderId);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, renderer->imageTextures[0]);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, renderer->imageTextures[1]);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, renderer->imageTextures[2]);
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, renderer->imageTextures[3]);

                // set uniforms
                GLint locScreen = glGetUniformLocation(renderer->quadShaderId, "uScreen");
                glUniform2f(locScreen,
                            (float)renderer->screenWidth,
                            (float)renderer->screenHeight);

                glBindVertexArray(renderer->quadVAO);

                // upload all instances at once
                glBindBuffer(GL_ARRAY_BUFFER, renderer->quadInstanceVBO);

                // rectangles are solid colour — disable atlas use
                glBufferSubData(GL_ARRAY_BUFFER,
                                0,
                                quads->count * sizeof(RectInstance),
                                quads->instData);

                // draw unit quad (4 verts) instanced
                glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, quads->count);
                renderer->totalDrawCallsToOpenGl += 1;

                glBindVertexArray(0);
                glUseProgram(0);
            }
            // Clrear instance arrays, as they were flushed to their render calls
            quads->count = 0;

            // Text rendering
            if (renderer->glyphVtxArray.count > 0)
            {
                glUseProgram(renderer->textShader);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, renderer->fontTextures[0]);

                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, renderer->fontTextures[1]);

                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, renderer->fontTextures[2]);

                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, renderer->fontTextures[3]);

                GLint uScreenLoc = glGetUniformLocation(renderer->textShader, "uScreen");
                glUniform2f(uScreenLoc, renderer->screenWidth, renderer->screenHeight);

                glBindVertexArray(renderer->textVAO);
                glBindBuffer(GL_ARRAY_BUFFER, renderer->textVBO);

                glBufferSubData(
                    GL_ARRAY_BUFFER,
                    0,
                    sizeof(struct GlyphVtx) * 6 * gVerts->count,
                    renderer->glyphVtxArray.instData);

                glDrawArrays(GL_TRIANGLES, 0, renderer->glyphVtxArray.count * 6);
                renderer->totalDrawCallsToOpenGl += 1;

                glBindVertexArray(0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            renderer->glyphVtxArray.count = 0;

            if (cmd->commandType == CLAY_RENDER_COMMAND_TYPE_SCISSOR_START)
            {
                Clay_BoundingBox bb = cmd->boundingBox;
                GLint x = (GLint)bb.x;
                GLint y = (GLint)(renderer->screenHeight - (bb.y + bb.height));
                GLsizei w = (GLsizei)bb.width;
                GLsizei h = (GLsizei)bb.height;

                glEnable(GL_SCISSOR_TEST);
                glScissor(x, y, w, h);
            }
            else
            {
                glDisable(GL_SCISSOR_TEST);
            }
        }
    }
}
#endif
#endif