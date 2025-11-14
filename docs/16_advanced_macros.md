# Chapter 16: Advanced Macros and Metaprogramming in C

## Complete Guide with Clay Library Examples

---

## Table of Contents
1. Variadic Macros
2. X-Macros Pattern
3. _Generic for Type Selection (C11)
4. Compound Literals
5. Statement Expressions (GCC)
6. For-Loop Macro Trick
7. Designated Initializers in Macros
8. Recursive Macros
9. Macro Debugging
10. Clay's Advanced Macro Techniques

---

## 16.1 Variadic Macros

Macros that accept variable number of arguments.

### Basic Variadic Macro

```c
#include <stdio.h>

#define LOG(format, ...) \
    printf("[LOG] " format "\n", __VA_ARGS__)

#define ERROR(format, ...) \
    fprintf(stderr, "[ERROR] " format "\n", __VA_ARGS__)

int main(void) {
    LOG("Program started");
    LOG("Value: %d", 42);
    LOG("X: %d, Y: %d", 10, 20);

    ERROR("File not found: %s", "config.txt");

    return 0;
}
```

**`__VA_ARGS__`** represents all variadic arguments.

### Empty Variadic Arguments Problem

```c
// Problem: No arguments after format
#define LOG(format, ...) printf(format, __VA_ARGS__)

LOG("Hello");  // ERROR: too few arguments (missing comma)

// Solution 1: GCC ##__VA_ARGS__ extension
#define LOG(format, ...) printf(format, ##__VA_ARGS__)

LOG("Hello");        // Works
LOG("Value: %d", 5); // Also works

// Solution 2: __VA_OPT__ (C++20, C23)
#define LOG(format, ...) printf(format __VA_OPT__(,) __VA_ARGS__)
```

### Named Variadic Arguments

```c
#define DEBUG_PRINT(level, format, ...) \
    do { \
        if (debug_level >= level) { \
            printf("[%d] " format "\n", level, __VA_ARGS__); \
        } \
    } while(0)

int debug_level = 2;

int main(void) {
    DEBUG_PRINT(1, "Low priority: %s", "message");
    DEBUG_PRINT(3, "High priority: %d", 42);
    return 0;
}
```

### Counting Arguments

```c
// Count macro arguments (up to 10)
#define COUNT_ARGS(...) \
    COUNT_ARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define COUNT_ARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N

int main(void) {
    int count1 = COUNT_ARGS(a);           // 1
    int count2 = COUNT_ARGS(a, b, c);     // 3
    int count3 = COUNT_ARGS(a, b, c, d, e); // 5

    printf("%d %d %d\n", count1, count2, count3);
    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Clay's CLAY() macro uses variadic arguments
#define CLAY(...) \
    for (CLAY__ELEMENT_DEFINITION_LATCH = ( \
            Clay__OpenElement(), \
            Clay__ConfigureOpenElement(__VA_ARGS__), \
            0 \
         ); \
         CLAY__ELEMENT_DEFINITION_LATCH < 1; \
         CLAY__ELEMENT_DEFINITION_LATCH = 1, Clay__CloseElement())

// Usage with variable configs
CLAY(CLAY_ID("Button")) { }
CLAY(CLAY_ID("Box"), CLAY_LAYOUT(.padding = 8)) { }
CLAY(CLAY_ID("Panel"), CLAY_LAYOUT(...), CLAY_RECTANGLE(...)) { }
```

---

## 16.2 X-Macros Pattern

Generate code from data tables.

### Basic X-Macro

```c
// Define data once
#define COLOR_TABLE \
    X(RED,    0xFF0000) \
    X(GREEN,  0x00FF00) \
    X(BLUE,   0x0000FF) \
    X(YELLOW, 0xFFFF00) \
    X(CYAN,   0x00FFFF) \
    X(MAGENTA, 0xFF00FF)

// Generate enum
typedef enum {
    #define X(name, value) COLOR_##name,
    COLOR_TABLE
    #undef X
    COLOR_COUNT
} Color;

// Generate array of values
const unsigned int colorValues[] = {
    #define X(name, value) value,
    COLOR_TABLE
    #undef X
};

// Generate array of names
const char* colorNames[] = {
    #define X(name, value) #name,
    COLOR_TABLE
    #undef X
};

int main(void) {
    printf("Color: %s = 0x%06X\n",
           colorNames[COLOR_RED],
           colorValues[COLOR_RED]);

    printf("Total colors: %d\n", COLOR_COUNT);

    return 0;
}
```

### Error Code System with X-Macros

```c
#define ERROR_CODES \
    X(OK,              0,   "Success") \
    X(FILE_NOT_FOUND,  1,   "File not found") \
    X(ACCESS_DENIED,   2,   "Access denied") \
    X(OUT_OF_MEMORY,   3,   "Out of memory") \
    X(INVALID_ARGUMENT, 4,  "Invalid argument")

// Generate enum
typedef enum {
    #define X(name, code, msg) ERR_##name = code,
    ERROR_CODES
    #undef X
} ErrorCode;

// Generate error messages
const char* getErrorMessage(ErrorCode err) {
    switch (err) {
        #define X(name, code, msg) case ERR_##name: return msg;
        ERROR_CODES
        #undef X
        default: return "Unknown error";
    }
}

int main(void) {
    ErrorCode err = ERR_FILE_NOT_FOUND;
    printf("Error %d: %s\n", err, getErrorMessage(err));
    return 0;
}
```

### Clay Example (conceptual):
```c
// Clay could use X-macros for element types
#define CLAY_ELEMENT_TYPES \
    X(RECTANGLE, "Rectangle") \
    X(TEXT,      "Text") \
    X(IMAGE,     "Image") \
    X(BORDER,    "Border") \
    X(CUSTOM,    "Custom")

typedef enum {
    #define X(name, str) CLAY_ELEMENT_TYPE_##name,
    CLAY_ELEMENT_TYPES
    #undef X
} Clay_ElementType;

const char* Clay__ElementTypeToString(Clay_ElementType type) {
    switch (type) {
        #define X(name, str) case CLAY_ELEMENT_TYPE_##name: return str;
        CLAY_ELEMENT_TYPES
        #undef X
        default: return "Unknown";
    }
}
```

---

## 16.3 _Generic for Type Selection (C11)

Compile-time type-based selection.

### Basic _Generic

```c
#include <stdio.h>

#define print(x) _Generic((x), \
    int: printf("%d\n", x), \
    float: printf("%.2f\n", x), \
    double: printf("%.4f\n", x), \
    char*: printf("%s\n", x), \
    default: printf("Unknown type\n") \
)

int main(void) {
    print(42);          // Prints: 42
    print(3.14f);       // Prints: 3.14
    print(2.71828);     // Prints: 2.7183
    print("Hello");     // Prints: Hello

    return 0;
}
```

### Type-Safe Generic Max

```c
#define max(a, b) _Generic((a), \
    int:    max_int, \
    long:   max_long, \
    float:  max_float, \
    double: max_double \
)(a, b)

static inline int max_int(int a, int b) {
    return a > b ? a : b;
}

static inline long max_long(long a, long b) {
    return a > b ? a : b;
}

static inline float max_float(float a, float b) {
    return a > b ? a : b;
}

static inline double max_double(double a, double b) {
    return a > b ? a : b;
}

int main(void) {
    int i = max(5, 10);           // Calls max_int
    float f = max(3.14f, 2.71f);  // Calls max_float
    double d = max(1.1, 2.2);     // Calls max_double

    printf("%d %.2f %.2f\n", i, f, d);
    return 0;
}
```

### Type Identification

```c
#define typename(x) _Generic((x), \
    int: "int", \
    float: "float", \
    double: "double", \
    char: "char", \
    char*: "char*", \
    int*: "int*", \
    default: "unknown" \
)

int main(void) {
    int x = 10;
    float y = 3.14f;
    char* str = "hello";

    printf("x is %s\n", typename(x));    // "int"
    printf("y is %s\n", typename(y));    // "float"
    printf("str is %s\n", typename(str)); // "char*"

    return 0;
}
```

---

## 16.4 Compound Literals

Create temporary struct/array values.

### Basic Compound Literals

```c
typedef struct {
    int x, y;
} Point;

void printPoint(Point p) {
    printf("(%d, %d)\n", p.x, p.y);
}

int main(void) {
    // Compound literal
    printPoint((Point){10, 20});

    // Can take address
    Point *p = &(Point){5, 15};
    printf("(%d, %d)\n", p->x, p->y);

    // Array compound literal
    int *arr = (int[]){1, 2, 3, 4, 5};
    printf("%d\n", arr[0]);  // 1

    return 0;
}
```

### Compound Literals in Macros

```c
#define POINT(x, y) ((Point){x, y})
#define COLOR(r, g, b, a) ((Color){r, g, b, a})

typedef struct {
    float r, g, b, a;
} Color;

int main(void) {
    Point p1 = POINT(10, 20);
    Color red = COLOR(255, 0, 0, 255);

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Clay uses compound literals extensively
#define CLAY__INIT(type) (type)

#define CLAY_COLOR(r, g, b, a) \
    CLAY__INIT(Clay_Color){.r = r, .g = g, .b = b, .a = a}

#define CLAY_DIMENSIONS(w, h) \
    CLAY__INIT(Clay_Dimensions){.width = w, .height = h}

// Usage
Clay_Color red = CLAY_COLOR(255, 0, 0, 255);
Clay_Dimensions size = CLAY_DIMENSIONS(100, 50);
```

---

## 16.5 Statement Expressions (GCC Extension)

Create macros that return values safely.

### Basic Statement Expression

```c
#define MAX(a, b) ({ \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    _a > _b ? _a : _b; \
})

int main(void) {
    int x = 5, y = 10;

    // Safe: expressions evaluated once
    int max = MAX(x++, y++);
    printf("max=%d, x=%d, y=%d\n", max, x, y);
    // max=10, x=6, y=11 (each incremented once)

    return 0;
}
```

### Complex Statement Expression

```c
#define SWAP(a, b) ({ \
    typeof(a) _temp = a; \
    a = b; \
    b = _temp; \
    _temp; \
})

int main(void) {
    int x = 10, y = 20;
    int old_x = SWAP(x, y);

    printf("x=%d, y=%d, old_x=%d\n", x, y, old_x);
    // x=20, y=10, old_x=10

    return 0;
}
```

**Note:** Statement expressions are GCC/Clang extension, not standard C.

---

## 16.6 For-Loop Macro Trick

Create scope-based macros with automatic cleanup.

### Basic For-Loop Trick

```c
#define WITH_LOCK(mutex) \
    for (int _i = (lock(mutex), 0); \
         _i < 1; \
         _i++, unlock(mutex))

// Usage
WITH_LOCK(&myMutex) {
    // Critical section
    // mutex automatically unlocked when block exits
}
```

### How It Works

```c
for (init; condition; increment) {
    body
}

// 1. init runs once: lock mutex, set _i = 0
// 2. condition checked: _i < 1 is true (0 < 1)
// 3. body executes
// 4. increment runs: _i++, unlock mutex
// 5. condition checked: _i < 1 is false (1 < 1)
// 6. loop exits
```

**Clay Example** (from clay.h:2016):
```c
// Clay's famous CLAY() macro
#define CLAY(...) \
    for (CLAY__ELEMENT_DEFINITION_LATCH = ( \
            Clay__OpenElement(), \
            Clay__ConfigureOpenElement(__VA_ARGS__), \
            0 \
         ); \
         CLAY__ELEMENT_DEFINITION_LATCH < 1; \
         CLAY__ELEMENT_DEFINITION_LATCH = 1, Clay__CloseElement())

// Usage: automatically opens and closes elements
CLAY(CLAY_ID("Button"), CLAY_LAYOUT(...)) {
    CLAY_TEXT(CLAY_STRING("Click me"), ...);
}
// Clay__CloseElement() called automatically
```

### Step-by-Step Breakdown

```c
// This code:
CLAY(config) {
    // children
}

// Expands to:
for (CLAY__ELEMENT_DEFINITION_LATCH = (
        Clay__OpenElement(),     // 1. Open element
        Clay__ConfigureOpenElement(config),  // 2. Configure
        0                        // 3. Set latch to 0
     );
     CLAY__ELEMENT_DEFINITION_LATCH < 1;  // 4. Check: 0 < 1 = true, enter loop
     CLAY__ELEMENT_DEFINITION_LATCH = 1, Clay__CloseElement())  // 7. Set latch=1, close
{
    // 5. User code runs (children)
}
// 6. Back to increment
// 8. Check condition: 1 < 1 = false, exit loop
```

---

## 16.7 Designated Initializers in Macros

C99 feature for clean initialization.

### Basic Designated Initializers

```c
typedef struct {
    int x, y, z;
    char *name;
} Data;

int main(void) {
    // Order doesn't matter
    Data d = {
        .z = 30,
        .name = "test",
        .x = 10,
        .y = 20
    };

    // Partial initialization (rest = 0/NULL)
    Data d2 = {.x = 5};

    return 0;
}
```

### Macros with Designated Initializers

```c
#define CREATE_POINT(x, y) \
    (Point){.x = (x), .y = (y)}

#define CREATE_COLOR(r, g, b) \
    (Color){.r = (r), .g = (g), .b = (b), .a = 255}

int main(void) {
    Point p = CREATE_POINT(10, 20);
    Color c = CREATE_COLOR(255, 0, 0);

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Clean API with designated initializers
#define CLAY_LAYOUT(...) \
    (Clay_LayoutConfig){__VA_ARGS__}

#define CLAY_SIZING_FIT(min, max) \
    (Clay_Sizing){ \
        .type = CLAY_SIZING_TYPE_FIT, \
        .size = {.minMax = {.min = min, .max = max}} \
    }

// Usage
Clay_LayoutConfig layout = CLAY_LAYOUT(
    .sizing = {
        .width = CLAY_SIZING_FIT(100, 500),
        .height = CLAY_SIZING_GROW(0)
    },
    .padding = CLAY_PADDING_ALL(16),
    .childGap = 8
);
```

---

## 16.8 Recursive Macros

Macros that appear to call themselves (with limitations).

### Deferred Expression

```c
#define EMPTY()
#define DEFER(x) x EMPTY()
#define EXPAND(...) __VA_ARGS__

#define A() 123
#define B() A()

// Direct expansion
int x = B();  // Expands to: A()

// Deferred expansion  
int y = EXPAND(DEFER(B)());  // Expands to: 123
```

### Recursive List Processing

```c
// Limited recursion using deferred expansion
#define EVAL(...)  EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL3(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL4(...) __VA_ARGS__

#define EMPTY()
#define DEFER(x) x EMPTY()
#define OBSTRUCT(x) x DEFER(EMPTY)()

#define REPEAT(count, macro, ...) \
    REPEAT_IMPL(count, macro, __VA_ARGS__)

// Complex implementation...
```

**Note:** True recursion is not possible in standard C preprocessor.

---

## 16.9 Macro Debugging

### Viewing Macro Expansion

```bash
# GCC/Clang: preprocess only
gcc -E program.c
gcc -E program.c | grep "main"  # Filter output

# Save preprocessed output
gcc -E program.c -o program.i

# With line markers
gcc -E -P program.c  # Remove line markers
```

### Debug Macros

```c
// Print macro expansion
#define SHOW(x) #x
#define EXPAND_SHOW(x) SHOW(x)

#define MY_MACRO(a, b) ((a) + (b))

#pragma message "MY_MACRO(1,2) = " EXPAND_SHOW(MY_MACRO(1, 2))
// Output: MY_MACRO(1,2) = ((1) + (2))
```

### Compile-Time Assertions

```c
// Static assertion
#define BUILD_BUG_ON(condition) \
    ((void)sizeof(char[1 - 2*!!(condition)]))

// C11 static_assert
_Static_assert(sizeof(int) == 4, "int must be 4 bytes");
```

**Clay Example:**
```c
// Clay uses assertions for debugging
#define CLAY__ASSERT(condition) \
    do { \
        if (!(condition)) { \
            Clay__ErrorHandler(...); \
        } \
    } while(0)
```

---

## 16.10 Advanced Clay Macro Techniques

### Element Configuration

```c
// Multiple configuration macros
CLAY(
    CLAY_ID("Panel"),
    CLAY_LAYOUT({
        .sizing = {
            .width = CLAY_SIZING_GROW(0),
            .height = CLAY_SIZING_FIXED(200)
        },
        .padding = CLAY_PADDING_ALL(16),
        .childGap = 8
    }),
    CLAY_RECTANGLE_CONFIG({
        .color = CLAY_COLOR(200, 200, 200, 255),
        .cornerRadius = CLAY_CORNER_RADIUS_ALL(8)
    }),
    CLAY_BORDER_CONFIG({
        .width = 2,
        .color = CLAY_COLOR(100, 100, 100, 255)
    })
) {
    // Children
}
```

### ID Generation

```c
// String-based IDs
#define CLAY_ID(label) \
    Clay__HashString(CLAY_STRING(label), 0, 0)

// Indexed IDs for lists
#define CLAY_IDI(label, index) \
    Clay__HashString(CLAY_STRING(label), index, 0)

// Usage
for (int i = 0; i < itemCount; i++) {
    CLAY(CLAY_IDI("ListItem", i)) {
        // Item content
    }
}
```

### Text Elements

```c
// Macro for text with configuration
#define CLAY_TEXT(text, config) \
    Clay__OpenTextElement(text, config)

// Usage
CLAY_TEXT(
    CLAY_STRING("Hello World"),
    CLAY_TEXT_CONFIG({
        .fontSize = 24,
        .textColor = CLAY_COLOR(0, 0, 0, 255)
    })
);
```

---

## 16.11 Complete Advanced Example

```c
#define CLAY_IMPLEMENTATION
#include "clay.h"

// Custom button macro
#define UI_BUTTON(id, text, onClick) \
    CLAY( \
        CLAY_ID(id), \
        CLAY_LAYOUT({ \
            .padding = CLAY_PADDING_ALL(12), \
            .sizing = { \
                .width = CLAY_SIZING_FIT(0, 0) \
            } \
        }), \
        CLAY_RECTANGLE_CONFIG({ \
            .color = CLAY_COLOR(70, 130, 180, 255), \
            .cornerRadius = CLAY_CORNER_RADIUS_ALL(4) \
        }) \
    ) { \
        CLAY_TEXT( \
            CLAY_STRING(text), \
            CLAY_TEXT_CONFIG({ \
                .fontSize = 16, \
                .textColor = CLAY_COLOR(255, 255, 255, 255) \
            }) \
        ); \
    }

// Custom panel macro
#define UI_PANEL(id, ...) \
    CLAY( \
        CLAY_ID(id), \
        CLAY_LAYOUT({ \
            .padding = CLAY_PADDING_ALL(16), \
            .childGap = 8, \
            .layoutDirection = CLAY_TOP_TO_BOTTOM \
        }), \
        CLAY_RECTANGLE_CONFIG({ \
            .color = CLAY_COLOR(240, 240, 240, 255) \
        }) \
    )

int main(void) {
    Clay_BeginLayout();

    UI_PANEL("MainPanel") {
        CLAY_TEXT(
            CLAY_STRING("Welcome"),
            CLAY_TEXT_CONFIG({.fontSize = 32})
        );

        UI_BUTTON("SubmitBtn", "Submit", handleSubmit);
        UI_BUTTON("CancelBtn", "Cancel", handleCancel);
    }

    Clay_RenderCommandArray commands = Clay_EndLayout();

    return 0;
}
```

---

## 16.12 Key Concepts Learned

- ✅ Variadic macros with __VA_ARGS__
- ✅ X-Macros for code generation
- ✅ _Generic for type-based selection
- ✅ Compound literals for temporary values
- ✅ Statement expressions (GCC)
- ✅ For-loop macro trick for scope
- ✅ Designated initializers
- ✅ Recursive macro techniques
- ✅ Macro debugging methods
- ✅ Clay's advanced macro patterns

---

## Practice Exercises

1. Create a logging system with different log levels using variadic macros
2. Implement a state machine using X-Macros
3. Build a type-safe print function using _Generic
4. Design a resource manager with for-loop cleanup macros
5. Create a unit testing framework using macros
6. Implement a configurable array type with macros
7. Build a simple OOP system with macros
8. Create a domain-specific language using Clay-style macros

