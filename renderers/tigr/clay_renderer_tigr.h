// Copyright (c) 2026 Noah Arthur Breedy
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the
// use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//     1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software in a
//     product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//
//     2. Altered source versions must be plainly marked as such, and must not
//     be misrepresented as being the original software.
//
//     3. This notice may not be removed or altered from any source
//     distribution.
//
// SPDX-License-Identifier: Zlib

#ifndef __CLAY_TIGR_RENDERER_H__
#define __CLAY_TIGR_RENDERER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../../clay.h"
#include "tigr.h"
#include "tigr_utils.h"  // for our rounded borders


#define CLAY_TO_TPIXEL(color) (TPixel){color.r, color.g, color.b, color.a}

/* Render the command queue to the `Tigr*` instance provided */
void Clay_Tigr_Render(Clay_RenderCommandArray commands, Tigr* ctx);

/* Return a null-terminated copy of Clay_String `str`
 * caller is required to free
 * */
static inline char *Clay_to_Cstr(Clay_String *str) {
	char* copy = (char*) malloc(str->length + 1);
	if (!copy) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	memcpy(copy, str->chars, str->length);
	copy[str->length] = '\0';
	return copy;
}

/* Measure text using built-in Tigr functions */
static inline Clay_Dimensions Clay_Tigr_MeasureText(Clay_StringSlice str, Clay_TextElementConfig *config, void *userData) {

    Clay_String toTerminate = (Clay_String){ .chars = str.chars, .length = str.length, .isStaticallyAllocated = false };

    char* cstr = Clay_to_Cstr(&toTerminate);

    int text_width  = tigrTextWidth(tfont, cstr)  * (config->fontSize/8);
    int text_height = tigrTextHeight(tfont, cstr) * (config->fontSize/8);

    free(cstr);

	// Return dimensions
	return (Clay_Dimensions){
		.width =  (float)text_width,
		.height = (float)text_height
	};
}

#endif /* __CLAY_TIGR_RENDERER_H__ */
