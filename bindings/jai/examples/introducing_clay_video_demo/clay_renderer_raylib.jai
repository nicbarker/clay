
RaylibFont :: struct {
	font_id: u16;
	font: Raylib.Font;
}

g_raylib_fonts: [10]RaylibFont;

to_raylib_color :: (color: Clay.Color) -> Raylib.Color {
	return .{cast(u8) color.r, cast(u8) color.g, cast(u8) color.b, cast(u8) color.a};
}

raylib_measure_text :: (text: *Clay.String, config: *Clay.TextElementConfig) -> Clay.Dimensions #c_call {
	text_size := Clay.Dimensions.{0, 0};

	max_text_width: float = 0;
	line_text_width: float = 0;

	text_height := cast(float)config.fontSize;
	font_to_use := g_raylib_fonts[config.fontId].font;

	if text.length > 0 {
		for 0..(text.length - 1) {
			if text.chars[it] == #char "\n" {
				max_text_width = c_max(max_text_width, line_text_width);
				line_text_width = 0;
				continue;
			}

			index := cast(s32, text.chars[it]) - 32;
			if font_to_use.glyphs[index].advanceX != 0 {
				line_text_width += cast(float) font_to_use.glyphs[index].advanceX;
			} else {
				line_text_width += (font_to_use.recs[index].width + cast(float) font_to_use.glyphs[index].offsetX);
			}
		}
	}

	max_text_width = c_max(max_text_width, line_text_width);
	text_size.width = max_text_width / 2;
	text_size.height = text_height;

	return text_size;
}

raylib_initialize :: (width: s32, height: s32, $$title: string, flags: Raylib.ConfigFlags) {
	c_string_title: *u8;
	#if is_constant(title) {
		// Constant strings in Jai are null-terminated
		c_string_title = title.data;
	} else {
		c_string_title = to_c_string(title);
	}
	
	Raylib.SetConfigFlags(flags);
	Raylib.InitWindow(width, height, c_string_title);
}

clay_raylib_render :: (render_commands: Clay.RenderCommandArray) {
	for 0..render_commands.length - 1 {
		render_command := Clay.RenderCommandArray_Get(*render_commands, cast(s32) it);
		bounding_box := render_command.boundingBox;

		if #complete render_command.commandType == {
		case .NONE;
		case .TEXT;
			text := string.{
				cast(s64) render_command.text.length,
				render_command.text.chars,
			};
			c_string_text := temp_c_string(text);

			font_to_use: Raylib.Font = g_raylib_fonts[render_command.config.textElementConfig.fontId].font;
			Raylib.DrawTextEx(
				font_to_use,
				c_string_text,
				.{bounding_box.x, bounding_box.y},
				cast(float) render_command.config.textElementConfig.fontSize,
				cast(float) render_command.config.textElementConfig.letterSpacing,
				to_raylib_color(render_command.config.textElementConfig.textColor),
			);
		case .IMAGE;
			// TODO image handling
			image_texture := cast(*Raylib.Texture2D, render_command.config.imageElementConfig.imageData).*;
			Raylib.DrawTextureEx(
				image_texture, 
				.{bounding_box.x, bounding_box.y}, 
				0, 
				bounding_box.width / cast(float) image_texture.width, 
				Raylib.WHITE
			);
		case .SCISSOR_START;
			Raylib.BeginScissorMode(
				cast(s32, round(bounding_box.x)),
				cast(s32, round(bounding_box.y)),
				cast(s32, round(bounding_box.width)),
				cast(s32, round(bounding_box.height)),
			);
		case .SCISSOR_END;
			Raylib.EndScissorMode();
		case .RECTANGLE;
			config := render_command.config.rectangleElementConfig;
			if config.cornerRadius.topLeft > 0 {
				radius := (config.cornerRadius.topLeft * 2.0) / min(bounding_box.width, bounding_box.height);
				Raylib.DrawRectangleRounded(
					.{bounding_box.x, bounding_box.y, bounding_box.width, bounding_box.height}, 
					radius, 
					8, 
					to_raylib_color(config.color),
				);
			} else {
				Raylib.DrawRectangle(
					cast(s32, bounding_box.x),
					cast(s32, bounding_box.y),
					cast(s32, bounding_box.width),
					cast(s32, bounding_box.height),
					to_raylib_color(config.color),
				);
			}
		case .BORDER;
			config := render_command.config.borderElementConfig;

			// Left border
			if config.left.width > 0 {
				Raylib.DrawRectangle(
					cast(s32, round(bounding_box.x)),
					cast(s32, round(bounding_box.y + config.cornerRadius.topLeft)),
					cast(s32, config.left.width),
					cast(s32, round(bounding_box.height - config.cornerRadius.topLeft - config.cornerRadius.bottomLeft)),
					to_raylib_color(config.right.color),
				);
			}

			// Right border
			if config.right.width > 0 {
				Raylib.DrawRectangle(
					cast(s32, round(bounding_box.x + bounding_box.width - cast(float, config.right.width))),
					cast(s32, round(bounding_box.y + config.cornerRadius.topRight)),
					cast(s32, config.right.width),
					cast(s32, round(bounding_box.height - config.cornerRadius.topRight - config.cornerRadius.bottomRight)),
					to_raylib_color(config.right.color),
				);
			}

			// Top border
			if config.top.width > 0 {
				Raylib.DrawRectangle(
					cast(s32, round(bounding_box.x + config.cornerRadius.topLeft)),
					cast(s32, round(bounding_box.y)),
					cast(s32, round(bounding_box.width - config.cornerRadius.topLeft - config.cornerRadius.topRight)),
					cast(s32, config.top.width),
					to_raylib_color(config.right.color),
				);
			}

			// Bottom border
			if config.bottom.width > 0 {
				Raylib.DrawRectangle(
					cast(s32, round(bounding_box.x + config.cornerRadius.bottomLeft)),
					cast(s32, round(bounding_box.y + bounding_box.height - cast(float, config.bottom.width))),
					cast(s32, round(bounding_box.width - config.cornerRadius.bottomLeft - config.cornerRadius.bottomRight)),
					cast(s32, config.top.width),
					to_raylib_color(config.right.color),
				);
			}

			if config.cornerRadius.topLeft > 0 {
				Raylib.DrawRing(
					.{
						round(bounding_box.x + config.cornerRadius.topLeft),
						round(bounding_box.y + config.cornerRadius.topLeft),
					},
					round(config.cornerRadius.topLeft - cast(float, config.top.width)),
					config.cornerRadius.topLeft,
					180,
					270,
					10,
					to_raylib_color(config.top.color),
				);
			}

			if config.cornerRadius.topRight > 0 {
				Raylib.DrawRing(
					.{
						round(bounding_box.x + bounding_box.width - config.cornerRadius.topRight),
						round(bounding_box.y + config.cornerRadius.topRight),
					},
					round(config.cornerRadius.topRight - cast(float, config.top.width)),
					config.cornerRadius.topRight,
					270,
					360,
					10,
					to_raylib_color(config.top.color),
				);
			}

			if config.cornerRadius.bottomLeft > 0 {
				Raylib.DrawRing(
					.{
						round(bounding_box.x + config.cornerRadius.bottomLeft),
						round(bounding_box.y + bounding_box.height - config.cornerRadius.bottomLeft),
					},
					round(config.cornerRadius.bottomLeft - cast(float, config.bottom.width)),
					config.cornerRadius.bottomLeft,
					90,
					180,
					10,
					to_raylib_color(config.bottom.color),
				);
			}

			if config.cornerRadius.bottomRight > 0 {
				Raylib.DrawRing(
					.{
						round(bounding_box.x + bounding_box.width - config.cornerRadius.bottomRight),
						round(bounding_box.y + bounding_box.height - config.cornerRadius.bottomRight),
					},
					round(config.cornerRadius.bottomRight - cast(float, config.bottom.width)),
					config.cornerRadius.bottomRight,
					0.1,
					90,
					10,
					to_raylib_color(config.bottom.color),
				);
			}
		case .CUSTOM;
		}
	}
}

#scope_file

round :: (x: float) -> float {
	rounded_int := cast(int, x + 0.5 * ifx x < 0 then -1 else 1);
    return cast(float, rounded_int);
}

c_max :: (a: $T, b: T) -> T #c_call {
    if b < a return a;
    return b;
}
