
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

make_string :: (str: string) -> String {
    return .{cast(u64, str.count), str.data};
}

// The way of handling this is inspired by the odin bindings
UI :: (configs: ..TypedConfig, $call := #caller_code) -> bool #must #expand {
    _OpenElement();
    for config : configs {
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

    // !IMPORTANT
    // TODO Fix the need to have to add the namespace here
    #insert,scope(call) #code defer Clay._CloseElement();;

    return true;
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

SizingGrow :: (size_min_max: SizingMinMax = .{}) -> SizingAxis {
    return .{type = .GROW, size = .{minMax = size_min_max}};
}

#scope_module

Math :: #import "Math";
Compiler :: #import "Compiler";
ProgramPrint :: #import "Program_Print";

#if OS == .WINDOWS {
    #load "windows.jai";
} else {
    assert(false);
}