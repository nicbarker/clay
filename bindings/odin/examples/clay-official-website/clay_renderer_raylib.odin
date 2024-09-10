package main

import clay "../../clay-odin"
import "core:math"
import "core:strings"
import "vendor:raylib"

RaylibFont :: struct {
    fontId: u16,
    font:   raylib.Font,
}

clayColorToRaylibColor :: proc(color: clay.Color) -> raylib.Color {
    return raylib.Color{cast(u8)color.r, cast(u8)color.g, cast(u8)color.b, cast(u8)color.a}
}

raylibFonts := [10]RaylibFont{}

measureText :: proc "c" (text: ^clay.String, config: ^clay.TextElementConfig) -> clay.Dimensions {
    // Measure string size for Font
    textSize: clay.Dimensions = {0, 0}

    maxTextWidth: f32 = 0
    lineTextWidth: f32 = 0

    textHeight := cast(f32)config.fontSize
    fontToUse := raylibFonts[config.fontId].font

    for i in 0 ..< int(text.length) {
        if (text.chars[i] == '\n') {
            maxTextWidth = max(maxTextWidth, lineTextWidth)
            lineTextWidth = 0
            continue
        }
        index := cast(i32)text.chars[i] - 32
        if (fontToUse.glyphs[index].advanceX != 0) {
            lineTextWidth += cast(f32)fontToUse.glyphs[index].advanceX
        } else {
            lineTextWidth += (fontToUse.recs[index].width + cast(f32)fontToUse.glyphs[index].offsetX)
        }
    }

    maxTextWidth = max(maxTextWidth, lineTextWidth)

    textSize.width = maxTextWidth / 2
    textSize.height = textHeight

    return textSize
}

clayRaylibRender :: proc(renderCommands: ^clay.ClayArray(clay.RenderCommand), allocator := context.temp_allocator) {
    for i in 0 ..< int(renderCommands.length) {
        renderCommand := clay.RenderCommandArray_Get(renderCommands, cast(i32)i)
        boundingBox := renderCommand.boundingBox
        switch (renderCommand.commandType) {
        case clay.RenderCommandType.None:
            {}
        case clay.RenderCommandType.Text:
            // Raylib uses standard C strings so isn't compatible with cheap slices, we need to clone the string to append null terminator
            text := string(renderCommand.text.chars[:renderCommand.text.length])
            cloned := strings.clone_to_cstring(text, allocator)
            fontToUse: raylib.Font = raylibFonts[renderCommand.config.textElementConfig.fontId].font
            raylib.DrawTextEx(
                fontToUse,
                cloned,
                raylib.Vector2{boundingBox.x, boundingBox.y},
                cast(f32)renderCommand.config.textElementConfig.fontSize,
                cast(f32)renderCommand.config.textElementConfig.letterSpacing,
                clayColorToRaylibColor(renderCommand.config.textElementConfig.textColor),
            )
        case clay.RenderCommandType.Image:
            // TODO image handling
            imageTexture := cast(^raylib.Texture2D)renderCommand.config.imageElementConfig.imageData
            raylib.DrawTextureEx(imageTexture^, raylib.Vector2{boundingBox.x, boundingBox.y}, 0, boundingBox.width / cast(f32)imageTexture.width, raylib.WHITE)
        case clay.RenderCommandType.ScissorStart:
            raylib.BeginScissorMode(
                cast(i32)math.round(boundingBox.x),
                cast(i32)math.round(boundingBox.y),
                cast(i32)math.round(boundingBox.width),
                cast(i32)math.round(boundingBox.height),
            )
        case clay.RenderCommandType.ScissorEnd:
            raylib.EndScissorMode()
        case clay.RenderCommandType.Rectangle:
            config: ^clay.RectangleElementConfig = renderCommand.config.rectangleElementConfig
            if (config.cornerRadius.topLeft > 0) {
                radius: f32 = (config.cornerRadius.topLeft * 2) / min(boundingBox.width, boundingBox.height)
                raylib.DrawRectangleRounded(raylib.Rectangle{boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height}, radius, 8, clayColorToRaylibColor(config.color))
            } else {
                raylib.DrawRectangle(cast(i32)boundingBox.x, cast(i32)boundingBox.y, cast(i32)boundingBox.width, cast(i32)boundingBox.height, clayColorToRaylibColor(config.color))
            }
        case clay.RenderCommandType.Border:
            config := renderCommand.config.borderElementConfig
            // Left border
            if (config.left.width > 0) {
                raylib.DrawRectangle(
                    cast(i32)math.round(boundingBox.x),
                    cast(i32)math.round(boundingBox.y + config.cornerRadius.topLeft),
                    cast(i32)config.left.width,
                    cast(i32)math.round(boundingBox.height - config.cornerRadius.topLeft - config.cornerRadius.bottomLeft),
                    clayColorToRaylibColor(config.left.color),
                )
            }
            // Right border
            if (config.right.width > 0) {
                raylib.DrawRectangle(
                    cast(i32)math.round(boundingBox.x + boundingBox.width - cast(f32)config.right.width),
                    cast(i32)math.round(boundingBox.y + config.cornerRadius.topRight),
                    cast(i32)config.right.width,
                    cast(i32)math.round(boundingBox.height - config.cornerRadius.topRight - config.cornerRadius.bottomRight),
                    clayColorToRaylibColor(config.right.color),
                )
            }
            // Top border
            if (config.top.width > 0) {
                raylib.DrawRectangle(
                    cast(i32)math.round(boundingBox.x + config.cornerRadius.topLeft),
                    cast(i32)math.round(boundingBox.y),
                    cast(i32)math.round(boundingBox.width - config.cornerRadius.topLeft - config.cornerRadius.topRight),
                    cast(i32)config.top.width,
                    clayColorToRaylibColor(config.top.color),
                )
            }
            // Bottom border
            if (config.bottom.width > 0) {
                raylib.DrawRectangle(
                    cast(i32)math.round(boundingBox.x + config.cornerRadius.bottomLeft),
                    cast(i32)math.round(boundingBox.y + boundingBox.height - cast(f32)config.bottom.width),
                    cast(i32)math.round(boundingBox.width - config.cornerRadius.bottomLeft - config.cornerRadius.bottomRight),
                    cast(i32)config.bottom.width,
                    clayColorToRaylibColor(config.bottom.color),
                )
            }
            if (config.cornerRadius.topLeft > 0) {
                raylib.DrawRing(
                    raylib.Vector2{math.round(boundingBox.x + config.cornerRadius.topLeft), math.round(boundingBox.y + config.cornerRadius.topLeft)},
                    math.round(config.cornerRadius.topLeft - cast(f32)config.top.width),
                    config.cornerRadius.topLeft,
                    180,
                    270,
                    10,
                    clayColorToRaylibColor(config.top.color),
                )
            }
            if (config.cornerRadius.topRight > 0) {
                raylib.DrawRing(
                    raylib.Vector2{math.round(boundingBox.x + boundingBox.width - config.cornerRadius.topRight), math.round(boundingBox.y + config.cornerRadius.topRight)},
                    math.round(config.cornerRadius.topRight - cast(f32)config.top.width),
                    config.cornerRadius.topRight,
                    270,
                    360,
                    10,
                    clayColorToRaylibColor(config.top.color),
                )
            }
            if (config.cornerRadius.bottomLeft > 0) {
                raylib.DrawRing(
                    raylib.Vector2{math.round(boundingBox.x + config.cornerRadius.bottomLeft), math.round(boundingBox.y + boundingBox.height - config.cornerRadius.bottomLeft)},
                    math.round(config.cornerRadius.bottomLeft - cast(f32)config.top.width),
                    config.cornerRadius.bottomLeft,
                    90,
                    180,
                    10,
                    clayColorToRaylibColor(config.bottom.color),
                )
            }
            if (config.cornerRadius.bottomRight > 0) {
                raylib.DrawRing(
                    raylib.Vector2 {
                        math.round(boundingBox.x + boundingBox.width - config.cornerRadius.bottomRight),
                        math.round(boundingBox.y + boundingBox.height - config.cornerRadius.bottomRight),
                    },
                    math.round(config.cornerRadius.bottomRight - cast(f32)config.bottom.width),
                    config.cornerRadius.bottomRight,
                    0.1,
                    90,
                    10,
                    clayColorToRaylibColor(config.bottom.color),
                )
            }
        case clay.RenderCommandType.Custom:
        // Implement custom element rendering here
        }
    }
}
