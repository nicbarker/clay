---
icon: lucide/triangle-alert
---

# Error handling

Clay reports internal problems through a single callback rather than return codes, so error handling stays out of the hot layout path.

## Clay_ErrorHandler

```c
typedef struct {
    void (*errorHandlerFunction)(Clay_ErrorData errorText);
    uintptr_t userData;
} Clay_ErrorHandler;
```

- **`.errorHandlerFunction`** — called whenever Clay encounters an internal error.
- **`.userData`** — generic pointer transparently passed from [`Clay_Initialize`](functions.md#clay_initialize) through to the callback. Defaults to `NULL`.

```c
void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
    switch (errorData.errorType) {
        // handle by type, see below
    }
}

Clay_Initialize(arena, dimensions, (Clay_ErrorHandler) { HandleClayErrors });
```

## Clay_ErrorData

```c
typedef struct {
    Clay_ErrorType errorType;
    Clay_String errorText;
    uintptr_t userData;
} Clay_ErrorData;
```

- **`.errorText`** — human-readable description. May change between versions; don't pattern-match on it.
- **`.userData`** — same value passed to `Clay_Initialize`.

## Clay_ErrorType

```c
typedef enum {
    CLAY_ERROR_TYPE_TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED,
    CLAY_ERROR_TYPE_ARENA_CAPACITY_EXCEEDED,
    CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED,
    CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED,
    CLAY_ERROR_TYPE_DUPLICATE_ID,
    CLAY_ERROR_TYPE_FLOATING_CONTAINER_PARENT_NOT_FOUND,
    CLAY_ERROR_TYPE_INTERNAL_ERROR,
} Clay_ErrorType;
```

| Error | Cause | Fix |
|---|---|---|
| `TEXT_MEASUREMENT_FUNCTION_NOT_PROVIDED` | `CLAY_TEXT` used without calling [`Clay_SetMeasureTextFunction`](functions.md#clay_setmeasuretextfunction), or a null pointer was passed | Set a valid measure function before laying out text |
| `ARENA_CAPACITY_EXCEEDED` | Arena too small for the configured [`Clay_SetMaxElementCount`](functions.md#clay_setmaxelementcount) | Recompute the required size with [`Clay_MinMemorySize()`](functions.md#clay_minmemorysize) |
| `ELEMENTS_CAPACITY_EXCEEDED` | UI hierarchy exceeds the configured max element count | Raise the max with `Clay_SetMaxElementCount`, then re-derive memory size and reinitialize |
| `TEXT_MEASUREMENT_CAPACITY_EXCEEDED` | UI hierarchy has more text than the configured measure-text cache | Raise the max with [`Clay_SetMaxMeasureTextCacheWordCount`](functions.md#clay_setmaxmeasuretextcachewordcount), then re-derive memory size and reinitialize |
| `DUPLICATE_ID` | Two elements declared with exactly the same ID | Set a breakpoint in your error handler for a stack trace back to the offending declaration |
| `FLOATING_CONTAINER_PARENT_NOT_FOUND` | A floating element's `.parentId` matched no element | Set a breakpoint in your error handler for a stack trace |
| `INTERNAL_ERROR` | Clay hit an internal logic/memory bug | Please [report it upstream](https://github.com/nicbarker/clay) with a stack trace |

!!! tip "Debugging duplicate IDs and missing parents"
    Both `DUPLICATE_ID` and `FLOATING_CONTAINER_PARENT_NOT_FOUND` are best diagnosed by setting a debugger breakpoint inside your error handler function — the call stack at that point leads directly back to the offending `CLAY()` declaration.
