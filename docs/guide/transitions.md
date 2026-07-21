---
icon: lucide/wand-sparkles
---

# Transitions

Clay's transition API smoothly animates between states for both layout-affecting properties (`width`, `height`, position) and non-layout properties (`backgroundColor`, `overlayColor`, etc). Transitions activate as soon as `.transition.handler` is set:

```c
// Note: for transitions to work, elements need a stable ID from one frame to the next —
// using loop indexes or CLAY_AUTO_ID will not work.
CLAY(CLAY_IDI("box", colors[index].id), {
    .layout.sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW() },
    .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
    .backgroundColor = boxColor,
    .overlayColor = Clay_Hovered() ? (Clay_Color) { 140, 140, 140, 80 } : (Clay_Color) { 255, 255, 255, 0 },
    .transition = {
        .handler = Clay_EaseOut,
        .duration = 0.5f,
        // Bitwise OR to transition multiple properties
        .properties = CLAY_TRANSITION_PROPERTY_WIDTH
                | CLAY_TRANSITION_PROPERTY_POSITION
                | CLAY_TRANSITION_PROPERTY_OVERLAY_COLOR
                | CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR,
        .enter = { .setInitialState = EnterExitSlideUp },
        .exit = { .setFinalState = EnterExitSlideUp },
    }
}) {
    CLAY_TEXT(CLAY_STRING("Animated Box"), { .fontSize = 32 });
}
```

## Fields ([`Clay_TransitionElementConfig`](../reference/structures.md#clay_transitionelementconfig))

- **`.handler`** — called each frame during a transition to compute current state; returns `true` when finished. Clay ships `Clay_EaseOut`, a standard [ease-out](https://easings.net/) curve. Write your own by inspecting `arguments.initial`, `.target`, `.elapsedTime`, and `.duration`.
- **`.duration`** — time in seconds to reach the target state.
- **`.properties`** — bitfield of which properties to animate; anything not listed snaps immediately.
- **`.interactionHandling`** — `CLAY_TRANSITION_DISABLE_INTERACTIONS_WHILE_TRANSITIONING_POSITION` (default) or `CLAY_TRANSITION_ALLOW_INTERACTIONS_WHILE_TRANSITIONING_POSITION`. Controls whether functions like `Clay_Hovered()` respond while an element's position is animating.

### Transition properties

```c
CLAY_TRANSITION_PROPERTY_NONE (default)
CLAY_TRANSITION_PROPERTY_X
CLAY_TRANSITION_PROPERTY_Y
CLAY_TRANSITION_PROPERTY_POSITION       = X | Y
CLAY_TRANSITION_PROPERTY_WIDTH
CLAY_TRANSITION_PROPERTY_HEIGHT
CLAY_TRANSITION_PROPERTY_DIMENSIONS     = WIDTH | HEIGHT
CLAY_TRANSITION_PROPERTY_BOUNDING_BOX   = POSITION | DIMENSIONS
CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR
CLAY_TRANSITION_PROPERTY_OVERLAY_COLOR
CLAY_TRANSITION_PROPERTY_CORNER_RADIUS
CLAY_TRANSITION_PROPERTY_BORDER_COLOR
CLAY_TRANSITION_PROPERTY_BORDER_WIDTH
CLAY_TRANSITION_PROPERTY_BORDER         = BORDER_COLOR | BORDER_WIDTH
```

## Enter and exit transitions

**Enter** transitions fire the first frame an element appears; **exit** transitions fire the frame an element disappears from the tree. Both require a function pointer to be set — omit it and no enter/exit animation occurs.

```c
Clay_TransitionData EnterSlideUp(Clay_TransitionData initialState, Clay_TransitionProperty properties) {
    Clay_TransitionData targetState = initialState;
    if (properties & CLAY_TRANSITION_PROPERTY_Y) {
        targetState.boundingBox.y += 20;
    }
    if (properties & CLAY_TRANSITION_PROPERTY_OVERLAY_COLOR) {
        targetState.overlayColor = (Clay_Color) { 255, 255, 255, 255 }; // fade in
    }
    return targetState;
}
```

- **`.enter.setInitialState`** — modifies the first-frame state to create the entry animation (e.g. offset Y to slide up, or set an overlay color to fade in).
- **`.enter.trigger`** — `CLAY_TRANSITION_ENTER_SKIP_ON_FIRST_PARENT_FRAME` (default, avoids all list items animating in at once when their parent first appears) or `CLAY_TRANSITION_ENTER_TRIGGER_ON_FIRST_PARENT_FRAME`.
- **`.exit.setFinalState`** — mirror of `.enter.setInitialState` for the element's last frame.
- **`.exit.trigger`** — `CLAY_TRANSITION_EXIT_SKIP_WHEN_PARENT_EXITS` (default) or `CLAY_TRANSITION_EXIT_TRIGGER_WHEN_PARENT_EXITS`.
- **`.exit.siblingOrdering`** — z-order of exiting elements relative to siblings: `CLAY_EXIT_TRANSITION_ORDERING_UNDERNEATH_SIBLINGS` (default), `CLAY_EXIT_TRANSITION_ORDERING_NATURAL_ORDER`, or `CLAY_EXIT_TRANSITION_ORDERING_ABOVE_SIBLINGS`.

!!! warning "Stable IDs required"
    Transitions correlate an element across frames by its ID. Elements using `CLAY_AUTO_ID()` or loop-index-derived IDs will not transition correctly — use [`CLAY_IDI()`](../reference/macros.md#clay_idi) with a stable index/key instead.
