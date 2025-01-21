# ClayMan - A Cpp Wrapper Library For Clay

[https://github.com/TimothyHoytBSME/ClayMan/](https://github.com/TimothyHoytBSME/ClayMan/)

### ClayMan is a C++ wrapper library for the `clay.h` library. This library is a work in progress, but is ready to be used.

## Features

- Handles the initialization of Clay.
- Exposes the underlying functions of Clay in a simplified form.
- Manages Clay_Strings to allow for std::string usage.
- Provides handy utility functions for configuration parameters.
- Allows to still use classic Clay layout macros.
- Holds closely to Clay's original macro API.
- Creates text element as a single function call.
- Provides several flexible ways to create your layout

## There are several ways to create your elements

- Pass params and child-lambda:
  ```cpp
  ClayMan.element(
      "someID", 
      (Clay_LayoutConfig){.padding = {1, 2, 3, 4}},
      [&]{
          /*children here*/
      }
  );
  ```

- Or pass params then manually close:
  ```cpp
  ClayMan.openElementWithParams(
      "someID", 
      {.padding = {1, 2, 3, 4}}
  );{
      /*children here*/
  } ClayMan.closeElement();
  ```

- Or manually open, configure, and close:
  ```cpp
  ClayMan.openElement();
  ClayMan.applyID("someID");
  ClayMan.applyLayout({.padding = {1, 2, 3, 4}});
  ClayMan.endConfig();  
  {
      /*children here*/
  } ClayMan.closeElement();
  ```

- Or use classic Clay macros:
  ```cpp
  CLAY(
      CLAY_ID("someID")
      CLAY_LAYOUT({.padding = {1, 2, 3, 4}})
  ){
      /*children here*/ 
  }
  ```


## Create text elements with single function call

```cpp
Clay_TextElementConfig textConfig = { 
    .textColor = COLOR_WHITE, 
    .fontId = 0, 
    .fontSize = 16 
};


ClayMan.textElement("Text Here", textConfig); 
```

## Any method can be nested inside another, just be sure that each element sticks to one method:

```cpp
ClayMan.openElementWithParams( //openElementWithParams METHOD
    "greatgreatgrandparent",
    (Clay_LayoutConfig){.padding = {4, 5, 6, 7}}
);{ 
    //children of "greatgreatgrandparent" inside this block
    ClayMan.element( //element METHOD
        "greatgrandparent",
        (Clay_LayoutConfig){.padding = {4, 5, 6, 7}}
        [&]{ 
            //children of "greatgrandparent" inside this lambda
            CLAY( //classic macro METHOD
                CLAY_ID("grandparent")
                CLAY_LAYOUT({.padding = 1, 2, 3, 4})
            ){ 
                //children of "grandparent" inside this block
                ClayMan.openElement(); //manual METHOD
                ClayMan.applyID("parent");
                ClayMan.applyLayout({.padding = {4, 5, 6, 7}});
                ClayMan.endConfig(); //end "parent" config
                {
                    //children of "parent" in this block
                    ClayMan.openElementWithParams("child", {.padding = {1, 2, 3, 4}});//openElementWithParams METHOD
                    {
                        //children of "child" inside this block
                    } ClayMan.closeElement(); //close "child"
                } ClayMan.closeElement(); //close "parent"

                Clay_TextElementConfig textConfig = { 
                    .textColor = COLOR_WHITE, 
                    .fontId = 0, 
                    .fontSize = 16 
                };

                ClayMan.element( //element METHOD
                    "sibling",
                    (Clay_LayoutConfig){.padding = {4, 5, 6, 7}},
                    [&]{
                        //Children of "sibling" inside this block
                        ClayMan.textElement("Sibling Text", textConfig); 
                    }
                );
            }
        }
    );
} ClayMan.closeElement(); //close "greatgreatgrandparent"
```

## How to use ClayMan
To use this library in your project, simply copy the `clayman.hpp` header, as well as the `include` folder (which has the compatible version of `clay.h` as well as the Raylib renderer) into your working directory. Then you will include the header, include the renderer, create an instance of ClayMan, initialize your renderer, then create your app. Full instructions and examples are available in the [repository](https://github.com/TimothyHoytBSME/ClayMan/).

```cpp
#include "clayman.hpp"
#include "include/raylib/clay_renderer_raylib.c"

ClayMan Clay(1024, 786, Raylib_MeasureText);

void myLayout(){
    //Example full-window parent container
    ClayMan.element(
        "YourElement", 
        (Clay_LayoutConfig){
            .sizing = ClayMan.expandXY(), 
            .padding = ClayMan.padAll(16), 
            .childGap = 16, 
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        [&]{
            //Child elements here
        }
    );
}

int main(void) {

    //Initialize Raylib as usual
    Clay_Raylib_Initialize(ClayMan.width, ClayMan.height, "Your Project Title", FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    Raylib_fonts[0] = (Raylib_Font) { 
        .fontId = 0,
        .font = LoadFontEx("resources/Roboto-Regular.ttf", 48, 0, 400)
    };
    SetTextureFilter(Raylib_fonts[0].font.texture, TEXTURE_FILTER_BILINEAR);

    //Raylib render loop
    while (!WindowShouldClose()) { 
       
        //Raylib mouse position and scroll vectors
        Vector2 mousePosition = GetMousePosition(); 
        Vector2 scrollDelta = GetMouseWheelMoveV(); 
        
        //Update clay state (window size, mouse position/scroll, time delta, left button state)
        ClayMan.updateClayState(
            GetScreenWidth(), //raylib window width
            GetScreenHeight(), //raylib window height
            mousePosition.x, 
            mousePosition.y, 
            scrollDelta.x, 
            scrollDelta.y, 
            GetFrameTime(), //raylib frame delta
            IsMouseButtonDown(0) //raylib left button down
        );

        //Pass your layout to the manager to get the render commands
        Clay_RenderCommandArray renderCommands = ClayMan.buildLayout(myLayout); 

        //Render as usual
        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands);
        EndDrawing();
        
    }
    return 0;
}
```
