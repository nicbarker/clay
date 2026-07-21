---
icon: lucide/layers-2
---

# Floating elements ("absolute" positioning)

Standard Clay elements are laid out on top of, and _within_, their parent, positioned according to the parent's layout rules, and they affect sibling and parent sizing. **Floating** elements are the exception — configured with `.floating`, they:

- Attach by default to the top-left corner of their "parent"
- Don't affect the width, height, or child positioning of their parent
- Don't affect the positioning of sibling elements
- Have a **z-index**, and can appear above or below other elements
- Otherwise behave like standard elements — expanding to fit children, etc.

The easiest mental model: a floating container is a **separate UI hierarchy**, attached to a specific x/y point on its "parent". Typical use cases are tooltips and modals.

```c
CLAY(CLAY_ID("Outer"), { .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM } }) {
    CLAY_TEXT(text, &headerTextConfig);
    CLAY(CLAY_ID("Tooltip"), { .floating = { .attachTo = CLAY_ATTACH_TO_PARENT } }) {}
    CLAY_TEXT(text, &headerTextConfig);
}
```

## Key fields ([`Clay_FloatingElementConfig`](../reference/structures.md#clay_floatingelementconfig))

- **`.offset`** — position offset applied *after* layout is calculated.
- **`.expand`** — expands width/height of the floating container *before* laying out children.
- **`.zIndex`** — floating elements (and their subtrees) are sorted by `.zIndex` before conversion to render commands; higher draws on top.
- **`.attachPoints`** — `{ .element, .parent }` pair that specifies which point on the floating container lines up with which point on the parent (e.g. `CLAY_ATTACH_POINT_LEFT_BOTTOM` on the element attached to `CLAY_ATTACH_POINT_CENTER_CENTER` on the parent).
- **`.pointerCaptureMode`** — `CLAY_POINTER_CAPTURE_MODE_CAPTURE` (default) or `CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH`, controlling whether hover/click pass through to content underneath.
- **`.attachTo`** — `CLAY_ATTACH_TO_NONE` (default), `CLAY_ATTACH_TO_PARENT`, `CLAY_ATTACH_TO_ELEMENT_WITH_ID`, `CLAY_ATTACH_TO_ROOT`.
- **`.clipTo`** — `CLAY_CLIP_TO_NONE` (default) or `CLAY_CLIP_TO_ATTACHED_PARENT`, to clip the floating element by its parent's `.clip` rectangle instead of always rendering above it.

`Clay_FloatingElementConfig` does not generate any render commands of its own.

## Attaching by ID instead of by declaration position

By default, a floating container attaches to whatever element it's declared inside. `.parentId` lets you attach it to **any** element by ID, declared anywhere in the hierarchy — before or after the floating container itself, and even to an ID that doesn't exist yet when the floating config is written:

```c
// Instead of conditionally nesting a tooltip inside every possible button...
CLAY(CLAY_IDI("SidebarButton", 1), { }) { /* ... */ }
CLAY(CLAY_IDI("SidebarButton", 2), { }) { /* ... */ }
// ...

// ...declare it once, anywhere, and point at the target by id:
CLAY(CLAY_ID("OptionTooltip"), {
    .floating = {
        .attachTo = CLAY_ATTACH_TO_ELEMENT_WITH_ID,
        .parentId = CLAY_IDI("SidebarButton", tooltip.attachedButtonIndex).id
    }
}) {
    // Tooltip contents...
}
```

This avoids polluting the layout declaration of many elements just to conditionally attach a tooltip to one of them.

## Example

```c
CLAY(CLAY_ID("OptionsList"), { .layout = { .childGap = 16 } }) {
    CLAY(CLAY_IDI("Option", 2), { .layout = { .padding = CLAY_PADDING_ALL(16) }, .backgroundColor = COLOR_BLUE }) {
        CLAY_TEXT(CLAY_STRING("Option 2"), {});
        // Attaches above "Option 2" without affecting sibling widths or positions
        CLAY(CLAY_ID("OptionTooltip"), { .floating = { .zIndex = 1, .attachPoints = { .element = CLAY_ATTACH_POINT_CENTER_BOTTOM, .parent = CLAY_ATTACH_POINT_CENTER_TOP } } }) {
            CLAY_TEXT(CLAY_STRING("Most popular!"), {});
        }
    }
}
```
