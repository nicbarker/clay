package main

import "core:unicode/utf8"
import "base:runtime"
import clay "../../clay-odin"
import "core:math"
import "core:strings"
import rl "vendor:raylib"

Raylib_Font :: struct {
    fontId: u16,
    font:   rl.Font,
}

clay_color_to_rl_color :: proc(color: clay.Color) -> rl.Color {
    return {u8(color.r), u8(color.g), u8(color.b), u8(color.a)}
}

raylib_fonts := [dynamic]Raylib_Font{}

// Alias for compatibility, default to ascii support
measure_text :: measure_text_ascii

measure_text_unicode :: proc "c" (text: clay.StringSlice, config: ^clay.TextElementConfig, userData: rawptr) -> clay.Dimensions {
    // Needed for grapheme_count
    context = runtime.default_context()
    
	line_width: f32 = 0
    
	font := raylib_fonts[config.fontId].font
	text_str := string(text.chars[:text.length])

    // This function seems somewhat expensive, if you notice performance issues, you could assume
    // - 1 codepoint per visual character (no grapheme clusters), where you can get the length from the loop
    // - 1 byte per visual character (ascii), where you can get the length with `text.length`
    // see `measure_text_ascii`
    grapheme_count, _, _ := utf8.grapheme_count(text_str)

	for letter, byte_idx in text_str {
		glyph_index := rl.GetGlyphIndex(font, letter)

        glyph := font.glyphs[glyph_index]

		if glyph.advanceX != 0 {
			line_width += f32(glyph.advanceX)
		} else {
			line_width += font.recs[glyph_index].width + f32(font.glyphs[glyph_index].offsetX)
		}
	}

	scaleFactor := f32(config.fontSize) / f32(font.baseSize)

    // Note: 
    //   I'd expect this to be `grapheme_count - 1`, 
    //   but that seems to be one letterSpacing too small
    //   maybe that's a raylib bug, maybe that's Clay?
	total_spacing := f32(grapheme_count) * f32(config.letterSpacing)

	return {width = line_width * scaleFactor + total_spacing, height = f32(config.fontSize)}
}

measure_text_ascii :: proc "c" (text: clay.StringSlice, config: ^clay.TextElementConfig, userData: rawptr) -> clay.Dimensions {    
	line_width: f32 = 0
    
	font := raylib_fonts[config.fontId].font
	text_str := string(text.chars[:text.length])

	for i in 0..<len(text_str) {
		glyph_index := text_str[i] - 32

        glyph := font.glyphs[glyph_index]

		if glyph.advanceX != 0 {
			line_width += f32(glyph.advanceX)
		} else {
			line_width += font.recs[glyph_index].width + f32(font.glyphs[glyph_index].offsetX)
		}
	}

	scaleFactor := f32(config.fontSize) / f32(font.baseSize)

    // Note: 
    //   I'd expect this to be `len(text_str) - 1`, 
    //   but that seems to be one letterSpacing too small
    //   maybe that's a raylib bug, maybe that's Clay?
	total_spacing := f32(len(text_str)) * f32(config.letterSpacing)

	return {width = line_width * scaleFactor + total_spacing, height = f32(config.fontSize)}
}

clay_raylib_render :: proc(render_commands: ^clay.ClayArray(clay.RenderCommand), allocator := context.temp_allocator) {
    for i in 0 ..< render_commands.length {
        render_command := clay.RenderCommandArray_Get(render_commands, i)
        bounds := render_command.boundingBox

        switch render_command.commandType {
        case .None: // None
        case .Text:
            config := render_command.renderData.text

            text := string(config.stringContents.chars[:config.stringContents.length])

            // Raylib uses C strings instead of Odin strings, so we need to clone
            // Assume this will be freed elsewhere since we default to the temp allocator
            cstr_text := strings.clone_to_cstring(text, allocator)

            font := raylib_fonts[config.fontId].font
            rl.DrawTextEx(font, cstr_text, {bounds.x, bounds.y}, f32(config.fontSize), f32(config.letterSpacing), clay_color_to_rl_color(config.textColor))
        case .Image:
            config := render_command.renderData.image
            tint := config.backgroundColor
            if tint == 0 {
                tint = {255, 255, 255, 255}
            }

            imageTexture := (^rl.Texture2D)(config.imageData)
            rl.DrawTextureEx(imageTexture^, {bounds.x, bounds.y}, 0, bounds.width / f32(imageTexture.width), clay_color_to_rl_color(tint))
        case .ScissorStart:
            rl.BeginScissorMode(i32(math.round(bounds.x)), i32(math.round(bounds.y)), i32(math.round(bounds.width)), i32(math.round(bounds.height)))
        case .ScissorEnd:
            rl.EndScissorMode()
        case .Rectangle:
            config := render_command.renderData.rectangle
            if config.cornerRadius.topLeft > 0 {
                radius: f32 = (config.cornerRadius.topLeft * 2) / min(bounds.width, bounds.height)
                draw_rect_rounded(bounds.x, bounds.y, bounds.width, bounds.height, radius, config.backgroundColor)
            } else {
                draw_rect(bounds.x, bounds.y, bounds.width, bounds.height, config.backgroundColor)
            }
        case .Border:
            config := render_command.renderData.border
            // Left border
            if config.width.left > 0 {
                draw_rect(
                    bounds.x,
                    bounds.y + config.cornerRadius.topLeft,
                    f32(config.width.left),
                    bounds.height - config.cornerRadius.topLeft - config.cornerRadius.bottomLeft,
                    config.color,
                )
            }
            // Right border
            if config.width.right > 0 {
                draw_rect(
                    bounds.x + bounds.width - f32(config.width.right),
                    bounds.y + config.cornerRadius.topRight,
                    f32(config.width.right),
                    bounds.height - config.cornerRadius.topRight - config.cornerRadius.bottomRight,
                    config.color,
                )
            }
            // Top border
            if config.width.top > 0 {
                draw_rect(
                    bounds.x + config.cornerRadius.topLeft,
                    bounds.y,
                    bounds.width - config.cornerRadius.topLeft - config.cornerRadius.topRight,
                    f32(config.width.top),
                    config.color,
                )
            }
            // Bottom border
            if config.width.bottom > 0 {
                draw_rect(
                    bounds.x + config.cornerRadius.bottomLeft,
                    bounds.y + bounds.height - f32(config.width.bottom),
                    bounds.width - config.cornerRadius.bottomLeft - config.cornerRadius.bottomRight,
                    f32(config.width.bottom),
                    config.color,
                )
            }

            // Rounded Borders
            if config.cornerRadius.topLeft > 0 {
                draw_arc(
                    bounds.x + config.cornerRadius.topLeft, 
                    bounds.y + config.cornerRadius.topLeft,
                    config.cornerRadius.topLeft - f32(config.width.top),
                    config.cornerRadius.topLeft,
                    180,
                    270,
                    config.color,
                )
            }
            if config.cornerRadius.topRight > 0 {
                draw_arc(
                    bounds.x + bounds.width - config.cornerRadius.topRight,
                    bounds.y + config.cornerRadius.topRight,
                    config.cornerRadius.topRight - f32(config.width.top),
                    config.cornerRadius.topRight,
                    270,
                    360,
                    config.color,
                )
            }
            if config.cornerRadius.bottomLeft > 0 {
                draw_arc(
                    bounds.x + config.cornerRadius.bottomLeft,
                    bounds.y + bounds.height - config.cornerRadius.bottomLeft,
                    config.cornerRadius.bottomLeft - f32(config.width.top),
                    config.cornerRadius.bottomLeft,
                    90,
                    180,
                    config.color,
                )
            }
            if config.cornerRadius.bottomRight > 0 {
                draw_arc(
                    bounds.x + bounds.width - config.cornerRadius.bottomRight, 
                    bounds.y + bounds.height - config.cornerRadius.bottomRight,
                    config.cornerRadius.bottomRight - f32(config.width.bottom),
                    config.cornerRadius.bottomRight,
                    0.1,
                    90,
                    config.color,
                )
            }
        case clay.RenderCommandType.Custom:
            // Implement custom element rendering here
        }
    }
}

// Helper procs, mainly for repeated conversions

@(private = "file")
draw_arc :: proc(x, y: f32, inner_rad, outer_rad: f32,start_angle, end_angle: f32, color: clay.Color){
    rl.DrawRing(
        {math.round(x),math.round(y)},
        math.round(inner_rad),
        outer_rad,
        start_angle,
        end_angle,
        10,
        clay_color_to_rl_color(color),
    )
}

@(private = "file")
draw_rect :: proc(x, y, w, h: f32, color: clay.Color) {
    rl.DrawRectangle(
        i32(math.round(x)), 
        i32(math.round(y)), 
        i32(math.round(w)), 
        i32(math.round(h)), 
        clay_color_to_rl_color(color)
    )
}

@(private = "file")
draw_rect_rounded :: proc(x,y,w,h: f32, radius: f32, color: clay.Color){
    rl.DrawRectangleRounded({x,y,w,h},radius,8,clay_color_to_rl_color(color))
}