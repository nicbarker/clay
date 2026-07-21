---
icon: lucide/mouse-pointer-click
---

# Mouse and pointer interactions

All pointer interactions depend on [`Clay_SetPointerState(position, isPointerDown)`](../reference/functions.md#clay_setpointerstate) being called every frame, before any other Clay functions.

## During UI declaration

### Clay_Hovered

`bool Clay_Hovered()` can be called during element construction, or in the body of an element, and returns `true` if the pointer is over the currently open element:

```c
// An orange button that turns blue when hovered
CLAY(CLAY_ID("Button"), { .backgroundColor = Clay_Hovered() ? COLOR_BLUE : COLOR_ORANGE }) {
    bool buttonHovered = Clay_Hovered();
    CLAY_TEXT(buttonHovered ? CLAY_STRING("Hovered") : CLAY_STRING("Hover me!"), headerTextConfig);
}
```

### Clay_OnHover

`void Clay_OnHover(handler, userData)` attaches a function pointer to the currently open element, called once per layout while the pointer is over it:

```c
void HandleButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerInfo, void *userData) {
    ButtonData *buttonData = (ButtonData *)userData;
    // Pointer state allows you to detect mouse down / hold / release
    if (pointerInfo.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        NavigateTo(buttonData->link);
    }
}

ButtonData linkButton = (ButtonData) { .link = "https://github.com/nicbarker/clay" };

CLAY(CLAY_ID("Button"), { .layout = { .padding = CLAY_PADDING_ALL(8) } }) {
    Clay_OnHover(HandleButtonInteraction, &linkButton);
    CLAY_TEXT(CLAY_STRING("Button"), &headerTextConfig);
}
```

## Before / after UI declaration

To query mouse overlap outside of layout declarations, use `bool Clay_PointerOver(Clay_ElementId id)`:

```c
// Reminder: Clay_SetPointerState must be called before functions that rely on pointer position otherwise it will have no effect
Clay_Vector2 mousePosition = { x, y };
Clay_SetPointerState(mousePosition, mouseButtonDown(0));
// ...
if (mouseButtonDown(0) && Clay_PointerOver(Clay_GetElementId("ProfilePicture"))) {
    // Handle profile picture clicked
}
```

!!! note "One frame of latency"
    `Clay_PointerOver` uses the bounding box from the **last** frame. This rarely matters except for fast-moving animations. If frame-accurate detection is required, run your layout code twice per frame with the same data — the second call will be accurate.

## Pointer interaction states

[`Clay_PointerData`](../reference/structures.md#clay_pointerdata) reports one of four states via `.state`:

```c
typedef enum {
    CLAY_POINTER_DATA_PRESSED_THIS_FRAME,
    CLAY_POINTER_DATA_PRESSED,
    CLAY_POINTER_DATA_RELEASED_THIS_FRAME,
    CLAY_POINTER_DATA_RELEASED,
} Clay_PointerDataInteractionState;
```

For example, a user hovering a button, clicking, holding, then releasing:

1. Hovering, button not pressed: `CLAY_POINTER_DATA_RELEASED`
2. First frame the button is pressed: `CLAY_POINTER_DATA_PRESSED_THIS_FRAME`
3. Subsequent frames held down: `CLAY_POINTER_DATA_PRESSED`
4. The frame the button is released: `CLAY_POINTER_DATA_RELEASED_THIS_FRAME`
5. Subsequent frames while still hovering: `CLAY_POINTER_DATA_RELEASED`

## Next steps

- [Scrolling elements](scrolling.md) builds on `Clay_SetPointerState` for drag scrolling.
- [Floating elements](floating-elements.md) covers `.pointerCaptureMode` for tooltips and modals.
