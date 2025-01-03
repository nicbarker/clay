// TODO Documentation about the for_expansion

Vector2 :: Math.Vector2;

ElementConfigType :: enum s32 {
    NONE               :: 0;
    RECTANGLE          :: 1;
    BORDER_CONTAINER   :: 2;
    FLOATING_CONTAINER :: 4;
    SCROLL_CONTAINER   :: 8;
    IMAGE              :: 16;
    TEXT               :: 32;
    CUSTOM             :: 64;
    CLAY__ELEMENT_CONFIG_TYPE_NONE               :: NONE;
    CLAY__ELEMENT_CONFIG_TYPE_RECTANGLE          :: RECTANGLE;
    CLAY__ELEMENT_CONFIG_TYPE_BORDER_CONTAINER   :: BORDER_CONTAINER;
    CLAY__ELEMENT_CONFIG_TYPE_FLOATING_CONTAINER :: FLOATING_CONTAINER;
    CLAY__ELEMENT_CONFIG_TYPE_SCROLL_CONTAINER   :: SCROLL_CONTAINER;
    CLAY__ELEMENT_CONFIG_TYPE_IMAGE              :: IMAGE;
    CLAY__ELEMENT_CONFIG_TYPE_TEXT               :: TEXT;
    CLAY__ELEMENT_CONFIG_TYPE_CUSTOM             :: CUSTOM;

    // Jai bindings specific types, please don't assume any value in those
    // a it might change if the enums above overlap with it
    // TODO Check if these values need to be powers of two
    ID :: 256;
    LAYOUT :: 257;
}

// This is passed to UI so that we can omit layout
TypedConfig :: struct {
    type:   ElementConfigType;
    config: *void;
    id:     ElementId;
}

BorderData :: struct {
    width: u32;
    color: Color;
}

BorderElementConfig :: struct {
    left:            BorderData;
    right:           BorderData;
    top:             BorderData;
    bottom:          BorderData;
    betweenChildren: BorderData;
    cornerRadius:    CornerRadius;
}

make_string :: (str: string) -> String {
    clay_string := String.{cast(s32, str.count), str.data};
    return clay_string;
}

global_counter := 0;

for_expansion :: (configs_array: InternalElementConfigArray, body: Code, _: For_Flags) #expand {
    // Jai forces the definition of these
    `it_index := 0;
    `it := 0;

    _OpenElement();
    for config : configs_array.configs {
        if config.type == {
        case .ID;
            _AttachId(config.id);
        case .LAYOUT;
            _AttachLayoutConfig(cast(*LayoutConfig, config.config));
        case;
            // config.config is a *void, it stores the address of the pointer that is stored in the union
            // as ElementConfigUnion is a union of structs. We can't cast pointers directly to structs,
            // we first cast the address of the *void and then dereference it.
            // Maybe there's a cast modifier to avoid this, but I don't know it (no_check and trunc didn't work).
            _AttachElementConfig(cast(*ElementConfigUnion, *config.config).*, config.type);
        }
    }
    _ElementPostConfiguration();

    #insert body;

    _CloseElement();
}

Element :: (configs: ..TypedConfig) -> InternalElementConfigArray #expand {
    return .{configs};
}

ID :: (label: string, index: u32 = 0) -> TypedConfig {
    return .{type = .ID, id = _HashString(make_string(label), index, 0)};
}

Layout :: (config: LayoutConfig) -> TypedConfig {
    return .{type = .LAYOUT, config = _StoreLayoutConfig(config)};
}

Rectangle :: (config: RectangleElementConfig) -> TypedConfig {
    return .{
        type = .RECTANGLE,
        config = _StoreRectangleElementConfig(config)
    };
}

Floating :: (config: FloatingElementConfig) -> TypedConfig {
    return .{type = .FLOATING_CONTAINER, config = _StoreFloatingElementConfig(config)};
}

Scroll :: (config: ScrollElementConfig) -> TypedConfig {
    return .{type = .SCROLL_CONTAINER, config = _StoreScrollElementConfig(config)};
}

Image :: (config: ImageElementConfig) -> TypedConfig {
    return .{type = .IMAGE, config = _StoreImageElementConfig(config)};
}

Custom :: (config: CustomElementConfig) -> TypedConfig {
    return .{type = .CUSTOM, config = _StoreCustomElementConfig(config)};
}

Border :: (config: BorderElementConfig) -> TypedConfig {
    return .{type = .BORDER, _StoreBorderElementConfig(config)};
}

BorderOutside :: (outside_borders: BorderData) -> TypedConfig {
    return .{
        type = .BORDER, 
        config = _StoreBorderElementConfig(BorderElementConfig.{
            left = outside_borders,
            right = outside_borders,
            top = outside_borders,
            bottom = outside_borders,
        }),
    };
}

BorderOutsideRadius :: (outside_borders: BorderData, radius: float) -> TypedConfig {
    return .{
        type = .BORDER,
        config = _StoreBorderElementConfig(.{
            left = outside_borders,
            right = outside_borders,
            top = outside_borders,
            bottom = outside_borders,
            cornerRadius = .{radius, radius, radius, radius},
        })
    };
}

BorderAll :: (all_borders: BorderData) -> TypedConfig {
    return .{
        type = .BORDER,
        config = _StoreBorderElementConfig(.{
            left = all_borders,
            right = all_borders,
            top = all_borders,
            bottom = all_borders,
            betweenChildren = all_borders,
        })
    };
}

BorderAllRadius :: (all_borders: BorderData, radius: float) -> TypedConfig {
    return .{
        type = .BORDER,
        config = _StoreBorderElementConfig(.{
            left = all_borders,
            right = all_borders,
            top = all_borders,
            bottom = all_borders,
            cornerRadius = .{radius, radius, radius, radius},
        })
    };
}

CornerRadiusAll :: (radius: float) -> CornerRadius {
    return .{radius, radius, radius, radius};
}

Text :: (text: string, config: *TextElementConfig) {
    _OpenTextElement(make_string(text), config);
}

TextConfig :: (config: TextElementConfig) -> *TextElementConfig {
    return _StoreTextElementConfig(config);
}

SizingFit :: (size_min_max: SizingMinMax) -> SizingAxis {
    return .{type = .FIT, size = .{minMax = size_min_max}};
}

SizingGrow :: (size_min_max: SizingMinMax = .{}) -> SizingAxis {
    return .{type = .GROW, size = .{minMax = size_min_max}};
}

SizingFixed :: (size: float) -> SizingAxis {
    return .{type = .FIXED, size = .{minMax = .{size, size}}};
}

SizingPercent :: (size_percent: float) -> SizingAxis {
    return .{type = .PERCENT, size = .{percent = size_percent}};
}

GetElementId :: (str: string) -> ElementId {
    return GetElementId(make_string(str));
}

#scope_module

Math :: #import "Math";
Compiler :: #import "Compiler";
ProgramPrint :: #import "Program_Print";

InternalElementConfigArray :: struct {
    configs: [] TypedConfig;
}

#if OS == .WINDOWS {
    #load "windows.jai";
} else {
    assert(false);
}