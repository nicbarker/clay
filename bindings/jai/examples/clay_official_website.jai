using Basic :: #import "Basic";

Clay :: #import,file "../module.jai";
Raylib :: #import "raylib-jai";

#load "clay_renderer_raylib.jai";

window_width: s32 = 1024;
window_height: s32 = 768;

main :: () {
	min_memory_size := Clay.MinMemorySize();
	memory := alloc(min_memory_size);
	arena := Clay.CreateArenaWithCapacityAndMemory(min_memory_size, memory);
	Clay.SetMeasureTextFunction(measure_text);
	Clay.Initialize(arena, .{cast(float)Raylib.GetScreenWidth(), cast(float)Raylib.GetScreenHeight()});

	Raylib.SetConfigFlags(.VSYNC_HINT | .WINDOW_RESIZABLE | .WINDOW_HIGHDPI | .MSAA_4X_HINT);
	Raylib.InitWindow(window_width, window_height, "raylib Jai Example");
	// Raylib.SetTargetFPS(60);

	while !Raylib.WindowShouldClose() {
		Raylib.BeginDrawing();
		Raylib.ClearBackground(Raylib.RAYWHITE);
		Raylib.DrawText("HELLO", 190, 200, 20, Raylib.LIGHTGRAY);
		Raylib.EndDrawing();
	}

	Raylib.CloseWindow();
}