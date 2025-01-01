/*
* 
* Copyright (c) 2021-24 ColleagueRiley ColleagueRiley@gmail.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
*
*/

/*
    define args
    (MAKE SURE RSGL_IMPLEMENTATION is in exactly one header or you use -DRSGL_IMPLEMENTATION)
	#define RSGL_IMPLEMENTATION - makes it so source code is included with header
    
    #define RSGL_NO_TEXT - do not include text rendering functions
    #define RSGL_NO_SAVE_IMAGE - do not save/load images (don't use RSGL_drawImage if you use this), 
                                    RSGL_drawImage saves the file name + texture so it can load it
                                    when you ask for it later. This disables that 
    #define RSGL_INIT_FONTS [number of fonts] - set how much room should be pre-allocated for fonts by fontstash
                                                this avoids performance issues related to RSGL_REALLOC
                                                RSGL_INIT_FONTS = 4 by default
    #define RSGL_INIT_IMAGES [number of fonts] - set how much room should be pre-allocated for images by RSGL
                                                this avoids performance issues related to RSGL_REALLOC
                                                RSGL_INIT_IMAGES = 20 by default
    #define RSGL_NEW_IMAGES [number of fonts] - set how much room should be RSGL_REALLOCated at a time for images by RSGL
                                                this avoids performance issues related to RSGL_REALLOC
                                                RSGL_NEW_IMAGES 10 by default

    #define RSGL_MAX_BATCHES [number of batches] - set max number of batches to be allocated
    #define RSGL_MAX_VERTS [number of verts] - set max number of verts to be allocated (global, not per batch)

    #define RSGL_RENDER_LEGACY - use legacy rendering (ex. opengl) functions
    
    #define RSGL_NO_STB_IMAGE - do not include stb_image.h (& don't define image loading funcs)
    #define RSGL_NO_STB_IMAGE_IMP - declare stb funcs but don't define
    #define RSGL_NO_DEPS_FOLDER - Do not use '/deps' for the deps includes, use "./"
*/ 
#ifndef RSGL_INIT_FONTS
#define RSGL_INIT_FONTS 4
#endif
#ifndef RSGL_NEW_FONTS
#define RSGL_NEW_FONTS 2
#endif
#ifndef RSGL_INIT_IMAGES
#define RSGL_INIT_IMAGES 20
#endif
#ifndef RSGL_NEW_IMAGES
#define RSGL_NEW_IMAGES 10
#endif
#ifndef RSGL_MAX_BATCHES
#define RSGL_MAX_BATCHES 2028
#endif
#ifndef RSGL_MAX_VERTS
#define RSGL_MAX_VERTS 8192
#endif

#ifndef RSGL_MALLOC
#include <stdlib.h>
#define RSGL_MALLOC malloc
#define RSGL_REALLOC realloc
#define RSGL_FREE free
#endif

#ifndef RSGL_UNUSED
#define RSGL_UNUSED(x) (void) (x);
#endif

/* 
RSGL basicDraw types
*/

#ifndef RSGL_QUADS
#define RSGL_POINTS                               0x0000
#define RSGL_LINES                                0x0001      
#define RSGL_LINE_LOOP                            0x0002
#define RSGL_LINE_STRIP                           0x0003
#define RSGL_TRIANGLES                            0x0004      
#define RSGL_TRIANGLE_STRIP                       0x0005
#define RSGL_TRIANGLE_FAN                         0x0006      
#define RSGL_QUADS                                 0x0007

/* these are to ensure GL_DEPTH_TEST is disabled when they're being rendered */
#define RSGL_POINTS_2D                               0x0010
#define RSGL_LINES_2D                                0x0011    
#define RSGL_LINE_LOOP_2D                            0x0012
#define RSGL_LINE_STRIP_2D                           0x0013
#define RSGL_TRIANGLES_2D                            0x0014     
#define RSGL_TRIANGLE_STRIP_2D                       0x0015
#define RSGL_TRIANGLE_FAN_2D                         0x0016

#define RSGL_TRIANGLES_2D_BLEND     0x0114
#endif

#ifndef RSGL_H
#define RSGL_H
#ifndef RSGLDEF
#ifdef __APPLE__
#define RSGLDEF extern inline
#else
#define RSGLDEF inline
#endif
#endif

#if !defined(u8)
	#define u8 u8
	#if defined(_MSC_VER) || defined(__SYMBIAN32__)
		typedef unsigned char 	u8;
		typedef signed char		i8;
		typedef unsigned short  u16;
		typedef signed short 	i16;
		typedef unsigned int 	u32;
		typedef signed int		i32;
		typedef unsigned long	u64;
		typedef signed long		i64;
	#else
		#include <stdint.h>

		typedef uint8_t     u8;
		typedef int8_t      i8;
		typedef uint16_t   u16;
		typedef int16_t    i16;
		typedef uint32_t   u32;
		typedef int32_t    i32;
		typedef uint64_t   u64;
		typedef int64_t    i64;
	#endif
#endif

#if !defined(b8)
	#define b8 b8
	typedef u8 b8;
	typedef u32 b32;
#endif

#ifndef RSGL_texture
#define RSGL_texture size_t
#endif

#include <stdbool.h>
#include <stddef.h>

#define RSGL_between(x, lower, upper) (((lower) <= (x)) && ((x) <= (upper)))
#define RSGL_ENUM(type, name) type name; enum

/* 
*******
RSGL_[shape]
*******
*/

#ifndef RSGL_rect
typedef struct RSGL_rect {
    i32 x, y, w, h;
} RSGL_rect;
#endif
#define RSGL_RECT(x, y, w, h) (RSGL_rect){x, y, w, h}

typedef struct RSGL_rectF { float x, y, w, h; } RSGL_rectF;
#define RSGL_RECTF(x, y, w, h) (RSGL_rectF){x, y, w, h}

#ifndef RSGL_point
typedef struct RSGL_point {
    i32 x, y;
} RSGL_point;
#endif
#define RSGL_POINT(x, y) (RSGL_point){x, y}

#ifndef RSGL_area
typedef struct RSGL_area {
    u32 w, h;
} RSGL_area;
#endif
#define RSGL_AREA(w, h) (RSGL_area){w, h}

/* 
******* 
RSGL_[shape]
*******
*/

typedef struct RSGL_pointF { float x, y; } RSGL_pointF;
#define RSGL_POINTF(x, y) (RSGL_pointF){x, y}

typedef struct RSGL_point3D {
    i32 x, y, z;
} RSGL_point3D;

typedef struct RSGL_point3DF { float x, y, z; } RSGL_point3DF;

#define RSGL_POINT3D(x, y, z) (RSGL_point3D){x, y, z}
#define RSGL_POINT3DF(x, y, z) (RSGL_point3DF){x, y, z}

typedef struct RSGL_areaF { float w, h;} RSGL_areaF;
#define RSGL_AREAF(w, h) (RSGL_areaF){w, h}

typedef struct RSGL_circle {
    i32 x, y, d;
} RSGL_circle;
#define RSGL_CIRCLE(x, y, d) (RSGL_circle){x, y, d}

typedef struct RSGL_circleF { float x, y, d; } RSGL_circleF;
#define RSGL_CIRCLEF(x, y, d) (RSGL_circleF){x, y, d}

typedef struct RSGL_triangle {
    RSGL_point p1, p2, p3;
} RSGL_triangle;
#define RSGL_TRIANGLE(p1, p2, p3) (RSGL_triangle){p1, p2, p3}

typedef struct RSGL_triangleF { RSGL_pointF p1, p2, p3; } RSGL_triangleF;
#define RSGL_TRIANGLEF(p1, p2, p3) (RSGL_triangleF){p1, p2, p3}

#define RSGL_createTriangle(x1, y1, x2, y2, x3, y3) (RSGL_triangle){{x1, y1}, {x2, y2}, {x3, y3}}
#define RSGL_createTriangleF(x1, y1, x2, y2, x3, y3) (RSGL_triangleF){{x1, y1}, {x2, y2}, {x3, y3}}

typedef struct RSGL_cube {
    i32 x, y, z, w, h, l;
} RSGL_cube;
#define RSGL_CUBE(x, y, z, w, h, l) (RSGL_cube){x, y, z, w, h, l}

typedef struct RSGL_cubeF { float x, y, z, w, h, l; } RSGL_cubeF;
#define RSGL_CUBEF(x, y, z, w, h, l) (RSGL_cubeF){x, y, z, w, h, l}

/* 
the color stucture is in 
ABGR by default for performance reasons 
(converting color to hex for example)
*/
typedef struct RSGL_color {
    u8 a, b, g, r;
} RSGL_color;

#define RSGL_RGBA(r, g, b, a) ((RSGL_color){(a), (b), (g), (r)})
#define RSGL_RGB(r, g, b) ((RSGL_color){255, (b), (g), (r)})

#define RSGL_COLOR_TO_HEX(color) ((u32)(color) & 0xFFFFFF00)
#define RSGL_RGB_TO_HEX(r, g, b, a) (RSGL_COLOR_TO_HEX(RSGL_RGBA(r, g, b, a)))
#define RSGL_RGBA_TO_HEX(r, g, b) (RSGL_COLOR_TO_HEX(RSGL_RGB(r, g, b, a)))

/* toggle the use of legacy OpenGL, on by default unless it fails to load */
RSGLDEF void RSGL_legacy(i32 legacy);

/* 
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/

RSGLDEF void RSGL_init(
                            RSGL_area r, /* graphics context size */
                            void* loader /* opengl prozc address ex. wglProcAddress */
                            ); 
RSGLDEF void RSGL_updateSize(RSGL_area r);
RSGLDEF void RSGL_clear(RSGL_color c);
RSGLDEF void RSGL_free(void);

/* 
*******
RSGL_draw
*******
*/


/* 
    RSGL_draw args 

    RSGL has internal args which control how RSGL draws certain things
    by default these args clear after each RSGL_draw<whatever> call 

    but you can run RSGL_setClearArgs to enable or disable this behavior
    you can also run RSGL_clearArgs to clear the args by hand
*/
/* RSGL_args */
typedef struct RSGL_drawArgs {
    float* gradient; /* does not allocate any memory */
    
    RSGL_texture texture;
    u32 gradient_len;

    RSGL_rect currentRect; /* size of current surface */
    RSGL_point3D rotate; 

    bool fill;
    RSGL_point3DF center;
    float lineWidth;
    i32 legacy;
    u32 program;
} RSGL_drawArgs;

RSGLDEF void RSGL_rotate(RSGL_point3D rotate); /* apply rotation to drawing */
RSGLDEF void RSGL_setTexture(RSGL_texture texture); /* apply texture to drawing */
RSGLDEF void RSGL_setProgram(u32 program); /* use shader program for drawing */
RSGLDEF void RSGL_setGradient(
                                float* gradient, /* array of gradients */
                                size_t len /* length of array */
                            ); /* apply gradient to drawing, based on color list*/
RSGLDEF void RSGL_fill(bool fill); /* toggle filling, if fill is false it runs RSGL_draw<whatever>_outline instead */
RSGLDEF void RSGL_center(RSGL_point3DF center); /* the center of the drawing (or shape), this is used for rotation */

/* args clear after a draw function by default, this toggles that */
RSGLDEF void RSGL_setClearArgs(bool clearArgs); /* toggles if args are cleared by default or not */
RSGLDEF void RSGL_clearArgs(void); /* clears the args */

/* calculate the align a smaller rect with larger rect */
typedef RSGL_ENUM(u8, RSGL_alignment) {
    RSGL_ALIGN_NONE = (1 << 0),
    /* horizontal */
    RSGL_ALIGN_LEFT = (1 << 1),
    RSGL_ALIGN_CENTER = (1 << 2),
    RSGL_ALIGN_RIGHT = (1 << 3),
    
    /* vertical */
    RSGL_ALIGN_UP = (1 << 4),
    RSGL_ALIGN_MIDDLE = (1 << 5),
    RSGL_ALIGN_DOWN = (1 << 6),

    RSGL_ALIGN_HORIZONTAL = RSGL_ALIGN_LEFT | RSGL_ALIGN_CENTER | RSGL_ALIGN_RIGHT,
    RSGL_ALIGN_VERTICAL = RSGL_ALIGN_UP | RSGL_ALIGN_MIDDLE | RSGL_ALIGN_DOWN,
/* ex : alignment = (RSGL_ALIGN_LEFT | RSGL_ALIGN_MIDDLE) */
};

/* align smaller rect onto larger rect based on a given alignment */
RSGLDEF RSGL_rect RSGL_alignRect(RSGL_rect larger, RSGL_rect smaller, u16 alignment);
RSGLDEF RSGL_rectF RSGL_alignRectF(RSGL_rectF larger, RSGL_rectF smaller, u16 alignment);

#ifndef RSGL_GET_WORLD_X
#define RSGL_GET_WORLD_X(x) (float)(2.0f * (x) / RSGL_args.currentRect.w - 1.0f)
#define RSGL_GET_WORLD_Y(y) (float)(1.0f + -2.0f * (y) / RSGL_args.currentRect.h)
#define RSGL_GET_WORLD_Z(z) (float)(z)
#endif

#define RSGL_GET_MATRIX_X(x, y, z) (matrix.m[0] * x + matrix.m[4] * y + matrix.m[8] * z + matrix.m[12])
#define RSGL_GET_MATRIX_Y(x, y, z) (matrix.m[1] * x + matrix.m[5] * y + matrix.m[9] * z + matrix.m[13])
#define RSGL_GET_MATRIX_Z(x, y, z) (matrix.m[2] * x + matrix.m[6] * y + matrix.m[10] * z + matrix.m[14])

#define RSGL_GET_MATRIX_POINT(x, y, z) RSGL_GET_MATRIX_X(x, y, z), RSGL_GET_MATRIX_Y(x, y, z), RSGL_GET_MATRIX_Z(x, y, z)
#define RSGL_GET_WORLD_POINT(x, y, z) RSGL_GET_WORLD_X((x)), RSGL_GET_WORLD_Y((y)), RSGL_GET_WORLD_Z((z))

#define RSGL_GET_FINAL_POINT(x, y, z) RSGL_GET_MATRIX_POINT(RSGL_GET_WORLD_X((x)), RSGL_GET_WORLD_Y((y)), RSGL_GET_WORLD_Z((z)))

typedef struct RSGL_MATRIX {
    float m[16];
} RSGL_MATRIX;

RSGLDEF RSGL_MATRIX RSGL_initDrawMatrix(RSGL_point3DF center);

/* 
RSGL_basicDraw is a function used internally by RSGL, but you can use it yourself
RSGL_basicDraw batches a given set of points based on the data to be rendered
*/
RSGLDEF void RSGL_basicDraw(
                u32 TYPE, /* type of shape, RSGL_QUADS, RSGL_TRIANGLES, RSGL_LINES, RSGL_QUADS_2D */
                float* points, /* array of 3D points */
                float* texPoints, /* array of 2D texture points (must be same length as points)*/
                RSGL_color c, /* the color to draw the shape */
                size_t len /* the length of the points array */
            );

typedef struct RSGL_BATCH {
    size_t start, len; /* when batch starts and it's length */
    u32 type;
    RSGL_texture tex;
    float lineWidth;
} RSGL_BATCH; /* batch data type for rendering */

typedef struct RSGL_RENDER_INFO {
    RSGL_BATCH* batches;

    float* verts;
    float* texCoords;
    float* colors;

    size_t len; /* number of batches*/
    size_t vert_len; /* number of verts */
} RSGL_RENDER_INFO; /* render data */

/* 
    All of these functions are to be defined by the external render backend
*/

/* renders the current batches */
RSGLDEF void RSGL_renderBatch(RSGL_RENDER_INFO* info);
RSGLDEF void RSGL_renderInit(void* proc, RSGL_RENDER_INFO* info); /* init render backend */
RSGLDEF void RSGL_renderFree(void); /* free render backend */
RSGLDEF void RSGL_renderClear(float r, float g, float b, float a);
RSGLDEF void RSGL_renderViewport(i32 x, i32 y, i32 w, i32 h);
/* create a texture based on a given bitmap, this must be freed later using RSGL_deleteTexture or opengl*/
RSGLDEF RSGL_texture RSGL_renderCreateTexture(u8* bitmap, RSGL_area memsize,  u8 channels);
/* updates an existing texture wiht a new bitmap */
RSGLDEF void RSGL_renderUpdateTexture(RSGL_texture texture, u8* bitmap, RSGL_area memsize, u8 channels);
/* delete a texture */
RSGLDEF void RSGL_renderDeleteTexture(RSGL_texture tex);

/* custom shader program */
typedef struct RSGL_programInfo {
    u32 vShader, fShader, program;
} RSGL_programInfo;

RSGLDEF RSGL_programInfo RSGL_renderCreateProgram(const char* VShaderCode, const char* FShaderCode, char* posName, char* texName, char* colorName);
RSGLDEF void RSGL_renderDeleteProgram(RSGL_programInfo program);
RSGLDEF void RSGL_renderSetShaderValue(u32 program, char* var, float value[], u8 len);

/* these are RFont functions that also must be defined by the renderer

32 RFont_create_atlas(u32 atlasWidth, u32 atlasHeight);
void RFont_bitmap_to_atlas(RSGL_rsoft_texture atlas, u8* bitmap, float x, float y, float w, float h);

*/

/* RSGL translation */
RSGLDEF RSGL_MATRIX RSGL_matrixMultiply(float left[16], float right[16]);
RSGLDEF RSGL_MATRIX RSGL_rotatef(RSGL_MATRIX* matrix, float angle, float x, float y, float z); 
RSGLDEF RSGL_MATRIX RSGL_translatef(RSGL_MATRIX* matrix, float x, float y, float z);
/* 2D shape drawing */
/* in the function names, F means float */

RSGLDEF void RSGL_drawPoint(RSGL_point p, RSGL_color c);
RSGLDEF void RSGL_drawPointF(RSGL_pointF p, RSGL_color c);
RSGLDEF void RSGL_plotLines(RSGL_pointF* lines, size_t points_count, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawTriangle(RSGL_triangle t, RSGL_color c);
RSGLDEF void RSGL_drawTriangleF(RSGL_triangleF t, RSGL_color c);

RSGLDEF void RSGL_drawTriangleHyp(RSGL_pointF p, size_t angle, float hypotenuse, RSGL_color color);

RSGLDEF void RSGL_drawRect(RSGL_rect r, RSGL_color c);
RSGLDEF void RSGL_drawRectF(RSGL_rectF r, RSGL_color c);


RSGLDEF void RSGL_drawRoundRect(RSGL_rect r, RSGL_point rounding, RSGL_color c);
RSGLDEF void RSGL_drawRoundRectF(RSGL_rectF r, RSGL_point rounding, RSGL_color c);

RSGLDEF void RSGL_drawPolygon(RSGL_rect r, u32 sides, RSGL_color c);
RSGLDEF void RSGL_drawPolygonF(RSGL_rectF r, u32 sides, RSGL_color c);

RSGLDEF void RSGL_drawArc(RSGL_rect o, RSGL_point arc, RSGL_color color);
RSGLDEF void RSGL_drawArcF(RSGL_rectF o, RSGL_pointF arc, RSGL_color color);

RSGLDEF void RSGL_drawCircle(RSGL_circle c, RSGL_color color);
RSGLDEF void RSGL_drawCircleF(RSGL_circleF c, RSGL_color color);

RSGLDEF void RSGL_drawOval(RSGL_rect o, RSGL_color c);
RSGLDEF void RSGL_drawOvalF(RSGL_rectF o, RSGL_color c);

RSGLDEF void RSGL_drawLine(RSGL_point p1, RSGL_point p2, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawLineF(RSGL_pointF p1, RSGL_pointF p2, u32 thickness, RSGL_color c);

/* 2D outlines */

/* thickness means the thickness of the line */

RSGLDEF void RSGL_drawTriangleOutline(RSGL_triangle t, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawTriangleFOutline(RSGL_triangleF t, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawRectOutline(RSGL_rect r, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawRectFOutline(RSGL_rectF r, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawRoundRectOutline(RSGL_rect r, RSGL_point rounding, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawRoundRectFOutline(RSGL_rectF r, RSGL_point rounding, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawPolygonOutline(RSGL_rect r, u32 sides, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawPolygonFOutline(RSGL_rectF r, u32 sides, u32 thickness, RSGL_color c);

RSGLDEF void RSGL_drawArcOutline(RSGL_rect o, RSGL_point arc, u32 thickness, RSGL_color color);
RSGLDEF void RSGL_drawArcFOutline(RSGL_rectF o, RSGL_pointF arc, u32 thickness, RSGL_color color);

RSGLDEF void RSGL_drawCircleOutline(RSGL_circle c, u32 thickness, RSGL_color color);
RSGLDEF void RSGL_drawCircleFOutline(RSGL_circleF c, u32 thickness, RSGL_color color);

RSGLDEF void RSGL_drawOvalFOutline(RSGL_rectF o, u32 thickness, RSGL_color c);
RSGLDEF void RSGL_drawOvalOutline(RSGL_rect o, u32 thickness, RSGL_color c);

/* format a string */
#ifndef RSGL_NO_TEXT
RSGLDEF const char* RFont_fmt(const char* string, ...);
#define RSGL_strFmt RFont_fmt

/* loads a font into RSGL, returns it's index into the RSGL_fonts array, this is used as an id in later functions */
RSGLDEF i32 RSGL_loadFont(const char* font);
/* sets font as the current font in use based on index in RSGL_font, given when it was loaded */
RSGLDEF void RSGL_setFont(i32 font);

/* sets source RFont font as the current font, given when it was loaded */
struct RFont_font;
RSGLDEF void RSGL_setRFont(struct RFont_font* font);

RSGLDEF void RSGL_drawText_len(const char* text, size_t len, RSGL_circle c, RSGL_color color);
RSGLDEF void RSGL_drawText(const char* text, RSGL_circle c, RSGL_color color);
#define RSGL_drawTextF(text, font, c, color) \
    RSGL_setFont(font);\
    RSGL_drawText(text, c, color);

/* align text onto larger rect based on a given alignment */
RSGLDEF RSGL_circle RSGL_alignText(const char* str, RSGL_circle c, RSGL_rectF larger, u8 alignment);
/* align text based on a length */
RSGLDEF RSGL_circle RSGL_alignText_len(const char* str, size_t str_len, RSGL_circle c, RSGL_rectF larger, u8 alignment);

/* 
    returns the width of a text when rendered with the set font with the size of `fontSize
    stops at `textEnd` or when it reaches '\0'
*/
RSGLDEF RSGL_area RSGL_textArea(const char* text, u32 fontSize, size_t textEnd);
RSGLDEF RSGL_area RSGL_textLineArea(const char* text, u32 fontSize, size_t textEnd, size_t line);
#define RSGL_textAreaF(text, fontSize, textEnd) \
    RSGL_setFont(font);\
    RSGL_textAreaF(text, fontSize, textEnd);
#endif /* RSGL_NO_TEXT */

/* 
    this creates a texture based on a given image, draws it on a rectangle and then returns the loaded texture 
    
    if the rectangle's width and height are 0 it doesn't draw the image
    the texture is loaded into RSGL_image, this means it doesn't need to be freed
    but you can still free it early
*/

typedef struct RSGL_image { RSGL_texture tex; RSGL_area srcSize; char file[255]; } RSGL_image;
RSGLDEF RSGL_image RSGL_drawImage(const char* image, RSGL_rect r);

#define RSGL_loadImage(image) ((RSGL_image) RSGL_drawImage(image, (RSGL_rect){0, 0, 0, 0}))

/* 
    these two functions can be used before RSGL_renderCreateTexture in order to create 
    an swizzle'd texutre or atlas
*/

/* 
*******
extra
*******
*/

/* ** collision functions ** */
RSGLDEF bool RSGL_circleCollidePoint(RSGL_circle c, RSGL_point p);
RSGLDEF bool RSGL_circleCollideRect(RSGL_circle c, RSGL_rect r);
RSGLDEF bool RSGL_circleCollide(RSGL_circle cir1, RSGL_circle cir2);
RSGLDEF bool RSGL_rectCollidePoint(RSGL_rect r, RSGL_point p);
RSGLDEF bool RSGL_rectCollide(RSGL_rect r, RSGL_rect r2);
RSGLDEF bool RSGL_pointCollide(RSGL_point p, RSGL_point p2);

RSGLDEF bool RSGL_circleCollidePointF(RSGL_circleF c, RSGL_pointF p);
RSGLDEF bool RSGL_circleCollideRectF(RSGL_circleF c, RSGL_rectF r);
RSGLDEF bool RSGL_circleCollideF(RSGL_circleF cir1, RSGL_circleF cir2);
RSGLDEF bool RSGL_rectCollidePointF(RSGL_rectF r, RSGL_pointF p);
RSGLDEF bool RSGL_rectCollideF(RSGL_rectF r, RSGL_rectF r2);
RSGLDEF bool RSGL_pointCollideF(RSGL_pointF p, RSGL_pointF p2);

#endif /* ndef RSGL_H */

/*
(Notes on how to manage Silicon (macos) included)

Example to get you started :

linux : gcc main.c -lX11 -lXcursor -lGL
windows : gcc main.c -lopengl32 -lshell32 -lgdi32
macos:
	<Silicon> can be replaced to where you have the Silicon headers stored
	<libSilicon.a> can be replaced to wherever you have libSilicon.a
	clang main.c -I<Silicon> <libSilicon.a> -framework Foundation -framework AppKit -framework OpenGL -framework CoreVideo

	NOTE(EimaMei): If you want the MacOS experience to be fully single header, then I'd be best to install Silicon (after compiling)
	by going to the `Silicon` folder and running `make install`. After this you can easily include Silicon via `#include <Silicon/silicon.h>'
	and link it by doing `-lSilicon`

	(TODO: make new example)

	compiling :

	if you wish to compile the library all you have to do is create a new file with this in it

	RSGL.c
	#define RSGL_IMPLEMENTATION
	#include "RSGL.h"

	then you can use gcc (or whatever compile you wish to use) to compile the library into object file

	ex. gcc -c RSGL.c -fPIC

	after you compile the library into an object file, you can also turn the object file into an static or shared library

	(commands ar and gcc can be replaced with whatever equivalent your system uses)
	static : ar rcs RSGL.a RSGL.o
	shared :
		windows:
			gcc -shared RSGL.o  -lshell32 -lgdi32 -o RSGL.dll
		linux:
			gcc -shared RSGL.o -lX11 -lXcursor -o RSGL.so
		macos:
			<Silicon/include> can be replaced to where you have the Silicon headers stored
			<libSilicon.a> can be replaced to wherever you have libSilicon.a
			gcc -shared RSGL.o -framework Foundation -framework AppKit -framework CoreVideo 

	ex.
	gcc main.c -framework Foundation -framework AppKit -framework CoreVideo
*/

#ifdef RSGL_IMPLEMENTATION

#ifdef RSGL_RENDER_LEGACY
#define RFONT_RENDER_LEGACY
#endif

#define STB_IMAGE_IMPLEMENTATION

#include <assert.h>

#ifndef RSGL_NO_TEXT
#define RFONT_IMPLEMENTATION

#define RFONT_RENDER_LEGACY
#define RFONT_NO_OPENGL

#define RFont_area RSGL_area

#define RFont_texture RSGL_texture
#define RFONT_MALLOC RSGL_MALLOC
#define RFONT_FREE RSGL_FREE

#ifndef RSGL_NO_DEPS_FOLDER
#include "deps/RFont.h"
#else
#include "RFont.h"
#endif
#endif /* RSGL_NO_TEXT */

#if !defined(RSGL_NO_STB_IMAGE) && !defined(RSGL_NO_STB_IMAGE_IMP)

#ifndef RSGL_NO_DEPS_FOLDER
#include "deps/stb_image.h"
#else
#include <stb_image.h>
#endif

#endif

#ifdef RSGL_NO_STB_IMAGE_IMP
u8* stbi_load            (char const *filename, int *x, int *y, int *channels_in_file, int desired_channels);
#endif

#include <time.h>

#include <stdbool.h>

#ifndef RSGL_NO_TEXT
typedef struct RSGL_fontData {
    char* name;
    RFont_font* f;
} RSGL_fontData;

typedef struct RSGL_fontsData {
    RFont_font* f;
    
    RSGL_fontData* fonts;
    size_t len;
    size_t cap;
} RSGL_fontsData;

RSGL_fontsData RSGL_font = {NULL, NULL, 0, 0};
#endif

RSGL_drawArgs RSGL_args = {NULL, 0, 0, { }, {0, 0, 0}, 1, RSGL_POINT3DF(-1, -1, -1), 1, 0, 0};
bool RSGL_argsClear = false;

RSGL_image* RSGL_images = NULL;
size_t RSGL_images_len = 0;

RSGLDEF bool RSGL_cstr_equal(const char* str, const char* str2);
bool RSGL_cstr_equal(const char* str, const char* str2) {
    char* s;
    char* s2 = (char*)str2;
    
    for (s = (char*)str; *s && *s2; s++) {
        if (*s != *s2)
            return false;

        s2++;
    }

    if (*s == '\0' && *s2 == '\0')
        return true;
    
    return false;
}

RSGL_rect RSGL_alignRect(RSGL_rect larger, RSGL_rect smaller, u16 alignment) {
    RSGL_rectF r = RSGL_alignRectF(
                                    RSGL_RECTF(larger.x, larger.y, larger.w, larger.y), 
                                    RSGL_RECTF(smaller.x, smaller.y, smaller.w, smaller.h), 
                                    alignment
                                );
    
    return RSGL_RECT(r.x, r.y, r.w, r.h);
}

RSGL_rectF RSGL_alignRectF(RSGL_rectF larger, RSGL_rectF smaller, u16 alignment) {
    RSGL_rectF aligned = smaller;

    switch (alignment & RSGL_ALIGN_HORIZONTAL) {
        case RSGL_ALIGN_LEFT:
            aligned.x = larger.x;
            break;
        case RSGL_ALIGN_CENTER:
            aligned.x = larger.x + ((larger.w - smaller.w) / 2.0);
            break;
        case RSGL_ALIGN_RIGHT:
            aligned.x = (larger.x + larger.w) - smaller.w;
            break;
        default: break;
    }

    switch (alignment & RSGL_ALIGN_VERTICAL) {
        case RSGL_ALIGN_UP:
            aligned.y = larger.y;
            break;
        case RSGL_ALIGN_MIDDLE:
            aligned.y = larger.y + ((larger.h - smaller.h) / 2.0);
            break;
        case RSGL_ALIGN_DOWN:
            aligned.y = (larger.y + larger.h) - smaller.h;
            break;
        default: break;
    }

    return aligned;
}

RSGL_MATRIX RSGL_initDrawMatrix(RSGL_point3DF center) {    
    RSGL_MATRIX matrix = (RSGL_MATRIX) { 
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        }
    };
        
    if (RSGL_args.rotate.x || RSGL_args.rotate.y || RSGL_args.rotate.z) {
        if (RSGL_args.center.x != -1 && RSGL_args.center.y != -1 &&  RSGL_args.center.z != -1)
            center = RSGL_args.center;
        
        matrix = RSGL_translatef(&matrix, center.x, center.y, center.z);
        matrix = RSGL_rotatef(&matrix, RSGL_args.rotate.z,  0, 0, 1);
        matrix = RSGL_rotatef(&matrix, RSGL_args.rotate.y, 0, 1, 0);
        matrix = RSGL_rotatef(&matrix, RSGL_args.rotate.x, 1, 0, 0);
        matrix = RSGL_translatef(&matrix, -center.x, -center.y, -center.z);
    }

    return matrix;
}

RSGL_RENDER_INFO RSGL_renderInfo = {NULL, NULL, NULL, NULL, 0, 0};

#ifndef RSGL_CUSTOM_RENDER
#include "RSGL_gl.h"
#endif

void RSGL_basicDraw(u32 type, float* points, float* texPoints, RSGL_color c, size_t len) {
    if (RSGL_renderInfo.len + 1 >= RSGL_MAX_BATCHES || RSGL_renderInfo.vert_len + len >= RSGL_MAX_VERTS) {
        RSGL_renderBatch(&RSGL_renderInfo);
    }

    RSGL_BATCH* batch = NULL;

    if (
        RSGL_renderInfo.len == 0 || 
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].tex != RSGL_args.texture  ||
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].lineWidth != RSGL_args.lineWidth ||
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].type != type ||
        RSGL_renderInfo.batches[RSGL_renderInfo.len - 1].type == RSGL_TRIANGLE_FAN_2D
    ) {
        RSGL_renderInfo.len += 1;
    
        batch = &RSGL_renderInfo.batches[RSGL_renderInfo.len - 1];
        batch->start = RSGL_renderInfo.vert_len;
        batch->len = 0;
        batch->type = type;
        batch->tex = RSGL_args.texture;
        batch->lineWidth = RSGL_args.lineWidth;
    } else {
        batch = &RSGL_renderInfo.batches[RSGL_renderInfo.len - 1];
    }

    if (batch == NULL)
        return;

    batch->len += len;

    memcpy(&RSGL_renderInfo.verts[RSGL_renderInfo.vert_len * 3], points, len * sizeof(float) * 3);
    memcpy(&RSGL_renderInfo.texCoords[RSGL_renderInfo.vert_len * 2], texPoints, len * sizeof(float) * 2);

    float color[4] = {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f};

    if (RSGL_args.gradient_len && RSGL_args.gradient && (i64)(len - 1) > 0) {
        memcpy(&RSGL_renderInfo.colors[RSGL_renderInfo.vert_len * 4], color, sizeof(float) * 4);
        memcpy(&RSGL_renderInfo.colors[RSGL_renderInfo.vert_len * 4 + 4], RSGL_args.gradient, (len - 1) * sizeof(float) * 4);
    }
    else {
        size_t i;
        for (i = 0; i < len * 4; i += 4)
            memcpy(&RSGL_renderInfo.colors[(RSGL_renderInfo.vert_len * 4) + i], color, sizeof(float) * 4);
    }

    RSGL_renderInfo.vert_len += len;

    if (RSGL_argsClear) {
        RSGL_setTexture(0);
        RSGL_clearArgs();
    }
}

void RSGL_legacy(i32 legacy) {
    if (RSGL_args.legacy != 2)
        RSGL_args.legacy = legacy;
}

/*
*********************
RSGL_GRAPHICS_CONTEXT
*********************
*/

void RSGL_init(RSGL_area r, void* loader) {
    RSGL_renderViewport(0, 0, r.w, r.h);
    
    RSGL_args.currentRect = (RSGL_rect){0, 0, r.w, r.h};

    #ifndef RSGL_NO_TEXT
    RFont_init(r.w, r.h);
    #endif

    if (RSGL_renderInfo.batches == NULL) {
        RSGL_renderInfo.len = 0;
        RSGL_renderInfo.vert_len = 0;
        RSGL_renderInfo.batches = (RSGL_BATCH*)RSGL_MALLOC(sizeof(RSGL_BATCH) * RSGL_MAX_BATCHES);
        RSGL_renderInfo.verts = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 3);
        RSGL_renderInfo.colors = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 4);
        RSGL_renderInfo.texCoords = (float*)RSGL_MALLOC(sizeof(float) * RSGL_MAX_VERTS * 2);
    
        RSGL_renderInit(loader, &RSGL_renderInfo);
    }
}

void RSGL_clear(RSGL_color color) {
    RSGL_renderClear(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

    RSGL_renderBatch(&RSGL_renderInfo);
}

void RSGL_updateSize(RSGL_area r) {
    RSGL_args.currentRect = (RSGL_rect){0, 0, r.w, r.h};
    RFont_update_framebuffer(r.w, r.h);
    RSGL_renderViewport(0, 0, r.w, r.h);
}

void RSGL_free() {
    u32 i;
    #ifndef RSGL_NO_TEXT
    for (i = 0; i < RSGL_font.len; i++)
        RFont_font_free(RSGL_font.fonts[i].f);

    RSGL_FREE(RSGL_font.fonts);
    #endif

    RSGL_renderFree();

    if (RSGL_renderInfo.batches != NULL) {
        RSGL_FREE(RSGL_renderInfo.batches);
        RSGL_FREE(RSGL_renderInfo.verts);
        RSGL_FREE(RSGL_renderInfo.colors);
        RSGL_FREE(RSGL_renderInfo.texCoords);
        RSGL_renderInfo.batches = NULL;
        RSGL_renderInfo.len = 0;
        RSGL_renderInfo.vert_len = 0;
    }
}

/* 
****
RSGL_draw
****
*/

/* RSGL_args */
void RSGL_rotate(RSGL_point3D rotate){
    RSGL_args.rotate = rotate;
}
void RSGL_setTexture(RSGL_texture texture) { 
    RSGL_args.texture = texture;
}
void RSGL_setProgram(u32 program) { 
    if (RSGL_args.program != program) {
        /* render using the current program */
        RSGL_renderBatch(&RSGL_renderInfo);
        RSGL_args.program = program;
    }
}

void RSGL_setGradient(float gradient[], size_t len) {
    RSGL_args.gradient_len = len;
    RSGL_args.gradient = gradient;
}
void RSGL_fill(bool fill) {
    RSGL_args.fill = fill;
}
void RSGL_center(RSGL_point3DF center) {
    RSGL_args.center = center;
}
void RSGL_setClearArgs(bool clearArgs) {
    RSGL_argsClear = clearArgs;
}
void RSGL_clearArgs(void) {
    RSGL_args = (RSGL_drawArgs){NULL, 0, 0, { }, {0, 0, 0}, 1, RSGL_POINT3DF(-1, -1, -1), 0, 0, 0};
}


void RSGL_drawPoint(RSGL_point p, RSGL_color c) {
    RSGL_drawPointF((RSGL_pointF){(float)p.x, (float)p.y}, c);
}

void RSGL_drawTriangle(RSGL_triangle t, RSGL_color c) {
    RSGL_drawTriangleF(RSGL_createTriangleF((float)t.p1.x, (float)t.p1.y, (float)t.p2.x, (float)t.p2.y, (float)t.p3.x, (float)t.p3.y), c);
}

void RSGL_drawRect(RSGL_rect r, RSGL_color c) {
    RSGL_drawRectF((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, c);
}

void RSGL_drawRoundRect(RSGL_rect r, RSGL_point rounding, RSGL_color c) {
    RSGL_drawRoundRectF((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, rounding, c);
}

void RSGL_drawPolygon(RSGL_rect r, u32 sides, RSGL_color c) {
    RSGL_drawPolygonF((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, sides, c);
}

void RSGL_drawArc(RSGL_rect o, RSGL_point arc, RSGL_color color) {
    RSGL_drawArcF((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, (RSGL_pointF){(float)arc.x, (float)arc.y}, color);
}

void RSGL_drawCircle(RSGL_circle c, RSGL_color color) {
    RSGL_drawCircleF((RSGL_circleF){(float)c.x, (float)c.y, (float)c.d}, color);
}

void RSGL_drawOval(RSGL_rect o, RSGL_color c) {
    RSGL_drawOvalF((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, c);
}

void RSGL_drawLine(RSGL_point p1, RSGL_point p2, u32 thickness, RSGL_color c) {
    RSGL_drawLineF((RSGL_pointF){(float)p1.x, (float)p1.y}, (RSGL_pointF){(float)p2.x, (float)p2.y}, thickness, c);
}

void RSGL_drawTriangleOutline(RSGL_triangle t, u32 thickness, RSGL_color c) {
    RSGL_drawTriangleFOutline(RSGL_createTriangleF((float)t.p1.x, (float)t.p1.y, (float)t.p2.x, (float)t.p2.y, (float)t.p3.x, (float)t.p3.y), thickness, c);
}

void RSGL_drawRectOutline(RSGL_rect r, u32 thickness, RSGL_color c) {
    RSGL_drawRectFOutline((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, thickness, c);
}

void RSGL_drawRoundRectOutline(RSGL_rect r, RSGL_point rounding, u32 thickness, RSGL_color c) {
    RSGL_drawRoundRectFOutline((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, rounding, thickness, c);
}

void RSGL_drawPolygonOutline(RSGL_rect r, u32 sides, u32 thickness, RSGL_color c) {
    RSGL_drawPolygonFOutline((RSGL_rectF){(float)r.x, (float)r.y, (float)r.w, (float)r.h}, sides, thickness, c);
}

void RSGL_drawArcOutline(RSGL_rect o, RSGL_point arc, u32 thickness, RSGL_color color) {
    RSGL_drawArcFOutline((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, (RSGL_pointF){(float)arc.x, (float)arc.y}, thickness, color);
}

void RSGL_drawCircleOutline(RSGL_circle c, u32 thickness, RSGL_color color) {
    RSGL_drawCircleFOutline((RSGL_circleF){(float)c.x, (float)c.y, (float)c.d}, thickness, color);
}

void RSGL_drawOvalOutline(RSGL_rect o, u32 thickness, RSGL_color c) {
    RSGL_drawOvalFOutline((RSGL_rectF){(float)o.x, (float)o.y, (float)o.w, (float)o.h}, thickness, c);
}

void RSGL_drawPointF(RSGL_pointF p, RSGL_color c) {
    RSGL_drawRectF((RSGL_rectF){p.x, p.y, 1.0f, 1.0f}, c);
}

void RSGL_plotLines(RSGL_pointF* lines, size_t points_count, u32 thickness, RSGL_color c) {
    size_t i;
    for (i = 0; i < points_count; i += 2) {
        RSGL_drawLineF(lines[i], lines[i + 1], thickness, c);
    }
}

void RSGL_drawTriangleF(RSGL_triangleF t, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawTriangleFOutline(t, 1, c);

    RSGL_point3DF center = {RSGL_GET_WORLD_POINT(t.p3.x, (t.p3.y + t.p1.y) / 2.0f, 0)};
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);
    
    float points[] = {RSGL_GET_FINAL_POINT((float)t.p1.x, (float)t.p1.y, 0.0f), 
                      RSGL_GET_FINAL_POINT((float)t.p2.x, (float)t.p2.y, 0.0f), 
                      RSGL_GET_FINAL_POINT((float)t.p3.x, (float)t.p3.y, 0.0f)};
    
    float texPoints[] = {   
                0.0f, 1.0f, 
                1.0f, 1.0f,
                ((float)(t.p3.x - t.p1.x)/t.p2.x < 1) ? (float)(t.p3.x - t.p1.x) / t.p2.x : 0, 0.0f,
    };
    
    RSGL_basicDraw(RSGL_TRIANGLES_2D, (float*)points, (float*)texPoints, c, 3);
}

#ifndef PI
    #define PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif
#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif

void RSGL_drawTriangleHyp(RSGL_pointF p, size_t angle, float hypotenuse, RSGL_color color) {
    float dir = (hypotenuse > 0);
    hypotenuse = fabsf(hypotenuse);

    float base = hypotenuse * (cos(angle) * DEG2RAD);
    float opp = hypotenuse * (sin(angle) * DEG2RAD); 
    
    RSGL_triangleF t = RSGL_TRIANGLEF(
        p,
        RSGL_POINTF(p.x + base, p.y),
        RSGL_POINTF(p.x + (base * dir), p.y - opp)
    );
    
    RSGL_drawTriangleF(t, color);   
}

void RSGL_drawRectF(RSGL_rectF r, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawRectFOutline(r, 1, c);
        
    float texPoints[] = {
                                0.0f, 0.0f,
                                0.0f, 1.0f,
                                1.0f, 0.0f,
                                1.0f, 1.0f,
                                1.0f, 0.0f, 
                                0.0f, 1.0f
                            };

    RSGL_point3DF center = (RSGL_point3DF){RSGL_GET_WORLD_POINT(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f)};
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);

    float points[] = {
                                RSGL_GET_FINAL_POINT(r.x, r.y, 0.0f), 
                                RSGL_GET_FINAL_POINT(r.x, r.y + r.h, 0.0f),       
                                RSGL_GET_FINAL_POINT(r.x + r.w, r.y, 0.0f), 

                                RSGL_GET_FINAL_POINT(r.x + r.w, r.y + r.h, 0.0f),        
                                RSGL_GET_FINAL_POINT(r.x + r.w, r.y, 0.0f),     
                                RSGL_GET_FINAL_POINT(r.x, r.y + r.h, 0.0f),  
                            };

    RSGL_basicDraw(RSGL_TRIANGLES_2D, (float*)points, (float*)texPoints, c, 6);
}

void RSGL_drawRoundRectF(RSGL_rectF r, RSGL_point rounding, RSGL_color c) {
    if (RSGL_args.fill == false)
        return RSGL_drawRoundRectFOutline(r, rounding, 1, c);

    RSGL_drawRect((RSGL_rect) {r.x + (rounding.x / 2), r.y, r.w - rounding.x, r.h}, c);
    RSGL_drawRect((RSGL_rect) {r.x, r.y + (rounding.y / 2), r.w,  r.h - rounding.y}, c);

    RSGL_drawArc((RSGL_rect) {r.x, r.y, rounding.x, rounding.y}, (RSGL_point){180, 270}, c);
    RSGL_drawArc((RSGL_rect) {r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y}, (RSGL_point){90, 180}, c);
    RSGL_drawArc((RSGL_rect) {r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y), rounding.x, rounding.y}, (RSGL_point){0, 90}, c);
    RSGL_drawArc((RSGL_rect) {r.x, r.y  + (r.h - rounding.y),  rounding.x, rounding.y}, (RSGL_point){270, 360}, c);
}

void RSGL_drawPolygonFOutlinePro(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c);

void RSGL_drawPolygonFPro(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c) {
    static float verts[360 * 3];
    static float texcoords[360 * 2];

    if (RSGL_args.fill == false)
        return RSGL_drawPolygonFOutlinePro(o, sides, arc, c);
    
    RSGL_point3DF center =  (RSGL_point3DF){RSGL_GET_WORLD_POINT(o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0)};
    
    o = (RSGL_rectF){o.x, o.y, o.w / 2, o.h / 2};    
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);

    float displacement = 360.0f / (float)sides;
    float angle = displacement * arc.x;
    
    size_t vIndex = 0;
    size_t tIndex = 0; 

    u32 i;
    for (i = 0; i < sides; i++) {
        RSGL_pointF p = {sinf(angle * DEG2RAD), cosf(angle * DEG2RAD)};

        texcoords[tIndex] = (p.x + 1.0f) * 0.5;
        texcoords[tIndex + 1] = (p.y + 1.0f) * 0.5;

        memcpy(verts + vIndex, (float[3]){RSGL_GET_FINAL_POINT(o.x + o.w + (p.x * o.w), o.y + o.h + (p.y * o.h), 0.0)}, 3 * sizeof(float));

        angle += displacement;
        tIndex += 2;
        vIndex += 3;
    }

    texcoords[tIndex + 1] = 0;
    texcoords[tIndex + 2] = 0;

    RSGL_basicDraw(RSGL_TRIANGLE_FAN_2D, verts, texcoords, c, vIndex / 3);
}

void RSGL_drawPolygonF(RSGL_rectF o, u32 sides, RSGL_color c) { RSGL_drawPolygonFPro(o, sides, (RSGL_pointF){0, (int)sides}, c); }


#ifndef M_PI
#define M_PI		3.14159265358979323846	/* pi */
#endif

void RSGL_drawArcF(RSGL_rectF o, RSGL_pointF arc, RSGL_color color) {  
    u32 verts = (u32)round((float)((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10));
    verts = (verts > 360 ? 360 : verts);

    RSGL_drawPolygonFPro(o, verts, arc, color); 
}

void RSGL_drawCircleF(RSGL_circleF c, RSGL_color color) {  
    float verts = ((2 * M_PI * c.d) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_drawPolygonFPro((RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, verts}, color); 
}

void RSGL_drawOvalF(RSGL_rectF o, RSGL_color c) { 
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_drawPolygonFPro(o, verts, (RSGL_pointF){0, verts}, c); 
}

/* 
outlines
*/

void RSGL_drawLineF(RSGL_pointF p1, RSGL_pointF p2, u32 thickness, RSGL_color c) {
    RSGL_args.lineWidth = thickness;
    
    RSGL_point3DF center = {RSGL_GET_WORLD_POINT((p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, 0.0f)};
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);

    float points[] = {RSGL_GET_FINAL_POINT(p1.x, p1.y, 0.0f), RSGL_GET_FINAL_POINT(p2.x, p2.y, 0.0f)};
    float texPoints[] = {0, 0.0f,          0, 0.0f};

    RSGL_basicDraw(RSGL_LINES_2D, (float*)points, (float*)texPoints, c, 2);
}

void RSGL_drawTriangleFOutline(RSGL_triangleF t, u32 thickness, RSGL_color c) {
    RSGL_args.lineWidth = thickness;
    RSGL_point3DF center = {RSGL_GET_WORLD_POINT(t.p3.x, (t.p3.y + t.p1.y) / 2.0f, 0)};
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);

    float points[] = {RSGL_GET_FINAL_POINT(t.p3.x, t.p3.y, 0.0f), 
                        RSGL_GET_FINAL_POINT(t.p1.x, t.p1.y, 0.0f),     
                        RSGL_GET_FINAL_POINT(t.p1.x, t.p1.y, 0.0f), 
                    RSGL_GET_FINAL_POINT(t.p2.x, t.p2.y, 0.0f),     
                    RSGL_GET_FINAL_POINT(t.p2.x, t.p2.y, 0.0f),     
                    RSGL_GET_FINAL_POINT(t.p3.x, t.p3.y, 0.0f)};
    
    float texCoords[18];

    RSGL_basicDraw(RSGL_LINES_2D, (float*)points, texCoords, c, 6);
}
void RSGL_drawRectFOutline(RSGL_rectF r, u32 thickness, RSGL_color c) {
    RSGL_point3DF oCenter = RSGL_args.center;

    RSGL_center((RSGL_point3DF){RSGL_GET_WORLD_POINT(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f)});
    RSGL_drawLineF((RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x + r.w, r.y}, thickness, c);

    RSGL_center((RSGL_point3DF){RSGL_GET_WORLD_POINT(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f)});
    RSGL_drawLineF((RSGL_pointF){r.x, r.y}, (RSGL_pointF){r.x, r.y + r.h}, thickness, c);

    RSGL_center((RSGL_point3DF){RSGL_GET_WORLD_POINT(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f)});
    RSGL_drawLineF((RSGL_pointF){r.x, r.y + r.h}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness, c);

    RSGL_center((RSGL_point3DF){RSGL_GET_WORLD_POINT(r.x + (r.w / 2.0f), r.y + (r.h / 2.0f), 0.0f)});
    RSGL_drawLineF((RSGL_pointF){r.x + r.w, r.y}, (RSGL_pointF){r.x + r.w, r.y + r.h}, thickness, c);

    RSGL_center(oCenter);
}
void RSGL_drawRoundRectFOutline(RSGL_rectF r, RSGL_point rounding, u32 thickness, RSGL_color c) {
    RSGL_drawRectF((RSGL_rectF) {r.x + (rounding.x/2), r.y, r.w - rounding.x, (int)(thickness + !thickness)}, c);
    RSGL_drawRectF((RSGL_rectF) {r.x + (rounding.x/2), r.y + r.h, r.w - rounding.x, (int)(thickness + !thickness)}, c);    
    RSGL_drawRectF((RSGL_rectF) {r.x, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y}, c);
    RSGL_drawRectF((RSGL_rectF) {r.x + r.w, r.y + (rounding.y/2), (int)(thickness + !thickness),  r.h - rounding.y}, c);

    RSGL_drawArcFOutline((RSGL_rectF) {r.x, r.y, rounding.x, rounding.y}, (RSGL_pointF){180, 270}, thickness, c);
    RSGL_drawArcFOutline((RSGL_rectF) {r.x + (r.w - rounding.x), r.y, rounding.x, rounding.y}, (RSGL_pointF){90, 180}, thickness, c);
    RSGL_drawArcFOutline((RSGL_rectF) {r.x + (r.w - rounding.x), r.y  + (r.h - rounding.y) - 1, rounding.x, rounding.y + 2}, (RSGL_pointF){0, 90}, thickness, c);
    RSGL_drawArcFOutline((RSGL_rectF) {r.x + 1, r.y  + (r.h - rounding.y) - 1,  rounding.x, rounding.y + 2}, (RSGL_pointF){270, 360}, thickness, c);
}

void RSGL_drawPolygonFOutlinePro(RSGL_rectF o, u32 sides, RSGL_pointF arc, RSGL_color c) {
    static float verts[360 * 2 * 3];
    static float texCoords[360 * 2 * 2];

    RSGL_point3DF center = (RSGL_point3DF) {RSGL_GET_WORLD_POINT(o.x + (o.w / 2.0f), o.y + (o.h / 2.0f), 0.0f)};
    RSGL_MATRIX matrix = RSGL_initDrawMatrix(center);

    o = (RSGL_rectF){o.x + (o.w / 2), o.y + (o.h / 2), o.w / 2, o.h / 2};
    
    float displacement = 360.0f / (float)sides;
    float centralAngle = displacement * arc.x;

    i32 i;
    u32 j;
    size_t index = 0;

    for (i = arc.x; i < arc.y; i++) {
        for (j = 0; j < 2; j++) {
            memcpy(verts + index, (float[3]) {
                        RSGL_GET_FINAL_POINT(
                            o.x + (sinf(DEG2RAD * centralAngle) * o.w),
                            o.y + (cosf(DEG2RAD * centralAngle) * o.h),
                            (0.0))
                        }, sizeof(float) * 3);
            
            if (!j) centralAngle += displacement;
            index += 3;
        }
    }

    RSGL_basicDraw(RSGL_LINES_2D, verts, texCoords, c, index / 3);
}

void RSGL_drawPolygonFOutline(RSGL_rectF o, u32 sides, u32 thickness, RSGL_color c) {
    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlinePro(o, sides, (RSGL_pointF){0, (int)sides}, c);
}
void RSGL_drawArcFOutline(RSGL_rectF o, RSGL_pointF arc, u32 thickness, RSGL_color color) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlinePro(o, verts, arc, color);
}
void RSGL_drawCircleFOutline(RSGL_circleF c, u32 thickness, RSGL_color color) {
    float verts = ((2 * M_PI * c.d) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlinePro((RSGL_rectF){c.x, c.y, c.d, c.d}, verts, (RSGL_pointF){0, verts}, color);
}
void RSGL_drawOvalFOutline(RSGL_rectF o, u32 thickness, RSGL_color c) {
    float verts = ((2 * M_PI * ((o.w + o.h) / 2.0f)) / 10);
    verts = (verts > 360 ? 360 : verts);

    RSGL_args.lineWidth = thickness;
    RSGL_drawPolygonFOutlinePro(o, verts, (RSGL_pointF){0, verts}, c);
}

#ifndef RSGL_NO_STB_IMAGE
RSGL_image RSGL_drawImage(const char* image, RSGL_rect r) {
    RSGL_image img;
    img.tex = 0;

    #ifndef RSGL_NO_SAVE_IMAGE
    static size_t images_comp = 0;

    if (images_comp == 0) {
        RSGL_images = (RSGL_image*)RSGL_MALLOC(sizeof(RSGL_image) * RSGL_INIT_IMAGES);
        images_comp = RSGL_INIT_IMAGES;
    }

    if (RSGL_images_len) {
        size_t i; 
        for (i = 0; i < RSGL_images_len; i++) {
            if (RSGL_cstr_equal(image, RSGL_images[i].file)) {
                img.tex = RSGL_images[i].tex;
                break;
            }
        }
    }
    #endif /* RSGL_NO_SAVE_IMAGE */

    if (img.tex == 0) {
        i32 c;
        u8* bitmap = stbi_load(image, (int*)&img.srcSize.w, (int*)&img.srcSize.h, &c, 0);
        
        img.tex = RSGL_renderCreateTexture(bitmap, (RSGL_area){img.srcSize.w, img.srcSize.h}, c);

        RSGL_FREE(bitmap);

        #ifndef RSGL_NO_SAVE_IMAGE
        if (RSGL_images_len + 1 > images_comp) {
            RSGL_images = (RSGL_image*)RSGL_REALLOC(RSGL_images, sizeof(RSGL_image) * (RSGL_NEW_IMAGES + images_comp));
            images_comp += RSGL_NEW_IMAGES;
        }

        strcpy(img.file, image);

        RSGL_images[RSGL_images_len] = img;
        RSGL_images_len++;
        #endif
    }

    if (r.w || r.h) {
        RSGL_texture tex = RSGL_args.texture;
        RSGL_setTexture(img.tex);

        RSGL_drawRect(r, RSGL_RGB(255, 255, 255));

        RSGL_args.texture = tex;
    }

    return img;
}
#endif

#ifndef RSGL_NO_TEXT

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

i32 RSGL_loadFont(const char* font) {
    u32 i;
    for (i = 0; i < RSGL_font.len; i++) 
        if (RSGL_font.fonts[i].name == font)
            return i;
    
    if (access(font, 0)) {
        printf("RSGL_loadFont File %s does not exist.\n", font);
        return -1;
    }

    if (RSGL_font.len == RSGL_font.cap) {
        RSGL_font.cap += RSGL_NEW_FONTS;

        RSGL_fontData* nFonts = (RSGL_fontData*)RSGL_MALLOC(sizeof(RSGL_fontData) * RSGL_font.cap);
        memcpy(nFonts, RSGL_font.fonts, sizeof(RSGL_fontData) * RSGL_font.len);
        RSGL_FREE(RSGL_font.fonts);

        RSGL_font.fonts = nFonts;
    }


    RSGL_font.fonts[RSGL_font.len].name = (char*)font; 
    RSGL_font.fonts[RSGL_font.len].f = RFont_font_init(font);
    RSGL_font.len++;

    return RSGL_font.len - 1;
}

void RSGL_setFont(i32 font) {
    if (font == -1) {
        printf("RSGL_setFont : invalid font\n");
        return;
    }

    RSGL_font.f = RSGL_font.fonts[font].f;
}

void RSGL_setRFont(RFont_font* font) {
    RSGL_font.f = font;
}

void RSGL_drawText_len(const char* text, size_t len, RSGL_circle c, RSGL_color color) {
    if (text == NULL || RSGL_font.f == NULL)
        return;

    RFont_set_color(color.r / 255.0f, color.b / 255.0f, color.g / 255.0f, color.a / 255.0f);
    RFont_draw_text_len(RSGL_font.f, text, len, c.x, c.y, c.d, 0.0f);
}

void RSGL_drawText(const char* text, RSGL_circle c, RSGL_color color) {
    RSGL_drawText_len(text, 0, c, color);
}

RSGL_circle RSGL_alignText(const char* str, RSGL_circle c, RSGL_rectF larger, u8 alignment) {
    return RSGL_alignText_len(str, 0, c, larger, alignment);
}

RSGL_circle RSGL_alignText_len(const char* str, size_t str_len, RSGL_circle c, RSGL_rectF larger, u8 alignment) {
    RSGL_area area = RSGL_textArea(str, c.d, str_len);
    
    RSGL_rectF smaller = RSGL_RECTF(c.x, c.y, area.w, c.d);
    RSGL_rectF r = RSGL_alignRectF(larger, smaller, alignment);

    return RSGL_CIRCLE(r.x, r.y + (c.d / 4), r.h);
}

RSGL_area RSGL_textArea(const char* text, u32 fontSize, size_t textEnd) {
    if (RSGL_font.f == NULL)
        return RSGL_AREA(0, 0);
    
    return RFont_text_area_len(RSGL_font.f, text, textEnd, fontSize, 0, 0.0);
}

RSGL_area RSGL_textLineArea(const char* text, u32 fontSize, size_t textEnd, size_t line) {
    return RFont_text_area_len(RSGL_font.f, text, textEnd, fontSize, line, 0.0);
}

RSGL_color RFontcolor = RSGL_RGBA(0, 0, 0, 0);
void RFont_render_set_color(float r, float g, float b, float a) {
    RFontcolor = RSGL_RGBA(r * 255, g * 255, b * 255, a * 255);
}

void RFont_render_text(RFont_texture atlas, float* verts, float* tcoords, size_t nverts) {
    RSGL_drawArgs save = RSGL_args;
    RSGL_rotate(RSGL_POINT3D(0, 0, 0));
    RSGL_setTexture(atlas);
    RSGL_basicDraw(RSGL_TRIANGLES_2D_BLEND, verts, tcoords, RFontcolor, nverts);
    RSGL_args = save;
}

void RFont_render_init(void) { }

void RFont_render_free(RFont_texture atlas) {
   RSGL_renderDeleteTexture(atlas);
}

void RFont_render_legacy(u8 legacy) { RSGL_UNUSED(legacy); }
#endif /* RSGL_NO_TEXT */

/* 
******
RSGL_Other
******
*/

/* collision detection */
bool RSGL_circleCollidePoint(RSGL_circle c, RSGL_point p) { return RSGL_circleCollideRect(c, (RSGL_rect) {p.x, p.y, 1, 1}); }
bool RSGL_circleCollidePointF(RSGL_circleF c, RSGL_pointF p) { return RSGL_circleCollideRectF(c, (RSGL_rectF) {p.x, p.y, 1, 1}); }

bool RSGL_circleCollideRectF(RSGL_circleF c, RSGL_rectF r) {
    // test cords
    float testX = c.x; 
    float testY = c.y;

    // fill cords based on x/ys of the shapes
    if (c.x < r.x)
      testX = r.x;  

    else if (c.x > r.x+r.w) 
      testX = r.x-r.w;

    if (c.y < r.y)  
      testY = r.y;  

    else if (c.y > r.y+r.h)
      testY = r.y+r.h; 
    
    // check
    return ( sqrt( ( (c.x - testX) * (c.x - testX) ) + ( (c.y - testY) * (c.y - testY) ) )  <= (c.d/2) );
}
bool RSGL_circleCollideRect(RSGL_circle c, RSGL_rect r) { return RSGL_circleCollideRectF(RSGL_CIRCLEF(c.x, c.y, c.d), RSGL_RECTF(r.x, r.y, r.w, r.h)); }

bool RSGL_circleCollideF(RSGL_circleF cir, RSGL_circleF cir2) {
    float distanceBetweenCircles = (float) sqrt(
        (cir2.x - cir.x) * (cir2.x - cir.x) + 
        (cir2.y - cir.y) * (cir2.y - cir.y)
    );

    return !(distanceBetweenCircles > (cir.d/2) + (cir2.d/2)); // check if there is a collide
}

bool RSGL_circleCollide(RSGL_circle cir, RSGL_circle cir2) { return RSGL_circleCollideF(RSGL_CIRCLEF(cir.x, cir.y, cir.d), RSGL_CIRCLEF(cir2.x, cir2.y, cir2.d)); }

bool RSGL_rectCollidePoint(RSGL_rect r, RSGL_point p){ return (p.x >= r.x &&  p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h); }
bool RSGL_rectCollide(RSGL_rect r, RSGL_rect r2){ return (r.x + r.w >= r2.x && r.x <= r2.x + r2.w && r.y + r.h >= r2.y && r.y <= r2.y + r2.h); }
bool RSGL_pointCollide(RSGL_point p, RSGL_point p2){ return (p.x == p2.x && p.y == p2.y); }

bool RSGL_rectCollidePointF(RSGL_rectF r, RSGL_pointF p){ return (p.x >= r.x &&  p.x <= r.x + r.w && p.y >= r.y && p.y <= r.y + r.h); }
bool RSGL_rectCollideF(RSGL_rectF r, RSGL_rectF r2){ return (r.x + r.w >= r2.x && r.x <= r2.x + r2.w && r.y + r.h >= r2.y && r.y <= r2.y + r2.h); }
bool RSGL_pointCollideF(RSGL_pointF p, RSGL_pointF p2){ return (p.x == p2.x && p.y == p2.y); }

/* Multiply the current matrix by a translation matrix */
RSGL_MATRIX RSGL_translatef(RSGL_MATRIX* matrix, float x, float y, float z) {
    RSGL_MATRIX matTranslation = { 
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            x,    y,    z,    1.0f
        }
    };

    /* NOTE: We transpose matrix with multiplication order */
    return RSGL_matrixMultiply(matTranslation.m, matrix->m);
}

/* Multiply the current matrix by a rotation matrix */
RSGL_MATRIX RSGL_rotatef(RSGL_MATRIX* matrix, float angle, float x, float y, float z) {
	/* Axis vector (x, y, z) normalization */
	float lengthSquared = x * x + y * y + z * z;
	if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f)) {
		float inverseLength = 1.0f / sqrtf(lengthSquared);
		x *= inverseLength;
		y *= inverseLength;
		z *= inverseLength;
	}

	/* Rotation matrix generation */
	float sinres = sinf(DEG2RAD * angle);
	float cosres = cosf(DEG2RAD * angle);
	float t = 1.0f - cosres;

	float matRotation[16] = 
				{
					x * x * t + cosres,   	  	y * x * t + z * sinres,   	z * x * t - y * sinres,   	0.0f,
					x * y * t - z * sinres,   	y * y * t + cosres,   		z * y * t + x * sinres,   	0.0f,
					x * z * t + y * sinres,   	y * z * t - x * sinres,  	z * z * t + cosres,   		0.0f,
					0.0f,   					0.0f,   					0.0f,   					1.0f
				};

	return RSGL_matrixMultiply(matRotation, matrix->m);
}

RSGL_MATRIX RSGL_matrixMultiply(float left[16], float right[16]) {
    return (RSGL_MATRIX) {
        {
            left[0] * right[0] + left[1] * right[4] + left[2] * right[8] + left[3] * right[12],
            left[0] * right[1] + left[1] * right[5] + left[2] * right[9] + left[3] * right[13],
            left[0] * right[2] + left[1] * right[6] + left[2] * right[10] + left[3] * right[14],
            left[0] * right[3] + left[1] * right[7] + left[2] * right[11] + left[3] * right[15],
            left[4] * right[0] + left[5] * right[4] + left[6] * right[8] + left[7] * right[12],
            left[4] * right[1] + left[5] * right[5] + left[6] * right[9] + left[7] * right[13],
            left[4] * right[2] + left[5] * right[6] + left[6] * right[10] + left[7] * right[14],
            left[4] * right[3] + left[5] * right[7] + left[6] * right[11] + left[7] * right[15],
            left[8] * right[0] + left[9] * right[4] + left[10] * right[8] + left[11] * right[12],
            left[8] * right[1] + left[9] * right[5] + left[10] * right[9] + left[11] * right[13],
            left[8] * right[2] + left[9] * right[6] + left[10] * right[10] + left[11] * right[14],
            left[8] * right[3] + left[9] * right[7] + left[10] * right[11] + left[11] * right[15],
            left[12] * right[0] + left[13] * right[4] + left[14] * right[8] + left[15] * right[12],
            left[12] * right[1] + left[13] * right[5] + left[14] * right[9] + left[15] * right[13],
            left[12] * right[2] + left[13] * right[6] + left[14] * right[10] + left[15] * right[14],
            left[12] * right[3] + left[13] * right[7] + left[14] * right[11] + left[15] * right[15]
        }
    };
}
#endif /* RSGL_IMPLEMENTATION */
