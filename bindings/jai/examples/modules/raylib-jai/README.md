
Module by [Grouflon](https://github.com/Grouflon/raylib-jai).

# Raylib Module for Jai
Module and generator script for [Raylib](https://www.raylib.com).
Current version is **Raylib 5.1**.

## How to use
- Checkout this repository and put it in the `modules` folder of your project.
- Include the module with the `#import "raylib";` directive.

## Example
```
#import "raylib";

main :: ()
{
    InitWindow(800, 600, "raylib example");
    SetTargetFPS(60);

    while !WindowShouldClose()
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
	    DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

	    EndDrawing();
    }

    CloseWindow();
}
```

## Supported Platforms 
- Windows
- Wasm (Not working yet because of some 32bits/64bits mismatch)

## Contributing
Feel free to submit pull requests if you want to add new platforms or any improvement.

