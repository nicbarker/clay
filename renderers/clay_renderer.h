#pragma once

#include "../clay.h"

struct Clay_Renderer_Data;
void Clay_Renderer_Initialize(struct Clay_Renderer_Data *data);
void Clay_Renderer_Render(Clay_RenderCommandArray renderCommands);
Clay_Dimensions Clay_Renderer_MeasureText(Clay_String *text, Clay_TextElementConfig *config);
