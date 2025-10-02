// Basic port of the C example SDL3 renderer, with a dynamic array of fonts.
package video_demo_sdl

import clay "../../clay-odin"
import "core:math"
import sdl "vendor:sdl3"
import "vendor:sdl3/ttf"

Clay_SDL3RendererData :: struct { 
	renderer:   ^sdl.Renderer,
	textEngine: ^ttf.TextEngine,
	fonts:      [dynamic]^ttf.Font,
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
sdl_Clay_RenderFillRoundedRect :: proc(
	rendererData: ^Clay_SDL3RendererData,
	rect: sdl.FRect,
	cornerRadius: f32,
	_color: clay.Color,
) {
	color := sdl.FColor(_color / 255)

	indexCount: i32 = 0
	vertexCount: i32 = 0

	minRadius := sdl.min(rect.w, rect.h) / 2
	clampedRadius := sdl.min(cornerRadius, minRadius)

	numCircleSegments := sdl.max(NUM_CIRCLE_SEGMENTS, i32(clampedRadius * 0.5))

	totalVertices := 4 + (4 * (numCircleSegments * 2)) + 2 * 4
	totalIndices := 6 + (4 * (numCircleSegments * 3)) + 6 * 4

	// Maybe instrinsics.alloca these?
	vertices := make([]sdl.Vertex, totalVertices, allocator = context.temp_allocator)
	indices := make([]i32, totalIndices, allocator = context.temp_allocator)

	//define center rectangle
	vertices[vertexCount + 0] = (sdl.Vertex) {
		{rect.x + clampedRadius, rect.y + clampedRadius},
		color,
		{0, 0},
	} //0 center TL
	vertices[vertexCount + 1] = (sdl.Vertex) {
		{rect.x + rect.w - clampedRadius, rect.y + clampedRadius},
		color,
		{1, 0},
	} //1 center TR
	vertices[vertexCount + 2] = (sdl.Vertex) {
		{rect.x + rect.w - clampedRadius, rect.y + rect.h - clampedRadius},
		color,
		{1, 1},
	} //2 center BR
	vertices[vertexCount + 3] = (sdl.Vertex) {
		{rect.x + clampedRadius, rect.y + rect.h - clampedRadius},
		color,
		{0, 1},
	} //3 center BL

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

		for j in i32(0) ..< 4 { 	// Iterate over four corners
			cx, cy, signX, signY: f32

			switch (j) {
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

			vertices[vertexCount + 0] = (sdl.Vertex) {
				{
					cx + sdl.cosf(angle1) * clampedRadius * signX,
					cy + sdl.sinf(angle1) * clampedRadius * signY,
				},
				color,
				{0, 0},
			}
			vertices[vertexCount + 1] = (sdl.Vertex) {
				{
					cx + sdl.cosf(angle2) * clampedRadius * signX,
					cy + sdl.sinf(angle2) * clampedRadius * signY,
				},
				color,
				{0, 0},
			}

			vertexCount += 2

			indices[indexCount + 0] = j // Connect to corresponding central rectangle vertex
			indices[indexCount + 1] = vertexCount - 2
			indices[indexCount + 2] = vertexCount - 1
			indexCount += 3
		}
	}

	//Define edge rectangles
	// Top edge
	vertices[vertexCount + 0] = (sdl.Vertex){{rect.x + clampedRadius, rect.y}, color, {0, 0}} //TL
	vertices[vertexCount + 1] = (sdl.Vertex) {
		{rect.x + rect.w - clampedRadius, rect.y},
		color,
		{1, 0},
	} //TR

	vertexCount += 2

	indices[indexCount + 0] = 0
	indices[indexCount + 1] = vertexCount - 2 //TL
	indices[indexCount + 2] = vertexCount - 1 //TR
	indices[indexCount + 3] = 1
	indices[indexCount + 4] = 0
	indices[indexCount + 5] = vertexCount - 1 //TR
	indexCount += 6
	// Right edge
	vertices[vertexCount + 0] = (sdl.Vertex) {
		{rect.x + rect.w, rect.y + clampedRadius},
		color,
		{1, 0},
	} //RT
	vertices[vertexCount + 1] = (sdl.Vertex) {
		{rect.x + rect.w, rect.y + rect.h - clampedRadius},
		color,
		{1, 1},
	} //RB
	vertexCount += 2

	indices[indexCount + 0] = 1
	indices[indexCount + 1] = vertexCount - 2 //RT
	indices[indexCount + 2] = vertexCount - 1 //RB
	indices[indexCount + 3] = 2
	indices[indexCount + 4] = 1
	indices[indexCount + 5] = vertexCount - 1 //RB
	indexCount += 6

	// Bottom edge
	vertices[vertexCount + 0] = (sdl.Vertex) {
		{rect.x + rect.w - clampedRadius, rect.y + rect.h},
		color,
		{1, 1},
	} //BR
	vertices[vertexCount + 1] = (sdl.Vertex) {
		{rect.x + clampedRadius, rect.y + rect.h},
		color,
		{0, 1},
	} //BL
	vertexCount += 2

	indices[indexCount + 0] = 2
	indices[indexCount + 1] = vertexCount - 2 //BR
	indices[indexCount + 2] = vertexCount - 1 //BL
	indices[indexCount + 3] = 3
	indices[indexCount + 4] = 2
	indices[indexCount + 5] = vertexCount - 1 //BL
	indexCount += 6

	// Left edge
	vertices[vertexCount + 0] = (sdl.Vertex) {
		{rect.x, rect.y + rect.h - clampedRadius},
		color,
		{0, 1},
	} //LB
	vertices[vertexCount + 1] = (sdl.Vertex){{rect.x, rect.y + clampedRadius}, color, {0, 0}} //LT
	vertexCount += 2

	indices[indexCount + 0] = 3
	indices[indexCount + 1] = vertexCount - 2 //LB
	indices[indexCount + 2] = vertexCount - 1 //LT
	indices[indexCount + 3] = 0
	indices[indexCount + 4] = 3
	indices[indexCount + 5] = vertexCount - 1 //LT
	indexCount += 6

	// Render everything
	sdl.RenderGeometry(
		rendererData.renderer,
		nil,
		raw_data(vertices),
		vertexCount,
		raw_data(indices),
		indexCount,
	)
}

sdl_Clay_RenderArc :: proc(
	rendererData: ^Clay_SDL3RendererData,
	center: sdl.FPoint,
	radius: f32,
	startAngle: f32,
	endAngle: f32,
	thickness: f32,
	color: clay.Color,
) {
	sdl.SetRenderDrawColor(
		rendererData.renderer,
		u8(color.r),
		u8(color.g),
		u8(color.b),
		u8(color.a),
	)

	radStart := startAngle * (math.PI / 180.0)
	radEnd := endAngle * (math.PI / 180.0)

	numCircleSegments := sdl.max(NUM_CIRCLE_SEGMENTS, i32(radius * 1.5)) //increase circle segments for larger circles, 1.5 is arbitrary.

	angleStep := (radEnd - radStart) / f32(numCircleSegments)
	thicknessStep: f32 = 0.4 //arbitrary value to avoid overlapping lines. Changing THICKNESS_STEP or numCircleSegments might cause artifacts.

	for t := thicknessStep; t < thickness - thicknessStep; t += thicknessStep {
		points := make([]sdl.FPoint, numCircleSegments + 1, allocator = context.temp_allocator)
		clampedRadius := sdl.max(radius - t, 1)

		for i in 0 ..= numCircleSegments {
			angle := radStart + f32(i) * angleStep
			points[i] = (sdl.FPoint) {
				sdl.roundf(center.x + sdl.cosf(angle) * clampedRadius),
				sdl.roundf(center.y + sdl.sinf(angle) * clampedRadius),
			}
		}
		sdl.RenderLines(rendererData.renderer, raw_data(points), numCircleSegments + 1)
	}
}

currentClippingRectangle: sdl.Rect

sdl_Clay_RenderClayCommands :: proc(
	rendererData: ^Clay_SDL3RendererData,
	rcommands: ^clay.ClayArray(clay.RenderCommand),
) {
	for i in 0 ..< rcommands.length {
		rcmd := clay.RenderCommandArray_Get(rcommands, i)
		bounding_box := rcmd.boundingBox
		rect := sdl.FRect{bounding_box.x, bounding_box.y, bounding_box.width, bounding_box.height}

		#partial switch (rcmd.commandType) {
		case .Rectangle:
			config := &rcmd.renderData.rectangle
			sdl.SetRenderDrawBlendMode(rendererData.renderer, sdl.BLENDMODE_BLEND)
			sdl.SetRenderDrawColor(
				rendererData.renderer,
				u8(config.backgroundColor.r),
				u8(config.backgroundColor.g),
				u8(config.backgroundColor.b),
				u8(config.backgroundColor.a),
			)
			if (config.cornerRadius.topLeft > 0) {
				sdl_Clay_RenderFillRoundedRect(
					rendererData,
					rect,
					config.cornerRadius.topLeft,
					config.backgroundColor,
				)
			} else {
				sdl.RenderFillRect(rendererData.renderer, &rect)
			}

		case .Text:
			config := &rcmd.renderData.text
			font := rendererData.fonts[config.fontId]
			ttf.SetFontSize(font, px_to_pt(f32(config.fontSize)))
			text := ttf.CreateText(
				rendererData.textEngine,
				font,
				cstring(config.stringContents.chars),
				uint(config.stringContents.length),
			)
			ttf.SetTextColor(
				text,
				u8(config.textColor.r),
				u8(config.textColor.g),
				u8(config.textColor.b),
				u8(config.textColor.a),
			)
			ttf.DrawRendererText(text, rect.x, rect.y)
			ttf.DestroyText(text)

		case .Border:
			config := &rcmd.renderData.border

			minRadius := sdl.min(rect.w, rect.h) / 2
			clampedRadii := clay.CornerRadius {
				topLeft     = sdl.min(config.cornerRadius.topLeft, minRadius),
				topRight    = sdl.min(config.cornerRadius.topRight, minRadius),
				bottomLeft  = sdl.min(config.cornerRadius.bottomLeft, minRadius),
				bottomRight = sdl.min(config.cornerRadius.bottomRight, minRadius),
			}
			//edges
			sdl.SetRenderDrawColor(
				rendererData.renderer,
				u8(config.color.r),
				u8(config.color.g),
				u8(config.color.b),
				u8(config.color.a),
			)
			if (config.width.left > 0) {
				starting_y := rect.y + clampedRadii.topLeft
				length := rect.h - clampedRadii.topLeft - clampedRadii.bottomLeft
				line := sdl.FRect{rect.x - 1, starting_y, f32(config.width.left), length}
				sdl.RenderFillRect(rendererData.renderer, &line)
			}
			if (config.width.right > 0) {
				starting_x := rect.x + rect.w - f32(config.width.right) + 1
				starting_y := rect.y + clampedRadii.topRight
				length := rect.h - clampedRadii.topRight - clampedRadii.bottomRight
				line := sdl.FRect{starting_x, starting_y, f32(config.width.right), length}
				sdl.RenderFillRect(rendererData.renderer, &line)
			}
			if (config.width.top > 0) {
				starting_x := rect.x + clampedRadii.topLeft
				length := rect.w - clampedRadii.topLeft - clampedRadii.topRight
				line := sdl.FRect{starting_x, rect.y - 1, length, f32(config.width.top)}
				sdl.RenderFillRect(rendererData.renderer, &line)
			}
			if (config.width.bottom > 0) {
				starting_x := rect.x + clampedRadii.bottomLeft
				starting_y := rect.y + rect.h - f32(config.width.bottom) + 1
				length := rect.w - clampedRadii.bottomLeft - clampedRadii.bottomRight
				line := sdl.FRect{starting_x, starting_y, length, f32(config.width.bottom)}
				sdl.SetRenderDrawColor(
					rendererData.renderer,
					u8(config.color.r),
					u8(config.color.g),
					u8(config.color.b),
					u8(config.color.a),
				)
				sdl.RenderFillRect(rendererData.renderer, &line)
			}
			//corners
			if (config.cornerRadius.topLeft > 0) {
				centerX := rect.x + clampedRadii.topLeft - 1
				centerY := rect.y + clampedRadii.topLeft - 1
				sdl_Clay_RenderArc(
					rendererData,
					(sdl.FPoint){centerX, centerY},
					clampedRadii.topLeft,
					180.0,
					270.0,
					f32(config.width.top),
					config.color,
				)
			}
			if (config.cornerRadius.topRight > 0) {
				centerX := rect.x + rect.w - clampedRadii.topRight
				centerY := rect.y + clampedRadii.topRight - 1
				sdl_Clay_RenderArc(
					rendererData,
					(sdl.FPoint){centerX, centerY},
					clampedRadii.topRight,
					270.0,
					360.0,
					f32(config.width.top),
					config.color,
				)
			}
			if (config.cornerRadius.bottomLeft > 0) {
				centerX := rect.x + clampedRadii.bottomLeft - 1
				centerY := rect.y + rect.h - clampedRadii.bottomLeft
				sdl_Clay_RenderArc(
					rendererData,
					(sdl.FPoint){centerX, centerY},
					clampedRadii.bottomLeft,
					90.0,
					180.0,
					f32(config.width.bottom),
					config.color,
				)
			}
			if (config.cornerRadius.bottomRight > 0) {
				centerX := rect.x + rect.w - clampedRadii.bottomRight
				centerY := rect.y + rect.h - clampedRadii.bottomRight
				sdl_Clay_RenderArc(
					rendererData,
					(sdl.FPoint){centerX, centerY},
					clampedRadii.bottomRight,
					0.0,
					90.0,
					f32(config.width.bottom),
					config.color,
				)
			}


		case .ScissorStart:
			boundingBox := rcmd.boundingBox
			currentClippingRectangle = (sdl.Rect) {
				x = i32(boundingBox.x),
				y = i32(boundingBox.y),
				w = i32(boundingBox.width),
				h = i32(boundingBox.height),
			}
			sdl.SetRenderClipRect(rendererData.renderer, &currentClippingRectangle)

		case .ScissorEnd:
			sdl.SetRenderClipRect(rendererData.renderer, nil)


		case .Image:
			texture := (^sdl.Texture)(rcmd.renderData.image.imageData)
			dest := sdl.FRect{rect.x, rect.y, rect.w, rect.h}
			sdl.RenderTexture(rendererData.renderer, texture, nil, &dest)


		case:
			sdl.Log("Unknown render command type: %d", rcmd.commandType)
		}
	}
}
