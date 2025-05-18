// Copyright (c) 2024 Justin Andreas Lacoste (@27justin)
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#include <cairo/cairo.h>

////////////////////////////////
//
// Public API
//

// Initialize the internal cairo pointer with the user provided instance.
// This is REQUIRED before calling Clay_Cairo_Render.
void Clay_Cairo_Initialize(cairo_t *cairo);

// Render the command queue to the `cairo_t*` instance you called
// `Clay_Cairo_Initialize` on.
void Clay_Cairo_Render(Clay_RenderCommandArray commands, char** fonts);
////////////////////////////////


////////////////////////////////
// Convencience macros
//
#define CLAY_TO_CAIRO(color) color.r / 255.0, color.g / 255.0, color.b / 255.0, color.a / 255.0
#define DEG2RAD(degrees) (degrees * ( M_PI / 180.0 ) )
////////////////////////////////


////////////////////////////////
// Implementation
//

// Cairo instance
static cairo_t *Clay__Cairo = NULL;

// Return a null-terminated copy of Clay_String `str`.
// Callee is required to free.
static inline char *Clay_Cairo__NullTerminate(Clay_String *str) {
	char *copy = (char*) malloc(str->length + 1);
	if (!copy) {
		fprintf(stderr, "Memory allocation failed\n");
		return NULL;
	}
	memcpy(copy, str->chars, str->length);
	copy[str->length] = '\0';
	return copy;
}

// Measure text using cairo's *toy* text API.
static inline Clay_Dimensions Clay_Cairo_MeasureText(Clay_StringSlice str, Clay_TextElementConfig *config, void *userData) {
	// Edge case: Clay computes the width of a whitespace character
	// once.  Cairo does not factor in whitespaces when computing text
	// extents, this edge-case serves as a short-circuit to introduce
	// (somewhat) sensible values into Clay.
    char** fonts = (char**)userData;
	if(str.length == 1 && str.chars[0] == ' ') {
		cairo_text_extents_t te;
		cairo_text_extents(Clay__Cairo, " ", &te);
		return (Clay_Dimensions) {
			// The multiplication here follows no real logic, just
			// brute-forcing it until the text boundaries look
			// okay-ish.  You should probably rather use a proper text
			// shaping engine like HarfBuzz or Pango.
			.width = ((float) te.x_advance) * 1.9f,
			.height = (float) config->fontSize
		};
	}

	// Ensure string is null-terminated for Cairo
    Clay_String toTerminate = (Clay_String){ .chars = str.chars, .length = str.length, .isStaticallyAllocated = false };
	char *text = Clay_Cairo__NullTerminate(&toTerminate);
	char *font_family = fonts[config->fontId];

	// Save and reset the Cairo context to avoid unwanted transformations
	cairo_save(Clay__Cairo);
	cairo_identity_matrix(Clay__Cairo);

	// Set font properties
	cairo_select_font_face(Clay__Cairo, font_family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(Clay__Cairo, config->fontSize);

	// Use glyph extents for better precision
	cairo_scaled_font_t *scaled_font = cairo_get_scaled_font(Clay__Cairo);
	if (!scaled_font) {
		fprintf(stderr, "Failed to get scaled font\n");
		cairo_restore(Clay__Cairo);
		free(text);
		return (Clay_Dimensions){0, 0};
	}

	cairo_glyph_t *glyphs = NULL;
	int num_glyphs = 0;
	cairo_status_t status = cairo_scaled_font_text_to_glyphs(
		scaled_font, 0, 0, text, -1, &glyphs, &num_glyphs, NULL, NULL, NULL
	);

	if (status != CAIRO_STATUS_SUCCESS || !glyphs || num_glyphs == 0) {
		fprintf(stderr, "Failed to generate glyphs: %s\n", cairo_status_to_string(status));
		cairo_restore(Clay__Cairo);
		free(text);
		return (Clay_Dimensions){0, 0};
	}

	// Measure the glyph extents
	cairo_text_extents_t glyph_extents;
	cairo_glyph_extents(Clay__Cairo, glyphs, num_glyphs, &glyph_extents);

	// Clean up glyphs
	cairo_glyph_free(glyphs);

	// Restore the Cairo context
	cairo_restore(Clay__Cairo);

	// Free temporary strings
	free(text);

	// Return dimensions
	return (Clay_Dimensions){
		.width =  (float) glyph_extents.width,
		.height = (float) glyph_extents.height
	};
}


void Clay_Cairo_Initialize(cairo_t *cairo) {
	Clay__Cairo = cairo;
}

// Internally used to copy images onto our document/active workspace.
void Clay_Cairo__Blit_Surface(cairo_surface_t *src_surface, cairo_surface_t *dest_surface,
							  double x, double y, double scale_x, double scale_y) {
	// Create a cairo context for the destination surface
	cairo_t *cr = cairo_create(dest_surface);

	// Save the context's state
	cairo_save(cr);

	// Apply translation to position the source at (x, y)
	cairo_translate(cr, x, y);

	// Apply scaling to the context
	cairo_scale(cr, scale_x, scale_y);

	// Set the source surface at (0, 0) after applying transformations
	cairo_set_source_surface(cr, src_surface, 0, 0);

	// Paint the scaled source surface onto the destination surface
	cairo_paint(cr);

	// Restore the context's state to remove transformations
	cairo_restore(cr);

	// Clean up
	cairo_destroy(cr);
}

void Clay_Cairo_Render(Clay_RenderCommandArray commands, char** fonts) {
	cairo_t *cr = Clay__Cairo;
	for(size_t i = 0; i < commands.length; i++) {
		Clay_RenderCommand *command = Clay_RenderCommandArray_Get(&commands, i);

		switch(command->commandType) {
		case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
            Clay_RectangleRenderData *config = &command->renderData.rectangle;
			Clay_BoundingBox bb = command->boundingBox;

			cairo_set_source_rgba(cr, CLAY_TO_CAIRO(config->backgroundColor));

			cairo_new_sub_path(cr);
			cairo_arc(cr, bb.x + config->cornerRadius.topLeft,
					  bb.y + config->cornerRadius.topLeft,
					  config->cornerRadius.topLeft,
					  M_PI, 3 * M_PI / 2); // 180° to 270°
			cairo_arc(cr, bb.x + bb.width - config->cornerRadius.topRight,
					  bb.y + config->cornerRadius.topRight,
					  config->cornerRadius.topRight,
					  3 * M_PI / 2, 2 * M_PI); // 270° to 360°
			cairo_arc(cr, bb.x + bb.width - config->cornerRadius.bottomRight,
					  bb.y + bb.height - config->cornerRadius.bottomRight,
					  config->cornerRadius.bottomRight,
					  0, M_PI / 2); // 0° to 90°
			cairo_arc(cr, bb.x + config->cornerRadius.bottomLeft,
					  bb.y + bb.height - config->cornerRadius.bottomLeft,
					  config->cornerRadius.bottomLeft,
					  M_PI / 2, M_PI); // 90° to 180°
			cairo_close_path(cr);

			cairo_fill(cr);
			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_TEXT: {
			// Cairo expects null terminated strings, we need to clone
			// to temporarily introduce one.
            Clay_TextRenderData *config = &command->renderData.text;
            Clay_String toTerminate = (Clay_String){ .chars = config->stringContents.chars, .length = config->stringContents.length, .isStaticallyAllocated = false };
			char *text = Clay_Cairo__NullTerminate(&toTerminate);
			char *font_family = fonts[config->fontId];

			Clay_BoundingBox bb = command->boundingBox;
			Clay_Color color = config->textColor;

			cairo_select_font_face(Clay__Cairo, font_family, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
			cairo_set_font_size(cr, config->fontSize);

			cairo_move_to(cr, bb.x, bb.y + bb.height);

			cairo_set_source_rgba(cr, CLAY_TO_CAIRO(color));
			cairo_show_text(cr, text);
			cairo_close_path(cr);

			free(text);
			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_BORDER: {
            Clay_BorderRenderData *config = &command->renderData.border;
			Clay_BoundingBox bb = command->boundingBox;

			double top_left_radius = config->cornerRadius.topLeft / 2.0;
			double top_right_radius = config->cornerRadius.topRight / 2.0;
			double bottom_right_radius = config->cornerRadius.bottomRight / 2.0;
			double bottom_left_radius = config->cornerRadius.bottomLeft / 2.0;

			// Draw the top border
			if (config->width.top > 0) {
				cairo_set_line_width(cr, config->width.top);
				cairo_set_source_rgba(cr, CLAY_TO_CAIRO(config->color));

				cairo_new_sub_path(cr);

				// Left half-arc for top-left corner
				cairo_arc(cr, bb.x + top_left_radius, bb.y + top_left_radius, top_left_radius, DEG2RAD(225), DEG2RAD(270));

				// Line to right half-arc
				cairo_line_to(cr, bb.x + bb.width - top_right_radius, bb.y);

				// Right half-arc for top-right corner
				cairo_arc(cr, bb.x + bb.width - top_right_radius, bb.y + top_right_radius, top_right_radius, DEG2RAD(270), DEG2RAD(305));

				cairo_stroke(cr);
			}

			// Draw the right border
			if (config->width.right > 0) {
				cairo_set_line_width(cr, config->width.right);
				cairo_set_source_rgba(cr, CLAY_TO_CAIRO(config->color));

				cairo_new_sub_path(cr);

				// Top half-arc for top-right corner
				cairo_arc(cr, bb.x + bb.width - top_right_radius, bb.y + top_right_radius, top_right_radius, DEG2RAD(305), DEG2RAD(350));

				// Line to bottom half-arc
				cairo_line_to(cr, bb.x + bb.width, bb.y + bb.height - bottom_right_radius);

				// Bottom half-arc for bottom-right corner
				cairo_arc(cr, bb.x + bb.width - bottom_right_radius, bb.y + bb.height - bottom_right_radius, bottom_right_radius, DEG2RAD(0), DEG2RAD(45));

				cairo_stroke(cr);
			}

			// Draw the bottom border
			if (config->width.bottom > 0) {
				cairo_set_line_width(cr, config->width.bottom);
				cairo_set_source_rgba(cr, CLAY_TO_CAIRO(config->color));

				cairo_new_sub_path(cr);

				// Right half-arc for bottom-right corner
				cairo_arc(cr, bb.x + bb.width - bottom_right_radius, bb.y + bb.height - bottom_right_radius, bottom_right_radius, DEG2RAD(45), DEG2RAD(90));

				// Line to left half-arc
				cairo_line_to(cr, bb.x + bottom_left_radius, bb.y + bb.height);

				// Left half-arc for bottom-left corner
				cairo_arc(cr, bb.x + bottom_left_radius, bb.y + bb.height - bottom_left_radius, bottom_left_radius, DEG2RAD(90), DEG2RAD(135));

				cairo_stroke(cr);
			}

			// Draw the left border
			if (config->width.left > 0) {
				cairo_set_line_width(cr, config->width.left);
				cairo_set_source_rgba(cr, CLAY_TO_CAIRO(config->color));

				cairo_new_sub_path(cr);

				// Bottom half-arc for bottom-left corner
				cairo_arc(cr, bb.x + bottom_left_radius, bb.y + bb.height - bottom_left_radius, bottom_left_radius, DEG2RAD(135), DEG2RAD(180));

				// Line to top half-arc
				cairo_line_to(cr, bb.x, bb.y + top_left_radius);

				// Top half-arc for top-left corner
				cairo_arc(cr, bb.x + top_left_radius, bb.y + top_left_radius, top_left_radius, DEG2RAD(180), DEG2RAD(225));

				cairo_stroke(cr);
			}
			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
            Clay_ImageRenderData *config = &command->renderData.image;
			Clay_BoundingBox bb = command->boundingBox;

			char *path = config->imageData;

			cairo_surface_t *surf = cairo_image_surface_create_from_png(path),
							*origin = cairo_get_target(cr);

			// Calculate the original image dimensions
			double image_w = cairo_image_surface_get_width(surf),
				image_h = cairo_image_surface_get_height(surf);

			// Calculate the scaling factor to fit within the bounding box while preserving aspect ratio
			double scale_w = bb.width / image_w;
			double scale_h = bb.height / image_h;
			double scale = (scale_w < scale_h) ? scale_w : scale_h; // Use the smaller scaling factor

			// Apply the same scale to both dimensions to preserve aspect ratio
			double scale_x = scale;
			double scale_y = scale;

			// Calculate the scaled image dimensions
			double scaled_w = image_w * scale_x;
			double scaled_h = image_h * scale_y;

			// Adjust the x and y coordinates to center the scaled image within the bounding box
			double centered_x = bb.x + (bb.width - scaled_w) / 2.0;
			double centered_y = bb.y + (bb.height - scaled_h) / 2.0;

			// Blit the scaled and centered image
			Clay_Cairo__Blit_Surface(surf, origin, centered_x, centered_y, scale_x, scale_y);

			// Clean up the source surface
			cairo_surface_destroy(surf);
			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
			// Slot your custom elements in here.
		}
		default: {
			fprintf(stderr, "Unknown command type %d\n", (int) command->commandType);
		}
		}
	}
}
