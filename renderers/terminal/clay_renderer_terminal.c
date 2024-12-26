#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#ifdef CLAY_OVERFLOW_TRAP
#include "signal.h"
#endif

static inline void Console_MoveCursor(int x, int y) {
	printf("\033[%d;%dH", y+1, x+1);
}

bool Clay_PointIsInsideRect(Clay_Vector2 point, Clay_BoundingBox rect) {
	// TODO this function is a copy of Clay__PointIsInsideRect but that one is internal, I don't know if we want
	// TODO to expose Clay__PointIsInsideRect
	return point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height;
}

static inline void Console_DrawRectangle(int x0, int y0, int width, int height, Clay_Color color,
										 Clay_BoundingBox scissorBox) {
	if (color.r < 127 || color.g < 127 || color.b < 127 || color.a < 127) {
		// For now there are only two colors,
		return;
	}

	for (int y = y0; y < height; y++) {
		for (int x = x0; x < width; x++) {
			if(!Clay_PointIsInsideRect((Clay_Vector2) { .x = x, .y = y }, scissorBox)) {
				continue;
			}

			Console_MoveCursor(x, y);
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

void Clay_Console_Render(Clay_RenderCommandArray renderCommands, int width, int height)
{
	printf("\033[H\033[J"); // Clear

	const Clay_BoundingBox fullWindow = {
		.x = 0,
		.y = 0,
		.width = (float) width,
		.height = (float) height,
	};

	Clay_BoundingBox scissorBox = fullWindow;

	for (int j = 0; j < renderCommands.length; j++)
	{
		Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, j);
		Clay_BoundingBox boundingBox = renderCommand->boundingBox;
		switch (renderCommand->commandType)
		{
			case CLAY_RENDER_COMMAND_TYPE_TEXT: {
				Clay_String text = renderCommand->text;
				int y = 0;
				for (int x = 0; x < text.length; x++) {
					if(text.chars[x] == '\n') {
						y++;
						continue;
					}

					int cursorX = (int) boundingBox.x + x;
					int cursorY = (int) boundingBox.y + y;
					if(!Clay_PointIsInsideRect((Clay_Vector2) { .x = cursorX, .y = cursorY }, scissorBox)) {
						continue;
					}

					Console_MoveCursor(cursorX, cursorY);
					printf("%c", text.chars[x]);
				}
				break;
			}
			case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
				scissorBox = boundingBox;
				break;
			}
			case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
				scissorBox = fullWindow;
				break;
			}
			case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
				Clay_RectangleElementConfig *config = renderCommand->config.rectangleElementConfig;
				Console_DrawRectangle(
					(int)boundingBox.x,
					(int)boundingBox.y,
					(int)boundingBox.width,
					(int)boundingBox.height,
					config->color,
					scissorBox);
				break;
			}
			case CLAY_RENDER_COMMAND_TYPE_BORDER: {
				Clay_BorderElementConfig *config = renderCommand->config.borderElementConfig;
				// Left border
				if (config->left.width > 0) {
					Console_DrawRectangle(
						(int)(boundingBox.x),
						(int)(boundingBox.y + config->cornerRadius.topLeft),
						(int)config->left.width,
						(int)(boundingBox.height - config->cornerRadius.topLeft - config->cornerRadius.bottomLeft),
						config->left.color,
						scissorBox);
				}
				// Right border
				if (config->right.width > 0) {
					Console_DrawRectangle(
						(int)(boundingBox.x + boundingBox.width - config->right.width),
						(int)(boundingBox.y + config->cornerRadius.topRight),
						(int)config->right.width,
						(int)(boundingBox.height - config->cornerRadius.topRight - config->cornerRadius.bottomRight),
						config->right.color,
						scissorBox);
				}
				// Top border
				if (config->top.width > 0) {
					Console_DrawRectangle(
						(int)(boundingBox.x + config->cornerRadius.topLeft),
						(int)(boundingBox.y),
						(int)(boundingBox.width - config->cornerRadius.topLeft - config->cornerRadius.topRight),
						(int)config->top.width,
						config->top.color,
						scissorBox);
				}
				// Bottom border
				if (config->bottom.width > 0) {
					Console_DrawRectangle(
						(int)(boundingBox.x + config->cornerRadius.bottomLeft),
						(int)(boundingBox.y + boundingBox.height - config->bottom.width),
						(int)(boundingBox.width - config->cornerRadius.bottomLeft - config->cornerRadius.bottomRight),
						(int)config->bottom.width,
						config->bottom.color,
						scissorBox);
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

	Console_MoveCursor(-1, -1);  // TODO make the user not be able to write
}
