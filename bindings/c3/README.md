# Clay-C3-Bindings
C3 Bindings for [Clay](https://github.com/nicbarker/clay.git), a UI layout library written in C.
This directory contains the clay.c3 bindings file as well as a recreation of the clay-raylib-renderer and the video-example raylib/clay project.

Special thanks to:
- [Christoffer L](https://github.com/lerno) C3's core developer (as I understand it)
- Book-reader in the [C3-lang Discord](https://discord.gg/qN76R87)

## TODO:
- Find out how to build a static-lib with additional C sources

## - C3 macros
Traditional Clay C Macro System

```cpp
/* FILTER BUTTON */
CLAY(
    CLAY_ID("FilterButton"),
    Clay_Hovered() ? CLAY_RECTANGLE({
        .color = Clay_Hovered() ? FIRE_ORANGE : (Clay_Color){80, 25, 200, 255},
        .cornerRadius = 8,
    }) : 0,
    CLAY_LAYOUT({
        .sizing = {
            .width = CLAY_SIZING_FIT(),
            .height = CLAY_SIZING_GROW()
        },
        .padding = 10
    }) 
) {
    // define children...
}
```

Clay C3 Macro System
```cpp
/* FILTER BUTTON */
@clay(
    clay::id("FilterButton"),
    clay::@bodyIf(clay::hovered(), clay::rectangle({
            .color = clay::hovered() ? FIRE_ORANGE : {80, 25, 200, 255},
            .cornerRadius = clay::cornerRadiusUni(8)
        })
    ),
    clay::layout({
        .sizing = {
            .width = clay::sizingFit(),
            .height = clay::sizingGrow()
        },
        .padding = clay::paddingUni(8)
    })
){
    // define children...
};
```

## To Get Started:
- Download c3c [here](https://c3-lang.org/getting-started/prebuilt-binaries/)
- If you wish to compile the website-example, I've already provided a target to build in the [project.json](project.json)
- - set your `cd` to [c3](./)
- - use the `c3c vendor-fetch raylib55` command to download a c3 compressed archive of raylib
- - - *once you have raylib55.c3l in the [lib](lib) folder you've got it right* 
- - then use the command `c3c run video-example` to compile and run the video example
- - - (*note: to use the `c3c build <target>` command with video-example, you'll need to copy the resource folder into the [build](build) directory to run it*
- - - *`run` executes the build result from the project directory, somehow. This means that `run` will look for the resource folder in [c3](../c3), while `build` will look for it in [build](build)*)

## RESOURCES:
### - [C3](https://github.com/c3lang/c3c.git) (A C-a-like, that aims to bring modern language QA features and a revamped Macro system to C)
### - [Raylib](https://github.com/raysan5/raylib.git) (C Videogame and Graphical API)
### - [Lexend](https://github.com/googlefonts/lexend.git) (Accessible/ Dyslexic Friendly Font)

