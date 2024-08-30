package main
import clay "clay-odin"
windowWidth :: 1024
windowHeight :: 768

COLOR_LIGHT :: clay.Color{224, 215, 210, 255}
COLOR_RED :: clay.Color{168, 66, 28, 255}
COLOR_ORANGE :: clay.Color{225, 138, 50, 255}

// Layout config is just a struct that can be declared statically, or inline
sidebarItemLayout := clay.LayoutConfig {
    sizing = {width = clay.SizingGrow({}), height = clay.SizingFixed(50)},
}

// Re-useable components are just normal functions
SidebarItemComponent :: proc(index: u32) {
    if clay.Rectangle(clay.IDI("SidebarBlob", index), &sidebarItemLayout, clay.RectangleConfig({color = COLOR_ORANGE})) {}
}

// An example function to begin the "root" of your layout tree
CreateLayout :: proc() -> clay.ClayArray(clay.RenderCommand) {
    clay.BeginLayout(windowWidth, windowHeight)

    // An example of laying out a UI with a fixed width sidebar and flexible width main content
    // NOTE: To create a scope for child components, the Odin api uses `if` with components that have children
    if clay.Rectangle(
        clay.ID("OuterContainer"),
        clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingGrow({})}, padding = {16, 16}, childGap = 16}),
        clay.RectangleConfig({color = {250, 250, 255, 255}}),
    ) {
        if clay.Rectangle(
            clay.ID("SideBar"),
            clay.Layout({layoutDirection = .TOP_TO_BOTTOM, sizing = {width = clay.SizingFixed(300), height = clay.SizingGrow({})}, padding = {16, 16}, childGap = 16}),
            clay.RectangleConfig({color = COLOR_LIGHT}),
        ) {
            if clay.Rectangle(
                clay.ID("ProfilePictureOuter"),
                clay.Layout({sizing = {width = clay.SizingGrow({})}, padding = {16, 16}, childGap = 16, childAlignment = {y = .CENTER}}),
                clay.RectangleConfig({color = COLOR_RED}),
            ) {
                if clay.Image(
                    clay.ID("ProfilePicture"),
                    clay.Layout({sizing = {width = clay.SizingFixed(60), height = clay.SizingFixed(60)}}),
                    clay.ImageConfig({imageData = &profilePicture, sourceDimensions = {height = 60, width = 60}}),
                ) {}
                clay.Text(clay.ID("ProfileTitle"), clay.MakeString("Clay - UI Library"), clay.TextConfig({fontSize = 24, textColor = {255, 255, 255, 255}}))
            }

            // Standard Odin code like loops etc work inside components
            for i: u32 = 0; i < 10; i += 1 {
                SidebarItemComponent(i)
            }
        }

        if clay.Rectangle(
            clay.ID("MainContent"),
            clay.Layout({sizing = {width = clay.SizingGrow({}), height = clay.SizingGrow({})}}),
            clay.RectangleConfig({color = COLOR_LIGHT}),
        ) {}
    }
    // ...
}
