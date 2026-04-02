package video_demo_raylib

import clay "../../clay-odin"
import layout "../shared_layouts"
import "base:runtime"
import "core:fmt"
import rl "vendor:raylib"

errorHandler :: proc "c" (errorData: clay.ErrorData) {
    runtime.print_string("Clay Error:\n\t")
    runtime.print_string(string(errorData.errorText.chars[:errorData.errorText.length]))
    runtime.print_byte('\n')
}

load_font :: proc(data: []byte, size: i32, id: u16) {
    // Multiply size by two because we divide by two in the measurement function.
    // This is for smooth fonts on retina displays, and helps with subpixel accuracy.
    font := rl.LoadFontFromMemory(".ttf", raw_data(data), i32(len(data)), size * 2, nil, 0)
    rl.SetTextureFilter(font.texture, .BILINEAR)
    assign_at(&raylib_fonts, int(id), Raylib_Font{font = font, fontId = id})
}

// Load at compile time, directly into the binary
ROBOTO :: #load("./Roboto-Regular.ttf")

main :: proc() {
    rl.SetConfigFlags({.VSYNC_HINT, .WINDOW_RESIZABLE, .MSAA_4X_HINT})
    rl.InitWindow(1280, 720, "Raylib Odin Example")

    minMemorySize := (uint)(clay.MinMemorySize())
    arena: clay.Arena = clay.CreateArenaWithCapacityAndMemory(minMemorySize, make([^]u8, minMemorySize))
    clay.Initialize(arena, {f32(rl.GetScreenWidth()), f32(rl.GetScreenHeight())}, {handler = errorHandler})
    clay.SetMeasureTextFunction(measure_text, nil)

    load_font(ROBOTO, 16, layout.VIDEO_DEMO_FONT_ID_BODY)

    data := layout.video_demo_init()

    for !rl.WindowShouldClose() {
        clay.SetLayoutDimensions({width = f32(rl.GetScreenWidth()), height = f32(rl.GetScreenHeight())})

        clay.SetPointerState(rl.GetMousePosition(), rl.IsMouseButtonDown(.LEFT))
        clay.UpdateScrollContainers(true, rl.GetMouseWheelMoveV(), rl.GetFrameTime())

        commands := layout.video_demo_layout(&data)

        rl.BeginDrawing()

        rl.ClearBackground(0)
        clay_raylib_render(&commands)

        rl.EndDrawing()
    }

}
