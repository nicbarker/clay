---
icon: lucide/puzzle
---

# Custom elements

Clay only ships primitives for rectangles, text, borders, and images. `.custom` ([`Clay_CustomElementConfig`](../reference/structures.md#clay_customelementconfig)) is Clay's escape hatch for anything else — 3D models, video players, or other renderer-specific content:

```c
typedef enum {
    CUSTOM_ELEMENT_TYPE_MODEL,
    CUSTOM_ELEMENT_TYPE_VIDEO
} CustomElementType;

typedef struct {
    CustomElementType type;
    union {
        Model model;
        Video video;
    };
} CustomElementData;
```

Custom elements only take a single `void *`, so extended data needs to live somewhere that outlives the current layout call — typically a per-frame arena:

```c
typedef struct {
    void* memory;
    uintptr_t offset;
} Arena;

// During init
Arena frameArena = (Arena) { .memory = malloc(1024) };

// During layout
CustomElementData *modelData = (CustomElementData *)(frameArena.memory + frameArena.offset);
*modelData = (CustomElementData) { .type = CUSTOM_ELEMENT_TYPE_MODEL, .model = myModel };
frameArena.offset += sizeof(CustomElementData);
CLAY(CLAY_ID("3DModelViewer"), { .custom = { .customData = modelData } }) {}
```

The pointer is passed straight through to the render command:

```c
switch (renderCommand->commandType) {
    case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
        CustomElementData *customElement = renderCommand->renderData.custom.customData;
        if (!customElement) continue;
        switch (customElement->type) {
            case CUSTOM_ELEMENT_TYPE_MODEL: /* render your 3d model here */ break;
            case CUSTOM_ELEMENT_TYPE_VIDEO: /* render your video here */ break;
        }
        break;
    }
}
```

Custom elements are subject to [visibility culling](advanced.md#visibility-culling); otherwise a single `Clay_RenderCommand` with `commandType = CLAY_RENDER_COMMAND_TYPE_CUSTOM` is emitted per element.
