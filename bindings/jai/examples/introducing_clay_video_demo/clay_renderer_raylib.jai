
RaylibFont :: struct {
	font_id: u16;
	font: Raylib.Font;
}

clay_color_to_raylib_color :: (color: Clay.Color) -> Raylib.Color {
	return .{cast(u8) color.x, cast(u8) color.y, cast(u8) color.z, cast(u8) color.w};
}

raylib_fonts: [10]RaylibFont;

c_max :: (a: $T, b: T) -> T #c_call {
    if b < a return a;
    return b;
}

measure_text :: (text: *Clay.String, config: *Clay.TextElementConfig) -> Clay.Dimensions #c_call {
	text_size := Clay.Dimensions.{0, 0};

	max_text_width: float = 0;
	line_text_width: float = 0;

	text_height := cast(float)config.fontSize;
	font_to_use := raylib_fonts[config.fontId].font;

	for 0..text.length - 1 {
		if text.chars[it] == #char "\n" {
			max_text_width = c_max(max_text_width, line_text_width);
			line_text_width = 0;
			continue;
		}

		index := cast(s32) text.chars[it] - 32;
		if font_to_use.glyphs[index].advanceX != 0 {
			line_text_width += cast(float) font_to_use.glyphs[index].advanceX;
		} else {
			line_text_width += (font_to_use.recs[index].width + cast(float) font_to_use.glyphs[index].offsetX);
		}
	}

	max_text_width = c_max(max_text_width, line_text_width);
	text_size.width = max_text_width / 2;
	text_size.height = text_height;

	return text_size;
}

clay_raylib_render :: (render_commands: *Clay.RenderCommandArray) {
	for i in 0..render_commands.length - 1 {
		render_command := Clay.RenderCommandArray_Get(render_commands, i);
		bounding_box := render_command.boundingBox;

		if render_command.commandType == {
			case .Text;
			
		}
	}
}
