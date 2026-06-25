#include "clay_renderer_tigr.h"

#define CLAY_TO_TPIXEL(color) (TPixel){color.r, color.g, color.b, color.a}

void Clay_Tigr_Render(Clay_RenderCommandArray commands, Tigr* ctx) {
	for(size_t i = 0; i < commands.length; i++) {
		Clay_RenderCommand *command = Clay_RenderCommandArray_Get(&commands, i);

		switch(command->commandType) {
		case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
            Clay_RectangleRenderData* config = &command->renderData.rectangle;
			Clay_BoundingBox bb = command->boundingBox;

            int box_radius = config->cornerRadius.topLeft;  // only take take top left radius

            tigrFillArcRect(ctx, bb.x, bb.y, bb.width, bb.height, box_radius, CLAY_TO_TPIXEL(config->backgroundColor));

			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_TEXT: {
            Clay_TextRenderData* config = &command->renderData.text;
            Clay_String toTerminate = (Clay_String){ .chars = config->stringContents.chars, .length = config->stringContents.length, .isStaticallyAllocated = false };
			char* text = Clay_to_Cstr(&toTerminate);

			Clay_BoundingBox bb = command->boundingBox;
			Clay_Color color = config->textColor;

            float scale = config->fontSize / 8.0f; // tfont is roughly 8px high

            Tigr* temp = tigrBitmap(
                tigrTextWidth(tfont, text),
                tfont->glyphs->h
            );

            tigrClear(temp, tigrRGBA(0,0,0,0));
            tigrPrint(temp, tfont, 0, 0, CLAY_TO_TPIXEL(color), text);

            tigrBlitScale(ctx, temp, bb.x, bb.y, 0, 0, temp->w, temp->h, temp->w * scale, temp->h * scale);

            tigrFree(temp);
			free(text);
			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_BORDER: {
            Clay_BorderRenderData* config = &command->renderData.border;
			Clay_BoundingBox bb = command->boundingBox;

			int box_radius = config->cornerRadius.topLeft;

            tigrArcRect(ctx, bb.x, bb.y, bb.width, bb.height, box_radius, CLAY_TO_TPIXEL(config->color));

			break;
		}
		case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
            Clay_ImageRenderData *config = &command->renderData.image;
			Clay_BoundingBox bb = command->boundingBox;

			char* path = config->imageData;

			Tigr* img = tigrLoadImage(path);

			double image_w = img->w;
            double image_h = img->h;

			/* Calculate the scaling factor to fit within the bounding box while preserving aspect ratio */
			double scale_w = bb.width / image_w;
			double scale_h = bb.height / image_h;
			double scale = (scale_w < scale_h) ? scale_w : scale_h; // Use the smaller scaling factor

			/* Apply the same scale to both dimensions to preserve aspect ratio */
			double scale_x = scale;
			double scale_y = scale;

			/* Calculate the scaled image dimensions */
			double scaled_w = image_w * scale_x;
			double scaled_h = image_h * scale_y;

			/* Adjust the x and y coordinates to center the scaled image within the bounding box */
			double centered_x = bb.x + (bb.width - scaled_w) / 2.0;
			double centered_y = bb.y + (bb.height - scaled_h) / 2.0;

			/* Blit the scaled and centered image */
            tigrBlit(ctx, img, centered_x, centered_y, 0, 0, scaled_w, scaled_h);

			/* Clean up the source surface */
            tigrFree(img);
			break;
		}
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
			Clay_BoundingBox bb = command->boundingBox;
            tigrClip(ctx, bb.x, bb.y, bb.width, bb.height);
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
            tigrClip(ctx, 0, 0, -1, -1);
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
