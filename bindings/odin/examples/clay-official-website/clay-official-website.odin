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

FONT_ID_TITLE_56 :: 0
FONT_ID_TITLE_52 :: 1
FONT_ID_TITLE_48 :: 2
FONT_ID_TITLE_36 :: 3
FONT_ID_TITLE_32 :: 4
FONT_ID_BODY_36 :: 5
FONT_ID_BODY_30 :: 6
FONT_ID_BODY_28 :: 7
FONT_ID_BODY_24 :: 8
FONT_ID_BODY_16 :: 9

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

LandingPageBlob :: proc(index: u32, fontSize: u16, fontId: u16, color: clay.Color, text: clay.String, image: ^raylib.Texture2D) {
    if clay.Border(
        clay.IDI("HeroBlob", index),
        clay.Layout({sizing = {width = clay.SizingGrow({max = 480})}, padding = clay.Padding{16, 16}, childGap = 16, childAlignment = clay.ChildAlignment{y = .CENTER}}),
        clay.BorderConfigOutsideRadius({2, color}, 10),
    ) {
        if clay.Image(
            clay.IDI("CheckImage", index),
            clay.Layout({sizing = {width = clay.SizingFixed(32)}}),
            clay.ImageConfig({imageData = image, sourceDimensions = {128, 128}}),
        ) {}
        clay.Text(clay.IDI("HeroBlobText", index), text, clay.TextConfig({fontSize = fontSize, fontId = fontId, textColor = color}))
    }
}

LandingPageDesktop :: proc() {
    if clay.Container(
        clay.ID("LandingPage1Desktop"),
        clay.Layout({sizing = {width = clay.SizingGrow({}), height = clay.SizingFit({min = cast(f32)windowHeight - 70})}, childAlignment = {y = .CENTER}, padding = {x = 50}}),
    ) {
        if clay.Border(
            clay.ID("LandingPage1"),
            clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingGrow({})}, childAlignment = {y = .CENTER}, padding = {32, 32}, childGap = 32}),
            clay.BorderConfig({left = {2, COLOR_RED}, right = {2, COLOR_RED}}),
        ) {
            if clay.Container(clay.ID("LeftText"), clay.Layout({sizing = {width = clay.SizingPercent(0.55)}, layoutDirection = .TOP_TO_BOTTOM, childGap = 8})) {
                clay.Text(
                    clay.ID("LeftTextTitle"),
                    clay.MakeString("Clay is a flex-box style UI auto layout library in C, with declarative syntax and microsecond performance."),
                    clay.TextConfig({fontSize = 56, fontId = FONT_ID_TITLE_56, textColor = COLOR_RED}),
                )
                if clay.Container(clay.ID("Spacer"), clay.Layout({sizing = {width = clay.SizingGrow({}), height = clay.SizingFixed(32)}})) {}
                clay.Text(
                    clay.ID("LeftTextTagline"),
                    clay.MakeString("Clay is laying out this webpage right now!"),
                    clay.TextConfig({fontSize = 36, fontId = FONT_ID_TITLE_36, textColor = COLOR_ORANGE}),
                )
            }
            if clay.Container(
                clay.ID("HeroImageOuter"),
                clay.Layout({layoutDirection = .TOP_TO_BOTTOM, sizing = {width = clay.SizingPercent(0.45)}, childAlignment = {x = .CENTER}, childGap = 16}),
            ) {
                LandingPageBlob(1, 30, FONT_ID_BODY_30, COLOR_BLOB_BORDER_5, clay.MakeString("High performance"), &checkImage5)
                LandingPageBlob(2, 30, FONT_ID_BODY_30, COLOR_BLOB_BORDER_4, clay.MakeString("Flexbox-style responsive layout"), &checkImage4)
                LandingPageBlob(3, 30, FONT_ID_BODY_30, COLOR_BLOB_BORDER_3, clay.MakeString("Declarative syntax"), &checkImage3)
                LandingPageBlob(4, 30, FONT_ID_BODY_30, COLOR_BLOB_BORDER_2, clay.MakeString("Single .h file for C/C++"), &checkImage2)
                LandingPageBlob(5, 30, FONT_ID_BODY_30, COLOR_BLOB_BORDER_1, clay.MakeString("Compile to 15kb .wasm"), &checkImage1)
            }
        }
    }
}

LandingPageMobile :: proc() {
    if clay.Container(
        clay.ID("LandingPage1Mobile"),
        clay.Layout(
            {
                layoutDirection = .TOP_TO_BOTTOM,
                sizing = {width = clay.SizingGrow({}), height = clay.SizingFit({min = cast(f32)windowHeight - 70})},
                childAlignment = {x = .CENTER, y = .CENTER},
                padding = {16, 32},
                childGap = 32,
            },
        ),
    ) {
        if clay.Container(clay.ID("LeftText"), clay.Layout({sizing = {width = clay.SizingGrow({})}, layoutDirection = .TOP_TO_BOTTOM, childGap = 8})) {
            clay.Text(
                clay.ID("LeftTextTitle"),
                clay.MakeString("Clay is a flex-box style UI auto layout library in C, with declarative syntax and microsecond performance."),
                clay.TextConfig({fontSize = 48, fontId = FONT_ID_TITLE_48, textColor = COLOR_RED}),
            )
            if clay.Container(clay.ID("Spacer"), clay.Layout({sizing = {width = clay.SizingGrow({}), height = clay.SizingFixed(32)}})) {}
            clay.Text(
                clay.ID("LeftTextTagline"),
                clay.MakeString("Clay is laying out this webpage right now!"),
                clay.TextConfig({fontSize = 32, fontId = FONT_ID_TITLE_32, textColor = COLOR_ORANGE}),
            )
        }
        if clay.Container(
            clay.ID("HeroImageOuter"),
            clay.Layout({layoutDirection = .TOP_TO_BOTTOM, sizing = {width = clay.SizingGrow({})}, childAlignment = {x = .CENTER}, childGap = 16}),
        ) {
            LandingPageBlob(1, 24, FONT_ID_BODY_24, COLOR_BLOB_BORDER_5, clay.MakeString("High performance"), &checkImage5)
            LandingPageBlob(2, 24, FONT_ID_BODY_24, COLOR_BLOB_BORDER_4, clay.MakeString("Flexbox-style responsive layout"), &checkImage4)
            LandingPageBlob(3, 24, FONT_ID_BODY_24, COLOR_BLOB_BORDER_3, clay.MakeString("Declarative syntax"), &checkImage3)
            LandingPageBlob(4, 24, FONT_ID_BODY_24, COLOR_BLOB_BORDER_2, clay.MakeString("Single .h file for C/C++"), &checkImage2)
            LandingPageBlob(5, 24, FONT_ID_BODY_24, COLOR_BLOB_BORDER_1, clay.MakeString("Compile to 15kb .wasm"), &checkImage1)
        }
    }
}

FeatureBlocks :: proc(widthSizing: clay.SizingAxis, outerPadding: u16) {
    textConfig := clay.TextConfig({fontSize = 24, fontId = FONT_ID_BODY_24, textColor = COLOR_RED})
    if clay.Container(
        clay.ID("HFileBoxOuter"),
        clay.Layout({layoutDirection = .TOP_TO_BOTTOM, sizing = {width = widthSizing}, childAlignment = {y = .CENTER}, padding = {outerPadding, 32}, childGap = 8}),
    ) {
        if clay.Rectangle(clay.ID("HFileIncludeOuter"), clay.Layout({padding = {8, 4}}), clay.RectangleConfig({color = COLOR_RED, cornerRadius = clay.CornerRadiusAll(8)})) {
            clay.Text(clay.IDI("HFileBoxText", 2), clay.MakeString("#include clay.h"), clay.TextConfig({fontSize = 24, fontId = FONT_ID_BODY_24, textColor = COLOR_LIGHT}))
        }
        clay.Text(clay.ID("HFileSecondLine"), clay.MakeString("~2000 lines of C99."), textConfig)
        clay.Text(clay.IDI("HFileBoxText", 5), clay.MakeString("Zero dependencies, including no C standard library."), textConfig)
    }
    if clay.Container(
        clay.ID("BringYourOwnRendererOuter"),
        clay.Layout({layoutDirection = .TOP_TO_BOTTOM, sizing = {width = widthSizing}, childAlignment = {y = .CENTER}, padding = {x = outerPadding, y = 32}, childGap = 8}),
    ) {
        clay.Text(clay.IDI("ZeroDependenciesText", 1), clay.MakeString("Renderer agnostic."), clay.TextConfig({fontId = FONT_ID_BODY_24, fontSize = 24, textColor = COLOR_ORANGE}))
        clay.Text(clay.IDI("ZeroDependenciesText", 2), clay.MakeString("Layout with clay, then render with Raylib, WebGL Canvas or even as HTML."), textConfig)
        clay.Text(clay.IDI("ZeroDependenciesText", 3), clay.MakeString("Flexible output for easy compositing in your custom engine or environment."), textConfig)
    }
}

FeatureBlocksDesktop :: proc() {
    if clay.Container(clay.ID("FeatureBlocksOuter"), clay.Layout({sizing = {width = clay.SizingGrow({})}})) {
        if clay.Border(
            clay.ID("FeatureBlocksInner"),
            clay.Layout({sizing = {width = clay.SizingGrow({})}, childAlignment = {y = .CENTER}}),
            clay.BorderConfig({betweenChildren = {width = 2, color = COLOR_RED}}),
        ) {
            FeatureBlocks(clay.SizingPercent(0.5), 50)
        }
    }
}

FeatureBlocksMobile :: proc() {
    if clay.Border(
        clay.ID("FeatureBlocksInner"),
        clay.Layout({layoutDirection = .TOP_TO_BOTTOM, sizing = {width = clay.SizingGrow({})}}),
        clay.BorderConfig({betweenChildren = {width = 2, color = COLOR_RED}}),
    ) {
        FeatureBlocks(clay.SizingGrow({}), 16)
    }
}

DeclarativeSyntaxPage :: proc(titleTextConfig: clay.TextElementConfig, widthSizing: clay.SizingAxis) {
    if clay.Container(clay.ID("SyntaxPageLeftText"), clay.Layout({sizing = {width = widthSizing}, layoutDirection = .TOP_TO_BOTTOM, childGap = 8})) {
        clay.Text(clay.ID("SyntaxPageTextTitle"), clay.MakeString("Declarative Syntax"), clay.TextConfig(titleTextConfig))
        if clay.Container(clay.ID("SyntaxSpacer"), clay.Layout({sizing = {width = clay.SizingGrow({max = 16})}})) {}
        clay.Text(
            clay.ID("SyntaxPageTextSubTitle1"),
            clay.MakeString("Flexible and readable declarative syntax with nested UI element hierarchies."),
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_28, textColor = COLOR_RED}),
        )
        clay.Text(
            clay.ID("SyntaxPageTextSubTitle2"),
            clay.MakeString("Mix elements with standard C code like loops, conditionals and functions."),
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_28, textColor = COLOR_RED}),
        )
        clay.Text(
            clay.ID("SyntaxPageTextSubTitle3"),
            clay.MakeString("Create your own library of re-usable components from UI primitives like text, images and rectangles."),
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_28, textColor = COLOR_RED}),
        )
    }
    if clay.Container(clay.ID("SyntaxPageRightImage"), clay.Layout({sizing = {width = widthSizing}, childAlignment = {x = .CENTER}})) {
        if clay.Image(
            clay.ID("SyntaxPageRightImage"),
            clay.Layout({sizing = {width = clay.SizingGrow({max = 568})}}),
            clay.ImageConfig({imageData = &syntaxImage, sourceDimensions = {1136, 1194}}),
        ) {}
    }
}

DeclarativeSyntaxPageDesktop :: proc() {
    if clay.Container(
        clay.ID("SyntaxPageDesktop"),
        clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingFit({min = cast(f32)windowHeight - 50})}, childAlignment = {y = .CENTER}, padding = {x = 50}}),
    ) {
        if clay.Border(
            clay.ID("SyntaxPage"),
            clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingGrow({})}, childAlignment = {y = .CENTER}, padding = {32, 32}, childGap = 32}),
            clay.BorderConfig({left = {2, COLOR_RED}, right = {2, COLOR_RED}}),
        ) {
            DeclarativeSyntaxPage({fontSize = 48, fontId = FONT_ID_TITLE_48, textColor = COLOR_LIGHT}, clay.SizingGrow({}))
        }
    }
}

DeclarativeSyntaxPageMobile :: proc() {
    if clay.Container(
        clay.ID("SyntaxPageDesktop"),
        clay.Layout(
            {
                layoutDirection = .TOP_TO_BOTTOM,
                sizing = {clay.SizingGrow({}), clay.SizingFit({min = cast(f32)windowHeight - 50})},
                childAlignment = {x = .CENTER, y = .CENTER},
                padding = {16, 32},
                childGap = 16,
            },
        ),
    ) {
        DeclarativeSyntaxPage({fontSize = 52, fontId = FONT_ID_TITLE_52, textColor = COLOR_LIGHT}, clay.SizingGrow({}))
    }
}

ColorLerp :: proc(a: clay.Color, b: clay.Color, amount: f32) -> clay.Color {
    return clay.Color{a.r + (b.r - a.r) * amount, a.g + (b.g - a.g) * amount, a.b + (b.b - a.b) * amount, a.a + (b.a - a.a) * amount}
}

LOREM_IPSUM_TEXT: clay.String = clay.MakeString("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.")

HighPerformancePage :: proc(lerpValue: f32, titleTextConfig: clay.TextElementConfig, widthSizing: clay.SizingAxis) {
    if clay.Container(clay.ID("PerformanceLeftText"), clay.Layout({sizing = {width = widthSizing}, layoutDirection = .TOP_TO_BOTTOM, childGap = 8})) {
        clay.Text(clay.ID("PerformanceTextTitle"), clay.MakeString("High Performance"), clay.TextConfig(titleTextConfig))
        if clay.Container(clay.ID("SyntaxSpacer"), clay.Layout({sizing = {width = clay.SizingGrow({max = 16})}})) {}
        clay.Text(
            clay.IDI("PerformanceTextSubTitle", 1),
            clay.MakeString("Fast enough to recompute your entire UI every frame."),
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_LIGHT}),
        )
        clay.Text(
            clay.IDI("PerformanceTextSubTitle", 2),
            clay.MakeString("Small memory footprint (3.5mb default) with static allocation & reuse. No malloc / free."),
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_LIGHT}),
        )
        clay.Text(
            clay.IDI("PerformanceTextSubTitle", 3),
            clay.MakeString("Simplify animations and reactive UI design by avoiding the standard performance hacks."),
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_LIGHT}),
        )
    }
    if clay.Container(clay.ID("PerformanceRightImageOuter"), clay.Layout({sizing = {width = widthSizing}, childAlignment = {x = .CENTER}})) {
        if clay.Border(
            clay.ID("PerformanceRightBorder"),
            clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingFixed(400)}}),
            clay.BorderConfigAll({width = 2, color = COLOR_LIGHT}),
        ) {
            if clay.Rectangle(
                clay.ID("AnimationDemoContainerLeft"),
                clay.Layout({sizing = {clay.SizingPercent(0.35 + 0.3 * lerpValue), clay.SizingGrow({})}, childAlignment = {y = .CENTER}, padding = {16, 16}}),
                clay.RectangleConfig({color = ColorLerp(COLOR_RED, COLOR_ORANGE, lerpValue)}),
            ) {
                clay.Text(clay.ID("AnimationDemoTextLeft"), LOREM_IPSUM_TEXT, clay.TextConfig({fontSize = 16, fontId = FONT_ID_BODY_16, textColor = COLOR_LIGHT}))
            }
            if clay.Rectangle(
                clay.ID("AnimationDemoContainerRight"),
                clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingGrow({})}, childAlignment = {y = .CENTER}, padding = {16, 16}}),
                clay.RectangleConfig({color = ColorLerp(COLOR_ORANGE, COLOR_RED, lerpValue)}),
            ) {
                clay.Text(clay.ID("AnimationDemoTextRight"), LOREM_IPSUM_TEXT, clay.TextConfig({fontSize = 16, fontId = FONT_ID_BODY_16, textColor = COLOR_LIGHT}))
            }
        }
    }
}

HighPerformancePageDesktop :: proc(lerpValue: f32) {
    if clay.Rectangle(
        clay.ID("PerformanceDesktop"),
        clay.Layout(
            {sizing = {clay.SizingGrow({}), clay.SizingFit({min = cast(f32)windowHeight - 50})}, childAlignment = {y = .CENTER}, padding = {x = 82, y = 32}, childGap = 64},
        ),
        clay.RectangleConfig({color = COLOR_RED}),
    ) {
        HighPerformancePage(lerpValue, {fontSize = 52, fontId = FONT_ID_TITLE_52, textColor = COLOR_LIGHT}, clay.SizingPercent(0.5))
    }
}

HighPerformancePageMobile :: proc(lerpValue: f32) {
    if clay.Rectangle(
        clay.ID("PerformanceMobile"),
        clay.Layout(
            {
                layoutDirection = .TOP_TO_BOTTOM,
                sizing = {clay.SizingGrow({}), clay.SizingFit({min = cast(f32)windowHeight - 50})},
                childAlignment = {x = .CENTER, y = .CENTER},
                padding = {x = 16, y = 32},
                childGap = 32,
            },
        ),
        clay.RectangleConfig({color = COLOR_RED}),
    ) {
        HighPerformancePage(lerpValue, {fontSize = 48, fontId = FONT_ID_TITLE_48, textColor = COLOR_LIGHT}, clay.SizingGrow({}))
    }
}

RendererButtonActive :: proc(id: u32, index: i32, text: clay.String) {
    if clay.Rectangle(
        id,
        clay.Layout({sizing = {width = clay.SizingFixed(300)}, padding = {16, 16}}),
        clay.RectangleConfig({color = clay.PointerOver(id) ? COLOR_RED_HOVER : COLOR_RED, cornerRadius = clay.CornerRadiusAll(10)}),
    ) {
        clay.Text(clay.ID("RendererButtonActiveText"), text, clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_28, textColor = COLOR_LIGHT}))
    }
}

RendererButtonInactive :: proc(id: u32, index: u32, text: clay.String) {
    if clay.Border(id, clay.Layout({}), clay.BorderConfigOutsideRadius({2, COLOR_RED}, 10)) {
        if clay.Rectangle(
            clay.IDI("RendererButtonInactiveInner", index),
            clay.Layout({sizing = {width = clay.SizingFixed(300)}, padding = {16, 16}}),
            clay.RectangleConfig({color = clay.PointerOver(id) ? COLOR_LIGHT_HOVER : COLOR_LIGHT, cornerRadius = clay.CornerRadiusAll(10)}),
        ) {
            clay.Text(clay.IDI("RendererButtonInactiveText", index), text, clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_28, textColor = COLOR_RED}))
        }
    }
}

RendererPage :: proc(titleTextConfig: clay.TextElementConfig, widthSizing: clay.SizingAxis) {
    if clay.Container(clay.ID("RendererLeftText"), clay.Layout({sizing = {width = widthSizing}, layoutDirection = .TOP_TO_BOTTOM, childGap = 8})) {
        clay.Text(clay.ID("RendererTextTitle"), clay.MakeString("Renderer & Platform Agnostic"), clay.TextConfig(titleTextConfig))
        if clay.Container(clay.ID("Spacer"), clay.Layout({sizing = {width = clay.SizingGrow({max = 16})}})) {}
        clay.Text(
            clay.IDI("RendererTextSubTitle", 1),
            clay.MakeString("Clay outputs a sorted array of primitive render commands, such as RECTANGLE, TEXT or IMAGE."),
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_RED}),
        )
        clay.Text(
            clay.IDI("RendererTextSubTitle", 2),
            clay.MakeString("Write your own renderer in a few hundred lines of code, or use the provided examples for Raylib, WebGL canvas and more."),
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_RED}),
        )
        clay.Text(
            clay.IDI("RendererTextSubTitle", 3),
            clay.MakeString("There's even an HTML renderer - you're looking at it right now!"),
            clay.TextConfig({fontSize = 28, fontId = FONT_ID_BODY_36, textColor = COLOR_RED}),
        )
    }
    if clay.Container(
        clay.ID("RendererRightText"),
        clay.Layout({sizing = {width = widthSizing}, childAlignment = {x = .CENTER}, layoutDirection = .TOP_TO_BOTTOM, childGap = 16}),
    ) {
        clay.Text(
            clay.ID("RendererTextRightTitle"),
            clay.MakeString("Try changing renderer!"),
            clay.TextConfig({fontSize = 36, fontId = FONT_ID_BODY_36, textColor = COLOR_ORANGE}),
        )
        if clay.Container(clay.ID("Spacer"), clay.Layout({sizing = {width = clay.SizingGrow({max = 32})}})) {}
        RendererButtonActive(clay.IDI("RendererSelectButtonActive", 0), 0, clay.MakeString("Raylib Renderer"))
    }
}

RendererPageDesktop :: proc() {
    if clay.Container(
        clay.ID("RendererPageDesktop"),
        clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingFit({min = cast(f32)windowHeight - 50})}, childAlignment = {y = .CENTER}, padding = {x = 50}}),
    ) {
        if clay.Border(
            clay.ID("RendererPage"),
            clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingGrow({})}, childAlignment = {y = .CENTER}, padding = {32, 32}, childGap = 32}),
            clay.BorderConfig({left = {2, COLOR_RED}, right = {2, COLOR_RED}}),
        ) {
            RendererPage({fontSize = 52, fontId = FONT_ID_TITLE_52, textColor = COLOR_RED}, clay.SizingPercent(0.5))
        }
    }
}

RendererPageMobile :: proc() {
    if clay.Rectangle(
        clay.ID("RendererMobile"),
        clay.Layout(
            {
                layoutDirection = .TOP_TO_BOTTOM,
                sizing = {clay.SizingGrow({}), clay.SizingFit({min = cast(f32)windowHeight - 50})},
                childAlignment = {x = .CENTER, y = .CENTER},
                padding = {x = 16, y = 32},
                childGap = 32,
            },
        ),
        clay.RectangleConfig({color = COLOR_LIGHT}),
    ) {
        RendererPage({fontSize = 48, fontId = FONT_ID_TITLE_48, textColor = COLOR_RED}, clay.SizingGrow({}))
    }
}

ScrollbarData :: struct {
    clickOrigin:    clay.Vector2,
    positionOrigin: clay.Vector2,
    mouseDown:      bool,
}

scrollbarData: ScrollbarData = ScrollbarData{}
animationLerpValue: f32 = -1.0

createLayout :: proc(lerpValue: f32) -> clay.ClayArray(clay.RenderCommand) {
    mobileScreen := windowWidth < 750
    clay.BeginLayout(windowWidth, windowHeight)
    if clay.Rectangle(
        clay.ID("OuterContainer"),
        clay.Layout({layoutDirection = .TOP_TO_BOTTOM, sizing = {clay.SizingGrow({}), clay.SizingGrow({})}}),
        clay.RectangleConfig({color = COLOR_LIGHT}),
    ) {
        if clay.Container(
            clay.ID("Header"),
            clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingFixed(50)}, childAlignment = {y = .CENTER}, childGap = 24, padding = {x = 32}}),
        ) {
            clay.Text(clay.ID("Logo"), clay.MakeString("Clay"), &headerTextConfig)
            if clay.Container(clay.ID("Spacer"), clay.Layout({sizing = {width = clay.SizingGrow({})}})) {}

            if (!mobileScreen) {
                if clay.Rectangle(clay.ID("LinkExamplesOuter"), clay.Layout({}), clay.RectangleConfig({color = {0, 0, 0, 0}})) {
                    clay.Text(clay.ID("LinkExamplesText"), clay.MakeString("Examples"), clay.TextConfig({fontId = FONT_ID_BODY_24, fontSize = 24, textColor = {61, 26, 5, 255}}))
                }
                if clay.Rectangle(clay.ID("LinkDocsOuter"), clay.Layout({}), clay.RectangleConfig({color = {0, 0, 0, 0}})) {
                    clay.Text(clay.ID("LinkDocsText"), clay.MakeString("Docs"), clay.TextConfig({fontId = FONT_ID_BODY_24, fontSize = 24, textColor = {61, 26, 5, 255}}))
                }
            }
            githubButtonId: u32 = clay.ID("HeaderButtonGithub")
            if clay.Border(clay.ID("LinkGithubOuter"), clay.Layout({}), clay.BorderConfigOutsideRadius({2, COLOR_RED}, 10)) {
                if clay.Rectangle(
                    githubButtonId,
                    clay.Layout({padding = {16, 6}}),
                    clay.RectangleConfig({cornerRadius = clay.CornerRadiusAll(10), color = clay.PointerOver(githubButtonId) ? COLOR_LIGHT_HOVER : COLOR_LIGHT}),
                ) {
                    clay.Text(clay.ID("LinkGithubText"), clay.MakeString("Github"), clay.TextConfig({fontId = FONT_ID_BODY_24, fontSize = 24, textColor = {61, 26, 5, 255}}))
                }
            }
        }
        if clay.Rectangle(clay.ID("TopBorder1"), clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingFixed(4)}}), clay.RectangleConfig({color = COLOR_TOP_BORDER_5})) {}
        if clay.Rectangle(clay.ID("TopBorder2"), clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingFixed(4)}}), clay.RectangleConfig({color = COLOR_TOP_BORDER_4})) {}
        if clay.Rectangle(clay.ID("TopBorder3"), clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingFixed(4)}}), clay.RectangleConfig({color = COLOR_TOP_BORDER_3})) {}
        if clay.Rectangle(clay.ID("TopBorder4"), clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingFixed(4)}}), clay.RectangleConfig({color = COLOR_TOP_BORDER_2})) {}
        if clay.Rectangle(clay.ID("TopBorder5"), clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingFixed(4)}}), clay.RectangleConfig({color = COLOR_TOP_BORDER_1})) {}
        if clay.Rectangle(
            clay.ID("ScrollContainerBackgroundRectangle"),
            clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingGrow({})}}),
            clay.RectangleConfig({color = COLOR_LIGHT}),
        ) {
            if clay.Scroll(clay.ID("OuterScrollContainer"), clay.Layout({sizing = {clay.SizingGrow({}), clay.SizingGrow({})}}), clay.ScrollConfig({vertical = true})) {
                if clay.Border(
                    clay.ID("ScrollContainerInner"),
                    clay.Layout({layoutDirection = .TOP_TO_BOTTOM, sizing = {width = clay.SizingGrow({})}}),
                    clay.BorderConfig({betweenChildren = {2, COLOR_RED}}),
                ) {
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
        }
    }
    return clay.EndLayout(windowWidth, windowHeight)
}

loadFont :: proc(fontId: u16, fontSize: u16, path: cstring) {
    raylibFonts[fontId] = RaylibFont {
        font   = raylib.LoadFontEx(path, cast(i32)fontSize * 2, nil, 0),
        fontId = cast(u16)fontId,
    }
    raylib.SetTextureFilter(raylibFonts[fontId].font.texture, raylib.TextureFilter.TRILINEAR)
}

main :: proc() {
    minMemorySize: c.uint32_t = clay.MinMemorySize()
    memory := make([^]u8, minMemorySize)
    arena: clay.Arena = clay.CreateArenaWithCapacityAndMemory(minMemorySize, memory)
    clay.SetMeasureTextFunction(measureText)
    clay.Initialize(arena)

    raylib.SetConfigFlags({.VSYNC_HINT, .WINDOW_RESIZABLE, .WINDOW_HIGHDPI, .MSAA_4X_HINT})
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

    syntaxImage = raylib.LoadTextureFromImage(raylib.LoadImage("resources/declarative.png"))
    checkImage1 = raylib.LoadTextureFromImage(raylib.LoadImage("resources/check_1.png"))
    checkImage2 = raylib.LoadTextureFromImage(raylib.LoadImage("resources/check_2.png"))
    checkImage3 = raylib.LoadTextureFromImage(raylib.LoadImage("resources/check_3.png"))
    checkImage4 = raylib.LoadTextureFromImage(raylib.LoadImage("resources/check_4.png"))
    checkImage5 = raylib.LoadTextureFromImage(raylib.LoadImage("resources/check_5.png"))

    for !raylib.WindowShouldClose() {
        animationLerpValue += raylib.GetFrameTime()
        if animationLerpValue > 1 {
            animationLerpValue = animationLerpValue - 2
        }
        windowWidth = raylib.GetScreenWidth()
        windowHeight = raylib.GetScreenHeight()
        clay.SetPointerPosition(transmute(clay.Vector2)raylib.GetMousePosition())
        clay.UpdateScrollContainers(false, transmute(clay.Vector2)raylib.GetMouseWheelMoveV(), raylib.GetFrameTime())
        renderCommands: clay.ClayArray(clay.RenderCommand) = createLayout(animationLerpValue < 0 ? (animationLerpValue + 1) : (1 - animationLerpValue))
        raylib.BeginDrawing()
        clayRaylibRender(&renderCommands)
        raylib.EndDrawing()
    }
}
