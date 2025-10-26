// Basic port of the C example SDL3 renderer, with a dynamic array of fonts.
package video_demo_sdl

import clay "../../clay-odin"
import "core:math"
import "core:math/linalg"
import sdl "vendor:sdl3"
import "vendor:sdl3/ttf"

Clay_SDL_Render_Data :: struct {
    renderer:    ^sdl.Renderer,
    text_engine: ^ttf.TextEngine,
    fonts:       [dynamic]^ttf.Font,
}

// SDL_ttf works in pts, but clay expects pixels.
// 0.85 looks correct from what I've seen, but this calculation is probably incorrect.
px_to_pt :: proc "contextless" (pixels: f32) -> f32 {
    return pixels * 0.85
}


/* Global for convenience. Even in 4K this is enough for smooth curves (low radius or rect size coupled with
 * no AA or low resolution might make it appear as jagged curves) */
NUM_CIRCLE_SEGMENTS :: 16

//all rendering is performed by a single SDL call, avoiding multiple RenderRect + plumbing choice for circles.
@(private = "file")
fill_rounded_rect :: proc(rendererData: ^Clay_SDL_Render_Data, rect: sdl.FRect, cornerRadius: f32, _color: clay.Color) {
    color := sdl.FColor(_color / 255)

    indexCount: i32 = 0
    vertexCount: i32 = 0

    minRadius := min(rect.w, rect.h) / 2
    clampedRadius := min(cornerRadius, minRadius)

    numCircleSegments := max(NUM_CIRCLE_SEGMENTS, i32(clampedRadius * 0.5))

    totalVertices := 4 + (4 * (numCircleSegments * 2)) + 2 * 4
    totalIndices := 6 + (4 * (numCircleSegments * 3)) + 6 * 4

    // Maybe instrinsics.alloca these?
    vertices := make([]sdl.Vertex, totalVertices, allocator = context.temp_allocator)
    indices := make([]i32, totalIndices, allocator = context.temp_allocator)

    //define center rectangle
    vertices[vertexCount + 0] = {{rect.x + clampedRadius, rect.y + clampedRadius}, color, {0, 0}} //0 center TL
    vertices[vertexCount + 1] = {{rect.x + rect.w - clampedRadius, rect.y + clampedRadius}, color, {1, 0}} //1 center TR
    vertices[vertexCount + 2] = {{rect.x + rect.w - clampedRadius, rect.y + rect.h - clampedRadius}, color, {1, 1}} //2 center BR
    vertices[vertexCount + 3] = {{rect.x + clampedRadius, rect.y + rect.h - clampedRadius}, color, {0, 1}} //3 center BL

    vertexCount += 4

    indices[indexCount + 0] = 0
    indices[indexCount + 1] = 1
    indices[indexCount + 2] = 3
    indices[indexCount + 3] = 1
    indices[indexCount + 4] = 2
    indices[indexCount + 5] = 3

    indexCount += 6

    //define rounded corners as triangle fans
    step := (f32(math.PI) / 2) / f32(numCircleSegments)
    for i in 0 ..< numCircleSegments {
        angle1 := f32(i) * step
        angle2 := (f32(i) + 1) * step

        for j in i32(0) ..< 4 {     // Iterate over four corners
            cx, cy, signX, signY: f32

            switch j {
            case 0:
                cx = rect.x + clampedRadius
                cy = rect.y + clampedRadius
                signX = -1
                signY = -1
            // Top-left
            case 1:
                cx = rect.x + rect.w - clampedRadius
                cy = rect.y + clampedRadius
                signX = 1
                signY = -1 // Top-right
            case 2:
                cx = rect.x + rect.w - clampedRadius
                cy = rect.y + rect.h - clampedRadius
                signX = 1
                signY = 1 // Bottom-right
            case 3:
                cx = rect.x + clampedRadius
                cy = rect.y + rect.h - clampedRadius
                signX = -1
                signY = 1 // Bottom-left
            case:
                return
            }

            vertices[vertexCount + 0] = {{cx + math.cos(angle1) * clampedRadius * signX, cy + math.sin(angle1) * clampedRadius * signY}, color, {0, 0}}
            vertices[vertexCount + 1] = {{cx + math.cos(angle2) * clampedRadius * signX, cy + math.sin(angle2) * clampedRadius * signY}, color, {0, 0}}

            vertexCount += 2

            indices[indexCount + 0] = j // Connect to corresponding central rectangle vertex
            indices[indexCount + 1] = vertexCount - 2
            indices[indexCount + 2] = vertexCount - 1
            indexCount += 3
        }
    }

    //Define edge rectangles
    // Top edge
    vertices[vertexCount + 0] = {{rect.x + clampedRadius, rect.y}, color, {0, 0}} //TL
    vertices[vertexCount + 1] = {{rect.x + rect.w - clampedRadius, rect.y}, color, {1, 0}} //TR

    vertexCount += 2

    indices[indexCount + 0] = 0
    indices[indexCount + 1] = vertexCount - 2 //TL
    indices[indexCount + 2] = vertexCount - 1 //TR
    indices[indexCount + 3] = 1
    indices[indexCount + 4] = 0
    indices[indexCount + 5] = vertexCount - 1 //TR
    indexCount += 6
    // Right edge
    vertices[vertexCount + 0] = {{rect.x + rect.w, rect.y + clampedRadius}, color, {1, 0}} //RT
    vertices[vertexCount + 1] = {{rect.x + rect.w, rect.y + rect.h - clampedRadius}, color, {1, 1}} //RB
    vertexCount += 2

    indices[indexCount + 0] = 1
    indices[indexCount + 1] = vertexCount - 2 //RT
    indices[indexCount + 2] = vertexCount - 1 //RB
    indices[indexCount + 3] = 2
    indices[indexCount + 4] = 1
    indices[indexCount + 5] = vertexCount - 1 //RB
    indexCount += 6

    // Bottom edge
    vertices[vertexCount + 0] = {{rect.x + rect.w - clampedRadius, rect.y + rect.h}, color, {1, 1}} //BR
    vertices[vertexCount + 1] = {{rect.x + clampedRadius, rect.y + rect.h}, color, {0, 1}} //BL
    vertexCount += 2

    indices[indexCount + 0] = 2
    indices[indexCount + 1] = vertexCount - 2 //BR
    indices[indexCount + 2] = vertexCount - 1 //BL
    indices[indexCount + 3] = 3
    indices[indexCount + 4] = 2
    indices[indexCount + 5] = vertexCount - 1 //BL
    indexCount += 6

    // Left edge
    vertices[vertexCount + 0] = {{rect.x, rect.y + rect.h - clampedRadius}, color, {0, 1}} //LB
    vertices[vertexCount + 1] = {{rect.x, rect.y + clampedRadius}, color, {0, 0}} //LT
    vertexCount += 2

    indices[indexCount + 0] = 3
    indices[indexCount + 1] = vertexCount - 2 //LB
    indices[indexCount + 2] = vertexCount - 1 //LT
    indices[indexCount + 3] = 0
    indices[indexCount + 4] = 3
    indices[indexCount + 5] = vertexCount - 1 //LT
    indexCount += 6

    // Render everything
    sdl.RenderGeometry(rendererData.renderer, nil, raw_data(vertices), vertexCount, raw_data(indices), indexCount)
}

@(private = "file")
render_arc :: proc(rendererData: ^Clay_SDL_Render_Data, center: sdl.FPoint, radius: f32, startAngle: f32, endAngle: f32, thickness: f32, color: clay.Color) {
    sdl.SetRenderDrawColor(rendererData.renderer, clay_to_sdl_color(color))

    radStart := math.to_radians(startAngle)
    radEnd := math.to_radians(endAngle)

    numCircleSegments := max(NUM_CIRCLE_SEGMENTS, i32(radius * 1.5)) //increase circle segments for larger circles, 1.5 is arbitrary.

    angleStep := (radEnd - radStart) / f32(numCircleSegments)
    thicknessStep: f32 = 0.4 //arbitrary value to avoid overlapping lines. Changing THICKNESS_STEP or numCircleSegments might cause artifacts.

    for t := thicknessStep; t < thickness - thicknessStep; t += thicknessStep {
        points := make([]sdl.FPoint, numCircleSegments + 1, allocator = context.temp_allocator)
        clampedRadius := max(radius - t, 1)

        for i in 0 ..= numCircleSegments {
            angle := radStart + f32(i) * angleStep
            points[i] = sdl.FPoint{math.round(center.x + math.cos(angle) * clampedRadius), math.round(center.y + math.sin(angle) * clampedRadius)}
        }
        sdl.RenderLines(rendererData.renderer, raw_data(points), numCircleSegments + 1)
    }
}

@(private = "file")
current_clipping_rect: sdl.Rect

clay_to_sdl_color :: proc(color: clay.Color) -> (r, g, b, a: u8) {
    return expand_values(linalg.array_cast(color, u8))
}

sdl_render_clay_commands :: proc(renderer_data: ^Clay_SDL_Render_Data, commands: ^clay.ClayArray(clay.RenderCommand)) {
    for i in 0 ..< commands.length {
        cmd := clay.RenderCommandArray_Get(commands, i)
        bounding_box := cmd.boundingBox
        rect := sdl.FRect{bounding_box.x, bounding_box.y, bounding_box.width, bounding_box.height}

        #partial switch cmd.commandType {
        case .Rectangle:
            config := &cmd.renderData.rectangle
            sdl.SetRenderDrawBlendMode(renderer_data.renderer, sdl.BLENDMODE_BLEND)
            sdl.SetRenderDrawColor(renderer_data.renderer, clay_to_sdl_color(config.backgroundColor))
            if config.cornerRadius.topLeft > 0 {
                fill_rounded_rect(renderer_data, rect, config.cornerRadius.topLeft, config.backgroundColor)
            } else {
                sdl.RenderFillRect(renderer_data.renderer, &rect)
            }

        case .Text:
            config := &cmd.renderData.text
            font := renderer_data.fonts[config.fontId]
            ttf.SetFontSize(font, px_to_pt(f32(config.fontSize)))
            text := ttf.CreateText(renderer_data.text_engine, font, cstring(config.stringContents.chars), uint(config.stringContents.length))
            ttf.SetTextColor(text, clay_to_sdl_color(config.textColor))
            ttf.DrawRendererText(text, rect.x, rect.y)
            ttf.DestroyText(text)

        case .Border:
            config := &cmd.renderData.border

            minRadius := min(rect.w, rect.h) / 2
            clampedRadii := clay.CornerRadius {
                topLeft     = min(config.cornerRadius.topLeft, minRadius),
                topRight    = min(config.cornerRadius.topRight, minRadius),
                bottomLeft  = min(config.cornerRadius.bottomLeft, minRadius),
                bottomRight = min(config.cornerRadius.bottomRight, minRadius),
            }
            //edges
            sdl.SetRenderDrawColor(renderer_data.renderer, clay_to_sdl_color(config.color))
            if config.width.left > 0 {
                starting_y := rect.y + clampedRadii.topLeft
                length := rect.h - clampedRadii.topLeft - clampedRadii.bottomLeft
                line := sdl.FRect{rect.x - 1, starting_y, f32(config.width.left), length}
                sdl.RenderFillRect(renderer_data.renderer, &line)
            }
            if config.width.right > 0 {
                starting_x := rect.x + rect.w - f32(config.width.right) + 1
                starting_y := rect.y + clampedRadii.topRight
                length := rect.h - clampedRadii.topRight - clampedRadii.bottomRight
                line := sdl.FRect{starting_x, starting_y, f32(config.width.right), length}
                sdl.RenderFillRect(renderer_data.renderer, &line)
            }
            if config.width.top > 0 {
                starting_x := rect.x + clampedRadii.topLeft
                length := rect.w - clampedRadii.topLeft - clampedRadii.topRight
                line := sdl.FRect{starting_x, rect.y - 1, length, f32(config.width.top)}
                sdl.RenderFillRect(renderer_data.renderer, &line)
            }
            if config.width.bottom > 0 {
                starting_x := rect.x + clampedRadii.bottomLeft
                starting_y := rect.y + rect.h - f32(config.width.bottom) + 1
                length := rect.w - clampedRadii.bottomLeft - clampedRadii.bottomRight
                line := sdl.FRect{starting_x, starting_y, length, f32(config.width.bottom)}
                sdl.SetRenderDrawColor(renderer_data.renderer, clay_to_sdl_color(config.color))
                sdl.RenderFillRect(renderer_data.renderer, &line)
            }
            //corners
            if config.cornerRadius.topLeft > 0 {
                centerX := rect.x + clampedRadii.topLeft - 1
                centerY := rect.y + clampedRadii.topLeft - 1
                render_arc(renderer_data, {centerX, centerY}, clampedRadii.topLeft, 180, 270, f32(config.width.top), config.color)
            }
            if config.cornerRadius.topRight > 0 {
                centerX := rect.x + rect.w - clampedRadii.topRight
                centerY := rect.y + clampedRadii.topRight - 1
                render_arc(renderer_data, {centerX, centerY}, clampedRadii.topRight, 270, 360, f32(config.width.top), config.color)
            }
            if config.cornerRadius.bottomLeft > 0 {
                centerX := rect.x + clampedRadii.bottomLeft - 1
                centerY := rect.y + rect.h - clampedRadii.bottomLeft
                render_arc(renderer_data, {centerX, centerY}, clampedRadii.bottomLeft, 90, 180, f32(config.width.bottom), config.color)
            }
            if config.cornerRadius.bottomRight > 0 {
                centerX := rect.x + rect.w - clampedRadii.bottomRight
                centerY := rect.y + rect.h - clampedRadii.bottomRight
                render_arc(renderer_data, {centerX, centerY}, clampedRadii.bottomRight, 0, 90, f32(config.width.bottom), config.color)
            }


        case .ScissorStart:
            boundingBox := cmd.boundingBox
            current_clipping_rect = sdl.Rect {
                x = i32(boundingBox.x),
                y = i32(boundingBox.y),
                w = i32(boundingBox.width),
                h = i32(boundingBox.height),
            }
            sdl.SetRenderClipRect(renderer_data.renderer, &current_clipping_rect)

        case .ScissorEnd:
            sdl.SetRenderClipRect(renderer_data.renderer, nil)


        case .Image:
            texture := (^sdl.Texture)(cmd.renderData.image.imageData)
            dest := sdl.FRect{rect.x, rect.y, rect.w, rect.h}
            sdl.RenderTexture(renderer_data.renderer, texture, nil, &dest)


        case:
            sdl.Log("Unknown render command type: %d", cmd.commandType)
        }
    }
}
