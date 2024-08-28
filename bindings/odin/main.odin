package main

import clay "clay-odin"
import "core:c"
import "core:fmt"

main :: proc() {
	minMemorySize: c.uint32_t = clay.MinMemorySize()
	memory := make([^]u8, minMemorySize)
	arena: clay.Arena = clay.CreateArenaWithCapacityAndMemory(minMemorySize, memory)
	clay.Initialize(arena)
	clay.BeginLayout(1024, 768)
	layoutConfig: clay.LayoutConfig = clay.LayoutConfig {
		sizing = {width = {type = clay.SizingType.GROW}, height = {type = clay.SizingType.GROW}},
		padding = {16, 16},
	}
	rectangleConfig: clay.ImageElementConfig = clay.ImageElementConfig {
		cornerRadius = {topLeft = 5},
	}

	if clay.Rectangle(1, &layoutConfig, &rectangleConfig) {
		if clay.Rectangle(1, &layoutConfig, &rectangleConfig) {

		}
	}

	renderCommands: clay.RenderCommandArray = clay.EndLayout(1024, 768)
}
