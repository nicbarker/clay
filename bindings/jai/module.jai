
Vector2 :: Math.Vector2;

make_string :: (str: string) -> String {
    return .{cast(u64, str.count), str.data};
}

UI :: (id: ElementId, layout: LayoutConfig, configs: ..ElementConfig, $call := #caller_code) -> bool #must #expand {
    _OpenElement();
    _AttachId(id);
    _AttachLayoutConfig(_StoreLayoutConfig(layout));
    for configs _AttachElementConfig(it.config, it.type);
    _ElementPostConfiguration();

    // TODO Fix the need to have to add the namespace here
    #insert,scope(call) #code defer Clay._CloseElement();;

    return true;

    // `defer _CloseElement();
}

ID :: (label: string, index: u32 = 0) -> ElementId {
    return _HashString(make_string(label), index, 0);
}

Layout :: (config: LayoutConfig) -> LayoutConfig {
    // We can just return the config because the layout is attached and stored in the UI function
    return config;
}

Rectangle :: (config: RectangleElementConfig) -> ElementConfig {
    return .{
        type = .RECTANGLE,
        config.rectangleElementConfig = _StoreRectangleElementConfig(config)
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