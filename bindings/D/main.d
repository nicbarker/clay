import clay;

import core.stdc.stdlib;

__gshared:

Clay_LayoutConfig layoutElement = { padding: {5} };

extern(C) void main()
{
    ulong totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = {
        label: str("Clay Memory Arena"),
        capacity: totalMemorySize,
        memory: cast(char*)malloc(totalMemorySize)
    };

    Clay_Initialize(clayMemory, Clay_Dimensions(1024,768));
    Clay_BeginLayout();
    if (ClayBegin( Rectangle(color: Clay_Color(255,255,255,0)), Layout(layoutElement)))
    {   }
    ClayEnd();
}


// helper functions
Clay_String str(string it)
{
    return Clay_String(cast(int)it.length, it.ptr);
}

bool ClayBegin(A...)(A configs)
{
    Clay__OpenElement();
    foreach(config; configs)
    {
        alias T = typeof(config);
        static if (is(T == Clay_ElementId))
        {
            Clay__AttachId(config);
        }
        else static if(is(T == Clay_LayoutConfig*))
        {
            Clay__AttachLayoutConfig(config);
        }
        else static if(is(T == Clay_ElementConfig))
        {
            Clay__AttachElementConfig(config.config, config.type);
        }
        else static assert(0, "unsupported " ~ typeof(config).stringof);
    }

    Clay__ElementPostConfiguration();
    return true;
}

void ClayEnd()
{
    Clay__CloseElement();
}

Clay_ElementId Id(string label)
{
     return Clay__HashString(str(label), 0, 0);
}

Clay_LayoutConfig* Layout(lazy Clay_Sizing sizing = Clay_Sizing.init)
{
    Clay_LayoutConfig config;
    config.sizing = sizing;
    return Clay__StoreLayoutConfig(config);
}

Clay_LayoutConfig* Layout(Clay_LayoutConfig config)
{
    return Clay__StoreLayoutConfig(config);
}

Clay_ElementConfig Rectangle(lazy Clay_Color color = Clay_Color.init)
{
    Clay_RectangleElementConfig config;
    config.color = color;

    Clay_ElementConfig ret;
    ret.type = Clay__ElementConfigType.CLAY__ELEMENT_CONFIG_TYPE_RECTANGLE;
    ret.config.rectangleElementConfig = Clay__StoreRectangleElementConfig(config);
    return ret;
}
