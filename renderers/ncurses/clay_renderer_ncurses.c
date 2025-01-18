//#include <stdio.h>
#include <curses.h>
#define CLAY_IMPLEMENTATION
#include "../../clay.h"

#define HPIXELS_PER_CHAR 5 //these are used to convert between Clay pixel space and terminal character locations
#define VPIXELS_PER_CHAR 8

void Clay_ncurses_Render(WINDOW * win, Clay_RenderCommandArray renderCommands);

void Clay_ncurses_Render(WINDOW * win, Clay_RenderCommandArray renderCommands){
    short color_pair = 1; //increment on use, 0 is reserved
    short color = 10; //get passed reserved colors
    //maybe keep a list of Clay colors and only init a new color if required.
    //clear the screen/window
    clear();//sets cursor to 0,0
    for(int i = 0; i < renderCommands.length; i++){
        //handle every command
        switch (renderCommands.internalArray[i].commandType){
            case CLAY_RENDER_COMMAND_TYPE_NONE:
                continue;
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
                Clay_RectangleElementConfig *rectangle_config = renderCommands.internalArray[i].config.rectangleElementConfig;
                init_color(color, rectangle_config->color.r, rectangle_config->color.g, rectangle_config->color.b);
                init_pair(color_pair, color, color);
                attr_on(color_set(color_pair,0),0);
                for(int j = 0; j < renderCommands.internalArray[i].boundingBox.height/VPIXELS_PER_CHAR; j++){
                    mvhline(renderCommands.internalArray[i].boundingBox.y/VPIXELS_PER_CHAR + j, renderCommands.internalArray[i].boundingBox.x/HPIXELS_PER_CHAR, '#', renderCommands.internalArray[i].boundingBox.width/HPIXELS_PER_CHAR);
                }
                attr_off(color_set(color_pair,0),0);
                color_pair++;
                color++;
                //TODO render radius corners
                break;
            case CLAY_RENDER_COMMAND_TYPE_BORDER:
                Clay_BorderElementConfig *border_config = renderCommands.internalArray[i].config.borderElementConfig;
                //just get a border on there for now
                if(border_config->top.width > 0){
                    init_color(color, border_config->top.color.r, border_config->top.color.g, border_config->top.color.b);
                    init_pair(color_pair, color, COLOR_CYAN);//TODO get color at target location and init pair with that background
                    attr_on(color_set(color_pair,0),0);
                    mvhline(renderCommands.internalArray[i].boundingBox.y/VPIXELS_PER_CHAR, renderCommands.internalArray[i].boundingBox.x/HPIXELS_PER_CHAR + 1, '-', renderCommands.internalArray[i].boundingBox.width/HPIXELS_PER_CHAR - 2);
                    attr_off(color_set(color_pair,0),0);
                    color_pair++; //can we just check of the color requested is already there?
                    color++;
                }
                if(border_config->bottom.width > 0){
                    init_color(color, border_config->bottom.color.r, border_config->bottom.color.g, border_config->bottom.color.b);
                    init_pair(color_pair, color, COLOR_CYAN);//TODO get color at target location and init pair with that background
                    attr_on(color_set(color_pair,0),0);
                    mvhline(renderCommands.internalArray[i].boundingBox.y/VPIXELS_PER_CHAR + renderCommands.internalArray[i].boundingBox.height/VPIXELS_PER_CHAR, renderCommands.internalArray[i].boundingBox.x/HPIXELS_PER_CHAR + 1, '-', renderCommands.internalArray[i].boundingBox.width/HPIXELS_PER_CHAR - 2);
                    attr_off(color_set(color_pair,0),0);
                    color_pair++;
                    color++;
                }
                if(border_config->left.width > 0){
                    init_color(color, border_config->left.color.r, border_config->left.color.g, border_config->left.color.b);
                    init_pair(color_pair, color, COLOR_CYAN);//TODO get color at target location and init pair with that background
                    attr_on(color_set(color_pair,0),0);
                    mvvline(renderCommands.internalArray[i].boundingBox.y/VPIXELS_PER_CHAR + 1, renderCommands.internalArray[i].boundingBox.x/HPIXELS_PER_CHAR, '|', renderCommands.internalArray[i].boundingBox.height/VPIXELS_PER_CHAR - 1);
                    attr_off(color_set(color_pair,0),0);
                    color_pair++;
                    color++;
                }
                if(border_config->right.width > 0){
                    init_color(color, border_config->right.color.r, border_config->right.color.g, border_config->right.color.b);
                    init_pair(color_pair, color, COLOR_CYAN);//TODO get color at target location and init pair with that background
                    attr_on(color_set(color_pair,0),0);
                    mvvline(renderCommands.internalArray[i].boundingBox.y/VPIXELS_PER_CHAR + 1, renderCommands.internalArray[i].boundingBox.x/HPIXELS_PER_CHAR + renderCommands.internalArray[i].boundingBox.width/HPIXELS_PER_CHAR - 1, '|', renderCommands.internalArray[i].boundingBox.height/VPIXELS_PER_CHAR - 1);
                    attr_off(color_set(color_pair,0),0);
                    color_pair++;
                    color++;
                }
                break;
            case CLAY_RENDER_COMMAND_TYPE_TEXT:
                Clay_TextElementConfig *text_config = renderCommands.internalArray[i].config.textElementConfig;
                attr_on(color_set(0,0),0);
                int x = renderCommands.internalArray[i].boundingBox.x/HPIXELS_PER_CHAR;
                int y = renderCommands.internalArray[i].boundingBox.y/VPIXELS_PER_CHAR; //text is referenced from bottom corner?
                int w = renderCommands.internalArray[i].boundingBox.width/HPIXELS_PER_CHAR;
                int h = renderCommands.internalArray[i].boundingBox.height/VPIXELS_PER_CHAR;
                int line = 0;
                int column = 0;
                for (int k = 0; k < renderCommands.internalArray[i].text.length; k++) {
                    if (column >= w) {
                        column = 0;
                        line += 1;
                    }
                    mvaddch(y + line, x + column, renderCommands.internalArray[i].text.chars[k]);
                    column += 1;
                }
                break;
            case CLAY_RENDER_COMMAND_TYPE_IMAGE:
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START:
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END:
            case CLAY_RENDER_COMMAND_TYPE_CUSTOM:
            default: continue;
        }
    }
    attr_on(color_set(0,0),0);
    mvwprintw(win, 0, 0, "Number of color pairs used: %i", color_pair);
    refresh();//update the screen/window
}


//written by EmmanuelMess: https://github.com/nicbarker/clay/pull/91/commits/7ce74ba46c01f32e4517032e9da76bf54ecf7a43
static inline Clay_Dimensions ncurses_MeasureText(Clay_String *text, Clay_TextElementConfig *config) {
	Clay_Dimensions textSize = { 0 };
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

	textSize.width = maxTextWidth*HPIXELS_PER_CHAR;
	textSize.height = textHeight*VPIXELS_PER_CHAR;

	return textSize;
}
