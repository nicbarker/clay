/*
These bindings adapt the CLAY macro using some for_expansion trickery, allowing a syntax similar to the one in the Odin bindings. 
I'll try to explain here why I did it that way.

In Odin, they can mark the procedure with deferred_none, allowing to call a proc after the current one, which works well with ifs.

@(require_results, deferred_none = _CloseElement)
UI :: proc(configs: ..TypedConfig) -> bool {}

You can then use it like so : 

if UI(Layout(), etc..) {Children ...}

So I tried to replicate this in Jai. The first thing I did was to try making a macro returning a bool with a backticked defer in it.

UI :: (configs: ..TypedConfig) -> bool #must #expand {
    `defer EndElement();
    return true;
}

But this doesn't work if you have two elements side to side, as the end of the first element will be called after the start and the end of the second one.

Another option used in these bindings : https://github.com/nick-celestin-zizic/clay-jai is to have that defer like above and have the macro return nothing. You can then use it like that : 

{ UI(Layout()); Children(); }

But I'm not a big fan of that since it's possible to forget the scope braces or to put the children before the element.

Another option to consider is to pass a code block to a macro that puts it between the start and the end.

UI :: (id: ElementId, layout: LayoutConfig, configs: ..ElementConfig, $code: Code) {
    OpenElement();
    ...
    #insert code;
    CloseElement();
}

UI(..., #code {
    Children();
});

However this prevents to refer to variables from the previous scope, and it's also a bit akward to type.

The final solution I found, inspired by the fact that CLAY uses a for loop behind the scene, was to use Jai's for_expansions.
Here is how it works :

InternalElementConfigArray :: struct {
    configs: [] TypedConfig;
}

Element :: (configs: ..TypedConfig) -> InternalElementConfigArray #expand {
    return .{configs};
}

for_expansion :: (configs_array: InternalElementConfigArray, body: Code, _: For_Flags) #expand {
    Jai forces the definition of these
    `it_index := 0;
    `it := 0;
    _OpenElement();
    ...
    #insert body;
    _CloseElement();
}

As you can see it's kinda similar to the previous one, but doesn't have the limitation on refering to variable from the calling scope.
Element builds an InternalElementConfigArray that has an array to the TypedConfigs, which will be passed to the for_expansion when placed after a for (this is a Jai feature).
This then allows to write something like :

for Element(Layout()) { Children(); }

With the downside that it's not obvious why the for is there before reading the documentation.
*/

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