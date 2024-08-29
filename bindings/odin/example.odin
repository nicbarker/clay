package main

import clay "clay-odin"
import "core:c"
import "core:fmt"
import "vendor:raylib"

createLayout :: proc() -> clay.ClayArray(clay.RenderCommand) {
    clay.BeginLayout(1024, 768)
    layoutConfig: clay.LayoutConfig = clay.LayoutConfig {
        sizing = {width = {type = clay.SizingType.GROW}, height = {type = clay.SizingType.GROW}},
        padding = {16, 16},
    }
    rectangleConfig: clay.RectangleElementConfig = clay.RectangleElementConfig {
        cornerRadius = {topLeft = 5},
    }

    if clay.Rectangle(
        clay.ID("Outer Container"),
        clay.Layout({sizing = {width = {type = clay.SizingType.GROW}, height = {type = clay.SizingType.GROW}}, padding = {16, 16}}),
        clay.RectangleConfig({cornerRadius = {topLeft = 5}}),
    ) {
        if clay.Rectangle(clay.ID("Inner Container"), &layoutConfig, &rectangleConfig) {
            if clay.Rectangle(clay.ID("percentContainer"), clay.Layout({sizing = {width = clay.SizingPercent(0.5)}}), clay.RectangleConfig({})) {}
            if clay.Rectangle(clay.ID("growContainer"), clay.Layout({sizing = {width = clay.SizingGrow({max = 200})}}), clay.RectangleConfig({})) {}
            clay.Text(clay.ID("textfield"), clay.MakeString("Texti"), clay.TextConfig({fontId = 0, fontSize = 24, textColor = {255, 255, 255, 255}}))
        }
    }

    return clay.EndLayout(1024, 768)
}

main :: proc() {
    minMemorySize: c.uint32_t = clay.MinMemorySize()
    memory := make([^]u8, minMemorySize)
    arena: clay.Arena = clay.CreateArenaWithCapacityAndMemory(minMemorySize, memory)
    clay.SetMeasureTextFunction(measureText)
    clay.Initialize(arena)

    raylib.InitWindow(1024, 768, "Raylib Odin Example")

    raylibFonts[0] = RaylibFont {
        font   = raylib.LoadFontEx("resources/Roboto-Regular.ttf", 32, nil, 0),
        fontId = 0,
    }
    raylib.SetTextureFilter(raylibFonts[0].font.texture, raylib.TextureFilter.TRILINEAR)

    for !raylib.WindowShouldClose() {
        renderCommands: clay.ClayArray(clay.RenderCommand) = createLayout()
        raylib.BeginDrawing()
        clayRaylibRender(&renderCommands)
        raylib.EndDrawing()
    }
}
