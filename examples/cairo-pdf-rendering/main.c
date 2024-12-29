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

#include <stdlib.h>

// The renderer includes clay.h while also providing the
// CLAY_IMPLEMENTATION
#include "../../renderers/cairo/clay_renderer_cairo.c"

// cairo-pdf, though this is optional and not required if you,
// e.g. render PNGs.
#include <cairo/cairo-pdf.h>

// Layout the first page.
void Layout() {
	static Clay_Color PRIMARY = { 0xa8, 0x42, 0x1c, 255 };
	static Clay_Color BACKGROUND = { 0xF4, 0xEB, 0xE6, 255 };
	static Clay_Color ACCENT = { 0xFA, 0xE0, 0xD4, 255 };

	CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
					   .layoutDirection = CLAY_TOP_TO_BOTTOM }),
		CLAY_RECTANGLE({ .color = BACKGROUND })) {
		CLAY(CLAY_ID("PageMargins"),
			 CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
					 .padding = { 70., 50. }, // Some nice looking page margins
					 .layoutDirection = CLAY_TOP_TO_BOTTOM,
					 .childGap = 10})) {

			// Section Title
			CLAY(CLAY_TEXT(
					 CLAY_STRING("Features Overview"),
					 CLAY_TEXT_CONFIG({
							 .fontFamily = CLAY_STRING("Calistoga"),
							 .textColor = PRIMARY,
							 .fontSize = 24
						 })
					 ));

			// Feature Box
			CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT() }, .childGap = 10 })) {
				CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT() }}), CLAY_RECTANGLE({
							.color = ACCENT,
							.cornerRadius = CLAY_CORNER_RADIUS(12),
						})) {
					CLAY(CLAY_LAYOUT({.padding = { 20., 20. }, .childGap = 4, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {
						CLAY_TEXT(CLAY_STRING("- High performance"),
								  CLAY_TEXT_CONFIG({ .textColor = PRIMARY, .fontSize = 14, .fontFamily = CLAY_STRING("Quicksand SemiBold") }));
						CLAY_TEXT(CLAY_STRING("- Declarative syntax"),
								  CLAY_TEXT_CONFIG({ .textColor = PRIMARY, .fontSize = 14, .fontFamily = CLAY_STRING("Quicksand SemiBold") }));
						CLAY_TEXT(CLAY_STRING("- Flexbox-style responsive layout"),
								  CLAY_TEXT_CONFIG({ .textColor = PRIMARY, .fontSize = 14, .fontFamily = CLAY_STRING("Quicksand SemiBold") }));
						CLAY_TEXT(CLAY_STRING("- Single .h file for C/C++"),
								  CLAY_TEXT_CONFIG({ .textColor = PRIMARY, .fontSize = 14, .fontFamily = CLAY_STRING("Quicksand SemiBold") }));
						CLAY_TEXT(CLAY_STRING("- And now with cairo!"),
								  CLAY_TEXT_CONFIG({ .textColor = PRIMARY, .fontSize = 14, .fontFamily = CLAY_STRING("Quicksand SemiBold") }));
					}
				}
				CLAY(CLAY_LAYOUT({
							.sizing = {CLAY_SIZING_FIT(), CLAY_SIZING_GROW()},
							.padding = { 10, 10 },
							.layoutDirection = CLAY_TOP_TO_BOTTOM,
							.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
							.childGap = 4
						}), CLAY_RECTANGLE({ .color = ACCENT, .cornerRadius = CLAY_CORNER_RADIUS(8) })) {
					// Profile picture
					CLAY(CLAY_LAYOUT({
								.sizing = {CLAY_SIZING_FIT(), CLAY_SIZING_GROW()},
								.padding = { 30, 0 },
								.layoutDirection = CLAY_TOP_TO_BOTTOM,
								.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }}), CLAY_BORDER_OUTSIDE_RADIUS(2, PRIMARY, 10)) {
						CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_FIXED(32), CLAY_SIZING_FIXED(32) } }), CLAY_IMAGE({ .sourceDimensions = { 32, 32 }, .path = CLAY_STRING("resources/check.png") }));
					}
				}
			}

			CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIXED(16) } }));

			CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() }, .childGap = 10, .layoutDirection = CLAY_TOP_TO_BOTTOM })) {
				CLAY_TEXT(CLAY_STRING("Cairo"), CLAY_TEXT_CONFIG({ .fontFamily = CLAY_STRING("Calistoga"), .fontSize = 24, .textColor = PRIMARY }));
				CLAY(CLAY_LAYOUT({ .padding = { 10, 10 } }), CLAY_RECTANGLE({ .color = ACCENT, .cornerRadius = CLAY_CORNER_RADIUS(10) })) {
					CLAY_TEXT(CLAY_STRING("Officiis quia quia qui inventore ratione voluptas et. Quidem sunt unde similique. Qui est et exercitationem cumque harum illum. Numquam placeat aliquid quo voluptatem. "
										  "Deleniti saepe nihil exercitationem nemo illo. Consequatur beatae repellat provident similique. Provident qui exercitationem deserunt sapiente. Quam qui dolor corporis odit. "
										  "Assumenda corrupti sunt culpa pariatur. Vero sit ut minima. In est consequatur minus et cum sint illum aperiam. Qui ipsa quas nisi omnis aut quia nobis. "
										  "Corporis deserunt eum mollitia modi rerum voluptas. Expedita non ab esse. Sit voluptates eos voluptatem labore aspernatur quia eum. Modi cumque atque non. Sunt officiis corrupti neque ut inventore excepturi rem minima. Possimus sed soluta qui ea aut ipsum laborum fugit. "
										  "Voluptate eum consectetur non. Quo autem voluptate soluta atque dolorum maxime. Officiis inventore omnis eveniet beatae ipsa optio. Unde voluptatum ut autem quia sit sit et. Ut inventore qui quia totam consequatur. Sit ea consequatur omnis rerum nulla aspernatur deleniti."), CLAY_TEXT_CONFIG({ .fontFamily = CLAY_STRING("Quicksand SemiBold"), .fontSize = 16, .textColor = PRIMARY, .lineHeight = 16 }));
				}
			}
		}
	}
}

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}

int main(void) {
	// First we set up our cairo surface.
	// In this example we will use the PDF backend,
	// but you should be able to use any of them.
	// Guaranteed to be working are: PDF, PNG

	// Create a PDF surface that is the same size as a DIN A4 sheet
	// When using the PDF backend, cairo calculates in points (1 point == 1/72.0 inch)
	double width = (21.0 / 2.54) * 72,  // cm in points
		   height = (29.7 / 2.54) * 72;

	cairo_surface_t *surface = cairo_pdf_surface_create("output.pdf", width, height);
	cairo_t *cr = cairo_create(surface);
	cairo_surface_destroy(surface); // Drop reference

	// Initialize internal global variable with `cr`.
	// We require some kind of global reference to a valid
	// cairo instance to properly measure text.
	// Note that due to this, this interface is not thread-safe!
	Clay_Cairo_Initialize(cr);

	uint64_t totalMemorySize = Clay_MinMemorySize();
	Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
	Clay_SetMeasureTextFunction(Clay_Cairo_MeasureText);

	// We initialize Clay with the same size
	Clay_Initialize(clayMemory, (Clay_Dimensions) { width, height }, (Clay_ErrorHandler) { HandleClayErrors });

	Clay_BeginLayout();

	// Here you can now create the declarative clay layout.
	// Moved into a separate function for brevity.
	Layout();

	Clay_RenderCommandArray commands = Clay_EndLayout();
	// Pass our layout to the cairo backend
	Clay_Cairo_Render(commands);

	// To keep this example short, we will not emit a second page in the PDF.
	// But to do so, you have to
	// 1. cairo_show_page(cr)
	// 2. Clay_BeginLayout();
	// 3. Create your layout
	// 4. commands = Clay_EndLayout();
	// 5. Clay_Cairo_Render(commands);

	cairo_destroy(cr);
	return 0;
}

