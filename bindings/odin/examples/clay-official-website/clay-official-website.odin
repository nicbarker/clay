package main

import clay "../../clay-odin"
import "core:c"
import "core:fmt"
import "vendor:raylib"

windowWidth: i32 = 1024
windowHeight: i32 = 768

syntaxImage: raylib.Texture2D = {}
checkImage1: raylib.Texture2D = {}
checkImage2: raylib.Texture2D = {}
checkImage3: raylib.Texture2D = {}
checkImage4: raylib.Texture2D = {}
checkImage5: raylib.Texture2D = {}

FONT_ID_BODY_16 :: 0
FONT_ID_TITLE_56 :: 9
FONT_ID_TITLE_52 :: 1
FONT_ID_TITLE_48 :: 2
FONT_ID_TITLE_36 :: 3
FONT_ID_TITLE_32 :: 4
FONT_ID_BODY_36 :: 5
FONT_ID_BODY_30 :: 6
FONT_ID_BODY_28 :: 7
FONT_ID_BODY_24 :: 8

COLOR_LIGHT :: clay.Color{244, 235, 230, 255}
COLOR_LIGHT_HOVER :: clay.Color{224, 215, 210, 255}
COLOR_BUTTON_HOVER :: clay.Color{238, 227, 225, 255}
COLOR_BROWN :: clay.Color{61, 26, 5, 255}
//COLOR_RED :: clay.Color {252, 67, 27, 255}
COLOR_RED :: clay.Color{168, 66, 28, 255}
COLOR_RED_HOVER :: clay.Color{148, 46, 8, 255}
COLOR_ORANGE :: clay.Color{225, 138, 50, 255}
COLOR_BLUE :: clay.Color{111, 173, 162, 255}
COLOR_TEAL :: clay.Color{111, 173, 162, 255}
COLOR_BLUE_DARK :: clay.Color{2, 32, 82, 255}

// Colors for top stripe
COLOR_TOP_BORDER_1 :: clay.Color{168, 66, 28, 255}
COLOR_TOP_BORDER_2 :: clay.Color{223, 110, 44, 255}
COLOR_TOP_BORDER_3 :: clay.Color{225, 138, 50, 255}
COLOR_TOP_BORDER_4 :: clay.Color{236, 189, 80, 255}
COLOR_TOP_BORDER_5 :: clay.Color{240, 213, 137, 255}

COLOR_BLOB_BORDER_1 :: clay.Color{168, 66, 28, 255}
COLOR_BLOB_BORDER_2 :: clay.Color{203, 100, 44, 255}
COLOR_BLOB_BORDER_3 :: clay.Color{225, 138, 50, 255}
COLOR_BLOB_BORDER_4 :: clay.Color{236, 159, 70, 255}
COLOR_BLOB_BORDER_5 :: clay.Color{240, 189, 100, 255}

headerTextConfig := clay.TextElementConfig {
    fontId    = FONT_ID_BODY_24,
    fontSize  = 24,
    textColor = {61, 26, 5, 255},
}

border2pxRed := clay.BorderElementConfig {
    width = { 2, 2, 2, 2, 0 },
    color = COLOR_RED
}

LandingPageBlob :: proc(index: u32, fontSize: u16, fontId: u16, color: clay.Color, $text: string, image: ^raylib.Texture2D) {
    if clay.UI()({
        id = clay.ID("HeroBlob", index),
        layout = { sizing = { width = clay.SizingGrow({ max = 480 }) }, padding = clay.PaddingAll(16), childGap = 16, childAlignment = clay.ChildAlignment{ y = .Center } },
        border = border2pxRed,
        cornerRadius = clay.CornerRadiusAll(10)
    }) {
        if clay.UI()({
            id = clay.ID("CheckImage", index),
            layout = { sizing = { width = clay.SizingFixed(32) } },
            aspectRatio = { 1.0 },
            image = { imageData = image },
        }) {}
        clay.Text(text, clay.TextConfig({fontSize = fontSize, fontId = fontId, textColor = color}))
    }
}

LandingPageDesktop :: proc() {
    if clay.UI()({
        id = clay.ID("LandingPage1Desktop"),
        layout = { sizing = { width = clay.SizingGrow(), height = clay.SizingFit({ min = cast(f32)windowHeight - 70 }) }, childAlignment = { y = .Center }, padding = { left = 50, right = 50 } },
    }) {
        if clay.UI()({
            id = clay.ID("LandingPage1"),
            layout = { sizing = { clay.SizingGrow(), clay.SizingGrow() }, childAlignment = { y = .Center }, padding = clay.PaddingAll(32), childGap = 32 },
            border = { COLOR_RED, { left = 2, right = 2 } },
        }) {
            if clay.UI()({ id = clay.ID("LeftText"), layout = { sizing = { width = clay.SizingPercent(0.55) }, layoutDirection = .TopToBottom, childGap = 8 } }) {
                clay.Text(
                    "Clay is a flex-box style UI auto layout library in C, with declarative syntax and microsecond performance.",
                    clay.TextConfig({fontSize = 56, fontId = FONT_ID_TITLE_56, textColor = COLOR_RED}),
                )
                if clay.UI()({ layout = { sizing = { width = clay.SizingGrow({}), height = clay.SizingFixed(32) } } }) {}
                clay.Text(
                    "Clay is laying out this webpage right now!",
                    clay.TextConfig({fontSize = 36, fontId = FONT_ID_TITLE_36, textColor = COLOR_ORANGE}),
                )
            }
            if clay.UI()({
                id = clay.ID("HeroImageOuter"),
                layout = { layoutDirection = .TopToBottom, sizing = { width = clay.SizingPercent(0.45) }, childAlignment = { x = .Center }, childGap = 16 },
            }) {
                LandingPageBlob(1, 30, FONT_ID_BODY_30, COLOR_BLOB_BORDER_5, "High performance", &checkImage5)
                LandingPageBlob(2, 30, FONT_ID_BODY_30, COLOR_BLOB_BORDER_4, "Flexbox-style responsive layout", &checkImage4)
                LandingPageBlob(3, 30, FONT_ID_BODY_30, COLOR_BLOB_BORDER_3, "Declarative syntax", &checkImage3)
                LandingPageBlob(4, 30, FONT_ID_BODY_30, COLOR_BLOB_BORDER_2, "Single .h file for C/C++", &checkImage2)
                LandingPageBlob(5, 30, FONT_ID_BODY_30, COLOR_BLOB_BORDER_1, "Compile to 15kb .wasm", &checkImage1)
            }
        }
    }
}

LandingPageMobile :: proc() {
    if clay.UI()({
        id = clay.ID("LandingPage1Mobile"),
        layout = {
            layoutDirection = .TopToBottom,
            sizing = { width = clay.SizingGrow(), height = clay.SizingFit({ min = cast(f32)windowHeight - 70 }) },
            childAlignment = { x = .Center, y = .Center },
            padding = { 16, 16, 32, 32 },
            childGap = 32,
        },
    }) {
        if clay.UI()({ id = clay.ID("LeftText"), layout = { sizing = { width = clay.SizingGrow() }, layoutDirection = .TopToBottom, childGap = 8 } }) {
            clay.Text(
                "Clay is a flex-box style UI auto layout library in C, with declarative syntax and microsecond performance.",
                clay.TextConfig({fontSize = 48, fontId = FONT_ID_TITLE_48, textColor = COLOR_RED}),
            )
            if clay.UI()({ layout = { sizing = { width = clay.SizingGrow({}), height = clay.SizingFixed(32) } } }) {}
            clay.Text(
                "Clay is laying out this webpage right now!",
                clay.TextConfig({fontSize = 32, fontId = FONT_ID_TITLE_32, textColor = COLOR_ORANGE}),
            )
        }
        if clay.UI()({
            id = clay.ID("HeroImageOuter"),
            layout = { layoutDirection = .TopToBottom, sizing = { width = clay.SizingGrow() }, childAlignment = { x = .Center }, childGap = 16 },
        }) {
            LandingPageBlob(1, 24, FONT_ID_BODY_24, COLOR_BLOB_BORDER_5, "High performance", &checkImage5)
            LandingPageBlob(2, 24, FONT_ID_BODY_24, COLOR_BLOB_BORDER_4, "Flexbox-style responsive layout", &checkImage4)
            LandingPageBlob(3, 24, FONT_ID_BODY_24, COLOR_BLOB_BORDER_3, "Declarative syntax", &checkImage3)
            LandingPageBlob(4, 24, FONT_ID_BODY_24, COLOR_BLOB_BORDER_2, "Single .h file for C/C++", &checkImage2)
            LandingPageBlob(5, 24, FONT_ID_BODY_24, COLOR_BLOB_BORDER_1, "Compile to 15kb .wasm", &checkImage1)
        }
    }
}

FeatureBlocks :: proc(widthSizing: clay.SizingAxis, outerPadding: u16) {
    textConfig := clay.TextConfig({fontSize = 24, fontId = FONT_ID_BODY_24, textColor = COLOR_RED})
    if clay.UI()({
        id = clay.ID("HFileBoxOuter"),
        layout = { layoutDirection = .TopToBottom, sizing = { width = widthSizing }, childAlignment = { y = .Center }, padding = { outerPadding, outerPadding, 32, 32 }, childGap = 8 },
    }) {
        if clay.UI()({ id = clay.ID("HFileIncludeOuter"), layout = { padding = { 8, 8, 4, 4 } }, backgroundColor = COLOR_RED, cornerRadius = clay.CornerRadiusAll(8) }) {
            clay.Text("#include clay.h", clay.TextConfig({fontSize = 24, fontId = FONT_ID_BODY_24, textColor = COLOR_LIGHT}))
        }
        clay.Text("~2000 lines of C99.", textConfig)
        clay.Text("Zero dependencies, including no C standard library.", textConfig)
    }
    if clay.UI()({
        id = clay.ID("BringYourOwnRendererOuter"),
        layout = { layoutDirection = .TopToBottom, sizing = { width = widthSizing }, childAlignment = { y = .Center }, padding = { outerPadding, outerPadding, 32, 32 }, childGap = 8 },
    }) {
        clay.Text("Renderer agnostic.", clay.TextConfig({fontId = FONT_ID_BODY_24, fontSize = 24, textColor = COLOR_ORANGE}))
        clay.Text("Layout with clay, then render with Raylib, WebGL Canvas or even as HTML.", textConfig)
        clay.Text("Flexible output for easy compositing in your custom engine or environment.", textConfig)
    }
}

FeatureBlocksDesktop :: proc() {
    if clay.UI()({ id = clay.ID("FeatureBlocksOuter"), layout = { sizing = { width = clay.SizingGrow({}) } } }) {
        if clay.UI()({
            id = clay.ID("FeatureBlocksInner"),
            layout = { sizing = { width = clay.SizingGrow() }, childAlignment = { y = .Center } },
            border = { width = { betweenChildren = 2}, color = COLOR_RED },
        }) {
            FeatureBlocks(clay.SizingPercent(0.5), 50)
        }
    }
}

FeatureBlocksMobile :: proc() {
    if clay.UI()({
        id = clay.ID("FeatureBlocksInner"),
        layout = { layoutDirection = .TopToBottom, sizing = { width = clay.SizingGrow() } },
        border = { width = { betweenChildren = 2}, color = COLOR_RED },
    }) {
        FeatureBlocks(clay.SizingGrow({}), 16)
    }
}

DeclarativeSyntaxPage :: proc(titleTextConfig: clay.TextElementConfig, widthSizing: clay.SizingAxis) {
    if clay.UI()({ id = clay.ID("SyntaxPageLeftText"), layout = { sizing = { width = widthSizing }, layoutDirection = .TopToBottom, childGap = 8 } }) {
        clay.Text("Declarative Syntax", clay.TextConfig(titleTextConfig))
        if clay.UI()({ id = clay.ID("SyntaxSpacer"), layout = { sizing = { width = clay.SizingGrow({ max = 16 }) } } }) {}
        clay.Text(
            "Flexible and readable declarative syntax with nested UI element hierarchies.",
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_28, textColor = COLOR_RED}),
        )
        clay.Text(
            "Mix elements with standard C code like loops, conditionals and functions.",
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_28, textColor = COLOR_RED}),
        )
        clay.Text(
            "Create your own library of re-usable components from UI primitives like text, images and rectangles.",
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_28, textColor = COLOR_RED}),
        )
    }
    if clay.UI()({ id = clay.ID("SyntaxPageRightImage"), layout = { sizing = { width = widthSizing }, childAlignment = { x = .Center } } }) {
        if clay.UI()({
            id = clay.ID("SyntaxPageRightImageInner"),
            layout = { sizing = { width = clay.SizingGrow({ max = 568 }) } },
            aspectRatio = { 1136.0 / 1194.0 },
            image = { imageData = &syntaxImage },
        }) {}
    }
}

DeclarativeSyntaxPageDesktop :: proc() {
    if clay.UI()({
        id = clay.ID("SyntaxPageDesktop"),
        layout = { sizing = { clay.SizingGrow(), clay.SizingFit({ min = cast(f32)windowHeight - 50 }) }, childAlignment = { y = .Center }, padding = { left = 50, right = 50 } },
    }) {
        if clay.UI()({
            id = clay.ID("SyntaxPage"),
            layout = { sizing = { clay.SizingGrow(), clay.SizingGrow() }, childAlignment = { y = .Center }, padding = clay.PaddingAll(32), childGap = 32 },
            border = border2pxRed,
        }) {
            DeclarativeSyntaxPage({fontSize = 52, fontId = FONT_ID_TITLE_52, textColor = COLOR_RED}, clay.SizingPercent(0.5))
        }
    }
}

DeclarativeSyntaxPageMobile :: proc() {
    if clay.UI()({
        id = clay.ID("SyntaxPageMobile"),
        layout = {
            layoutDirection = .TopToBottom,
            sizing = { clay.SizingGrow(), clay.SizingFit({ min = cast(f32)windowHeight - 50 }) },
            childAlignment = { x = .Center, y = .Center },
            padding = { 16, 16, 32, 32 },
            childGap = 16,
        },
    }) {
        DeclarativeSyntaxPage({fontSize = 48, fontId = FONT_ID_TITLE_48, textColor = COLOR_RED}, clay.SizingGrow({}))
    }
}

ColorLerp :: proc(a: clay.Color, b: clay.Color, amount: f32) -> clay.Color {
    return clay.Color{a.r + (b.r - a.r) * amount, a.g + (b.g - a.g) * amount, a.b + (b.b - a.b) * amount, a.a + (b.a - a.a) * amount}
}

LOREM_IPSUM_TEXT :: "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."

HighPerformancePage :: proc(lerpValue: f32, titleTextConfig: clay.TextElementConfig, widthSizing: clay.SizingAxis) {
    if clay.UI()({ id = clay.ID("PerformanceLeftText"), layout = { sizing = { width = widthSizing }, layoutDirection = .TopToBottom, childGap = 8 } }) {
        clay.Text("High Performance", clay.TextConfig(titleTextConfig))
        if clay.UI()({ layout = { sizing = { width = clay.SizingGrow({ max = 16 }) } }}) {}
        clay.Text(
            "Fast enough to recompute your entire UI every frame.",
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_LIGHT}),
        )
        clay.Text(
            "Small memory footprint (3.5mb default) with static allocation & reuse. No malloc / free.",
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_LIGHT}),
        )
        clay.Text(
            "Simplify animations and reactive UI design by avoiding the standard performance hacks.",
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_LIGHT}),
        )
    }
    if clay.UI()({ id = clay.ID("PerformanceRightImageOuter"), layout = { sizing = { width = widthSizing }, childAlignment = { x = .Center } } }) {
        if clay.UI()({
            id = clay.ID("PerformanceRightBorder"),
            layout = { sizing = { clay.SizingGrow(), clay.SizingFixed(400) } },
            border = {  COLOR_LIGHT, {2, 2, 2, 2, 2} },
        }) {
            if clay.UI()({
                id = clay.ID("AnimationDemoContainerLeft"),
                layout = { sizing = { clay.SizingPercent(0.35 + 0.3 * lerpValue), clay.SizingGrow() }, childAlignment = { y = .Center }, padding = clay.PaddingAll(16) },
                backgroundColor = ColorLerp(COLOR_RED, COLOR_ORANGE, lerpValue),
            }) {
                clay.Text(LOREM_IPSUM_TEXT, clay.TextConfig({fontSize = 16, fontId = FONT_ID_BODY_16, textColor = COLOR_LIGHT}))
            }
            if clay.UI()({
                id = clay.ID("AnimationDemoContainerRight"),
                layout = { sizing = { clay.SizingGrow(), clay.SizingGrow() }, childAlignment = { y = .Center }, padding = clay.PaddingAll(16) },
                backgroundColor = ColorLerp(COLOR_ORANGE, COLOR_RED, lerpValue),
            }) {
                clay.Text(LOREM_IPSUM_TEXT, clay.TextConfig({fontSize = 16, fontId = FONT_ID_BODY_16, textColor = COLOR_LIGHT}))
            }
        }
    }
}

HighPerformancePageDesktop :: proc(lerpValue: f32) {
    if clay.UI()({
        id = clay.ID("PerformanceDesktop"),
        layout = { sizing = { clay.SizingGrow(), clay.SizingFit({ min = cast(f32)windowHeight - 50 }) }, childAlignment = { y = .Center }, padding = { 82, 82, 32, 32 }, childGap = 64 },
        backgroundColor = COLOR_RED,
    }) {
        HighPerformancePage(lerpValue, {fontSize = 52, fontId = FONT_ID_TITLE_52, textColor = COLOR_LIGHT}, clay.SizingPercent(0.5))
    }
}

HighPerformancePageMobile :: proc(lerpValue: f32) {
    if clay.UI()({
        id = clay.ID("PerformanceMobile"),
        layout = {
            layoutDirection = .TopToBottom,
            sizing = { clay.SizingGrow(), clay.SizingFit({ min = cast(f32)windowHeight - 50 }) },
            childAlignment = { x = .Center, y = .Center },
            padding = { 16, 16, 32, 32 },
            childGap = 32,
        },
        backgroundColor = COLOR_RED,
    }) {
        HighPerformancePage(lerpValue, {fontSize = 48, fontId = FONT_ID_TITLE_48, textColor = COLOR_LIGHT}, clay.SizingGrow({}))
    }
}

RendererButtonActive :: proc(index: i32, $text: string) {
    if clay.UI()({
        layout = { sizing = { width = clay.SizingFixed(300) }, padding = clay.PaddingAll(16) },
        backgroundColor = COLOR_RED,
        cornerRadius = clay.CornerRadiusAll(10)
    }) {
        clay.Text(text, clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_28, textColor = COLOR_LIGHT}))
    }
}

RendererButtonInactive :: proc(index: u32, $text: string) {
    if clay.UI()({ border = border2pxRed }) {
        if clay.UI()({
            id = clay.ID("RendererButtonInactiveInner", index),
            layout = { sizing = { width = clay.SizingFixed(300) }, padding = clay.PaddingAll(16) },
            backgroundColor = COLOR_LIGHT,
            cornerRadius = clay.CornerRadiusAll(10)
        }) {
            clay.Text(text, clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_28, textColor = COLOR_RED}))
        }
    }
}

RendererPage :: proc(titleTextConfig: clay.TextElementConfig, widthSizing: clay.SizingAxis) {
    if clay.UI()({ id = clay.ID("RendererLeftText"), layout = { sizing = { width = widthSizing }, layoutDirection = .TopToBottom, childGap = 8 } }) {
        clay.Text("Renderer & Platform Agnostic", clay.TextConfig(titleTextConfig))
        if clay.UI()({ layout = { sizing = { width = clay.SizingGrow({ max = 16 }) } } }) {}
        clay.Text(
            "Clay outputs a sorted array of primitive render commands, such as RECTANGLE, TEXT or IMAGE.",
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_RED}),
        )
        clay.Text(
            "Write your own renderer in a few hundred lines of code, or use the provided examples for Raylib, WebGL canvas and more.",
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_RED}),
        )
        clay.Text(
            "There's even an HTML renderer - you're looking at it right now!",
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_RED}),
        )
    }
    if clay.UI()({
        id = clay.ID("RendererRightText"),
        layout = { sizing = { width = widthSizing }, childAlignment = { x = .Center }, layoutDirection = .TopToBottom, childGap = 16 },
    }) {
        clay.Text("Try changing renderer!", clay.TextConfig({fontSize = 36, fontId = FONT_ID_BODY_36, textColor = COLOR_ORANGE}))
        if clay.UI()({ layout = { sizing = { width = clay.SizingGrow({ max = 32 }) } } }) {}
        RendererButtonActive(0, "Raylib Renderer")
    }
}

RendererPageDesktop :: proc() {
    if clay.UI()({
        id = clay.ID("RendererPageDesktop"),
        layout = { sizing = { clay.SizingGrow(), clay.SizingFit({ min = cast(f32)windowHeight - 50 }) }, childAlignment = { y = .Center }, padding = { left = 50, right = 50 } },
    }) {
        if clay.UI()({
            id = clay.ID("RendererPage"),
            layout = { sizing = { clay.SizingGrow(), clay.SizingGrow() }, childAlignment = { y = .Center }, padding = clay.PaddingAll(32), childGap = 32 },
            border = { COLOR_RED, { left = 2, right = 2 } },
        }) {
            RendererPage({fontSize = 52, fontId = FONT_ID_TITLE_52, textColor = COLOR_RED}, clay.SizingPercent(0.5))
        }
    }
}

RendererPageMobile :: proc() {
    if clay.UI()({
        id = clay.ID("RendererMobile"),
        layout = {
            layoutDirection = .TopToBottom,
            sizing = { clay.SizingGrow(), clay.SizingFit({ min = cast(f32)windowHeight - 50 }) },
            childAlignment = { x = .Center, y = .Center },
            padding = { 16, 16, 32, 32 },
            childGap = 32,
        },
        backgroundColor = COLOR_LIGHT,
    }) {
        RendererPage({fontSize = 48, fontId = FONT_ID_TITLE_48, textColor = COLOR_RED}, clay.SizingGrow({}))
    }
}

ScrollbarData :: struct {
    clickOrigin:    clay.Vector2,
    positionOrigin: clay.Vector2,
    mouseDown:      bool,
}

scrollbarData := ScrollbarData{}
animationLerpValue: f32 = -1.0

createLayout :: proc(lerpValue: f32) -> clay.ClayArray(clay.RenderCommand) {
    mobileScreen := windowWidth < 750
    clay.BeginLayout()
    if clay.UI()({
        id = clay.ID("OuterContainer"),
        layout = { layoutDirection = .TopToBottom, sizing = { clay.SizingGrow(), clay.SizingGrow() } },
        backgroundColor = COLOR_LIGHT,
    }) {
        if clay.UI()({
            id = clay.ID("Header"),
            layout = { sizing = { clay.SizingGrow(), clay.SizingFixed(50) }, childAlignment = { y = .Center }, childGap = 24, padding = { left = 32, right = 32 } },
        }) {
            clay.Text("Clay", &headerTextConfig)
            if clay.UI()({ layout = { sizing = { width = clay.SizingGrow() } } }) {}

            if (!mobileScreen) {
                if clay.UI()({ id = clay.ID("LinkExamplesOuter"), backgroundColor = {0, 0, 0, 0} }) {
                    clay.Text("Examples", clay.TextConfig({fontId = FONT_ID_BODY_24, fontSize = 24, textColor = {61, 26, 5, 255}}))
                }
                if clay.UI()({ id = clay.ID("LinkDocsOuter"), backgroundColor = {0, 0, 0, 0} }) {
                    clay.Text("Docs", clay.TextConfig({fontId = FONT_ID_BODY_24, fontSize = 24, textColor = {61, 26, 5, 255}}))
                }
            }
            if clay.UI()({
                id = clay.ID("LinkGithubOuter"),
                layout = { padding = { 16, 16, 6, 6 } },
                border = border2pxRed,
                backgroundColor = clay.Hovered() ? COLOR_LIGHT_HOVER : COLOR_LIGHT,
                cornerRadius = clay.CornerRadiusAll(10)
            }) {
                clay.Text("Github", clay.TextConfig({fontId = FONT_ID_BODY_24, fontSize = 24, textColor = {61, 26, 5, 255}}))
            }
        }
        if clay.UI()({ id = clay.ID("TopBorder1"), layout = { sizing = { clay.SizingGrow(), clay.SizingFixed(4) } }, backgroundColor = COLOR_TOP_BORDER_5 } ) {}
        if clay.UI()({ id = clay.ID("TopBorder2"), layout = { sizing = { clay.SizingGrow(), clay.SizingFixed(4) } }, backgroundColor = COLOR_TOP_BORDER_4 } ) {}
        if clay.UI()({ id = clay.ID("TopBorder3"), layout = { sizing = { clay.SizingGrow(), clay.SizingFixed(4) } }, backgroundColor = COLOR_TOP_BORDER_3 } ) {}
        if clay.UI()({ id = clay.ID("TopBorder4"), layout = { sizing = { clay.SizingGrow(), clay.SizingFixed(4) } }, backgroundColor = COLOR_TOP_BORDER_2 } ) {}
        if clay.UI()({ id = clay.ID("TopBorder5"), layout = { sizing = { clay.SizingGrow(), clay.SizingFixed(4) } }, backgroundColor = COLOR_TOP_BORDER_1 } ) {}
        if clay.UI()({
            id = clay.ID("ScrollContainerBackgroundRectangle"),
            clip = { vertical = true, childOffset = clay.GetScrollOffset() },
            layout = { sizing = { clay.SizingGrow(), clay.SizingGrow() }, layoutDirection = clay.LayoutDirection.TopToBottom },
            backgroundColor = COLOR_LIGHT,
            border = { COLOR_RED, { betweenChildren = 2} },
        }) {
            if (!mobileScreen) {
                LandingPageDesktop()
                FeatureBlocksDesktop()
                DeclarativeSyntaxPageDesktop()
                HighPerformancePageDesktop(lerpValue)
                RendererPageDesktop()
            } else {
                LandingPageMobile()
                FeatureBlocksMobile()
                DeclarativeSyntaxPageMobile()
                HighPerformancePageMobile(lerpValue)
                RendererPageMobile()
            }
        }
    }
    return clay.EndLayout()
}

loadFont :: proc(fontId: u16, fontSize: u16, path: cstring) {
    assign_at(&raylib_fonts,fontId,Raylib_Font{
        font   = raylib.LoadFontEx(path, cast(i32)fontSize * 2, nil, 0),
        fontId = cast(u16)fontId,
    })
    raylib.SetTextureFilter(raylib_fonts[fontId].font.texture, raylib.TextureFilter.TRILINEAR)
}

errorHandler :: proc "c" (errorData: clay.ErrorData) {
    if (errorData.errorType == clay.ErrorType.DuplicateId) {
        // etc
    }
}

main :: proc() {
    minMemorySize: c.size_t = cast(c.size_t)clay.MinMemorySize()
    memory := make([^]u8, minMemorySize)
    arena: clay.Arena = clay.CreateArenaWithCapacityAndMemory(minMemorySize, memory)
    clay.Initialize(arena, {cast(f32)raylib.GetScreenWidth(), cast(f32)raylib.GetScreenHeight()}, { handler = errorHandler })
    clay.SetMeasureTextFunction(measure_text, nil)

    raylib.SetConfigFlags({.VSYNC_HINT, .WINDOW_RESIZABLE, .MSAA_4X_HINT})
    raylib.InitWindow(windowWidth, windowHeight, "Raylib Odin Example")
    raylib.SetTargetFPS(raylib.GetMonitorRefreshRate(0))
    loadFont(FONT_ID_TITLE_56, 56, "resources/Calistoga-Regular.ttf")
    loadFont(FONT_ID_TITLE_52, 52, "resources/Calistoga-Regular.ttf")
    loadFont(FONT_ID_TITLE_48, 48, "resources/Calistoga-Regular.ttf")
    loadFont(FONT_ID_TITLE_36, 36, "resources/Calistoga-Regular.ttf")
    loadFont(FONT_ID_TITLE_32, 32, "resources/Calistoga-Regular.ttf")
    loadFont(FONT_ID_BODY_36, 36, "resources/Quicksand-Semibold.ttf")
    loadFont(FONT_ID_BODY_30, 30, "resources/Quicksand-Semibold.ttf")
    loadFont(FONT_ID_BODY_28, 28, "resources/Quicksand-Semibold.ttf")
    loadFont(FONT_ID_BODY_24, 24, "resources/Quicksand-Semibold.ttf")
    loadFont(FONT_ID_BODY_16, 16, "resources/Quicksand-Semibold.ttf")

    syntaxImage = raylib.LoadTexture("resources/declarative.png")
    checkImage1 = raylib.LoadTexture("resources/check_1.png")
    checkImage2 = raylib.LoadTexture("resources/check_2.png")
    checkImage3 = raylib.LoadTexture("resources/check_3.png")
    checkImage4 = raylib.LoadTexture("resources/check_4.png")
    checkImage5 = raylib.LoadTexture("resources/check_5.png")

    debugModeEnabled: bool = false

    for !raylib.WindowShouldClose() {
        defer free_all(context.temp_allocator)

        animationLerpValue += raylib.GetFrameTime()
        if animationLerpValue > 1 {
            animationLerpValue = animationLerpValue - 2
        }
        windowWidth = raylib.GetScreenWidth()
        windowHeight = raylib.GetScreenHeight()
        if (raylib.IsKeyPressed(.D)) {
            debugModeEnabled = !debugModeEnabled
            clay.SetDebugModeEnabled(debugModeEnabled)
        }
        clay.SetPointerState(transmute(clay.Vector2)raylib.GetMousePosition(), raylib.IsMouseButtonDown(raylib.MouseButton.LEFT))
        clay.UpdateScrollContainers(false, transmute(clay.Vector2)raylib.GetMouseWheelMoveV(), raylib.GetFrameTime())
        clay.SetLayoutDimensions({cast(f32)raylib.GetScreenWidth(), cast(f32)raylib.GetScreenHeight()})
        renderCommands: clay.ClayArray(clay.RenderCommand) = createLayout(animationLerpValue < 0 ? (animationLerpValue + 1) : (1 - animationLerpValue))
        raylib.BeginDrawing()
        clay_raylib_render(&renderCommands)
        raylib.EndDrawing()
    }
}
