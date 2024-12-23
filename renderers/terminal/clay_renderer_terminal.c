#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#ifdef CLAY_OVERFLOW_TRAP
#include "signal.h"
#endif

#define gotoxy(x, y) printf("\033[%d;%dH", y+1, x+1)

static inline void Console_DrawRectangle(int x0, int y0, int width, int height, Clay_Color color) {
	if (color.r < 127 || color.g < 127 || color.b < 127 || color.a < 127) {
		// For now there are only two colors,
		return;
	}

	for (int y = y0; y < height; y++) {
		for (int x = x0; x < width; x++) {
			gotoxy(x, y);
			// TODO there are only two colors actually drawn, the background and white
			if (color.r < 127 || color.g < 127 || color.b < 127 || color.a < 127) {
				printf(" ");
			} else {
				printf("▪");
			}
		}
	}
}

static inline Clay_Dimensions Console_MeasureText(Clay_String *text, Clay_TextElementConfig *config) {
	Clay_Dimensions textSize = { 0 };

	// TODO this function is very wrong, it measures in characters, I have no idea what is the size in pixels

	float maxTextWidth = 0.0f;
	float lineTextWidth = 0;

	float textHeight = 1;

	for (int i = 0; i < text->length; ++i)
	{
		if (text->chars[i] == '\n') {
			maxTextWidth = maxTextWidth > lineTextWidth ? maxTextWidth : lineTextWidth;
			lineTextWidth = 0;
			textHeight++;
			continue;
		}
		lineTextWidth++;
	}

	maxTextWidth = maxTextWidth > lineTextWidth ? maxTextWidth : lineTextWidth;

	textSize.width = maxTextWidth;
	textSize.height = textHeight;

	return textSize;
}

void Clay_Raylib_Initialize(int width, int height, const char *title, unsigned int flags) {
	//TODO
}

void Clay_Console_Render(Clay_RenderCommandArray renderCommands)
{
	printf("\033[H\033[J"); // Clear

	for (int j = 0; j < renderCommands.length; j++)
	{
		Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, j);
		Clay_BoundingBox boundingBox = renderCommand->boundingBox;
		switch (renderCommand->commandType)
		{
			case CLAY_RENDER_COMMAND_TYPE_TEXT: {
				Clay_String text = renderCommand->text;
				int k = 0;
				for (int i = 0; i < text.length; i++) {
					if(text.chars[i] == '\n') {
						k++;
						continue;
					}

					gotoxy((int) boundingBox.x + i, (int) boundingBox.y + k);
					printf("%c", text.chars[i]);
				}
				break;
			}
			case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
				//TODO
				break;
			}
			case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
				//TODO
				break;
			}
			case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
				Clay_RectangleElementConfig *config = renderCommand->config.rectangleElementConfig;
				Console_DrawRectangle((int)boundingBox.x, (int)boundingBox.y, (int)boundingBox.width, (int)boundingBox.height, config->color);
				break;
			}
			case CLAY_RENDER_COMMAND_TYPE_BORDER: {
				Clay_BorderElementConfig *config = renderCommand->config.borderElementConfig;
				// Left border
				if (config->left.width > 0) {
					Console_DrawRectangle((int)(boundingBox.x), (int)(boundingBox.y + config->cornerRadius.topLeft), (int)config->left.width, (int)(boundingBox.height - config->cornerRadius.topLeft - config->cornerRadius.bottomLeft), config->left.color);
				}
				// Right border
				if (config->right.width > 0) {
					Console_DrawRectangle((int)(boundingBox.x + boundingBox.width - config->right.width), (int)(boundingBox.y + config->cornerRadius.topRight), (int)config->right.width, (int)(boundingBox.height - config->cornerRadius.topRight - config->cornerRadius.bottomRight), config->right.color);
				}
				// Top border
				if (config->top.width > 0) {
					Console_DrawRectangle((int)(boundingBox.x + config->cornerRadius.topLeft), (int)(boundingBox.y), (int)(boundingBox.width - config->cornerRadius.topLeft - config->cornerRadius.topRight), (int)config->top.width, config->top.color);
				}
				// Bottom border
				if (config->bottom.width > 0) {
					Console_DrawRectangle((int)(boundingBox.x + config->cornerRadius.bottomLeft), (int)(boundingBox.y + boundingBox.height - config->bottom.width), (int)(boundingBox.width - config->cornerRadius.bottomLeft - config->cornerRadius.bottomRight), (int)config->bottom.width, config->bottom.color);
				}
				break;
			}
			default: {
				printf("Error: unhandled render command.");
#ifdef CLAY_OVERFLOW_TRAP
				raise(SIGTRAP);
#endif
				exit(1);
			}
		}
	}
}
