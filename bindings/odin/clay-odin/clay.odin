package clay

import "core:c"
import "core:strings"
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

BorderData :: struct {
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

BorderElementConfig :: struct {
	left:            BorderData,
	right:           BorderData,
	top:             BorderData,
	bottom:          BorderData,
	betweenChildren: BorderData,
	cornerRadius:    CornerRadius,
}

ScrollElementConfig :: struct {
	horizontal: c.bool,
	vertical:   c.bool,
}

FloatingAttachPointType :: enum u8 {
	LEFT_TOP,
	LEFT_CENTER,
	LEFT_BOTTOM,
	CENTER_TOP,
	CENTER_CENTER,
	CENTER_BOTTOM,
	RIGHT_TOP,
	RIGHT_CENTER,
	RIGHT_BOTTOM,
}

FloatingAttachPoints :: struct {
	element: FloatingAttachPointType,
	parent:  FloatingAttachPointType,
}

FloatingElementConfig :: struct {
	offset:     Vector2,
	expand:     Dimensions,
	zIndex:     c.uint16_t,
	parentId:   c.uint32_t,
	attachment: FloatingAttachPoints,
}

ElementConfigUnion :: struct #raw_union {
	rectangleElementConfig: ^RectangleElementConfig,
	textElementConfig:      ^TextElementConfig,
	imageElementConfig:     ^ImageElementConfig,
	customElementConfig:    ^CustomElementConfig,
	borderElementConfig:    ^BorderElementConfig,
}

RenderCommand :: struct {
	boundingBox: BoundingBox,
	config:      ElementConfigUnion,
	text:        String,
	id:          c.uint32_t,
	commandType: RenderCommandType,
}

ScrollContainerData :: struct {
	// Note: This is a pointer to the real internal scroll position, mutating it may cause a change in final layout.
	// Intended for use with external functionality that modifies scroll position, such as scroll bars or auto scrolling.
	scrollPosition:            ^Vector2,
	scrollContainerDimensions: Dimensions,
	contentDimensions:         Dimensions,
	config:                    ScrollElementConfig,
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

ClayArray :: struct($type: typeid) {
	capacity:      c.uint32_t,
	length:        c.uint32_t,
	internalArray: [^]type,
}

foreign Clay {
	Clay_MinMemorySize :: proc() -> c.uint32_t ---
	Clay_CreateArenaWithCapacityAndMemory :: proc(capacity: c.uint32_t, offset: [^]u8) -> Arena ---
	Clay_SetPointerPosition :: proc(position: Vector2) ---
	Clay_Initialize :: proc(arena: Arena) ---
	Clay_UpdateScrollContainers :: proc(isPointerActive: c.bool, scrollDelta: Vector2, deltaTime: c.float) ---
	Clay_BeginLayout :: proc(screenWidth: c.int, screenHeight: c.int) ---
	Clay_EndLayout :: proc(screenWidth: c.int, screenHeight: c.int) -> ClayArray(RenderCommand) ---
	Clay_PointerOver :: proc(id: c.uint32_t) -> c.bool ---
	Clay_GetScrollContainerData :: proc(id: c.uint32_t) -> ScrollContainerData ---
	Clay_SetMeasureTextFunction :: proc(measureTextFunction: proc(text: [^]String, config: [^]TextElementConfig) -> Dimensions) ---
	Clay__OpenContainerElement :: proc(id: c.uint32_t, layoutConfig: ^LayoutConfig) ---
	Clay__OpenRectangleElement :: proc(id: c.uint32_t, layoutConfig: ^LayoutConfig, rectangleConfig: ^RectangleElementConfig) ---
	Clay__OpenTextElement :: proc(id: c.uint32_t, text: String, textConfig: ^TextElementConfig) ---
	Clay__OpenImageElement :: proc(id: c.uint32_t, layoutConfig: ^LayoutConfig, imageConfig: ^ImageElementConfig) ---
	Clay__OpenScrollElement :: proc(id: c.uint32_t, layoutConfig: ^LayoutConfig, imageConfig: ^ScrollElementConfig) ---
	Clay__OpenFloatingElement :: proc(id: c.uint32_t, layoutConfig: ^LayoutConfig, imageConfig: ^FloatingElementConfig) ---
	Clay__OpenBorderElement :: proc(id: c.uint32_t, layoutConfig: ^LayoutConfig, imageConfig: ^BorderElementConfig) ---
	Clay__OpenCustomElement :: proc(id: c.uint32_t, layoutConfig: ^LayoutConfig, imageConfig: ^CustomElementConfig) ---
	Clay__CloseElementWithChildren :: proc() ---
	Clay__CloseScrollElement :: proc() ---
	Clay__CloseFloatingElement :: proc() ---
	Clay__layoutConfigs: ClayArray(LayoutConfig)
	Clay__LayoutConfigArray_Add :: proc(array: ^ClayArray(LayoutConfig), config: LayoutConfig) -> ^LayoutConfig ---
	Clay__rectangleElementConfigs: ClayArray(RectangleElementConfig)
	Clay__RectangleElementConfigArray_Add :: proc(array: ^ClayArray(RectangleElementConfig), config: RectangleElementConfig) -> ^RectangleElementConfig ---
	Clay__textElementConfigs: ClayArray(TextElementConfig)
	Clay__TextElementConfigArray_Add :: proc(array: ^ClayArray(TextElementConfig), config: TextElementConfig) -> ^TextElementConfig ---
	Clay__imageElementConfigs: ClayArray(ImageElementConfig)
	Clay__ImageElementConfigArray_Add :: proc(array: ^ClayArray(ImageElementConfig), config: ImageElementConfig) -> ^ImageElementConfig ---
	Clay__floatingElementConfigs: ClayArray(FloatingElementConfig)
	Clay__FloatingElementConfigArray_Add :: proc(array: ^ClayArray(FloatingElementConfig), config: FloatingElementConfig) -> ^FloatingElementConfig ---
	Clay__customElementConfigs: ClayArray(CustomElementConfig)
	Clay__CustomElementConfigArray_Add :: proc(array: ^ClayArray(CustomElementConfig), config: CustomElementConfig) -> ^CustomElementConfig ---
	Clay__scrollElementConfigs: ClayArray(ScrollElementConfig)
	Clay__ScrollElementConfigArray_Add :: proc(array: ^ClayArray(ScrollElementConfig), config: ScrollElementConfig) -> ^ScrollElementConfig ---
	Clay__borderElementConfigs: ClayArray(BorderElementConfig)
	Clay__BorderElementConfigArray_Add :: proc(array: ^ClayArray(BorderElementConfig), config: BorderElementConfig) -> ^BorderElementConfig ---
	Clay__HashString :: proc(toHash: String, index: c.uint32_t) -> c.uint32_t ---
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

EndLayout :: proc(screenWidth: c.int, screenHeight: c.int) -> ClayArray(RenderCommand) {
	return Clay_EndLayout(screenWidth, screenHeight)
}

PointerOver :: proc(id: c.uint32_t) -> c.bool {
	return Clay_PointerOver(id)
}

GetScrollContainerData :: proc(id: c.uint32_t) -> ScrollContainerData {
	return Clay_GetScrollContainerData(id)
}

@(deferred_none = Clay__CloseElementWithChildren)
Container :: proc(id: c.uint32_t, layoutConfig: ^LayoutConfig) -> bool {
	Clay__OpenContainerElement(id, layoutConfig)
	return true
}

@(deferred_none = Clay__CloseElementWithChildren)
Rectangle :: proc(
	id: c.uint32_t,
	layoutConfig: ^LayoutConfig,
	rectangleConfig: ^RectangleElementConfig,
) -> bool {
	Clay__OpenRectangleElement(id, layoutConfig, rectangleConfig)
	return true
}

Text :: proc(id: c.uint32_t, text: String, textConfig: ^TextElementConfig) -> bool {
	Clay__OpenTextElement(id, text, textConfig)
	return true
}

@(deferred_none = Clay__CloseElementWithChildren)
Image :: proc(
	id: c.uint32_t,
	layoutConfig: ^LayoutConfig,
	imageConfig: ^ImageElementConfig,
) -> bool {
	Clay__OpenImageElement(id, layoutConfig, imageConfig)
	return true
}

@(deferred_none = Clay__CloseScrollElement)
Scroll :: proc(
	id: c.uint32_t,
	layoutConfig: ^LayoutConfig,
	scrollConfig: ^ScrollElementConfig,
) -> bool {
	Clay__OpenScrollElement(id, layoutConfig, scrollConfig)
	return true
}

@(deferred_none = Clay__CloseFloatingElement)
Floating :: proc(
	id: c.uint32_t,
	layoutConfig: ^LayoutConfig,
	floatingConfig: ^FloatingElementConfig,
) -> bool {
	Clay__OpenFloatingElement(id, layoutConfig, floatingConfig)
	return true
}

@(deferred_none = Clay__CloseElementWithChildren)
Border :: proc(
	id: c.uint32_t,
	layoutConfig: ^LayoutConfig,
	borderConfig: ^BorderElementConfig,
) -> bool {
	Clay__OpenBorderElement(id, layoutConfig, borderConfig)
	return true
}

@(deferred_none = Clay__CloseElementWithChildren)
Custom :: proc(
	id: c.uint32_t,
	layoutConfig: ^LayoutConfig,
	customConfig: ^CustomElementConfig,
) -> bool {
	Clay__OpenCustomElement(id, layoutConfig, customConfig)
	return true
}

Layout :: proc(config: LayoutConfig) -> ^LayoutConfig {
	return Clay__LayoutConfigArray_Add(&Clay__layoutConfigs, config)
}

RectangleConfig :: proc(config: RectangleElementConfig) -> ^RectangleElementConfig {
	return Clay__RectangleElementConfigArray_Add(&Clay__rectangleElementConfigs, config)
}

TextConfig :: proc(config: TextElementConfig) -> ^TextElementConfig {
	return Clay__TextElementConfigArray_Add(&Clay__textElementConfigs, config)
}

ImageConfig :: proc(config: ImageElementConfig) -> ^ImageElementConfig {
	return Clay__ImageElementConfigArray_Add(&Clay__imageElementConfigs, config)
}

FloatingConfig :: proc(config: FloatingElementConfig) -> ^FloatingElementConfig {
	return Clay__FloatingElementConfigArray_Add(&Clay__floatingElementConfigs, config)
}

Custom_elementConfig :: proc(config: CustomElementConfig) -> ^CustomElementConfig {
	return Clay__CustomElementConfigArray_Add(&Clay__customElementConfigs, config)
}

ScrollConfig :: proc(config: ScrollElementConfig) -> ^ScrollElementConfig {
	return Clay__ScrollElementConfigArray_Add(&Clay__scrollElementConfigs, config)
}

BorderConfig :: proc(config: BorderElementConfig) -> ^BorderElementConfig {
	return Clay__BorderElementConfigArray_Add(&Clay__borderElementConfigs, config)
}

BorderConfigOutside :: proc(outsideBorders: BorderData) -> ^BorderElementConfig {
	return Clay__BorderElementConfigArray_Add(
		&Clay__borderElementConfigs,
		(BorderElementConfig) {
			left = outsideBorders,
			right = outsideBorders,
			top = outsideBorders,
			bottom = outsideBorders,
		},
	)
}

// BorderConfig_outside_radius :: proc(width, color, radius)  Clay_BorderElementConfigArray_Add(&Clay__borderElementConfigs, (Clay_BorderElementConfig ) { .left = { width, color }, .right = { width, color }, .top = { width, color }, .bottom = { width, color }, .cornerRadius = { radius, radius, radius, radius } })) -> CLAY_BORDER_CONFIG_OUTSIDE_RADIUS

// BorderConfig_all :: proc(...)  Clay_BorderElementConfigArray_Add(&Clay__borderElementConfigs, (Clay_BorderElementConfig ) { .left = { __VA_ARGS__ }, .right = { __VA_ARGS__ }, .top = { __VA_ARGS__ }, .bottom = { __VA_ARGS__ }, .betweenChildren = { __VA_ARGS__ } })) -> CLAY_BORDER_CONFIG_ALL

// BorderConfig_all_radius :: proc(width, color, radius)  Clay_BorderElementConfigArray_Add(&Clay__borderElementConfigs, (Clay_BorderElementConfig ) { .left = { __VA_ARGS__ }, .right = { __VA_ARGS__ }, .top = { __VA_ARGS__ }, .bottom = { __VA_ARGS__ }, .betweenChildren = { __VA_ARGS__ }, .cornerRadius = { radius, radius, radius, radius }})) -> CLAY_BORDER_CONFIG_ALL_RADIUS

// Corner_radius :: proc(radius) (Clay_CornerRadius) { radius, radius, radius, radius }) -> CLAY_CORNER_RADIUS

// Sizing_fit :: proc(...) (Clay_SizingAxis) { .type = CLAY__SIZING_TYPE_FIT, .sizeMinMax = (Clay_SizingMinMax) {__VA_ARGS__} }) -> CLAY_SIZING_FIT

// Sizing_grow :: proc(...) (Clay_SizingAxis) { .type = CLAY__SIZING_TYPE_GROW, .sizeMinMax = (Clay_SizingMinMax) {__VA_ARGS__} }) -> CLAY_SIZING_GROW

// Sizing_fixed :: proc(fixedSize) (Clay_SizingAxis) { .type = CLAY__SIZING_TYPE_GROW, .sizeMinMax = { fixedSize, fixedSize } }) -> CLAY_SIZING_FIXED

// Sizing_percent :: proc(percentOfParent) (Clay_SizingAxis) { .type = CLAY__SIZING_TYPE_PERCENT, .sizePercent = percentOfParent }) -> CLAY_SIZING_PERCENT

ClayString :: proc(label: string) -> String {
	return String{chars = raw_data(label), length = cast(c.int)len(label)}
}

ID :: proc(label: string) -> c.uint32_t {
	return Clay__HashString(ClayString(label), 0)
}

IDI :: proc(label: string, index: u32) -> c.uint32_t {
	return Clay__HashString(ClayString(label), index)
}

// _string_length :: proc(s) ((sizeof(s) / sizeof(s[0])) - sizeof(s[0]))) -> CLAY__STRING_LENGTH
