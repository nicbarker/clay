package clay

import "core:c"
import "core:fmt"
foreign import Clay "clay.a"

String :: struct {
	length: c.int,
	chars:  [^]c.char,
}

Vector2 :: struct {
	x: c.float,
	y: c.float,
}

Dimensions :: struct {
	width:  c.float,
	height: c.float,
}

Arena :: struct {
	label:          String,
	nextAllocation: c.uint64_t,
	capacity:       c.uint64_t,
	memory:         [^]c.char,
}

BoundingBox :: struct {
	x:      c.float,
	y:      c.float,
	width:  c.float,
	height: c.float,
}

Color :: struct {
	r: c.float,
	g: c.float,
	b: c.float,
	a: c.float,
}

CornerRadius :: struct {
	topLeft:     c.float,
	topRight:    c.float,
	bottomLeft:  c.float,
	bottomRight: c.float,
}

Border :: struct {
	width: c.uint32_t,
	color: Color,
}

RenderCommandType :: enum u8 {
	None,
	Rectangle,
	Border,
	Text,
	Image,
	ScissorStart,
	ScissorEnd,
	Custom,
}

RectangleElementConfig :: struct {
	color:        Color,
	cornerRadius: CornerRadius,
}

TextElementConfig :: struct {
	textColor:     Color,
	fontId:        c.uint16_t,
	fontSize:      c.uint16_t,
	letterSpacing: c.uint16_t,
	lineSpacing:   c.uint16_t,
}

ImageElementConfig :: struct {
	imageData:        rawptr,
	sourceDimensions: Dimensions,
}

CustomElementConfig :: struct {
	customData: rawptr,
}

BorderContainerElementConfig :: struct {
	left:            Border,
	right:           Border,
	top:             Border,
	bottom:          Border,
	betweenChildren: Border,
	cornerRadius:    CornerRadius,
}

ScrollContainerElementConfig :: struct {
	horizontal: c.bool,
	vertical:   c.bool,
}

ElementConfigUnion :: struct #raw_union {
	rectangleElementConfig:       ^RectangleElementConfig,
	textElementConfig:            ^TextElementConfig,
	imageElementConfig:           ^ImageElementConfig,
	customElementConfig:          ^CustomElementConfig,
	borderContainerElementConfig: ^BorderContainerElementConfig,
}

RenderCommand :: struct {
	boundingBox: BoundingBox,
	config:      ElementConfigUnion,
	text:        String,
	id:          c.uint32_t,
	commandType: RenderCommandType,
}

RenderCommandArray :: struct {
	capacity:      c.uint32_t,
	length:        c.uint32_t,
	internalArray: [^]RenderCommand,
}

ScrollContainerData :: struct {
	// Note: This is a pointer to the real internal scroll position, mutating it may cause a change in final layout.
	// Intended for use with external functionality that modifies scroll position, such as scroll bars or auto scrolling.
	scrollPosition:            ^Vector2,
	scrollContainerDimensions: Dimensions,
	contentDimensions:         Dimensions,
	config:                    ScrollContainerElementConfig,
	// Indicates whether an actual scroll container matched the provided ID or if the default struct was returned.
	found:                     c.bool,
}

SizingType :: enum u8 {
	FIT,
	GROW,
	PERCENT,
}

SizingAxis :: struct {
	// Note: `min` is used for CLAY_SIZING_PERCENT, slightly different to clay.h due to lack of C anonymous unions
	min:  c.float,
	max:  c.float,
	type: SizingType,
}

Sizing :: struct {
	width:  SizingAxis,
	height: SizingAxis,
}

Padding :: struct {
	x: c.uint16_t,
	y: c.uint16_t,
}

LayoutDirection :: enum u8 {
	LEFT_TO_RIGHT,
	TOP_TO_BOTTOM,
}

LayoutAlignmentX :: enum u8 {
	LEFT,
	RIGHT,
	CENTER,
}

LayoutAlignmentY :: enum u8 {
	TOP,
	BOTTOM,
	CENTER,
}

ChildAlignment :: struct {
	x: LayoutAlignmentX,
	y: LayoutAlignmentY,
}

LayoutConfig :: struct {
	sizing:          Sizing,
	padding:         Padding,
	childGap:        c.uint16_t,
	layoutDirection: LayoutDirection,
	childAlignment:  ChildAlignment,
}

foreign Clay {
	Clay_MinMemorySize :: proc() -> c.uint32_t ---
	Clay_CreateArenaWithCapacityAndMemory :: proc(capacity: c.uint32_t, offset: [^]u8) -> Arena ---
	Clay_SetPointerPosition :: proc(position: Vector2) ---
	Clay_Initialize :: proc(arena: Arena) ---
	Clay_UpdateScrollContainers :: proc(isPointerActive: c.bool, scrollDelta: Vector2, deltaTime: c.float) ---
	Clay_BeginLayout :: proc(screenWidth: c.int, screenHeight: c.int) ---
	Clay_EndLayout :: proc(screenWidth: c.int, screenHeight: c.int) -> RenderCommandArray ---
	Clay_PointerOver :: proc(id: c.uint32_t) -> c.bool ---
	Clay_GetScrollContainerData :: proc(id: c.uint32_t) -> ScrollContainerData ---
	Clay__CloseContainerElement :: proc() ---
	Clay__OpenRectangleElement :: proc(id: c.uint32_t, layoutConfig: ^LayoutConfig, rectangleConfig: ^RectangleElementConfig) ---
}

MinMemorySize :: proc() -> c.uint32_t {
	return Clay_MinMemorySize()
}

CreateArenaWithCapacityAndMemory :: proc(capacity: c.uint32_t, offset: [^]u8) -> Arena {
	return Clay_CreateArenaWithCapacityAndMemory(capacity, offset)
}

SetPointerPosition :: proc(position: Vector2) {
	Clay_SetPointerPosition(position)
}

Initialize :: proc(arena: Arena) {
	Clay_Initialize(arena)
}

UpdateScrollContainers :: proc(isPointerActive: c.bool, scrollDelta: Vector2, deltaTime: c.float) {
	Clay_UpdateScrollContainers(isPointerActive, scrollDelta, deltaTime)
}

BeginLayout :: proc(screenWidth: c.int, screenHeight: c.int) {
	Clay_BeginLayout(screenWidth, screenHeight)
}

EndLayout :: proc(screenWidth: c.int, screenHeight: c.int) -> RenderCommandArray {
	return Clay_EndLayout(screenWidth, screenHeight)
}

PointerOver :: proc(id: c.uint32_t) -> c.bool {
	return Clay_PointerOver(id)
}

GetScrollContainerData :: proc(id: c.uint32_t) -> ScrollContainerData {
	return Clay_GetScrollContainerData(id)
}

@(deferred_none = Clay__CloseContainerElement)
Rectangle :: proc(
	id: c.uint32_t,
	layoutConfig: ^LayoutConfig,
	rectangleConfig: ^RectangleElementConfig,
) -> bool {
	Clay__OpenRectangleElement(id, layoutConfig, rectangleConfig)
	return true
}
