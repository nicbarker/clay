using Basic :: #import "Basic";

Clay :: #import,file "../module.jai";
Raylib :: #import "raylib-jai";

#load "clay_renderer_raylib.jai";

window_width := 1024;
window_height := 768;

main :: () {
	min_memory_size := Clay.MinMemorySize();
	memory := alloc(min_memory_size);
	arena := Clay.CreateArenaWithCapacityAndMemory(min_memory_size, memory);
	Clay.SetMeasureTextFunction(measure_text);
}