package video_demo_sdl

import "core:math"
import clay "../../clay-odin"
import layout "../shared_layouts"
import "base:runtime"
import "core:fmt"
import sdl "vendor:sdl3"
import "vendor:sdl3/ttf"

App_State :: struct {
    window:       ^sdl.Window,
    rendererData: Clay_SDL3RendererData,
}

state: App_State

errorHandler :: proc "c" (errorData: clay.ErrorData) {
    runtime.print_string("Clay Error:\n\t")
    runtime.print_string(string(errorData.errorText.chars[:errorData.errorText.length]))
    runtime.print_byte('\n')
}

load_font :: proc(data: []byte, size: f32, id: u16, fonts: ^[dynamic]^ttf.Font) {
    font_stream := sdl.IOFromConstMem(raw_data(ROBOTO), uint(len(ROBOTO)))
    font := ttf.OpenFontIO(font_stream, true, size * 2)
    assign_at(fonts, int(id), font)
}

measure_text :: proc "c" (text: clay.StringSlice, config: ^clay.TextElementConfig, userData: rawptr) -> clay.Dimensions {

    fonts := (^[dynamic]^ttf.Font)(userData)
    font := fonts[config.fontId]
    width, height: i32

    ttf.SetFontSize(font, px_to_pt(f32(config.fontSize)))
    if (!ttf.GetStringSize(font, cstring(text.chars), uint(text.length), &width, &height)) {
        sdl.LogError(i32(sdl.LogCategory.ERROR), "Failed to measure text: %s", sdl.GetError())
    }

    return clay.Dimensions{f32(width), f32(height)}
}
// Load at compile time, directly into the binary
ROBOTO := #load("./Roboto-Regular.ttf")

main :: proc() {
    if !sdl.Init({.VIDEO}) {
        fmt.panicf("Failed to initialize sdl: %s", sdl.GetError())
    }

    if !ttf.Init() {
        fmt.panicf("Failed to initialize SDL TTF: %s", sdl.GetError())
    }

    window: ^sdl.Window
    renderer: ^sdl.Renderer
    sdl.CreateWindowAndRenderer("SDL Odin Example", 1280, 720, {.RESIZABLE}, &window, &renderer)

    state.window = window
    state.rendererData.renderer = renderer
    state.rendererData.textEngine = ttf.CreateRendererTextEngine(renderer)

    minMemorySize := (uint)(clay.MinMemorySize())
    arena: clay.Arena = clay.CreateArenaWithCapacityAndMemory(minMemorySize, make([^]u8, minMemorySize))
    clay.Initialize(arena, {1280, 720}, {handler = errorHandler})
    clay.SetMeasureTextFunction(measure_text, &state.rendererData.fonts)

    load_font(ROBOTO, 16, layout.VIDEO_DEMO_FONT_ID_BODY, &state.rendererData.fonts)

    data := layout.video_demo_init()

    event: sdl.Event
    done := false

    NOW := sdl.GetPerformanceCounter()
    LAST: u64 = 0

    window_width, window_height: i32

    for !done {
        defer free_all(context.temp_allocator)

        scrollDelta: clay.Vector2

        for (sdl.PollEvent(&event)) {
            if (event.type == .QUIT) {
                done = true
            } else if event.type == .MOUSE_WHEEL {
                scrollDelta.x = event.wheel.x
                scrollDelta.y = event.wheel.y
            }
        }

        LAST = NOW
        NOW = sdl.GetPerformanceCounter()
        deltaTime := (f64(NOW - LAST) / f64(sdl.GetPerformanceFrequency()))

        sdl.GetWindowSize(window, &window_width, &window_height)

        clay.SetLayoutDimensions({width = f32(window_width), height = f32(window_height)})

		mouseX: f32 = 0
		mouseY: f32 = 0
		mouseState := sdl.GetMouseState(&mouseX, &mouseY)
		mousePosition := (clay.Vector2){mouseX, mouseY}
		clay.SetPointerState(mousePosition, .LEFT in mouseState)

		clay.UpdateScrollContainers(false, scrollDelta, f32(deltaTime))
		clay.SetLayoutDimensions({f32(window_width), f32(window_height)})

        commands := layout.video_demo_layout(&data)

		sdl.SetRenderDrawColor(renderer, 0, 0, 0, 255)
		sdl.RenderClear(renderer)
		sdl_Clay_RenderClayCommands(&state.rendererData, &commands)

		sdl.RenderPresent(renderer)
    }

}
