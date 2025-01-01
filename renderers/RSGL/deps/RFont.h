/*
* Copyright (c) 2021-24 ColleagueRiley ColleagueRiley@gmail.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following r estrictions:
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
preprocessor args

make sure 

** #define RFONT_IMPLEMENTATION ** - include function defines

is in exactly one of your files or arguments

#define RFONT_NO_OPENGL - do not define graphics functions (that use opengl)
#define RFONT_NO_STDIO - do not include stdio.h
#define RFONT_EXTERNAL_STB - load stb_truetype from stb_truetype.h instead of using the internal version
#define RFONT_NO_GRAPHICS - do not include any graphics functions at all
#define RFONT_RENDER_RGL - use RGL functions for rendering
#define RFONT_RENDER_LEGACY - use opengl legacy functions for rendering (if RGL is not chosen)
-- NOTE: By default, opengl 3.3 vbos are used for rendering --
*/

/*
credits :

stb_truetype.h - a dependency for RFont, most of (a slightly motified version of) stb_truetype.h is included directly into RFont.h
http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ - UTF-8 decoding function
fontstash - fontstash was used as a refference for some parts
*/

/*

... = [add code here]

BASIC TEMPLATE :
#define RFONT_IMPLEMENTATION
#include "RFont.h"

...

int main () {
   ...

   RFont_init(window_width, window_height);

   RFont_font* font = RFont_font_init("font.ttf");

   while (1) {
      ...
      RFont_draw_text(font, "text", 100, 100, 20);
      ...
   }

   RFont_font_free(font);
   RFont_close();
   ...
}
*/

#ifndef RFONT_NO_STDIO
#include <stdio.h>
#endif

#ifndef RFONT_MALLOC
#include <stdlib.h>
#define RFONT_MALLOC malloc
#define RFONT_FREE free
#endif

#include <math.h>
#include <assert.h>
#include <string.h>

#if !defined(u8)
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
	typedef u8 b8;
#endif

/* 
You can define these yourself if 
you want to change anything
*/

#ifndef RFont_texture
typedef u32 RFont_texture;
#endif

#ifndef RFONT_MAX_GLYPHS
#define RFONT_MAX_GLYPHS 652
#endif

#ifndef RFONT_ATLAS_WIDTH
#define RFONT_ATLAS_WIDTH 6000
#endif

#ifndef RFONT_ATLAS_HEIGHT
#define RFONT_ATLAS_HEIGHT 400
#endif

#ifndef RFONT_INIT_TEXT_SIZE
#define RFONT_INIT_TEXT_SIZE 500
#endif

#ifndef RFONT_INIT_VERTS
#define RFONT_INIT_VERTS 1024 * 600
#endif

#ifndef RFONT_TEXTFORMAT_MAX_SIZE
   #define RFONT_TEXTFORMAT_MAX_SIZE 923
#endif

#ifndef RFONT_VSNPRINTF
#define RFONT_VSNPRINTF vsnprintf
#endif

#ifndef RFONT_UNUSED
#define RFONT_UNUSED(x) (void) (x);
#endif

/* make sure RFont declares aren't declared twice */
#ifndef RFONT_H
#define RFONT_H

#ifndef RFont_area
typedef struct { u32 w, h; } RFont_area;
#endif

typedef struct RFont_font RFont_font;

typedef struct {
   u32 codepoint; /* the character (for checking) */
   size_t size; /* the size of the glyph */
   i32 x, x2;  /* coords of the character on the texture */

   /* source glyph data */
   i32 src;
   float w, h, x1, y1, advance;
} RFont_glyph;

/**
 * @brief Sets the framebuffer size AND runs the graphics init function.
 * @param width The framebuffer width.
 * @param height The framebuffer height.
*/
inline void RFont_init(size_t width, size_t height);
/**
 * @brief Frees data allocated by the RFont for the RFont
*/
inline void RFont_close(void);
/**
 * @brief Just updates the framebuffer size.
 * @param width The framebuffer width.
 * @param height The framebuffer height.
*/
inline void RFont_update_framebuffer(size_t width, size_t height);

#ifndef RFONT_NO_STDIO
/**
 * @brief Init font stucture with a TTF file path.
 * @param font_name The TTF file path.
 * @return The `RFont_font` created using the TTF file data.
*/
inline RFont_font* RFont_font_init(const char* font_name);
#endif


/**
 * @brief Init font stucture with raw TTF data.
 * @param font_data The raw TTF data.
 * @param auto_free If the memory should be automatically freed by `RFont_font_free`.
 * @return The `RFont_font` created from the data.
*/
inline RFont_font* RFont_font_init_data(u8* font_data, b8 auto_free);

/**
 * @brief Free data from the font stucture, including the stucture itself
 * @param font The font stucture to free
*/
inline void RFont_font_free(RFont_font* font);

/**
 * @brief Add a character to the font's atlas.
 * @param font The font to use.
 * @param ch The character to add to the atlas.
 * @param size The size of the character.
 * @return The `RFont_glyph` created from the data and added to the atlas.
*/
inline RFont_glyph RFont_font_add_char(RFont_font* font, char ch, size_t size);

#ifndef RFONT_NO_FMT
/**
 * @brief Formats a string.
 * @param string The source string
 * @param ... format data
 * @return The formatted string 
*/
inline const char* RFont_fmt(const char* string, ...);
#endif

/**
 * @brief Add a string to the font's atlas.
 * @param font The font to use.
 * @param ch The character to add to the atlas.
 * @param sizes The supported sizes of the character.
 * @param sizeLen length of the size array
*/
inline void RFont_font_add_string(RFont_font* font, const char* string, size_t* sizes, size_t sizeLen);

/**
 * @brief Add a string to the font's atlas based on a given string length.
 * @param font The font to use.
 * @param ch The character to add to the atlas.
 * @param strLen length of the string
 * @param sizes The supported sizes of the character.
 * @param sizeLen length of the size array
*/
inline void RFont_font_add_string_len(RFont_font* font, const char* string, size_t strLen, size_t* sizes, size_t sizeLen);

/**
 * @brief Get the area of the text based on the size using the font.
 * @param font The font stucture to use for drawing
 * @param text The string to draw 
 * @param size The size of the text
 * @return The area of the text based on the size
*/
inline RFont_area RFont_text_area(RFont_font* font, const char* text, u32 size);

/**
 * @brief Get the area of the text based on the size using the font, using a given length.
 * @param font The font stucture to use for drawing
 * @param text The string to draw 
 * @param size The size of the text
 * @param spacing The spacing of the text
 * @return The area of the text based on the size
*/
inline RFont_area RFont_text_area_spacing(RFont_font* font, const char* text, float spacing, u32 size);

/**
 * @brief Get the area of the text based on the size using the font, using a given length.
 * @param font The font stucture to use for drawing
 * @param text The string to draw 
 * @param len The length of the string
 * @param size The size of the text
 * @param stopNL the number of \n s until it stops (0 = don't stop until the end)
 * @param spacing The spacing of the text
 * @return The area of the text based on the size
*/
inline RFont_area RFont_text_area_len(RFont_font* font, const char* text, size_t len, u32 size, size_t stopNL, float spacing);

/**
 * @brief Draw a text string using the font.
 * @param font The font stucture to use for drawing
 * @param text The string to draw 
 * @param x The x position of the text
 * @param y The y position of the text
 * @param size The size of the text
 * @return The area of the text based on the size
*/
inline RFont_area RFont_draw_text(RFont_font* font, const char* text, float x, float y, u32 size);

/**
 * @brief Draw a text string using the font and a given spacing.
 * @param font The font stucture to use for drawing
 * @param text The string to draw 
 * @param x The x position of the text
 * @param y The y position of the text
 * @param size The size of the text
 * @param spacing The spacing of the text
 * @return The area of the text based on the size
*/
inline RFont_area RFont_draw_text_spacing(RFont_font* font, const char* text, float x, float y, u32 size, float spacing);

/**
 * @brief Draw a text string using the font using a given length and a given spacing.
 * @param font The font stucture to use for drawing
 * @param text The string to draw 
 * @param len The length of the string
 * @param x The x position of the text
 * @param y The y position of the text
 * @param size The size of the text
 * @param spacing The spacing of the text
 * @return The area of the text based on the size
*/
inline RFont_area RFont_draw_text_len(RFont_font* font, const char* text, size_t len, float x, float y, u32 size, float spacing);

#define RFont_set_color RFont_render_set_color

#ifndef RFONT_NO_GRAPHICS
/* 
    if you do not want to use opengl (or want to create your own implemntation of these functions), 
    you'll have to define these yourself 
    and add `#define RFONT_NO_OPENGL`
*/
inline void RFont_render_set_color(float r, float g, float b, float a); /* set the current rendering color */
inline void RFont_render_init(void); /* any initalizations the renderer needs to do */
inline RFont_texture RFont_create_atlas(u32 atlasWidth, u32 atlasHeight); /* create a bitmap texture based on the given size */
inline void RFont_bitmap_to_atlas(RFont_texture atlas, u8* bitmap, float x, float y, float w, float h); /* add the given bitmap to the texture based on the given coords and size data */
inline void RFont_render_text(RFont_texture atlas, float* verts, float* tcoords, size_t nverts); /* render the text, using the vertices, atlas texture, and texture coords given. */
inline void RFont_render_free(RFont_texture atlas); /* free any memory the renderer might need to free */

/* (if modern opengl is being used) switch to rendering using opengl legacy or not */
inline void RFont_render_legacy(u8 legacy);
#endif

#endif /* RFONT_H */

#ifdef RFONT_IMPLEMENTATION

#ifdef RFONT_EXTERNAL_STB
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#endif

#ifndef RFONT_GET_TEXPOSX
#define RFONT_GET_TEXPOSX(x) (float)((float)(x) / (float)(RFONT_ATLAS_WIDTH))
#define RFONT_GET_TEXPOSY(y) (float)((float)(y) / (float)(RFONT_ATLAS_HEIGHT))
#endif

#ifndef RFONT_GET_WORLD_X
#define RFONT_GET_WORLD_X(x, w) (float)((x) / (((w) / 2.0f)) - 1.0f)
#define RFONT_GET_WORLD_Y(y, h) (float)(1.0f - ((y) / ((h) / 2.0f)))
#endif

/* 
stb defines required by RFont

you probably don't care about this part if you're reading just the RFont code
*/

#ifndef RFONT_EXTERNAL_STB
// private structure
typedef struct
{
   unsigned char *data;
   int cursor;
   int size;
} stbtt__buf;

typedef struct stbtt_fontinfo stbtt_fontinfo;

struct stbtt_fontinfo
{
   void           * userdata;
   unsigned char  * data;              // pointer to .ttf file
   int              fontstart;         // offset of start of font

   int numGlyphs;                     // number of glyphs, needed for range checking

   int loca,head,glyf,hhea,hmtx,kern,gpos,svg; // table locations as offset from start of .ttf
   int index_map;                     // a cmap mapping for our chosen character encoding
   int indexToLocFormat;              // format needed to map from glyph index to glyph

   stbtt__buf cff;                    // cff font data
   stbtt__buf charstrings;            // the charstring index
   stbtt__buf gsubrs;                 // global charstring subroutines index
   stbtt__buf subrs;                  // private charstring subroutines index
   stbtt__buf fontdicts;              // array of font dicts
   stbtt__buf fdselect;               // map from glyph to fontdict
};

#ifdef STBTT_STATIC
#define STBTT_DEF static
#else
#define STBTT_DEF extern inline
#endif

STBTT_DEF i16 ttSHORT(u8 *p);
STBTT_DEF u16 ttUSHORT(u8 *p); 
STBTT_DEF u32 ttULONG(u8 *p);

STBTT_DEF int stbtt_InitFont(stbtt_fontinfo *info, const unsigned char *data, int offset);

STBTT_DEF unsigned char* stbtt_GetGlyphBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int *width, int *height, int *xoff, int *yoff);

STBTT_DEF int stbtt_FindGlyphIndex(const stbtt_fontinfo *info, int unicode_codepoint);

STBTT_DEF int  stbtt_GetGlyphKernAdvance(const stbtt_fontinfo *info, int glyph1, int glyph2);
STBTT_DEF int  stbtt_GetGlyphBox(const stbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1);
#endif /* RFONT_EXTERNAL_STB */

/* 
END of stb defines required by RFont

you probably care about this part 
*/

#ifndef RFONT_NO_FMT
#include <stdarg.h>

const char* RFont_fmt(const char* string, ...) {
   static char output[RFONT_TEXTFORMAT_MAX_SIZE];

   va_list args;
   va_start(args, string);
   
   RFONT_VSNPRINTF(output, RFONT_TEXTFORMAT_MAX_SIZE, string, args);
   va_end(args);

   return output;
}
#endif

struct RFont_font {
   stbtt_fontinfo info; /* source stb font */
   b8 free_font_memory;
   float fheight; /* font height from stb */
   float descent; /* font descent */
   float numOfLongHorMetrics;
   float space_adv;

   RFont_glyph glyphs[RFONT_MAX_GLYPHS]; /* glyphs */
   size_t glyph_len;

   RFont_texture atlas; /* atlas texture */
   float atlasX; /* the current x position inside the atlas */
};

size_t RFont_width = 0, RFont_height = 0;

float* RFont_verts;
float* RFont_tcoords;

RFont_font* font2;

void RFont_update_framebuffer(size_t width, size_t height) {
   /* set size of the framebuffer (for rendering later on) */
   RFont_width = width;
   RFont_height = height;
}

void RFont_init(size_t width, size_t height) {
    RFont_update_framebuffer(width, height);

    #ifndef RFONT_NO_GRAPHICS
    /* init any rendering stuff that needs to be initalized (eg. vbo objects) */
    RFont_render_init();
    #endif

   RFont_verts = RFONT_MALLOC(sizeof(float) * RFONT_INIT_VERTS);
   RFont_tcoords = RFONT_MALLOC(sizeof(float) * RFONT_INIT_VERTS);
}

#ifndef RFONT_NO_STDIO
RFont_font* RFont_font_init(const char* font_name) {
   FILE* ttf_file = fopen(font_name, "rb");

   fseek(ttf_file, 0U, SEEK_END);
   size_t size = ftell(ttf_file);

   char* ttf_buffer = (char*)RFONT_MALLOC(sizeof(char) * size); 
   fseek(ttf_file, 0U, SEEK_SET);

   size_t out = fread(ttf_buffer, 1, size, ttf_file);
   RFONT_UNUSED(out)


   return RFont_font_init_data((u8*)ttf_buffer, 1);
}
#endif

RFont_font* RFont_font_init_data(u8* font_data, b8 auto_free) {
   RFont_font* font = (RFont_font*)RFONT_MALLOC(sizeof(RFont_font));

   stbtt_InitFont(&font->info, font_data, 0);

   font->fheight = ttSHORT(font->info.data + font->info.hhea + 4) - ttSHORT(font->info.data + font->info.hhea + 6);
   font->descent = ttSHORT(font->info.data + font->info.hhea + 6);

   font->numOfLongHorMetrics = ttUSHORT(font->info.data + font->info.hhea + 34);
   font->space_adv = ttSHORT(font->info.data + font->info.hmtx + 4 * (u32)(font->numOfLongHorMetrics - 1));
 

   #ifndef RFONT_NO_GRAPHICS
   font->atlas = RFont_create_atlas(RFONT_ATLAS_WIDTH, RFONT_ATLAS_HEIGHT);
   #endif
   font->atlasX = 0;
   font->glyph_len = 0;

   font->free_font_memory = auto_free;
   
   return font;
}

void RFont_font_free(RFont_font* font) {
   #ifndef RFONT_NO_GRAPHICS
   RFont_render_free(font->atlas);
   #endif

   if (font->free_font_memory)
      RFONT_FREE(font->info.data);
   
   RFONT_FREE  (font);
}

void RFont_close(void) {
   RFONT_FREE(RFont_verts);
   RFONT_FREE(RFont_tcoords);
}


/*
decode utf8 character to codepoint 
*/

// Copyright (c) 2008-2010 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

#define RFONT_UTF8_ACCEPT 0
#define RFont_UTF8_REJECT 12

inline static u32 RFont_decode_utf8(u32* state, u32* codep, u32 byte);

static u32 RFont_decode_utf8(u32* state, u32* codep, u32 byte) {
   static const uint8_t utf8d[] = {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
      8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
      0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
      0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
      0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
      1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
      1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
      1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
      1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
   };

   uint32_t type = utf8d[byte];

   *codep = (*state != RFONT_UTF8_ACCEPT) ?
      (byte & 0x3fu) | (*codep << 6) :
      (0xff >> type) & (byte);

   *state = utf8d[256 + *state * 16 + type];
   return *state;
}

void RFont_font_add_string(RFont_font* font, const char* string, size_t* sizes, size_t sizeLen) {
   RFont_font_add_string_len(font, string, 0, sizes, sizeLen);
}

void RFont_font_add_string_len(RFont_font* font, const char* string, size_t strLen, size_t* sizes, size_t sizeLen) {
   u32 i;
   char* str;
   for (str = (char*)string; (!strLen || (size_t)(str - string) < strLen) && *str; str++)
      for (i = 0; i < sizeLen; i++)
         RFont_font_add_char(font, *str, sizes[i]);
}


RFont_glyph RFont_font_add_char(RFont_font* font, char ch, size_t size) {
   static u32 utf8state = 0, codepoint = 0; 

   if (RFont_decode_utf8(&utf8state, &codepoint, (u8)ch) != RFONT_UTF8_ACCEPT)
      return (RFont_glyph){0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	u32 i;
   for (i = 0; i < font->glyph_len; i++)
      if (font->glyphs[i].codepoint == codepoint && font->glyphs[i].size == size)
         return font->glyphs[i];

   RFont_glyph* glyph = &font->glyphs[i];

   glyph->src = stbtt_FindGlyphIndex(&font->info, codepoint);

   if (glyph->src == 0 && font2 != NULL && font2->info.data != font->info.data) {
      stbtt_fontinfo saveInfo = font->info;

      RFont_font* fakeFont = font;
      fakeFont->info = font2->info;

      RFont_glyph g = RFont_font_add_char(fakeFont, 't', size);

      fakeFont->info = saveInfo;

      return g;
   }

   font->glyph_len++;

   i32 x0, y0, x1, y1, w = 0, h = 0;
   if (stbtt_GetGlyphBox(&font->info, glyph->src, &x0, &y0, &x1, &y1) == 0)
      return (RFont_glyph){0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

   float scale = ((float)size) / font->fheight;

   u8* bitmap =  stbtt_GetGlyphBitmapSubpixel(&font->info, 0, scale, 0.0f, 0.0f, glyph->src, &w, &h, 0, 0);

   glyph->w = (float)w;
   glyph->h = (float)h;

   glyph->codepoint = codepoint;
   glyph->size = size;
   glyph->x = font->atlasX;
   glyph->x2 = font->atlasX + glyph->w;
   glyph->x1 = floorf(x0 * scale);
   glyph->y1 = floor(-y1 * scale);

   #ifndef RFONT_NO_GRAPHICS
   RFont_bitmap_to_atlas(font->atlas, bitmap, font->atlasX, 0, glyph->w, glyph->h);
   #endif

   font->atlasX += glyph->w;

   RFONT_FREE(bitmap);

   i32 advanceX;
   
   if (glyph->src < font->numOfLongHorMetrics)
      advanceX = ttSHORT(font->info.data + font->info.hmtx + 4 * glyph->src);
   else
      advanceX = ttSHORT(font->info.data + font->info.hmtx + 4 * (u32)(font->numOfLongHorMetrics - 1));

   glyph->advance = advanceX * scale;

   return *glyph;
}

RFont_area RFont_text_area(RFont_font* font, const char* text, u32 size) {
   return RFont_text_area_len(font, text, 0, size, 0, 0.0f);
}

RFont_area RFont_text_area_spacing(RFont_font* font, const char* text, float spacing, u32 size) {
   return RFont_text_area_len(font, text, 0, size, 0, spacing);
}

RFont_area RFont_text_area_len(RFont_font* font, const char* text, size_t len, u32 size, size_t stopNL, float spacing) {
   float x = 0;
   size_t y = 1;

   char* str;

   float scale = (((float)size) / font->fheight);

   float space_adv = (scale * font->space_adv) / 2;
   
   for (str = (char*)text; (len == 0 || (size_t)(str - text) < len) && *str; str++) {        
      if (*str == '\n') { 
         if (y == stopNL)
            return (RFont_area){(u32)x, y * size};
         
         y++;
         x = 0;
         continue;
      }
      
      if (*str == ' ' || *str == '\t') {
         x += space_adv + spacing;
         continue;
      }

      RFont_glyph glyph = RFont_font_add_char(font,  *str, size);

      if (glyph.codepoint == 0 && glyph.size == 0)
         continue;
      
      x += (float)glyph.advance + spacing;
   }

   return (RFont_area){(u32)x, y * size};
}

RFont_area RFont_draw_text(RFont_font* font, const char* text, float x, float y, u32 size) {
   return RFont_draw_text_len(font, text, 0, x, y, size, 0.0f);
}

RFont_area RFont_draw_text_spacing(RFont_font* font, const char* text, float x, float y, u32 size, float spacing) {
   return RFont_draw_text_len(font, text, 0, x, y, size, spacing);
}

RFont_area RFont_draw_text_len(RFont_font* font, const char* text, size_t len, float x, float y, u32 size, float spacing) {
   float* verts = RFont_verts;
   float* tcoords = RFont_tcoords;

   float startX = x;
   float startY = y;
   
   y += size;

   u32 i = 0;
   u32 tIndex = 0;

   char* str;

   float scale = (((float)size) / font->fheight);
   float space_adv = (scale * font->space_adv) / 2;

   y -= (-font->descent * scale);

   for (str = (char*)text; (len == 0 || (size_t)(str - text) < len) && *str; str++) {        
      if (*str == '\n') { 
         x = startX;
         y += size;
         continue;
      }

      if (*str == ' ' || *str == '\t') {
         x += space_adv + spacing;
         continue;
      }

      RFont_glyph glyph = RFont_font_add_char(font, *str, size);

      if (glyph.codepoint == 0 && glyph.size == 0)
         continue;

      float realX = x + glyph.x1;
      float realY = y + glyph.y1;

      verts[i] = RFONT_GET_WORLD_X((i32)realX, RFont_width); 
      verts[i + 1] = RFONT_GET_WORLD_Y(realY, RFont_height);
      verts[i + 2] = 0;
      /*  */
      verts[i + 3] = RFONT_GET_WORLD_X((i32)realX, RFont_width);
      verts[i + 4] = RFONT_GET_WORLD_Y(realY + glyph.h , RFont_height);
      verts[i + 5] = 0;
      /*  */
      verts[i + 6] = RFONT_GET_WORLD_X((i32)(realX + glyph.w), RFont_width);
      verts[i + 7] = RFONT_GET_WORLD_Y(realY + glyph.h , RFont_height);
      verts[i + 8] = 0;
      /*  */
      /*  */
      verts[i + 9] = RFONT_GET_WORLD_X((i32)(realX + glyph.w), RFont_width);
      verts[i + 10] = RFONT_GET_WORLD_Y(realY, RFont_height);
      verts[i + 11] = 0;
      /*  */
      verts[i + 12] = RFONT_GET_WORLD_X((i32)realX, RFont_width); 
      verts[i + 13] = RFONT_GET_WORLD_Y(realY, RFont_height);
      verts[i + 14] = 0;
      /*  */

      verts[i + 15] = RFONT_GET_WORLD_X((i32)(realX + glyph.w), RFont_width);
      verts[i + 16] = RFONT_GET_WORLD_Y(realY + glyph.h , RFont_height);
      verts[i + 17] = 0;

      /* texture coords */

      //#if defined(RFONT_RENDER_LEGACY) || defined(RFONT_RENDER_RGL)
      tcoords[tIndex] = RFONT_GET_TEXPOSX(glyph.x);
      tcoords[tIndex + 1] = 0;
      //#endif

      /*  */
      tcoords[tIndex + 2] = RFONT_GET_TEXPOSX(glyph.x); 
      tcoords[tIndex + 3] = RFONT_GET_TEXPOSY(glyph.h);
      /*  */
      tcoords[tIndex + 4] = RFONT_GET_TEXPOSX(glyph.x2);
      tcoords[tIndex + 5] = RFONT_GET_TEXPOSY(glyph.h);
      /*  */
      /*  */
      tcoords[tIndex + 6] = RFONT_GET_TEXPOSX(glyph.x2);
      tcoords[tIndex + 7] = 0;
      /*  */
      tcoords[tIndex + 8] = RFONT_GET_TEXPOSX(glyph.x);
      tcoords[tIndex + 9] = 0;
      /*  */ 
      tcoords[tIndex + 10] = RFONT_GET_TEXPOSX(glyph.x2);
      tcoords[tIndex + 11] = RFONT_GET_TEXPOSY(glyph.h);

      i += 18;
      tIndex += 12;

      x += glyph.advance + spacing;
   }

   #ifndef RFONT_NO_GRAPHICS
   RFont_render_text(font->atlas, verts, tcoords, i / 3);
   #endif
   
   return (RFont_area){(u32)(x - startX), (u32)(y - startY) + (-font->descent * scale)};
}

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#if !defined(RFONT_NO_OPENGL) && !defined(RFONT_NO_GRAPHICS)

#if !defined(RFONT_RENDER_LEGACY) && !defined(RFONT_RENDER_RGL)
#define GL_GLEXT_PROTOTYPES
#endif

#ifndef GL_PERSPECTIVE_CORRECTION_HINT
#define GL_PERSPECTIVE_CORRECTION_HINT		0x0C50
#endif

#ifndef GL_TEXTURE_SWIZZLE_RGBA
#define GL_TEXTURE_SWIZZLE_RGBA           0x8E46
#endif

#ifndef GL_TEXTURE0
#define GL_TEXTURE0				0x84C0
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE			0x812F
#endif

#ifdef RFONT_DEBUG

#ifndef GL_DEBUG_TYPE_ERROR
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_OUTPUT                   0x92E0
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84 
#endif

void RFont_debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
    RFONT_UNUSED(source) RFONT_UNUSED(id) RFONT_UNUSED(severity) RFONT_UNUSED(length) RFONT_UNUSED(userParam)

    if (type != GL_DEBUG_TYPE_ERROR)
        return;

    printf("OpenGL Debug Message: %s\n", message);
}

void RFont_opengl_getError(void) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
         switch (err) {
            case GL_INVALID_ENUM:
                  printf("OpenGL error: GL_INVALID_ENUM\n");
                  break;
            case GL_INVALID_VALUE:
                  printf("OpenGL error: GL_INVALID_VALUE\n");
                  break;
            case GL_INVALID_OPERATION:
                  printf("OpenGL error: GL_INVALID_OPERATION\n");
                  break;
            case GL_STACK_OVERFLOW:
                  printf("OpenGL error: GL_STACK_OVERFLOW\n");
                  break;
            case GL_STACK_UNDERFLOW:
                  printf("OpenGL error: GL_STACK_UNDERFLOW\n");
                  break;	
            default:
                  printf("OpenGL error: Unknown error code 0x%x\n", err);
                  break;
         }
         exit(1);
    }
}

#endif

RFont_texture RFont_create_atlas(u32 atlasWidth, u32 atlasHeight) {
 #if defined(RFONT_DEBUG) && !defined(RFONT_RENDER_LEGACY)
   glEnable(GL_DEBUG_OUTPUT);
   #endif
   
   u32 id = 0;
   glEnable(GL_TEXTURE_2D);
   
   glBindTexture(GL_TEXTURE_2D, 0);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenTextures(1, &id);

   glBindTexture(GL_TEXTURE_2D, id);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   
   u8* data = (u8*)calloc(atlasWidth * atlasHeight * 4, sizeof(u8));

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

   RFONT_FREE(data);

   glBindTexture(GL_TEXTURE_2D, id);
	static GLint swizzleRgbaParams[4] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleRgbaParams);

   glBindTexture(GL_TEXTURE_2D, 0);
   return id;
}

#ifndef GL_UNPACK_ROW_LENGTH
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#endif


void RFont_push_pixel_values(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows);
void RFont_push_pixel_values(GLint alignment, GLint rowLength, GLint skipPixels, GLint skipRows) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipPixels);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, skipRows);
}

void RFont_bitmap_to_atlas(RFont_texture atlas, u8* bitmap, float x, float y, float w, float h) {
   glEnable(GL_TEXTURE_2D);
   
	GLint alignment, rowLength, skipPixels, skipRows;
   glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowLength);
	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skipPixels);
	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skipRows);
   
   #if !defined(RFONT_RENDER_LEGACY)
   glActiveTexture(GL_TEXTURE0 + atlas - 1);
   #endif

	glBindTexture(GL_TEXTURE_2D, atlas);

	RFont_push_pixel_values(1, w, 0, 0);

	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, GL_RED, GL_UNSIGNED_BYTE, bitmap);

	RFont_push_pixel_values(alignment, rowLength, skipPixels, skipRows);

   glBindTexture(GL_TEXTURE_2D, 0);
}

#if defined(RFONT_RENDER_RGL) && !defined(RFONT_CUSTOM_GL)

void RFont_render_set_color(float r, float g, float b, float a) {
   rglColor4f(r, g, b, a);
}

void RFont_render_text(RFont_texture atlas, float* verts, float* tcoords, size_t nverts) {
   glEnable(GL_TEXTURE_2D);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   glShadeModel(GL_SMOOTH);

   rglMatrixMode(RGL_MODELVIEW);
   rglLoadIdentity();
	rglPushMatrix();

   glDisable(GL_DEPTH_TEST);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_CULL_FACE);    

   glEnable(GL_BLEND);
   glEnable(GL_TEXTURE_2D);
   
   glActiveTexture(GL_TEXTURE0);

   rglSetTexture(atlas);
   
	rglBegin(RGL_TRIANGLES_2D);

	size_t i;
   size_t tIndex = 0;

	for (i = 0; i < (nverts * 3); i += 3) {
		rglTexCoord2f(tcoords[tIndex], tcoords[tIndex + 1]);
      tIndex += 2;
		
      rglVertex2f(verts[i], verts[i + 1]);
	}
	rglEnd();
	rglPopMatrix();
   
   rglSetTexture(0);
   glBindTexture(GL_TEXTURE_2D, 0);
   glEnable(GL_DEPTH_TEST);
}

void RFont_render_free(RFont_texture atlas) { glDeleteTextures(1, &atlas); }
void RFont_render_legacy(u8 legacy) { rglLegacy(legacy); }
void RFont_render_init() {}
#endif /* RFONT_RENDER_RGL */

#if defined(RFONT_RENDER_LEGACY) && !defined(RFONT_RENDER_RGL) && !defined(RFONT_CUSTOM_GL)

void RFont_render_set_color(float r, float g, float b, float a) {
   glColor4f(r, g, b, a);
}

void RFont_render_text(RFont_texture atlas, float* verts, float* tcoords, size_t nverts) {
   glEnable(GL_TEXTURE_2D);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   glShadeModel(GL_SMOOTH);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glDisable(GL_DEPTH_TEST);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_CULL_FACE);    

   glEnable(GL_BLEND);
   glEnable(GL_TEXTURE_2D);
   #if !defined(RFONT_RENDER_LEGACY)
   glActiveTexture(GL_TEXTURE0 + atlas - 1);
   #endif

   glBindTexture(GL_TEXTURE_2D, atlas);

	glPushMatrix();

	glBegin(GL_TRIANGLES);

	size_t i;
   size_t tIndex = 0;

	for (i = 0; i < (nverts * 3); i += 3) {
		glTexCoord2f(tcoords[tIndex], tcoords[tIndex + 1]);
		tIndex += 2;

      glVertex2f(verts[i], verts[i + 1]);
	}
	glEnd();
	glPopMatrix();

   glBindTexture(GL_TEXTURE_2D, 0);
   glEnable(GL_DEPTH_TEST);
}

void RFont_render_free(RFont_texture atlas) { glDeleteTextures(1, &atlas); }
void RFont_render_legacy(u8 legacy) { RFONT_UNUSED(legacy) }
void RFont_render_init() {}
#endif /* defined(RFONT_RENDER_LEGACY) && !defined(RFONT_RENDER_RGL)  */

#if !defined(RFONT_RENDER_LEGACY) && !defined(RFONT_RENDER_RGL)
typedef struct {
   GLuint vao, vbo, tbo, cbo, ebo,
            program, vShader, fShader;
   
   u8 legacy;
} RFont_gl_info;

RFont_gl_info RFont_gl = { 0 };

float RFont_color[4] = {0, 0, 0, 1};

#ifdef RFONT_DEBUG
inline void RFont_debug_shader(u32 src, const char* shader, const char* action);
void RFont_debug_shader(u32 src, const char* shader, const char* action) {
    GLint status;
    if (action[0] == 'l')
        glGetProgramiv(src, GL_LINK_STATUS, &status);
    else
        glGetShaderiv(src, GL_COMPILE_STATUS, &status);

    if (status == GL_TRUE) 
        printf("%s Shader %s successfully.\n", shader, action);
    else {
        printf("%s Shader failed to %s.\n", shader, action);

        if (action[0] == 'c') {
            GLint infoLogLength;
            glGetShaderiv(src, GL_INFO_LOG_LENGTH, &infoLogLength);

            if (infoLogLength > 0) {
                GLchar* infoLog = (GLchar*)RFONT_MALLOC(infoLogLength);
                glGetShaderInfoLog(src, infoLogLength, NULL, infoLog);
                printf("%s Shader info log:\n%s\n", shader, infoLog);
                RFONT_FREE(infoLog);
            }
        }
        
        RFont_opengl_getError();
    }
}
#endif

#define RFONT_MULTILINE_STR(...) #__VA_ARGS__

void RFont_render_set_color(float r, float g, float b, float a) {
   if (RFont_gl.legacy)
      return glColor4f(r, g, b, a);
   
   RFont_color[0] = r;
   RFont_color[1] = g;
   RFont_color[2] = b;
   RFont_color[3] = a;
}

void RFont_render_init() {
   if (RFont_gl.vao != 0 || RFont_gl.legacy)
      return;

   static const char* defaultVShaderCode = RFONT_MULTILINE_STR(
      \x23version 330 core       \n
      layout (location = 0) in vec3 vertexPosition;
      layout (location = 1) in vec2 vertexTexCoord;
      layout (location = 2) in vec4 inColor;
      out vec2 fragTexCoord;
      out vec4 fragColor;

      uniform mat4 mvp;          \n
      void main() {
         fragColor = inColor;
         gl_Position = vec4(vertexPosition, 1.0);
         fragTexCoord = vertexTexCoord;
      }
   );

   static const char* defaultFShaderCode = RFONT_MULTILINE_STR(
      \x23version 330 core                \n
      out vec4 FragColor;
      
      in vec4 fragColor;
      in vec2 fragTexCoord;

      uniform sampler2D texture0;

      void main() {
         FragColor = texture(texture0, fragTexCoord) * fragColor;
      }
   );
   
   glGenVertexArrays(1, &RFont_gl.vao);
   glBindVertexArray(RFont_gl.vao);

   glGenBuffers(1, &RFont_gl.vbo);
   glGenBuffers(1, &RFont_gl.tbo);
   glGenBuffers(1, &RFont_gl.cbo);
   glGenBuffers(1, &RFont_gl.ebo);
   /* compile vertex shader */
   RFont_gl.vShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(RFont_gl.vShader, 1, &defaultVShaderCode, NULL);
   glCompileShader(RFont_gl.vShader);

   #ifdef RFONT_DEBUG
   RFont_debug_shader(RFont_gl.vShader, "Vertex", "compile");
   #endif

   /* compile fragment shader */
   RFont_gl.fShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(RFont_gl.fShader, 1, &defaultFShaderCode, NULL);
   glCompileShader(RFont_gl.fShader);


   #ifdef RFONT_DEBUG
   RFont_debug_shader(RFont_gl.fShader, "Fragment", "compile");
   #endif

   /* create program and link vertex and fragment shaders */
   RFont_gl.program = glCreateProgram();

   glAttachShader(RFont_gl.program, RFont_gl.vShader);
   glAttachShader(RFont_gl.program, RFont_gl.fShader);

   glBindAttribLocation(RFont_gl.program, 0, "vertexPosition");
   glBindAttribLocation(RFont_gl.program, 1, "vertexTexCoord");
   glBindAttribLocation(RFont_gl.program, 2, "inColor");

   glLinkProgram(RFont_gl.program);

   #ifdef RFONT_DEBUG
   RFont_debug_shader(RFont_gl.program, "Both", "link to the program");
   #endif
}
     
void RFont_render_text(RFont_texture atlas, float* verts, float* tcoords, size_t nverts) {
   glEnable(GL_TEXTURE_2D);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

   glDisable(GL_DEPTH_TEST);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_CULL_FACE);    

   glEnable(GL_BLEND);
   glShadeModel(GL_SMOOTH);

   if (RFont_gl.legacy) {
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      glBindTexture(GL_TEXTURE_2D, atlas);
      glPushMatrix();

      glBegin(GL_TRIANGLES);

      size_t i;
      size_t tIndex = 0;

      for (i = 0; i < (nverts * 3); i += 3) {
         glTexCoord2f(tcoords[tIndex], tcoords[tIndex + 1]);
         tIndex += 2;
         
         glVertex2f(verts[i], verts[i + 1]);
      }
      glEnd();
      glPopMatrix();
   } else {
      glBindVertexArray(RFont_gl.vao);

      glUseProgram(RFont_gl.program);

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, RFont_gl.vbo);
      glBufferData(GL_ARRAY_BUFFER, nverts * 3 * sizeof(float), verts, GL_DYNAMIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, RFont_gl.tbo);
      glBufferData(GL_ARRAY_BUFFER, nverts * 2 * sizeof(float), tcoords, GL_DYNAMIC_DRAW);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

      float* colors = RFONT_MALLOC(sizeof(float) * nverts * 4);

      u32 i = 0;
      for (i = 0; i < (nverts * 4); i += 4) {
         colors[i] = RFont_color[0];
         colors[i + 1] = RFont_color[1];
         colors[i + 2] = RFont_color[2];
         colors[i + 3] = RFont_color[3];
      }

      glEnableVertexAttribArray(2);
      glBindBuffer(GL_ARRAY_BUFFER, RFont_gl.cbo);
      glBufferData(GL_ARRAY_BUFFER, nverts * 4 * sizeof(float), colors, GL_DYNAMIC_DRAW);
      glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);

      RFONT_FREE(colors);

      GLushort* indices = RFONT_MALLOC(sizeof(GLushort) * 6 * nverts);
      int k = 0;

      u32 j;
      for (j = 0; j < (6 * nverts); j += 6) {
         indices[j] = 4*  k;
         indices[j + 1] = 4*k + 1;
         indices[j + 2] = 4*k + 2;
         indices[j + 3] = 4*k;
         indices[j + 4] = 4*k + 2;
         indices[j + 5] = 4*k + 3;

         k++;
      }

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RFont_gl.ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * nverts, indices, GL_STATIC_DRAW);

      RFONT_FREE(indices);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, atlas);

      glDrawArrays(GL_TRIANGLES, 0, nverts);   
      glUseProgram(0);
   }

   glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
   glEnable(GL_DEPTH_TEST);
}

void RFont_render_free(RFont_texture atlas) {
   glDeleteTextures(1, &atlas);

   if (RFont_gl.vao == 0 || RFont_gl.legacy)
      return;
   
   /* free vertex array */
   glDeleteVertexArrays(1, &RFont_gl.vao);
   RFont_gl.vao = 0;

   /* free buffers */
   glDeleteBuffers(1, &RFont_gl.tbo);
   glDeleteBuffers(1, &RFont_gl.vbo);

   /* free program data */
   glDeleteShader(RFont_gl.vShader);
   glDeleteShader(RFont_gl.fShader);
   glDeleteProgram(RFont_gl.program);
}

void RFont_render_legacy(u8 legacy) { RFont_gl.legacy = legacy; }

#endif /* !defined(RFONT_RENDER_LEGACY) && !defined(RFONT_RENDER_RGL) */
#endif /*  !defined(RFONT_NO_OPENGL) && !defined(RFONT_NO_GRAPHICS) */

/* 
stb_truetype defines and source code required by RFont

you probably don't care about this part if you're reading just the RFont code
*/

#ifndef RFONT_EXTERNAL_STB
   typedef char stbtt__check_size32[sizeof(i32)==4 ? 1 : -1];
   typedef char stbtt__check_size16[sizeof(i16)==2 ? 1 : -1];

   // #define your own functions "STBTT_malloc" / "STBTT_free" to avoid RFONT_MALLOC
   #ifndef STBTT_malloc
   #define STBTT_malloc(x,u)  ((void)(u),RFONT_MALLOC(x))
   #define STBTT_free(x,u)    ((void)(u),RFONT_FREE(x))
   #endif

#ifdef __cplusplus
extern "C" {
#endif

// as above, but takes one or more glyph indices for greater efficiency

#ifndef STBTT_vmove // you can predefine these to use different values (but why?)
   enum {
      STBTT_vmove=1,
      STBTT_vline,
      STBTT_vcurve,
      STBTT_vcubic
   };
#endif

#ifndef stbtt_vertex // you can predefine this to use different values
                   // (we share this with other code at RAD)
   #define stbtt_vertex_type short // can't use i16 because that's not visible in the header file
   typedef struct
   {
      stbtt_vertex_type x,y,cx,cy,cx1,cy1;
      unsigned char type,padding;
   } stbtt_vertex;
#endif

STBTT_DEF int stbtt_GetGlyphShape(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **vertices);

// @TODO: don't expose this structure
typedef struct
{
   int w,h,stride;
   unsigned char *pixels;
} stbtt__bitmap;

// rasterize a shape with quadratic beziers into a bitmap
STBTT_DEF void stbtt_Rasterize(stbtt__bitmap *result,        // 1-channel bitmap to draw into
                               float flatness_in_pixels,     // allowable error of curve in pixels
                               stbtt_vertex *vertices,       // array of vertices defining shape
                               int num_verts,                // number of vertices in above array
                               float scale_x, float scale_y, // scale applied to input vertices
                               float shift_x, float shift_y, // translation applied to input vertices
                               int x_off, int y_off,         // another translation applied to input
                               int invert,                   // if non-zero, vertically flip shape
                               void *userdata);              // context for to STBTT_MALLOC

enum { // platformID
   STBTT_PLATFORM_ID_UNICODE   =0,
   STBTT_PLATFORM_ID_MAC       =1,
   STBTT_PLATFORM_ID_ISO       =2,
   STBTT_PLATFORM_ID_MICROSOFT =3
};

enum { // encodingID for STBTT_PLATFORM_ID_UNICODE
   STBTT_UNICODE_EID_UNICODE_1_0    =0,
   STBTT_UNICODE_EID_UNICODE_1_1    =1,
   STBTT_UNICODE_EID_ISO_10646      =2,
   STBTT_UNICODE_EID_UNICODE_2_0_BMP=3,
   STBTT_UNICODE_EID_UNICODE_2_0_FULL=4
};

enum { // encodingID for STBTT_PLATFORM_ID_MICROSOFT
   STBTT_MS_EID_SYMBOL        =0,
   STBTT_MS_EID_UNICODE_BMP   =1,
   STBTT_MS_EID_SHIFTJIS      =2,
   STBTT_MS_EID_UNICODE_FULL  =10
};

enum { // encodingID for STBTT_PLATFORM_ID_MAC; same as Script Manager codes
   STBTT_MAC_EID_ROMAN        =0,   STBTT_MAC_EID_ARABIC       =4,
   STBTT_MAC_EID_JAPANESE     =1,   STBTT_MAC_EID_HEBREW       =5,
   STBTT_MAC_EID_CHINESE_TRAD =2,   STBTT_MAC_EID_GREEK        =6,
   STBTT_MAC_EID_KOREAN       =3,   STBTT_MAC_EID_RUSSIAN      =7
};

enum { // languageID for STBTT_PLATFORM_ID_MICROSOFT; same as LCID...
       // problematic because there are e.g. 16 english LCIDs and 16 arabic LCIDs
   STBTT_MS_LANG_ENGLISH     =0x0409,   STBTT_MS_LANG_ITALIAN     =0x0410,
   STBTT_MS_LANG_CHINESE     =0x0804,   STBTT_MS_LANG_JAPANESE    =0x0411,
   STBTT_MS_LANG_DUTCH       =0x0413,   STBTT_MS_LANG_KOREAN      =0x0412,
   STBTT_MS_LANG_FRENCH      =0x040c,   STBTT_MS_LANG_RUSSIAN     =0x0419,
   STBTT_MS_LANG_GERMAN      =0x0407,   STBTT_MS_LANG_SPANISH     =0x0409,
   STBTT_MS_LANG_HEBREW      =0x040d,   STBTT_MS_LANG_SWEDISH     =0x041D
};

enum { // languageID for STBTT_PLATFORM_ID_MAC
   STBTT_MAC_LANG_ENGLISH      =0 ,   STBTT_MAC_LANG_JAPANESE     =11,
   STBTT_MAC_LANG_ARABIC       =12,   STBTT_MAC_LANG_KOREAN       =23,
   STBTT_MAC_LANG_DUTCH        =4 ,   STBTT_MAC_LANG_RUSSIAN      =32,
   STBTT_MAC_LANG_FRENCH       =1 ,   STBTT_MAC_LANG_SPANISH      =6 ,
   STBTT_MAC_LANG_GERMAN       =2 ,   STBTT_MAC_LANG_SWEDISH      =5 ,
   STBTT_MAC_LANG_HEBREW       =10,   STBTT_MAC_LANG_CHINESE_SIMPLIFIED =33,
   STBTT_MAC_LANG_ITALIAN      =3 ,   STBTT_MAC_LANG_CHINESE_TRAD =19
};

#ifdef __cplusplus
}
#endif

#ifndef STBTT_MAX_OVERSAMPLE
#define STBTT_MAX_OVERSAMPLE   8
#endif

#if STBTT_MAX_OVERSAMPLE > 255
#error "STBTT_MAX_OVERSAMPLE cannot be > 255"
#endif

typedef int stbtt__test_oversample_pow2[(STBTT_MAX_OVERSAMPLE & (STBTT_MAX_OVERSAMPLE-1)) == 0 ? 1 : -1];

#ifndef STBTT_RASTERIZER_VERSION
#define STBTT_RASTERIZER_VERSION 2
#endif

#ifdef _MSC_VER
#define STBTT__NOTUSED(v)  (void)(v)
#else
#define STBTT__NOTUSED(v)  (void)sizeof(v)
#endif

STBTT_DEF u8 stbtt__buf_get8(stbtt__buf *b)
{
   if (b->cursor >= b->size)
      return 0;
   return b->data[b->cursor++];
}

STBTT_DEF u8 stbtt__buf_peek8(stbtt__buf *b)
{
   if (b->cursor >= b->size)
      return 0;
   return b->data[b->cursor];
}

STBTT_DEF void stbtt__buf_seek(stbtt__buf *b, int o)
{
   assert(!(o > b->size || o < 0));
   b->cursor = (o > b->size || o < 0) ? b->size : o;
}

STBTT_DEF void stbtt__buf_skip(stbtt__buf *b, int o)
{
   stbtt__buf_seek(b, b->cursor + o);
}

STBTT_DEF u32 stbtt__buf_get(stbtt__buf *b, int n)
{
   u32 v = 0;
   int i;
   assert(n >= 1 && n <= 4);
   for (i = 0; i < n; i++)
      v = (v << 8) | stbtt__buf_get8(b);
   return v;
}

STBTT_DEF stbtt__buf stbtt__new_buf(const void *p, size_t size)
{
   stbtt__buf r;
   assert(size < 0x40000000);
   r.data = (u8*) p;
   r.size = (int) size;
   r.cursor = 0;
   return r;
}

#define stbtt__buf_get16(b)  stbtt__buf_get((b), 2)
#define stbtt__buf_get32(b)  stbtt__buf_get((b), 4)

STBTT_DEF stbtt__buf stbtt__buf_range(const stbtt__buf *b, int o, int s)
{
   stbtt__buf r = stbtt__new_buf(NULL, 0);
   if (o < 0 || s < 0 || o > b->size || s > b->size - o) return r;
   r.data = b->data + o;
   r.size = s;
   return r;
}

STBTT_DEF stbtt__buf stbtt__cff_get_index(stbtt__buf *b)
{
   int count, start, offsize;
   start = b->cursor;
   count = stbtt__buf_get16(b);
   if (count) {
      offsize = stbtt__buf_get8(b);
      assert(offsize >= 1 && offsize <= 4);
      stbtt__buf_skip(b, offsize * count);
      stbtt__buf_skip(b, stbtt__buf_get(b, offsize) - 1);
   }
   return stbtt__buf_range(b, start, b->cursor - start);
}

STBTT_DEF u32 stbtt__cff_int(stbtt__buf *b)
{
   int b0 = stbtt__buf_get8(b);
   if (b0 >= 32 && b0 <= 246)       return b0 - 139;
   else if (b0 >= 247 && b0 <= 250) return (b0 - 247)*256 + stbtt__buf_get8(b) + 108;
   else if (b0 >= 251 && b0 <= 254) return -(b0 - 251)*256 - stbtt__buf_get8(b) - 108;
   else if (b0 == 28)               return stbtt__buf_get16(b);
   else if (b0 == 29)               return stbtt__buf_get32(b);
   assert(0);
   return 0;
}

STBTT_DEF void stbtt__cff_skip_operand(stbtt__buf *b) {
   int v, b0 = stbtt__buf_peek8(b);
   assert(b0 >= 28);
   if (b0 == 30) {
      stbtt__buf_skip(b, 1);
      while (b->cursor < b->size) {
         v = stbtt__buf_get8(b);
         if ((v & 0xF) == 0xF || (v >> 4) == 0xF)
            break;
      }
   } else {
      stbtt__cff_int(b);
   }
}

STBTT_DEF stbtt__buf stbtt__dict_get(stbtt__buf *b, int key)
{
   stbtt__buf_seek(b, 0);
   while (b->cursor < b->size) {
      int start = b->cursor, end, op;
      while (stbtt__buf_peek8(b) >= 28)
         stbtt__cff_skip_operand(b);
      end = b->cursor;
      op = stbtt__buf_get8(b);
      if (op == 12)  op = stbtt__buf_get8(b) | 0x100;
      if (op == key) return stbtt__buf_range(b, start, end-start);
   }
   return stbtt__buf_range(b, 0, 0);
}

STBTT_DEF void stbtt__dict_get_ints(stbtt__buf *b, int key, int outcount, u32 *out)
{
   int i;
   stbtt__buf operands = stbtt__dict_get(b, key);
   for (i = 0; i < outcount && operands.cursor < operands.size; i++)
      out[i] = stbtt__cff_int(&operands);
}

STBTT_DEF int stbtt__cff_index_count(stbtt__buf *b)
{
   stbtt__buf_seek(b, 0);
   return stbtt__buf_get16(b);
}

STBTT_DEF stbtt__buf stbtt__cff_index_get(stbtt__buf b, int i)
{
   int count, offsize, start, end;
   stbtt__buf_seek(&b, 0);
   count = stbtt__buf_get16(&b);
   offsize = stbtt__buf_get8(&b);
   assert(i >= 0 && i < count);
   assert(offsize >= 1 && offsize <= 4);
   stbtt__buf_skip(&b, i*offsize);
   start = stbtt__buf_get(&b, offsize);
   end = stbtt__buf_get(&b, offsize);
   return stbtt__buf_range(&b, 2+(count+1)*offsize+start, end - start);
}

#define ttBYTE(p)     (* (u8 *) (p))
#define ttCHAR(p)     (* (i8 *) (p))
#define ttFixed(p)    ttLONG(p)

STBTT_DEF i16 ttSHORT(u8 *p)   { return p[0]*256 + p[1]; }
STBTT_DEF u16 ttUSHORT(u8 *p) { return p[0]*256 + p[1]; }
STBTT_DEF u32 ttULONG(u8 *p)  { return (p[0]<<24) + (p[1]<<16) + (p[2]<<8) + p[3]; }

#define stbtt_tag4(p,c0,c1,c2,c3) ((p)[0] == (c0) && (p)[1] == (c1) && (p)[2] == (c2) && (p)[3] == (c3))
#define stbtt_tag(p,str)          stbtt_tag4(p,str[0],str[1],str[2],str[3])

// @OPTIMIZE: binary search
STBTT_DEF u32 stbtt__find_table(u8 *data, u32 fontstart, const char *tag)
{
   i32 num_tables = ttUSHORT(data+fontstart+4);
   u32 tabledir = fontstart + 12;
   i32 i;
   for (i=0; i < num_tables; ++i) {
      u32 loc = tabledir + 16*i;
      if (stbtt_tag(data+loc+0, tag))
         return ttULONG(data+loc+8);
   }
   return 0;
}

STBTT_DEF stbtt__buf stbtt__get_subrs(stbtt__buf cff, stbtt__buf fontdict)
{
   u32 subrsoff = 0, private_loc[2] = { 0, 0 };
   stbtt__buf pdict;
   stbtt__dict_get_ints(&fontdict, 18, 2, private_loc);
   if (!private_loc[1] || !private_loc[0]) return stbtt__new_buf(NULL, 0);
   pdict = stbtt__buf_range(&cff, private_loc[1], private_loc[0]);
   stbtt__dict_get_ints(&pdict, 19, 1, &subrsoff);
   if (!subrsoff) return stbtt__new_buf(NULL, 0);
   stbtt__buf_seek(&cff, private_loc[1]+subrsoff);
   return stbtt__cff_get_index(&cff);
}

STBTT_DEF void stbtt_setvertex(stbtt_vertex *v, u8 type, i32 x, i32 y, i32 cx, i32 cy)
{
   v->type = type;
   v->x = (i16) x;
   v->y = (i16) y;
   v->cx = (i16) cx;
   v->cy = (i16) cy;
}

STBTT_DEF int stbtt__close_shape(stbtt_vertex *vertices, int num_vertices, int was_off, int start_off,
    i32 sx, i32 sy, i32 scx, i32 scy, i32 cx, i32 cy)
{
   if (start_off) {
      if (was_off)
         stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, (cx+scx)>>1, (cy+scy)>>1, cx,cy);
      stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, sx,sy,scx,scy);
   } else {
      if (was_off)
         stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve,sx,sy,cx,cy);
      else
         stbtt_setvertex(&vertices[num_vertices++], STBTT_vline,sx,sy,0,0);
   }
   return num_vertices;
}

STBTT_DEF int stbtt__GetGlyfOffset(const stbtt_fontinfo *info, int glyph_index);

STBTT_DEF int stbtt__GetGlyphShapeTT(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **pvertices)
{
   i16 numberOfContours;
   u8 *endPtsOfContours;
   u8 *data = info->data;
   stbtt_vertex *vertices=0;
   int num_vertices=0;
   int g = stbtt__GetGlyfOffset(info, glyph_index);

   *pvertices = NULL;

   if (g < 0) return 0;

   numberOfContours = ttSHORT(data + g);

   if (numberOfContours > 0) {
      u8 flags=0,flagcount;
      i32 ins, i,j=0,m,n, next_move, was_off=0, off, start_off=0;
      i32 x,y,cx,cy,sx,sy, scx,scy;
      u8 *points;
      endPtsOfContours = (data + g + 10);
      ins = ttUSHORT(data + g + 10 + numberOfContours * 2);
      points = data + g + 10 + numberOfContours * 2 + 2 + ins;

      n = 1+ttUSHORT(endPtsOfContours + numberOfContours*2-2);

      m = n + 2*numberOfContours;  // a loose bound on how many vertices we might need
      vertices = (stbtt_vertex *) STBTT_malloc(m * sizeof(vertices[0]), info->userdata);
      if (vertices == 0)
         return 0;

      next_move = 0;
      flagcount=0;

      off = m - n; // starting offset for uninterpreted data, regardless of how m ends up being calculated

      // first load flags

      for (i=0; i < n; ++i) {
         if (flagcount == 0) {
            flags = *points++;
            if (flags & 8)
               flagcount = *points++;
         } else
            --flagcount;
         vertices[off+i].type = flags;
      }

      // now load x coordinates
      x=0;
      for (i=0; i < n; ++i) {
         flags = vertices[off+i].type;
         if (flags & 2) {
            i16 dx = *points++;
            x += (flags & 16) ? dx : -dx; // ???
         } else {
            if (!(flags & 16)) {
               x = x + (i16) (points[0]*256 + points[1]);
               points += 2;
            }
         }
         vertices[off+i].x = (i16) x;
      }

      // now load y coordinates
      y=0;
      for (i=0; i < n; ++i) {
         flags = vertices[off+i].type;
         if (flags & 4) {
            i16 dy = *points++;
            y += (flags & 32) ? dy : -dy; // ???
         } else {
            if (!(flags & 32)) {
               y = y + (i16) (points[0]*256 + points[1]);
               points += 2;
            }
         }
         vertices[off+i].y = (i16) y;
      }

      // now convert them to our format
      num_vertices=0;
      sx = sy = cx = cy = scx = scy = 0;
      for (i=0; i < n; ++i) {
         flags = vertices[off+i].type;
         x     = (i16) vertices[off+i].x;
         y     = (i16) vertices[off+i].y;

         if (next_move == i) {
            if (i != 0)
               num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx,sy,scx,scy,cx,cy);

            // now start the new one
            start_off = !(flags & 1);
            if (start_off) {
               // if we start off with an off-curve point, then when we need to find a point on the curve
               // where we can start, and we need to save some state for when we wraparound.
               scx = x;
               scy = y;
               if (!(vertices[off+i+1].type & 1)) {
                  // next point is also a curve point, so interpolate an on-point curve
                  sx = (x + (i32) vertices[off+i+1].x) >> 1;
                  sy = (y + (i32) vertices[off+i+1].y) >> 1;
               } else {
                  // otherwise just use the next point as our start point
                  sx = (i32) vertices[off+i+1].x;
                  sy = (i32) vertices[off+i+1].y;
                  ++i; // we're using point i+1 as the starting point, so skip it
               }
            } else {
               sx = x;
               sy = y;
            }
            stbtt_setvertex(&vertices[num_vertices++], STBTT_vmove,sx,sy,0,0);
            was_off = 0;
            next_move = 1 + ttUSHORT(endPtsOfContours+j*2);
            ++j;
         } else {
            if (!(flags & 1)) { // if it's a curve
               if (was_off) // two off-curve control points in a row means interpolate an on-curve midpoint
                  stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, (cx+x)>>1, (cy+y)>>1, cx, cy);
               cx = x;
               cy = y;
               was_off = 1;
            } else {
               if (was_off)
                  stbtt_setvertex(&vertices[num_vertices++], STBTT_vcurve, x,y, cx, cy);
               else
                  stbtt_setvertex(&vertices[num_vertices++], STBTT_vline, x,y,0,0);
               was_off = 0;
            }
         }
      }
      num_vertices = stbtt__close_shape(vertices, num_vertices, was_off, start_off, sx,sy,scx,scy,cx,cy);
   } else if (numberOfContours < 0) {
      // Compound shapes.
      int more = 1;
      u8 *comp = data + g + 10;
      num_vertices = 0;
      vertices = 0;
      while (more) {
         u16 flags, gidx;
         int comp_num_verts = 0, i;
         stbtt_vertex *comp_verts = 0, *tmp = 0;
         float mtx[6] = {1,0,0,1,0,0}, m, n;

         flags = ttSHORT(comp); comp+=2;
         gidx = ttSHORT(comp); comp+=2;

         if (flags & 2) { // XY values
            if (flags & 1) { // shorts
               mtx[4] = ttSHORT(comp); comp+=2;
               mtx[5] = ttSHORT(comp); comp+=2;
            } else {
               mtx[4] = ttCHAR(comp); comp+=1;
               mtx[5] = ttCHAR(comp); comp+=1;
            }
         }
         else {
            // @TODO handle matching point
            assert(0);
         }
         if (flags & (1<<3)) { // WE_HAVE_A_SCALE
            mtx[0] = mtx[3] = ttSHORT(comp)/16384.0f; comp+=2;
            mtx[1] = mtx[2] = 0;
         } else if (flags & (1<<6)) { // WE_HAVE_AN_X_AND_YSCALE
            mtx[0] = ttSHORT(comp)/16384.0f; comp+=2;
            mtx[1] = mtx[2] = 0;
            mtx[3] = ttSHORT(comp)/16384.0f; comp+=2;
         } else if (flags & (1<<7)) { // WE_HAVE_A_TWO_BY_TWO
            mtx[0] = ttSHORT(comp)/16384.0f; comp+=2;
            mtx[1] = ttSHORT(comp)/16384.0f; comp+=2;
            mtx[2] = ttSHORT(comp)/16384.0f; comp+=2;
            mtx[3] = ttSHORT(comp)/16384.0f; comp+=2;
         }

         // Find transformation scales.
         m = (float) sqrt(mtx[0]*mtx[0] + mtx[1]*mtx[1]);
         n = (float) sqrt(mtx[2]*mtx[2] + mtx[3]*mtx[3]);

         // Get indexed glyph.
         comp_num_verts = stbtt_GetGlyphShape(info, gidx, &comp_verts);
         if (comp_num_verts > 0) {
            // Transform vertices.
            for (i = 0; i < comp_num_verts; ++i) {
               stbtt_vertex* v = &comp_verts[i];
               stbtt_vertex_type x,y;
               x=v->x; y=v->y;
               v->x = (stbtt_vertex_type)(m * (mtx[0]*x + mtx[2]*y + mtx[4]));
               v->y = (stbtt_vertex_type)(n * (mtx[1]*x + mtx[3]*y + mtx[5]));
               x=v->cx; y=v->cy;
               v->cx = (stbtt_vertex_type)(m * (mtx[0]*x + mtx[2]*y + mtx[4]));
               v->cy = (stbtt_vertex_type)(n * (mtx[1]*x + mtx[3]*y + mtx[5]));
            }
            // Append vertices.
            tmp = (stbtt_vertex*)STBTT_malloc((num_vertices+comp_num_verts)*sizeof(stbtt_vertex), info->userdata);
            if (!tmp) {
               if (vertices) STBTT_free(vertices, info->userdata);
               if (comp_verts) STBTT_free(comp_verts, info->userdata);
               return 0;
            }
            if (num_vertices > 0) memcpy(tmp, vertices, num_vertices*sizeof(stbtt_vertex));
            memcpy(tmp+num_vertices, comp_verts, comp_num_verts*sizeof(stbtt_vertex));
            if (vertices) STBTT_free(vertices, info->userdata);
            vertices = tmp;
            STBTT_free(comp_verts, info->userdata);
            num_vertices += comp_num_verts;
         }
         // More components ?
         more = flags & (1<<5);
      }
   } else {
      // numberOfCounters == 0, do nothing
   }

   *pvertices = vertices;
   return num_vertices;
}

typedef struct
{
   int bounds;
   int started;
   float first_x, first_y;
   float x, y;
   i32 min_x, max_x, min_y, max_y;

   stbtt_vertex *pvertices;
   int num_vertices;
} stbtt__csctx;

#define STBTT__CSCTX_INIT(bounds) {bounds,0, 0,0, 0,0, 0,0,0,0, NULL, 0}

STBTT_DEF void stbtt__track_vertex(stbtt__csctx *c, i32 x, i32 y)
{
   if (x > c->max_x || !c->started) c->max_x = x;
   if (y > c->max_y || !c->started) c->max_y = y;
   if (x < c->min_x || !c->started) c->min_x = x;
   if (y < c->min_y || !c->started) c->min_y = y;
   c->started = 1;
}

STBTT_DEF void stbtt__csctx_v(stbtt__csctx *c, u8 type, i32 x, i32 y, i32 cx, i32 cy, i32 cx1, i32 cy1)
{
   if (c->bounds) {
      stbtt__track_vertex(c, x, y);
      if (type == STBTT_vcubic) {
         stbtt__track_vertex(c, cx, cy);
         stbtt__track_vertex(c, cx1, cy1);
      }
   } else {
      stbtt_setvertex(&c->pvertices[c->num_vertices], type, x, y, cx, cy);
      c->pvertices[c->num_vertices].cx1 = (i16) cx1;
      c->pvertices[c->num_vertices].cy1 = (i16) cy1;
   }
   c->num_vertices++;
}

STBTT_DEF void stbtt__csctx_close_shape(stbtt__csctx *ctx)
{
   if (ctx->first_x != ctx->x || ctx->first_y != ctx->y)
      stbtt__csctx_v(ctx, STBTT_vline, (int)ctx->first_x, (int)ctx->first_y, 0, 0, 0, 0);
}

STBTT_DEF void stbtt__csctx_rmove_to(stbtt__csctx *ctx, float dx, float dy)
{
   stbtt__csctx_close_shape(ctx);
   ctx->first_x = ctx->x = ctx->x + dx;
   ctx->first_y = ctx->y = ctx->y + dy;
   stbtt__csctx_v(ctx, STBTT_vmove, (int)ctx->x, (int)ctx->y, 0, 0, 0, 0);
}

STBTT_DEF void stbtt__csctx_rline_to(stbtt__csctx *ctx, float dx, float dy)
{
   ctx->x += dx;
   ctx->y += dy;
   stbtt__csctx_v(ctx, STBTT_vline, (int)ctx->x, (int)ctx->y, 0, 0, 0, 0);
}

STBTT_DEF void stbtt__csctx_rccurve_to(stbtt__csctx *ctx, float dx1, float dy1, float dx2, float dy2, float dx3, float dy3)
{
   float cx1 = ctx->x + dx1;
   float cy1 = ctx->y + dy1;
   float cx2 = cx1 + dx2;
   float cy2 = cy1 + dy2;
   ctx->x = cx2 + dx3;
   ctx->y = cy2 + dy3;
   stbtt__csctx_v(ctx, STBTT_vcubic, (int)ctx->x, (int)ctx->y, (int)cx1, (int)cy1, (int)cx2, (int)cy2);
}

STBTT_DEF stbtt__buf stbtt__get_subr(stbtt__buf idx, int n)
{
   int count = stbtt__cff_index_count(&idx);
   int bias = 107;
   if (count >= 33900)
      bias = 32768;
   else if (count >= 1240)
      bias = 1131;
   n += bias;
   if (n < 0 || n >= count)
      return stbtt__new_buf(NULL, 0);
   return stbtt__cff_index_get(idx, n);
}

STBTT_DEF stbtt__buf stbtt__cid_get_glyph_subrs(const stbtt_fontinfo *info, int glyph_index)
{
   stbtt__buf fdselect = info->fdselect;
   int nranges, start, end, v, fmt, fdselector = -1, i;

   stbtt__buf_seek(&fdselect, 0);
   fmt = stbtt__buf_get8(&fdselect);
   if (fmt == 0) {
      // untested
      stbtt__buf_skip(&fdselect, glyph_index);
      fdselector = stbtt__buf_get8(&fdselect);
   } else if (fmt == 3) {
      nranges = stbtt__buf_get16(&fdselect);
      start = stbtt__buf_get16(&fdselect);
      for (i = 0; i < nranges; i++) {
         v = stbtt__buf_get8(&fdselect);
         end = stbtt__buf_get16(&fdselect);
         if (glyph_index >= start && glyph_index < end) {
            fdselector = v;
            break;
         }
         start = end;
      }
   }
   if (fdselector == -1) stbtt__new_buf(NULL, 0);
   return stbtt__get_subrs(info->cff, stbtt__cff_index_get(info->fontdicts, fdselector));
}

STBTT_DEF int stbtt__run_charstring(const stbtt_fontinfo *info, int glyph_index, stbtt__csctx *c)
{
   int in_header = 1, maskbits = 0, subr_stack_height = 0, sp = 0, v, i, b0;
   int has_subrs = 0, clear_stack;
   float s[48];
   stbtt__buf subr_stack[10], subrs = info->subrs, b;
   float f;

#define STBTT__CSERR(s) (0)

   // this currently ignores the initial width value, which isn't needed if we have hmtx
   b = stbtt__cff_index_get(info->charstrings, glyph_index);
   while (b.cursor < b.size) {
      i = 0;
      clear_stack = 1;
      b0 = stbtt__buf_get8(&b);
      switch (b0) {
      // @TODO implement hinting
      case 0x13: // hintmask
      case 0x14: // cntrmask
         if (in_header)
            maskbits += (sp / 2); // implicit "vstem"
         in_header = 0;
         stbtt__buf_skip(&b, (maskbits + 7) / 8);
         break;

      case 0x01: // hstem
      case 0x03: // vstem
      case 0x12: // hstemhm
      case 0x17: // vstemhm
         maskbits += (sp / 2);
         break;

      case 0x15: // rmoveto
         in_header = 0;
         if (sp < 2) return STBTT__CSERR("rmoveto stack");
         stbtt__csctx_rmove_to(c, s[sp-2], s[sp-1]);
         break;
      case 0x04: // vmoveto
         in_header = 0;
         if (sp < 1) return STBTT__CSERR("vmoveto stack");
         stbtt__csctx_rmove_to(c, 0, s[sp-1]);
         break;
      case 0x16: // hmoveto
         in_header = 0;
         if (sp < 1) return STBTT__CSERR("hmoveto stack");
         stbtt__csctx_rmove_to(c, s[sp-1], 0);
         break;

      case 0x05: // rlineto
         if (sp < 2) return STBTT__CSERR("rlineto stack");
         for (; i + 1 < sp; i += 2)
            stbtt__csctx_rline_to(c, s[i], s[i+1]);
         break;

      // hlineto/vlineto and vhcurveto/hvcurveto alternate horizontal and vertical
      // starting from a different place.

      case 0x07: // vlineto
         if (sp < 1) return STBTT__CSERR("vlineto stack");
         goto vlineto;
      case 0x06: // hlineto
         if (sp < 1) return STBTT__CSERR("hlineto stack");
         for (;;) {
            if (i >= sp) break;
            stbtt__csctx_rline_to(c, s[i], 0);
            i++;
      vlineto:
            if (i >= sp) break;
            stbtt__csctx_rline_to(c, 0, s[i]);
            i++;
         }
         break;

      case 0x1F: // hvcurveto
         if (sp < 4) return STBTT__CSERR("hvcurveto stack");
         goto hvcurveto;
      case 0x1E: // vhcurveto
         if (sp < 4) return STBTT__CSERR("vhcurveto stack");
         for (;;) {
            if (i + 3 >= sp) break;
            stbtt__csctx_rccurve_to(c, 0, s[i], s[i+1], s[i+2], s[i+3], (sp - i == 5) ? s[i + 4] : 0.0f);
            i += 4;
      hvcurveto:
            if (i + 3 >= sp) break;
            stbtt__csctx_rccurve_to(c, s[i], 0, s[i+1], s[i+2], (sp - i == 5) ? s[i+4] : 0.0f, s[i+3]);
            i += 4;
         }
         break;

      case 0x08: // rrcurveto
         if (sp < 6) return STBTT__CSERR("rcurveline stack");
         for (; i + 5 < sp; i += 6)
            stbtt__csctx_rccurve_to(c, s[i], s[i+1], s[i+2], s[i+3], s[i+4], s[i+5]);
         break;

      case 0x18: // rcurveline
         if (sp < 8) return STBTT__CSERR("rcurveline stack");
         for (; i + 5 < sp - 2; i += 6)
            stbtt__csctx_rccurve_to(c, s[i], s[i+1], s[i+2], s[i+3], s[i+4], s[i+5]);
         if (i + 1 >= sp) return STBTT__CSERR("rcurveline stack");
         stbtt__csctx_rline_to(c, s[i], s[i+1]);
         break;

      case 0x19: // rlinecurve
         if (sp < 8) return STBTT__CSERR("rlinecurve stack");
         for (; i + 1 < sp - 6; i += 2)
            stbtt__csctx_rline_to(c, s[i], s[i+1]);
         if (i + 5 >= sp) return STBTT__CSERR("rlinecurve stack");
         stbtt__csctx_rccurve_to(c, s[i], s[i+1], s[i+2], s[i+3], s[i+4], s[i+5]);
         break;

      case 0x1A: // vvcurveto
      case 0x1B: // hhcurveto
         if (sp < 4) return STBTT__CSERR("(vv|hh)curveto stack");
         f = 0.0;
         if (sp & 1) { f = s[i]; i++; }
         for (; i + 3 < sp; i += 4) {
            if (b0 == 0x1B)
               stbtt__csctx_rccurve_to(c, s[i], f, s[i+1], s[i+2], s[i+3], 0.0);
            else
               stbtt__csctx_rccurve_to(c, f, s[i], s[i+1], s[i+2], 0.0, s[i+3]);
            f = 0.0;
         }
         break;

      case 0x0A: // callsubr
         if (!has_subrs) {
            if (info->fdselect.size)
               subrs = stbtt__cid_get_glyph_subrs(info, glyph_index);
            has_subrs = 1;
         }
         // fallthrough
      case 0x1D: // callgsubr
         if (sp < 1) return STBTT__CSERR("call(g|)subr stack");
         v = (int) s[--sp];
         if (subr_stack_height >= 10) return STBTT__CSERR("recursion limit");
         subr_stack[subr_stack_height++] = b;
         b = stbtt__get_subr(b0 == 0x0A ? subrs : info->gsubrs, v);
         if (b.size == 0) return STBTT__CSERR("subr not found");
         b.cursor = 0;
         clear_stack = 0;
         break;

      case 0x0B: // return
         if (subr_stack_height <= 0) return STBTT__CSERR("return outside subr");
         b = subr_stack[--subr_stack_height];
         clear_stack = 0;
         break;

      case 0x0E: // endchar
         stbtt__csctx_close_shape(c);
         return 1;

      case 0x0C: { // two-byte escape
         float dx1, dx2, dx3, dx4, dx5, dx6, dy1, dy2, dy3, dy4, dy5, dy6;
         float dx, dy;
         int b1 = stbtt__buf_get8(&b);
         switch (b1) {
         // @TODO These "flex" implementations ignore the flex-depth and resolution,
         // and always draw beziers.
         case 0x22: // hflex
            if (sp < 7) return STBTT__CSERR("hflex stack");
            dx1 = s[0];
            dx2 = s[1];
            dy2 = s[2];
            dx3 = s[3];
            dx4 = s[4];
            dx5 = s[5];
            dx6 = s[6];
            stbtt__csctx_rccurve_to(c, dx1, 0, dx2, dy2, dx3, 0);
            stbtt__csctx_rccurve_to(c, dx4, 0, dx5, -dy2, dx6, 0);
            break;

         case 0x23: // flex
            if (sp < 13) return STBTT__CSERR("flex stack");
            dx1 = s[0];
            dy1 = s[1];
            dx2 = s[2];
            dy2 = s[3];
            dx3 = s[4];
            dy3 = s[5];
            dx4 = s[6];
            dy4 = s[7];
            dx5 = s[8];
            dy5 = s[9];
            dx6 = s[10];
            dy6 = s[11];
            //fd is s[12]
            stbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, dy3);
            stbtt__csctx_rccurve_to(c, dx4, dy4, dx5, dy5, dx6, dy6);
            break;

         case 0x24: // hflex1
            if (sp < 9) return STBTT__CSERR("hflex1 stack");
            dx1 = s[0];
            dy1 = s[1];
            dx2 = s[2];
            dy2 = s[3];
            dx3 = s[4];
            dx4 = s[5];
            dx5 = s[6];
            dy5 = s[7];
            dx6 = s[8];
            stbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, 0);
            stbtt__csctx_rccurve_to(c, dx4, 0, dx5, dy5, dx6, -(dy1+dy2+dy5));
            break;

         case 0x25: // flex1
            if (sp < 11) return STBTT__CSERR("flex1 stack");
            dx1 = s[0];
            dy1 = s[1];
            dx2 = s[2];
            dy2 = s[3];
            dx3 = s[4];
            dy3 = s[5];
            dx4 = s[6];
            dy4 = s[7];
            dx5 = s[8];
            dy5 = s[9];
            dx6 = dy6 = s[10];
            dx = dx1+dx2+dx3+dx4+dx5;
            dy = dy1+dy2+dy3+dy4+dy5;
            if (fabs(dx) > fabs(dy))
               dy6 = -dy;
            else
               dx6 = -dx;
            stbtt__csctx_rccurve_to(c, dx1, dy1, dx2, dy2, dx3, dy3);
            stbtt__csctx_rccurve_to(c, dx4, dy4, dx5, dy5, dx6, dy6);
            break;

         default:
            return STBTT__CSERR("unimplemented");
         }
      } break;

      default:
         if (b0 != 255 && b0 != 28 && (b0 < 32 || b0 > 254))
            return STBTT__CSERR("reserved operator");

         // push immediate
         if (b0 == 255) {
            f = (float)(i32)stbtt__buf_get32(&b) / 0x10000;
         } else {
            stbtt__buf_skip(&b, -1);
            f = (float)(i16)stbtt__cff_int(&b);
         }
         if (sp >= 48) return STBTT__CSERR("push stack overflow");
         s[sp++] = f;
         clear_stack = 0;
         break;
      }
      if (clear_stack) sp = 0;
   }
   return STBTT__CSERR("no endchar");

#undef STBTT__CSERR
}

STBTT_DEF int stbtt__GetGlyphShapeT2(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **pvertices)
{
   // runs the charstring twice, once to count and once to output (to avoid realloc)
   stbtt__csctx count_ctx = STBTT__CSCTX_INIT(1);
   stbtt__csctx output_ctx = STBTT__CSCTX_INIT(0);
   if (stbtt__run_charstring(info, glyph_index, &count_ctx)) {
      *pvertices = (stbtt_vertex*)STBTT_malloc(count_ctx.num_vertices*sizeof(stbtt_vertex), info->userdata);
      output_ctx.pvertices = *pvertices;
      if (stbtt__run_charstring(info, glyph_index, &output_ctx)) {
         assert(output_ctx.num_vertices == count_ctx.num_vertices);
         return output_ctx.num_vertices;
      }
   }
   *pvertices = NULL;
   return 0;
}

STBTT_DEF int stbtt_GetGlyphShape(const stbtt_fontinfo *info, int glyph_index, stbtt_vertex **pvertices)
{
   if (!info->cff.size)
      return stbtt__GetGlyphShapeTT(info, glyph_index, pvertices);
   else
      return stbtt__GetGlyphShapeT2(info, glyph_index, pvertices);
}

STBTT_DEF int  stbtt__GetGlyphKernInfoAdvance(const stbtt_fontinfo *info, int glyph1, int glyph2)
{
   u8 *data = info->data + info->kern;
   u32 needle, straw;
   int l, r, m;

   // we only look at the first table. it must be 'horizontal' and format 0.
   if (!info->kern)
      return 0;
   if (ttUSHORT(data+2) < 1) // number of tables, need at least 1
      return 0;
   if (ttUSHORT(data+8) != 1) // horizontal flag must be set in format
      return 0;

   l = 0;
   r = ttUSHORT(data+10) - 1;
   needle = glyph1 << 16 | glyph2;
   while (l <= r) {
      m = (l + r) >> 1;
      straw = ttULONG(data+18+(m*6)); // note: unaligned read
      if (needle < straw)
         r = m - 1;
      else if (needle > straw)
         l = m + 1;
      else
         return ttSHORT(data+22+(m*6));
   }
   return 0;
}

STBTT_DEF i32  stbtt__GetCoverageIndex(u8 *coverageTable, int glyph)
{
    u16 coverageFormat = ttUSHORT(coverageTable);
    switch(coverageFormat) {
        case 1: {
            u16 glyphCount = ttUSHORT(coverageTable + 2);

            // Binary search.
            i32 l=0, r=glyphCount-1, m;
            int straw, needle=glyph;
            while (l <= r) {
                u8 *glyphArray = coverageTable + 4;
                u16 glyphID;
                m = (l + r) >> 1;
                glyphID = ttUSHORT(glyphArray + 2 * m);
                straw = glyphID;
                if (needle < straw)
                    r = m - 1;
                else if (needle > straw)
                    l = m + 1;
                else {
                     return m;
                }
            }
        } break;

        case 2: {
            u16 rangeCount = ttUSHORT(coverageTable + 2);
            u8 *rangeArray = coverageTable + 4;

            // Binary search.
            i32 l=0, r=rangeCount-1, m;
            int strawStart, strawEnd, needle=glyph;
            while (l <= r) {
                u8 *rangeRecord;
                m = (l + r) >> 1;
                rangeRecord = rangeArray + 6 * m;
                strawStart = ttUSHORT(rangeRecord);
                strawEnd = ttUSHORT(rangeRecord + 2);
                if (needle < strawStart)
                    r = m - 1;
                else if (needle > strawEnd)
                    l = m + 1;
                else {
                    u16 startCoverageIndex = ttUSHORT(rangeRecord + 4);
                    return startCoverageIndex + glyph - strawStart;
                }
            }
        } break;

        default: {
            // There are no other cases.
            assert(0);
        } break;
    }

    return -1;
}

STBTT_DEF i32  stbtt__GetGlyphClass(u8 *classDefTable, int glyph)
{
    u16 classDefFormat = ttUSHORT(classDefTable);
    switch(classDefFormat)
    {
        case 1: {
            u16 startGlyphID = ttUSHORT(classDefTable + 2);
            u16 glyphCount = ttUSHORT(classDefTable + 4);
            u8 *classDef1ValueArray = classDefTable + 6;

            if (glyph >= startGlyphID && glyph < startGlyphID + glyphCount)
                return (i32)ttUSHORT(classDef1ValueArray + 2 * (glyph - startGlyphID));

            classDefTable = classDef1ValueArray + 2 * glyphCount;
        } break;

        case 2: {
            u16 classRangeCount = ttUSHORT(classDefTable + 2);
            u8 *classRangeRecords = classDefTable + 4;

            // Binary search.
            i32 l=0, r=classRangeCount-1, m;
            int strawStart, strawEnd, needle=glyph;
            while (l <= r) {
                u8 *classRangeRecord;
                m = (l + r) >> 1;
                classRangeRecord = classRangeRecords + 6 * m;
                strawStart = ttUSHORT(classRangeRecord);
                strawEnd = ttUSHORT(classRangeRecord + 2);
                if (needle < strawStart)
                    r = m - 1;
                else if (needle > strawEnd)
                    l = m + 1;
                else
                    return (i32)ttUSHORT(classRangeRecord + 4);
            }

            classDefTable = classRangeRecords + 6 * classRangeCount;
        } break;

        default: {
            // There are no other cases.
            assert(0);
        } break;
    }

    return -1;
}

// Define to assert(x) if you want to break on unimplemented formats.
#define STBTT_GPOS_TODO_assert(x) assert(x)

STBTT_DEF i32  stbtt__GetGlyphGPOSInfoAdvance(const stbtt_fontinfo *info, int glyph1, int glyph2)
{
    u16 lookupListOffset;
    u8 *lookupList;
    u16 lookupCount;
    u8 *data;
    i32 i;

    if (!info->gpos) return 0;

    data = info->data + info->gpos;

    if (ttUSHORT(data+0) != 1) return 0; // Major version 1
    if (ttUSHORT(data+2) != 0) return 0; // Minor version 0

    lookupListOffset = ttUSHORT(data+8);
    lookupList = data + lookupListOffset;
    lookupCount = ttUSHORT(lookupList);

    for (i=0; i<lookupCount; ++i) {
        u16 lookupOffset = ttUSHORT(lookupList + 2 + 2 * i);
        u8 *lookupTable = lookupList + lookupOffset;

        u16 lookupType = ttUSHORT(lookupTable);
        u16 subTableCount = ttUSHORT(lookupTable + 4);
        u8 *subTableOffsets = lookupTable + 6;
        switch(lookupType) {
            case 2: { // Pair Adjustment Positioning Subtable
                i32 sti;
                for (sti=0; sti<subTableCount; sti++) {
                    u16 subtableOffset = ttUSHORT(subTableOffsets + 2 * sti);
                    u8 *table = lookupTable + subtableOffset;
                    u16 posFormat = ttUSHORT(table);
                    u16 coverageOffset = ttUSHORT(table + 2);
                    i32 coverageIndex = stbtt__GetCoverageIndex(table + coverageOffset, glyph1);
                    if (coverageIndex == -1) continue;

                    switch (posFormat) {
                        case 1: {
                            i32 l, r, m;
                            int straw, needle;
                            u16 valueFormat1 = ttUSHORT(table + 4);
                            u16 valueFormat2 = ttUSHORT(table + 6);
                            i32 valueRecordPairSizeInBytes = 2;
                            u16 pairSetCount = ttUSHORT(table + 8);
                            u16 pairPosOffset = ttUSHORT(table + 10 + 2 * coverageIndex);
                            u8 *pairValueTable = table + pairPosOffset;
                            u16 pairValueCount = ttUSHORT(pairValueTable);
                            u8 *pairValueArray = pairValueTable + 2;
                            // TODO: Support more formats.
                            STBTT_GPOS_TODO_assert(valueFormat1 == 4);
                            if (valueFormat1 != 4) return 0;
                            STBTT_GPOS_TODO_assert(valueFormat2 == 0);
                            if (valueFormat2 != 0) return 0;

                            assert(coverageIndex < pairSetCount);
                            STBTT__NOTUSED(pairSetCount);

                            needle=glyph2;
                            r=pairValueCount-1;
                            l=0;

                            // Binary search.
                            while (l <= r) {
                                u16 secondGlyph;
                                u8 *pairValue;
                                m = (l + r) >> 1;
                                pairValue = pairValueArray + (2 + valueRecordPairSizeInBytes) * m;
                                secondGlyph = ttUSHORT(pairValue);
                                straw = secondGlyph;
                                if (needle < straw)
                                    r = m - 1;
                                else if (needle > straw)
                                    l = m + 1;
                                else {
                                    i16 xAdvance = ttSHORT(pairValue + 2);
                                    return xAdvance;
                                }
                            }
                        } break;

                        case 2: {
                            u16 valueFormat1 = ttUSHORT(table + 4);
                            u16 valueFormat2 = ttUSHORT(table + 6);

                            u16 classDef1Offset = ttUSHORT(table + 8);
                            u16 classDef2Offset = ttUSHORT(table + 10);
                            int glyph1class = stbtt__GetGlyphClass(table + classDef1Offset, glyph1);
                            int glyph2class = stbtt__GetGlyphClass(table + classDef2Offset, glyph2);

                            u16 class1Count = ttUSHORT(table + 12);
                            u16 class2Count = ttUSHORT(table + 14);
                            assert(glyph1class < class1Count);
                            assert(glyph2class < class2Count);

                            // TODO: Support more formats.
                            STBTT_GPOS_TODO_assert(valueFormat1 == 4);
                            if (valueFormat1 != 4) return 0;
                            STBTT_GPOS_TODO_assert(valueFormat2 == 0);
                            if (valueFormat2 != 0) return 0;

                            if (glyph1class >= 0 && glyph1class < class1Count && glyph2class >= 0 && glyph2class < class2Count) {
                                u8 *class1Records = table + 16;
                                u8 *class2Records = class1Records + 2 * (glyph1class * class2Count);
                                i16 xAdvance = ttSHORT(class2Records + 2 * glyph2class);
                                return xAdvance;
                            }
                        } break;

                        default: {
                            // There are no other cases.
                            assert(0);
                            break;
                        };
                    }
                }
                break;
            };

            default:
                // TODO: Implement other stuff.
                break;
        }
    }

    return 0;
}

STBTT_DEF int  stbtt_GetGlyphKernAdvance(const stbtt_fontinfo *info, int g1, int g2)
{
   int xAdvance = 0;

   if (info->gpos)
      xAdvance += stbtt__GetGlyphGPOSInfoAdvance(info, g1, g2);
   else if (info->kern)
      xAdvance += stbtt__GetGlyphKernInfoAdvance(info, g1, g2);

   return xAdvance;
}

typedef struct stbtt__hheap_chunk
{
   struct stbtt__hheap_chunk *next;
} stbtt__hheap_chunk;

typedef struct stbtt__hheap
{
   struct stbtt__hheap_chunk *head;
   void   *first_free;
   int    num_remaining_in_head_chunk;
} stbtt__hheap;

STBTT_DEF void *stbtt__hheap_alloc(stbtt__hheap *hh, size_t size, void *userdata)
{
   if (hh->first_free) {
      void *p = hh->first_free;
      hh->first_free = * (void **) p;
      return p;
   } else {
      if (hh->num_remaining_in_head_chunk == 0) {
         int count = (size < 32 ? 2000 : size < 128 ? 800 : 100);
         stbtt__hheap_chunk *c = (stbtt__hheap_chunk *) STBTT_malloc(sizeof(stbtt__hheap_chunk) + size * count, userdata);
         if (c == NULL)
            return NULL;
         c->next = hh->head;
         hh->head = c;
         hh->num_remaining_in_head_chunk = count;
      }
      --hh->num_remaining_in_head_chunk;
      return (char *) (hh->head) + sizeof(stbtt__hheap_chunk) + size * hh->num_remaining_in_head_chunk;
   }
}

STBTT_DEF void stbtt__hheap_free(stbtt__hheap *hh, void *p)
{
   *(void **) p = hh->first_free;
   hh->first_free = p;
}

STBTT_DEF void stbtt__hheap_cleanup(stbtt__hheap *hh, void *userdata)
{
   stbtt__hheap_chunk *c = hh->head;
   while (c) {
      stbtt__hheap_chunk *n = c->next;
      STBTT_free(c, userdata);
      c = n;
   }
}

typedef struct stbtt__edge {
   float x0,y0, x1,y1;
   int invert;
} stbtt__edge;


typedef struct stbtt__active_edge
{
   struct stbtt__active_edge *next;
   #if STBTT_RASTERIZER_VERSION==1
   int x,dx;
   float ey;
   int direction;
   #elif STBTT_RASTERIZER_VERSION==2
   float fx,fdx,fdy;
   float direction;
   float sy;
   float ey;
   #else
   #error "Unrecognized value of STBTT_RASTERIZER_VERSION"
   #endif
} stbtt__active_edge;

#if STBTT_RASTERIZER_VERSION == 1
#define STBTT_FIXSHIFT   10
#define STBTT_FIX        (1 << STBTT_FIXSHIFT)
#define STBTT_FIXMASK    (STBTT_FIX-1)

STBTT_DEF stbtt__active_edge *stbtt__new_active(stbtt__hheap *hh, stbtt__edge *e, int off_x, float start_point, void *userdata)
{
   stbtt__active_edge *z = (stbtt__active_edge *) stbtt__hheap_alloc(hh, sizeof(*z), userdata);
   float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
   assert(z != NULL);
   if (!z) return z;

   // round dx down to avoid overshooting
   if (dxdy < 0)
      z->dx = -floor(STBTT_FIX * -dxdy);
   else
      z->dx = floor(STBTT_FIX * dxdy);

   z->x = floor(STBTT_FIX * e->x0 + z->dx * (start_point - e->y0)); // use z->dx so when we offset later it's by the same amount
   z->x -= off_x * STBTT_FIX;

   z->ey = e->y1;
   z->next = 0;
   z->direction = e->invert ? 1 : -1;
   return z;
}
#elif STBTT_RASTERIZER_VERSION == 2
STBTT_DEF stbtt__active_edge *stbtt__new_active(stbtt__hheap *hh, stbtt__edge *e, int off_x, float start_point, void *userdata)
{
   stbtt__active_edge *z = (stbtt__active_edge *) stbtt__hheap_alloc(hh, sizeof(*z), userdata);
   float dxdy = (e->x1 - e->x0) / (e->y1 - e->y0);
   assert(z != NULL);
   //assert(e->y0 <= start_point);
   if (!z) return z;
   z->fdx = dxdy;
   z->fdy = dxdy != 0.0f ? (1.0f/dxdy) : 0.0f;
   z->fx = e->x0 + dxdy * (start_point - e->y0);
   z->fx -= off_x;
   z->direction = e->invert ? 1.0f : -1.0f;
   z->sy = e->y0;
   z->ey = e->y1;
   z->next = 0;
   return z;
}
#else
#error "Unrecognized value of STBTT_RASTERIZER_VERSION"
#endif

#if STBTT_RASTERIZER_VERSION == 1

STBTT_DEF void stbtt__fill_active_edges(unsigned char *scanline, int len, stbtt__active_edge *e, int max_weight)
{
   // non-zero winding fill
   int x0=0, w=0;

   while (e) {
      if (w == 0) {
         // if we're currently at zero, we need to record the edge start point
         x0 = e->x; w += e->direction;
      } else {
         int x1 = e->x; w += e->direction;
         // if we went to zero, we need to draw
         if (w == 0) {
            int i = x0 >> STBTT_FIXSHIFT;
            int j = x1 >> STBTT_FIXSHIFT;

            if (i < len && j >= 0) {
               if (i == j) {
                  // x0,x1 are the same pixel, so compute combined coverage
                  scanline[i] = scanline[i] + (u8) ((x1 - x0) * max_weight >> STBTT_FIXSHIFT);
               } else {
                  if (i >= 0) // add antialiasing for x0
                     scanline[i] = scanline[i] + (u8) (((STBTT_FIX - (x0 & STBTT_FIXMASK)) * max_weight) >> STBTT_FIXSHIFT);
                  else
                     i = -1; // clip

                  if (j < len) // add antialiasing for x1
                     scanline[j] = scanline[j] + (u8) (((x1 & STBTT_FIXMASK) * max_weight) >> STBTT_FIXSHIFT);
                  else
                     j = len; // clip

                  for (++i; i < j; ++i) // fill pixels between x0 and x1
                     scanline[i] = scanline[i] + (u8) max_weight;
               }
            }
         }
      }

      e = e->next;
   }
}

STBTT_DEF void stbtt__rasterize_sorted_edges(stbtt__bitmap *result, stbtt__edge *e, int n, int vsubsample, int off_x, int off_y, void *userdata)
{
   stbtt__hheap hh = { 0, 0, 0 };
   stbtt__active_edge *active = NULL;
   int y,j=0;
   int max_weight = (255 / vsubsample);  // weight per vertical scanline
   int s; // vertical subsample index
   unsigned char scanline_data[512], *scanline;

   if (result->w > 512)
      scanline = (unsigned char *) STBTT_malloc(result->w, userdata);
   else
      scanline = scanline_data;

   y = off_y * vsubsample;
   e[n].y0 = (off_y + result->h) * (float) vsubsample + 1;

   while (j < result->h) {
      memset(scanline, 0, result->w);
      for (s=0; s < vsubsample; ++s) {
         // find center of pixel for this scanline
         float scan_y = y + 0.5f;
         stbtt__active_edge **step = &active;

         // update all active edges;
         // remove all active edges that terminate before the center of this scanline
         while (*step) {
            stbtt__active_edge * z = *step;
            if (z->ey <= scan_y) {
               *step = z->next; // delete from list
               assert(z->direction);
               z->direction = 0;
               stbtt__hheap_free(&hh, z);
            } else {
               z->x += z->dx; // advance to position for current scanline
               step = &((*step)->next); // advance through list
            }
         }

         // resort the list if needed
         for(;;) {
            int changed=0;
            step = &active;
            while (*step && (*step)->next) {
               if ((*step)->x > (*step)->next->x) {
                  stbtt__active_edge *t = *step;
                  stbtt__active_edge *q = t->next;

                  t->next = q->next;
                  q->next = t;
                  *step = q;
                  changed = 1;
               }
               step = &(*step)->next;
            }
            if (!changed) break;
         }

         // insert all edges that start before the center of this scanline -- omit ones that also end on this scanline
         while (e->y0 <= scan_y) {
            if (e->y1 > scan_y) {
               stbtt__active_edge *z = stbtt__new_active(&hh, e, off_x, scan_y, userdata);
               if (z != NULL) {
                  // find insertion point
                  if (active == NULL)
                     active = z;
                  else if (z->x < active->x) {
                     // insert at front
                     z->next = active;
                     active = z;
                  } else {
                     // find thing to insert AFTER
                     stbtt__active_edge *p = active;
                     while (p->next && p->next->x < z->x)
                        p = p->next;
                     // at this point, p->next->x is NOT < z->x
                     z->next = p->next;
                     p->next = z;
                  }
               }
            }
            ++e;
         }

         // now process all active edges in XOR fashion
         if (active)
            stbtt__fill_active_edges(scanline, result->w, active, max_weight);

         ++y;
      }
      memcpy(result->pixels + j * result->stride, scanline, result->w);
      ++j;
   }

   stbtt__hheap_cleanup(&hh, userdata);

   if (scanline != scanline_data)
      STBTT_free(scanline, userdata);
}

#elif STBTT_RASTERIZER_VERSION == 2

STBTT_DEF void stbtt__handle_clipped_edge(float *scanline, int x, stbtt__active_edge *e, float x0, float y0, float x1, float y1)
{
   if (y0 == y1) return;
   assert(y0 < y1);
   assert(e->sy <= e->ey);
   if (y0 > e->ey) return;
   if (y1 < e->sy) return;
   if (y0 < e->sy) {
      x0 += (x1-x0) * (e->sy - y0) / (y1-y0);
      y0 = e->sy;
   }
   if (y1 > e->ey) {
      x1 += (x1-x0) * (e->ey - y1) / (y1-y0);
      y1 = e->ey;
   }

   if (x0 == x)
      assert(x1 <= x+1);
   else if (x0 == x+1)
      assert(x1 >= x);
   else if (x0 <= x)
      assert(x1 <= x);
   else if (x0 >= x+1)
      assert(x1 >= x+1);
   else
      assert(x1 >= x && x1 <= x+1);

   if (x0 <= x && x1 <= x)
      scanline[x] += e->direction * (y1-y0);
   else if (x0 >= x+1 && x1 >= x+1)
      ;
   else {
      assert(x0 >= x && x0 <= x+1 && x1 >= x && x1 <= x+1);
      scanline[x] += e->direction * (y1-y0) * (1-((x0-x)+(x1-x))/2); // coverage = 1 - average x position
   }
}

STBTT_DEF void stbtt__fill_active_edges_new(float *scanline, float *scanline_fill, int len, stbtt__active_edge *e, float y_top)
{
   float y_bottom = y_top+1;

   while (e) {
      // brute force every pixel

      // compute intersection points with top & bottom
      assert(e->ey >= y_top);

      if (e->fdx == 0) {
         float x0 = e->fx;
         if (x0 < len) {
            if (x0 >= 0) {
               stbtt__handle_clipped_edge(scanline,(int) x0,e, x0,y_top, x0,y_bottom);
               stbtt__handle_clipped_edge(scanline_fill-1,(int) x0+1,e, x0,y_top, x0,y_bottom);
            } else {
               stbtt__handle_clipped_edge(scanline_fill-1,0,e, x0,y_top, x0,y_bottom);
            }
         }
      } else {
         float x0 = e->fx;
         float dx = e->fdx;
         float xb = x0 + dx;
         float x_top, x_bottom;
         float sy0,sy1;
         float dy = e->fdy;
         assert(e->sy <= y_bottom && e->ey >= y_top);

         if (e->sy > y_top) {
            x_top = x0 + dx * (e->sy - y_top);
            sy0 = e->sy;
         } else {
            x_top = x0;
            sy0 = y_top;
         }
         if (e->ey < y_bottom) {
            x_bottom = x0 + dx * (e->ey - y_top);
            sy1 = e->ey;
         } else {
            x_bottom = xb;
            sy1 = y_bottom;
         }

         if (x_top >= 0 && x_bottom >= 0 && x_top < len && x_bottom < len) {
            // from here on, we don't have to range check x values

            if ((int) x_top == (int) x_bottom) {
               float height;
               // simple case, only spans one pixel
               int x = (int) x_top;
               height = sy1 - sy0;
               assert(x >= 0 && x < len);
               scanline[x] += e->direction * (1-((x_top - x) + (x_bottom-x))/2)  * height;
               scanline_fill[x] += e->direction * height; // everything right of this pixel is filled
            } else {
               int x,x1,x2;
               float y_crossing, step, sign, area;
               // covers 2+ pixels
               if (x_top > x_bottom) {
                  // flip scanline vertically; signed area is the same
                  float t;
                  sy0 = y_bottom - (sy0 - y_top);
                  sy1 = y_bottom - (sy1 - y_top);
                  t = sy0, sy0 = sy1, sy1 = t;
                  t = x_bottom, x_bottom = x_top, x_top = t;
                  dx = -dx;
                  dy = -dy;
                  t = x0, x0 = xb, xb = t;
               }

               x1 = (int) x_top;
               x2 = (int) x_bottom;
               // compute intersection with y axis at x1+1
               y_crossing = (x1+1 - x0) * dy + y_top;

               sign = e->direction;
               // area of the rectangle covered from y0..y_crossing
               area = sign * (y_crossing-sy0);
               // area of the triangle (x_top,y0), (x+1,y0), (x+1,y_crossing)
               scanline[x1] += area * (1-((x_top - x1)+(x1+1-x1))/2);

               step = sign * dy;
               for (x = x1+1; x < x2; ++x) {
                  scanline[x] += area + step/2;
                  area += step;
               }
               y_crossing += dy * (x2 - (x1+1));

               assert(fabs(area) <= 1.01f);

               scanline[x2] += area + sign * (1-((x2-x2)+(x_bottom-x2))/2) * (sy1-y_crossing);

               scanline_fill[x2] += sign * (sy1-sy0);
            }
         } else {
            int x;
            for (x=0; x < len; ++x) {

               float y0 = y_top;
               float x1 = (float) (x);
               float x2 = (float) (x+1);
               float x3 = xb;
               float y3 = y_bottom;

               float y1 = (x - x0) / dx + y_top;
               float y2 = (x+1 - x0) / dx + y_top;

               if (x0 < x1 && x3 > x2) {         // three segments descending down-right
                  stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x1,y1);
                  stbtt__handle_clipped_edge(scanline,x,e, x1,y1, x2,y2);
                  stbtt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
               } else if (x3 < x1 && x0 > x2) {  // three segments descending down-left
                  stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x2,y2);
                  stbtt__handle_clipped_edge(scanline,x,e, x2,y2, x1,y1);
                  stbtt__handle_clipped_edge(scanline,x,e, x1,y1, x3,y3);
               } else if (x0 < x1 && x3 > x1) {  // two segments across x, down-right
                  stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x1,y1);
                  stbtt__handle_clipped_edge(scanline,x,e, x1,y1, x3,y3);
               } else if (x3 < x1 && x0 > x1) {  // two segments across x, down-left
                  stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x1,y1);
                  stbtt__handle_clipped_edge(scanline,x,e, x1,y1, x3,y3);
               } else if (x0 < x2 && x3 > x2) {  // two segments across x+1, down-right
                  stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x2,y2);
                  stbtt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
               } else if (x3 < x2 && x0 > x2) {  // two segments across x+1, down-left
                  stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x2,y2);
                  stbtt__handle_clipped_edge(scanline,x,e, x2,y2, x3,y3);
               } else {  // one segment
                  stbtt__handle_clipped_edge(scanline,x,e, x0,y0, x3,y3);
               }
            }
         }
      }
      e = e->next;
   }
}

// directly AA rasterize edges w/o supersampling
STBTT_DEF void stbtt__rasterize_sorted_edges(stbtt__bitmap *result, stbtt__edge *e, int n, int vsubsample, int off_x, int off_y, void *userdata)
{
   stbtt__hheap hh = { 0, 0, 0 };
   stbtt__active_edge *active = NULL;
   int y,j=0, i;
   float scanline_data[129], *scanline, *scanline2;

   STBTT__NOTUSED(vsubsample);

   if (result->w > 64)
      scanline = (float *) STBTT_malloc((result->w*2+1) * sizeof(float), userdata);
   else
      scanline = scanline_data;

   scanline2 = scanline + result->w;

   y = off_y;
   e[n].y0 = (float) (off_y + result->h) + 1;

   while (j < result->h) {
      // find center of pixel for this scanline
      float scan_y_top    = y + 0.0f;
      float scan_y_bottom = y + 1.0f;
      stbtt__active_edge **step = &active;

      memset(scanline , 0, result->w*sizeof(scanline[0]));
      memset(scanline2, 0, (result->w+1)*sizeof(scanline[0]));

      // update all active edges;
      // remove all active edges that terminate before the top of this scanline
      while (*step) {
         stbtt__active_edge * z = *step;
         if (z->ey <= scan_y_top) {
            *step = z->next; // delete from list
            assert(z->direction);
            z->direction = 0;
            stbtt__hheap_free(&hh, z);
         } else {
            step = &((*step)->next); // advance through list
         }
      }

      // insert all edges that start before the bottom of this scanline
      while (e->y0 <= scan_y_bottom) {
         if (e->y0 != e->y1) {
            stbtt__active_edge *z = stbtt__new_active(&hh, e, off_x, scan_y_top, userdata);
            if (z != NULL) {
               if (j == 0 && off_y != 0) {
                  if (z->ey < scan_y_top) {
                     // this can happen due to subpixel positioning and some kind of fp rounding error i think
                     z->ey = scan_y_top;
                  }
               }
               assert(z->ey >= scan_y_top); // if we get really unlucky a tiny bit of an edge can be out of bounds
               // insert at front
               z->next = active;
               active = z;
            }
         }
         ++e;
      }

      // now process all active edges
      if (active)
         stbtt__fill_active_edges_new(scanline, scanline2+1, result->w, active, scan_y_top);

      {
         float sum = 0;
         for (i=0; i < result->w; ++i) {
            float k;
            int m;
            sum += scanline2[i];
            k = scanline[i] + sum;
            k = (float) fabs(k)*255 + 0.5f;
            m = (int) k;
            if (m > 255) m = 255;
            result->pixels[j*result->stride + i] = (unsigned char) m;
         }
      }
      // advance all the edges
      step = &active;
      while (*step) {
         stbtt__active_edge *z = *step;
         z->fx += z->fdx; // advance to position for current scanline
         step = &((*step)->next); // advance through list
      }

      ++y;
      ++j;
   }

   stbtt__hheap_cleanup(&hh, userdata);

   if (scanline != scanline_data)
      STBTT_free(scanline, userdata);
}
#else
#error "Unrecognized value of STBTT_RASTERIZER_VERSION"
#endif

#define STBTT__COMPARE(a,b)  ((a)->y0 < (b)->y0)

STBTT_DEF void stbtt__sort_edges_ins_sort(stbtt__edge *p, int n)
{
   int i,j;
   for (i=1; i < n; ++i) {
      stbtt__edge t = p[i], *a = &t;
      j = i;
      while (j > 0) {
         stbtt__edge *b = &p[j-1];
         int c = STBTT__COMPARE(a,b);
         if (!c) break;
         p[j] = p[j-1];
         --j;
      }
      if (i != j)
         p[j] = t;
   }
}

STBTT_DEF void stbtt__sort_edges_quicksort(stbtt__edge *p, int n)
{
   /* threshold for transitioning to insertion sort */
   while (n > 12) {
      stbtt__edge t;
      int c01,c12,c,m,i,j;

      /* compute median of three */
      m = n >> 1;
      c01 = STBTT__COMPARE(&p[0],&p[m]);
      c12 = STBTT__COMPARE(&p[m],&p[n-1]);
      /* if 0 >= mid >= end, or 0 < mid < end, then use mid */
      if (c01 != c12) {
         /* otherwise, we'll need to swap something else to middle */
         int z;
         c = STBTT__COMPARE(&p[0],&p[n-1]);
         /* 0>mid && mid<n:  0>n => n; 0<n => 0 */
         /* 0<mid && mid>n:  0>n => 0; 0<n => n */
         z = (c == c12) ? 0 : n-1;
         t = p[z];
         p[z] = p[m];
         p[m] = t;
      }
      /* now p[m] is the median-of-three */
      /* swap it to the beginning so it won't move around */
      t = p[0];
      p[0] = p[m];
      p[m] = t;

      /* partition loop */
      i=1;
      j=n-1;
      for(;;) {
         /* handling of equality is crucial here */
         /* for sentinels & efficiency with duplicates */
         for (;;++i) {
            if (!STBTT__COMPARE(&p[i], &p[0])) break;
         }
         for (;;--j) {
            if (!STBTT__COMPARE(&p[0], &p[j])) break;
         }
         /* make sure we haven't crossed */
         if (i >= j) break;
         t = p[i];
         p[i] = p[j];
         p[j] = t;

         ++i;
         --j;
      }
      /* recurse on smaller side, iterate on larger */
      if (j < (n-i)) {
         stbtt__sort_edges_quicksort(p,j);
         p = p+i;
         n = n-i;
      } else {
         stbtt__sort_edges_quicksort(p+i, n-i);
         n = j;
      }
   }
}

STBTT_DEF void stbtt__sort_edges(stbtt__edge *p, int n)
{
   stbtt__sort_edges_quicksort(p, n);
   stbtt__sort_edges_ins_sort(p, n);
}

typedef struct
{
   float x,y;
} stbtt__point;

STBTT_DEF void stbtt__rasterize(stbtt__bitmap *result, stbtt__point *pts, int *wcount, int windings, float scale_x, float scale_y, float shift_x, float shift_y, int off_x, int off_y, int invert, void *userdata)
{
   float y_scale_inv = invert ? -scale_y : scale_y;
   stbtt__edge *e;
   int n,i,j,k,m;
#if STBTT_RASTERIZER_VERSION == 1
   int vsubsample = result->h < 8 ? 15 : 5;
#elif STBTT_RASTERIZER_VERSION == 2
   int vsubsample = 1;
#else
   #error "Unrecognized value of STBTT_RASTERIZER_VERSION"
#endif
   // vsubsample should divide 255 evenly; otherwise we won't reach full opacity

   // now we have to blow out the windings into explicit edge lists
   n = 0;
   for (i=0; i < windings; ++i)
      n += wcount[i];

   e = (stbtt__edge *) STBTT_malloc(sizeof(*e) * (n+1), userdata); // add an extra one as a sentinel
   if (e == 0) return;
   n = 0;

   m=0;
   for (i=0; i < windings; ++i) {
      stbtt__point *p = pts + m;
      m += wcount[i];
      j = wcount[i]-1;
      for (k=0; k < wcount[i]; j=k++) {
         int a=k,b=j;
         // skip the edge if horizontal
         if (p[j].y == p[k].y)
            continue;
         // add edge from j to k to the list
         e[n].invert = 0;
         if (invert ? p[j].y > p[k].y : p[j].y < p[k].y) {
            e[n].invert = 1;
            a=j,b=k;
         }
         e[n].x0 = p[a].x * scale_x + shift_x;
         e[n].y0 = (p[a].y * y_scale_inv + shift_y) * vsubsample;
         e[n].x1 = p[b].x * scale_x + shift_x;
         e[n].y1 = (p[b].y * y_scale_inv + shift_y) * vsubsample;
         ++n;
      }
   }

   // now sort the edges by their highest point (should snap to integer, and then by x)
   //STBTT_sort(e, n, sizeof(e[0]), stbtt__edge_compare);
   stbtt__sort_edges(e, n);

   // now, traverse the scanlines and find the intersections on each scanline, use xor winding rule
   stbtt__rasterize_sorted_edges(result, e, n, vsubsample, off_x, off_y, userdata);

   STBTT_free(e, userdata);
}

STBTT_DEF void stbtt__add_point(stbtt__point *points, int n, float x, float y)
{
   if (!points) return; // during first pass, it's unallocated
   points[n].x = x;
   points[n].y = y;
}

// tessellate until threshold p is happy... @TODO warped to compensate for non-linear stretching
STBTT_DEF int stbtt__tesselate_curve(stbtt__point *points, int *num_points, float x0, float y0, float x1, float y1, float x2, float y2, float objspace_flatness_squared, int n)
{
   // midpoint
   float mx = (x0 + 2*x1 + x2)/4;
   float my = (y0 + 2*y1 + y2)/4;
   // versus directly drawn line
   float dx = (x0+x2)/2 - mx;
   float dy = (y0+y2)/2 - my;
   if (n > 16) // 65536 segments on one curve better be enough!
      return 1;
   if (dx*dx+dy*dy > objspace_flatness_squared) { // half-pixel error allowed... need to be smaller if AA
      stbtt__tesselate_curve(points, num_points, x0,y0, (x0+x1)/2.0f,(y0+y1)/2.0f, mx,my, objspace_flatness_squared,n+1);
      stbtt__tesselate_curve(points, num_points, mx,my, (x1+x2)/2.0f,(y1+y2)/2.0f, x2,y2, objspace_flatness_squared,n+1);
   } else {
      stbtt__add_point(points, *num_points,x2,y2);
      *num_points = *num_points+1;
   }
   return 1;
}

STBTT_DEF void stbtt__tesselate_cubic(stbtt__point *points, int *num_points, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float objspace_flatness_squared, int n)
{
   // @TODO this "flatness" calculation is just made-up nonsense that seems to work well enough
   float dx0 = x1-x0;
   float dy0 = y1-y0;
   float dx1 = x2-x1;
   float dy1 = y2-y1;
   float dx2 = x3-x2;
   float dy2 = y3-y2;
   float dx = x3-x0;
   float dy = y3-y0;
   float longlen = (float) (sqrt(dx0*dx0+dy0*dy0)+sqrt(dx1*dx1+dy1*dy1)+sqrt(dx2*dx2+dy2*dy2));
   float shortlen = (float) sqrt(dx*dx+dy*dy);
   float flatness_squared = longlen*longlen-shortlen*shortlen;

   if (n > 16) // 65536 segments on one curve better be enough!
      return;

   if (flatness_squared > objspace_flatness_squared) {
      float x01 = (x0+x1)/2;
      float y01 = (y0+y1)/2;
      float x12 = (x1+x2)/2;
      float y12 = (y1+y2)/2;
      float x23 = (x2+x3)/2;
      float y23 = (y2+y3)/2;

      float xa = (x01+x12)/2;
      float ya = (y01+y12)/2;
      float xb = (x12+x23)/2;
      float yb = (y12+y23)/2;

      float mx = (xa+xb)/2;
      float my = (ya+yb)/2;

      stbtt__tesselate_cubic(points, num_points, x0,y0, x01,y01, xa,ya, mx,my, objspace_flatness_squared,n+1);
      stbtt__tesselate_cubic(points, num_points, mx,my, xb,yb, x23,y23, x3,y3, objspace_flatness_squared,n+1);
   } else {
      stbtt__add_point(points, *num_points,x3,y3);
      *num_points = *num_points+1;
   }
}

// returns number of contours
STBTT_DEF stbtt__point *stbtt_FlattenCurves(stbtt_vertex *vertices, int num_verts, float objspace_flatness, int **contour_lengths, int *num_contours, void *userdata)
{
   stbtt__point *points=0;
   int num_points=0;

   float objspace_flatness_squared = objspace_flatness * objspace_flatness;
   int i,n=0,start=0, pass;

   // count how many "moves" there are to get the contour count
   for (i=0; i < num_verts; ++i)
      if (vertices[i].type == STBTT_vmove)
         ++n;

   *num_contours = n;
   if (n == 0) return 0;

   *contour_lengths = (int *) STBTT_malloc(sizeof(**contour_lengths) * n, userdata);

   if (*contour_lengths == 0) {
      *num_contours = 0;
      return 0;
   }

   // make two passes through the points so we don't need to realloc
   for (pass=0; pass < 2; ++pass) {
      float x=0,y=0;
      if (pass == 1) {
         points = (stbtt__point *) STBTT_malloc(num_points * sizeof(points[0]), userdata);
         if (points == NULL) goto error;
      }
      num_points = 0;
      n= -1;
      for (i=0; i < num_verts; ++i) {
         switch (vertices[i].type) {
            case STBTT_vmove:
               // start the next contour
               if (n >= 0)
                  (*contour_lengths)[n] = num_points - start;
               ++n;
               start = num_points;

               x = vertices[i].x, y = vertices[i].y;
               stbtt__add_point(points, num_points++, x,y);
               break;
            case STBTT_vline:
               x = vertices[i].x, y = vertices[i].y;
               stbtt__add_point(points, num_points++, x, y);
               break;
            case STBTT_vcurve:
               stbtt__tesselate_curve(points, &num_points, x,y,
                                        vertices[i].cx, vertices[i].cy,
                                        vertices[i].x,  vertices[i].y,
                                        objspace_flatness_squared, 0);
               x = vertices[i].x, y = vertices[i].y;
               break;
            case STBTT_vcubic:
               stbtt__tesselate_cubic(points, &num_points, x,y,
                                        vertices[i].cx, vertices[i].cy,
                                        vertices[i].cx1, vertices[i].cy1,
                                        vertices[i].x,  vertices[i].y,
                                        objspace_flatness_squared, 0);
               x = vertices[i].x, y = vertices[i].y;
               break;
         }
      }
      (*contour_lengths)[n] = num_points - start;
   }

   return points;
error:
   STBTT_free(points, userdata);
   STBTT_free(*contour_lengths, userdata);
   *contour_lengths = 0;
   *num_contours = 0;
   return NULL;
}

STBTT_DEF void stbtt_Rasterize(stbtt__bitmap *result, float flatness_in_pixels, stbtt_vertex *vertices, int num_verts, float scale_x, float scale_y, float shift_x, float shift_y, int x_off, int y_off, int invert, void *userdata)
{
   float scale            = scale_x > scale_y ? scale_y : scale_x;
   int winding_count      = 0;
   int *winding_lengths   = NULL;
   stbtt__point *windings = stbtt_FlattenCurves(vertices, num_verts, flatness_in_pixels / scale, &winding_lengths, &winding_count, userdata);
   if (windings) {
      stbtt__rasterize(result, windings, winding_lengths, winding_count, scale_x, scale_y, shift_x, shift_y, x_off, y_off, invert, userdata);
      STBTT_free(winding_lengths, userdata);
      STBTT_free(windings, userdata);
   }
}

STBTT_DEF void stbtt_GetGlyphBitmapBoxSubpixel(const stbtt_fontinfo *font, int glyph, float scale_x, float scale_y,float shift_x, float shift_y, int *ix0, int *iy0, int *ix1, int *iy1)
{
   int x0=0,y0=0,x1,y1; // =0 suppresses compiler warning
   if (!stbtt_GetGlyphBox(font, glyph, &x0,&y0,&x1,&y1)) {
      // e.g. space character
      if (ix0) *ix0 = 0;
      if (iy0) *iy0 = 0;
      if (ix1) *ix1 = 0;
      if (iy1) *iy1 = 0;
   } else {
      // move to integral bboxes (treating pixels as little squares, what pixels get touched)?
      if (ix0) *ix0 = floor( x0 * scale_x + shift_x);
      if (iy0) *iy0 = floor(-y1 * scale_y + shift_y);
      if (ix1) *ix1 = ceil ( x1 * scale_x + shift_x);
      if (iy1) *iy1 = ceil (-y0 * scale_y + shift_y);
   }
}

STBTT_DEF unsigned char *stbtt_GetGlyphBitmapSubpixel(const stbtt_fontinfo *info, float scale_x, float scale_y, float shift_x, float shift_y, int glyph, int *width, int *height, int *xoff, int *yoff)
{
   int ix0,iy0,ix1,iy1;
   stbtt__bitmap gbm;
   stbtt_vertex *vertices;
   int num_verts = stbtt_GetGlyphShape(info, glyph, &vertices);

   if (scale_x == 0) scale_x = scale_y;
   if (scale_y == 0) {
      if (scale_x == 0) {
         STBTT_free(vertices, info->userdata);
         return NULL;
      }
      scale_y = scale_x;
   }

   stbtt_GetGlyphBitmapBoxSubpixel(info, glyph, scale_x, scale_y, shift_x, shift_y, &ix0,&iy0,&ix1,&iy1);

   // now we get the size
   gbm.w = (ix1 - ix0);
   gbm.h = (iy1 - iy0);
   gbm.pixels = NULL; // in case we error

   if (width ) *width  = gbm.w;
   if (height) *height = gbm.h;
   if (xoff  ) *xoff   = ix0;
   if (yoff  ) *yoff   = iy0;

   if (gbm.w && gbm.h) {
      gbm.pixels = (unsigned char *) STBTT_malloc(gbm.w * gbm.h, info->userdata);
      if (gbm.pixels) {
         gbm.stride = gbm.w;

         stbtt_Rasterize(&gbm, 0.35f, vertices, num_verts, scale_x, scale_y, shift_x, shift_y, ix0, iy0, 1, info->userdata);
      }
   }
   STBTT_free(vertices, info->userdata);
   return gbm.pixels;
}


STBTT_DEF int stbtt_InitFont(stbtt_fontinfo *info, const unsigned char* const_data, int fontstart)
{
   unsigned char* data = (unsigned char*)const_data;

   u32 cmap, t;
   i32 i,numTables;

   info->data = (unsigned char*)data;
   info->fontstart = fontstart;
   info->cff = stbtt__new_buf(NULL, 0);

   cmap = stbtt__find_table(data, fontstart, "cmap");       // required
   info->loca = stbtt__find_table(data, fontstart, "loca"); // required
   info->head = stbtt__find_table(data, fontstart, "head"); // required
   info->glyf = stbtt__find_table(data, fontstart, "glyf"); // required
   info->hhea = stbtt__find_table(data, fontstart, "hhea"); // required
   info->hmtx = stbtt__find_table(data, fontstart, "hmtx"); // required
   info->kern = stbtt__find_table(data, fontstart, "kern"); // not required
   info->gpos = stbtt__find_table(data, fontstart, "GPOS"); // not required

   if (!cmap || !info->head || !info->hhea || !info->hmtx)
      return 0;
   if (info->glyf) {
      // required for truetype
      if (!info->loca) return 0;
   } else {
      // initialization for CFF / Type2 fonts (OTF)
      stbtt__buf b, topdict, topdictidx;
      u32 cstype = 2, charstrings = 0, fdarrayoff = 0, fdselectoff = 0;
      u32 cff;

      cff = stbtt__find_table(data, fontstart, "CFF ");
      if (!cff) return 0;

      info->fontdicts = stbtt__new_buf(NULL, 0);
      info->fdselect = stbtt__new_buf(NULL, 0);

      // @TODO this should use size from table (not 512MB)
      info->cff = stbtt__new_buf(data+cff, 512*1024*1024);
      b = info->cff;

      // read the header
      stbtt__buf_skip(&b, 2);
      stbtt__buf_seek(&b, stbtt__buf_get8(&b)); // hdrsize

      // @TODO the name INDEX could list multiple fonts,
      // but we just use the first one.
      stbtt__cff_get_index(&b);  // name INDEX
      topdictidx = stbtt__cff_get_index(&b);
      topdict = stbtt__cff_index_get(topdictidx, 0);
      stbtt__cff_get_index(&b);  // string INDEX
      info->gsubrs = stbtt__cff_get_index(&b);

      stbtt__dict_get_ints(&topdict, 17, 1, &charstrings);
      stbtt__dict_get_ints(&topdict, 0x100 | 6, 1, &cstype);
      stbtt__dict_get_ints(&topdict, 0x100 | 36, 1, &fdarrayoff);
      stbtt__dict_get_ints(&topdict, 0x100 | 37, 1, &fdselectoff);
      info->subrs = stbtt__get_subrs(b, topdict);

      // we only support Type 2 charstrings
      if (cstype != 2) return 0;
      if (charstrings == 0) return 0;

      if (fdarrayoff) {
         // looks like a CID font
         if (!fdselectoff) return 0;
         stbtt__buf_seek(&b, fdarrayoff);
         info->fontdicts = stbtt__cff_get_index(&b);
         info->fdselect = stbtt__buf_range(&b, fdselectoff, b.size-fdselectoff);
      }

      stbtt__buf_seek(&b, charstrings);
      info->charstrings = stbtt__cff_get_index(&b);
   }

   t = stbtt__find_table(data, fontstart, "maxp");
   if (t)
      info->numGlyphs = ttUSHORT(data+t+4);
   else
      info->numGlyphs = 0xffff;

   info->svg = -1;

   numTables = ttUSHORT(data + cmap + 2);
   info->index_map = 0;
   for (i=0; i < numTables; ++i) {
      u32 encoding_record = cmap + 4 + 8 * i;
      // find an encoding we understand:
      switch(ttUSHORT(data+encoding_record)) {
         case STBTT_PLATFORM_ID_MICROSOFT:
            switch (ttUSHORT(data+encoding_record+2)) {
               case STBTT_MS_EID_UNICODE_BMP:
               case STBTT_MS_EID_UNICODE_FULL:
                  // MS/Unicode
                  info->index_map = cmap + ttULONG(data+encoding_record+4);
                  break;
            }
            break;
        case STBTT_PLATFORM_ID_UNICODE:
            // Mac/iOS has these
            // all the encodingIDs are unicode, so we don't bother to check it
            info->index_map = cmap + ttULONG(data+encoding_record+4);
            break;
      }
   }
   if (info->index_map == 0)
      return 0;

   info->indexToLocFormat = ttUSHORT((unsigned char*)data + info->head + 50);
   return 1;
}

STBTT_DEF int stbtt_FindGlyphIndex(const stbtt_fontinfo *info, int unicode_codepoint) {
   u8 *data = info->data;
   u32 index_map = info->index_map;

   u16 format = ttUSHORT(data + index_map + 0);
   if (format == 0) { // apple byte encoding
      i32 bytes = ttUSHORT(data + index_map + 2);
      if (unicode_codepoint < bytes-6)
         return ttBYTE(data + index_map + 6 + unicode_codepoint);
      return 0;
   } else if (format == 6) {
      u32 first = ttUSHORT(data + index_map + 6);
      u32 count = ttUSHORT(data + index_map + 8);
      if ((u32) unicode_codepoint >= first && (u32) unicode_codepoint < first+count)
         return ttUSHORT(data + index_map + 10 + (unicode_codepoint - first)*2);
      return 0;
   } else if (format == 2) {
      assert(0); // @TODO: high-byte mapping for japanese/chinese/korean
      return 0;
   } else if (format == 4) { // standard mapping for windows fonts: binary search collection of ranges
      u16 segcount = ttUSHORT(data+index_map+6) >> 1;
      u16 searchRange = ttUSHORT(data+index_map+8) >> 1;
      u16 entrySelector = ttUSHORT(data+index_map+10);
      u16 rangeShift = ttUSHORT(data+index_map+12) >> 1;

      // do a binary search of the segments
      u32 endCount = index_map + 14;
      u32 search = endCount;

      if (unicode_codepoint > 0xffff)
         return 0;

      // they lie from endCount .. endCount + segCount
      // but searchRange is the nearest power of two, so...
      if (unicode_codepoint >= ttUSHORT(data + search + rangeShift*2))
         search += rangeShift*2;

      // now decrement to bias correctly to find smallest
      search -= 2;
      while (entrySelector) {
         u16 end;
         searchRange >>= 1;
         end = ttUSHORT(data + search + searchRange*2);
         if (unicode_codepoint > end)
            search += searchRange*2;
         --entrySelector;
      }
      search += 2;

      {
         u16 offset, start;
         u16 item = (u16) ((search - endCount) >> 1);

         assert(unicode_codepoint <= ttUSHORT(data + endCount + 2*item));
         start = ttUSHORT(data + index_map + 14 + segcount*2 + 2 + 2*item);
         if (unicode_codepoint < start)
            return 0;

         offset = ttUSHORT(data + index_map + 14 + segcount*6 + 2 + 2*item);
         if (offset == 0)
            return (u16) (unicode_codepoint + ttSHORT(data + index_map + 14 + segcount*4 + 2 + 2*item));

         return ttUSHORT(data + offset + (unicode_codepoint-start)*2 + index_map + 14 + segcount*6 + 2 + 2*item);
      }
   } else if (format == 12 || format == 13) {
      u32 ngroups = ttULONG(data+index_map+12);
      i32 low,high;
      low = 0; high = (i32)ngroups;
      // Binary search the right group.
      while (low < high) {
         i32 mid = low + ((high-low) >> 1); // rounds down, so low <= mid < high
         u32 start_char = ttULONG(data+index_map+16+mid*12);
         u32 end_char = ttULONG(data+index_map+16+mid*12+4);
         if ((u32) unicode_codepoint < start_char)
            high = mid;
         else if ((u32) unicode_codepoint > end_char)
            low = mid+1;
         else {
            u32 start_glyph = ttULONG(data+index_map+16+mid*12+8);
            if (format == 12)
               return start_glyph + unicode_codepoint-start_char;
            else // format == 13
               return start_glyph;
         }
      }
      return 0; // not found
   }
   // @TODO
   assert(0);
   return 0;
}

STBTT_DEF int stbtt__GetGlyfOffset(const stbtt_fontinfo *info, int glyph_index)
{
   int g1,g2;

   assert(!info->cff.size);

   if (glyph_index >= info->numGlyphs) return -1; // glyph index out of range
   if (info->indexToLocFormat >= 2)    return -1; // unknown index->glyph map format

   if (info->indexToLocFormat == 0) {
      g1 = info->glyf + ttUSHORT(info->data + info->loca + glyph_index * 2) * 2;
      g2 = info->glyf + ttUSHORT(info->data + info->loca + glyph_index * 2 + 2) * 2;
   } else {
      g1 = info->glyf + ttULONG (info->data + info->loca + glyph_index * 4);
      g2 = info->glyf + ttULONG (info->data + info->loca + glyph_index * 4 + 4);
   }

   return g1==g2 ? -1 : g1; // if length is 0, return -1
}

STBTT_DEF int stbtt__GetGlyphInfoT2(const stbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1)
{
   stbtt__csctx c = STBTT__CSCTX_INIT(1);
   int r = stbtt__run_charstring(info, glyph_index, &c);
   if (x0)  *x0 = r ? c.min_x : 0;
   if (y0)  *y0 = r ? c.min_y : 0;
   if (x1)  *x1 = r ? c.max_x : 0;
   if (y1)  *y1 = r ? c.max_y : 0;
   return r ? c.num_vertices : 0;
}

STBTT_DEF int stbtt_GetGlyphBox(const stbtt_fontinfo *info, int glyph_index, int *x0, int *y0, int *x1, int *y1)
{
   if (info->cff.size) {
      stbtt__GetGlyphInfoT2(info, glyph_index, x0, y0, x1, y1);
   } else {
      int g = stbtt__GetGlyfOffset(info, glyph_index);
      if (g < 0) return 0;

      if (x0) *x0 = ttSHORT(info->data + g + 2);
      if (y0) *y0 = ttSHORT(info->data + g + 4);
      if (x1) *x1 = ttSHORT(info->data + g + 6);
      if (y1) *y1 = ttSHORT(info->data + g + 8);
   }
   return 1;
}

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
#endif /* n RFONT_EXTERNAL_STB */

/* 
END of stb_truetype defines and source code required by RFont
*/

#endif /* RFONT_IMPLEMENTATION */
