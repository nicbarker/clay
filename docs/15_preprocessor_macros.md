# Chapter 15: Preprocessor and Macros in C

## Complete Guide with Clay Library Examples

---

## Table of Contents
1. What is the Preprocessor?
2. #include Directive
3. #define - Simple Macros
4. Conditional Compilation
5. Header Guards
6. Function-like Macros
7. Multi-line Macros
8. Stringification (#)
9. Token Pasting (##)
10. Predefined Macros
11. Macro Best Practices
12. Clay's Macro Usage

---

## 15.1 What is the Preprocessor?

The preprocessor runs **before** compilation and performs text substitution.

```c
// Source file (.c)
#define MAX 100

int main(void) {
    int arr[MAX];  // Preprocessor replaces MAX with 100
    return 0;
}

// After preprocessing
int main(void) {
    int arr[100];
    return 0;
}
```

**Preprocessing steps:**
1. Remove comments
2. Process #include directives
3. Expand macros
4. Handle conditional compilation
5. Output preprocessed code to compiler

**View preprocessed output:**
```bash
gcc -E program.c        # Output to stdout
gcc -E program.c -o program.i  # Save to file
```

---

## 15.2 #include Directive

Include header files in your code.

### Standard Library Headers

```c
#include <stdio.h>   // System header (angle brackets)
#include <stdlib.h>
#include <string.h>
```

**Search path:** System include directories (/usr/include, etc.)

### User Headers

```c
#include "myheader.h"  // User header (quotes)
#include "utils/math.h"
```

**Search path:**
1. Current directory
2. Directories specified with -I
3. System directories

### How #include Works

```c
// Before preprocessing
#include "myheader.h"

int main(void) {
    // code
}

// After preprocessing (myheader.h contents inserted)
// Contents of myheader.h
void myFunction(void);
typedef struct { int x, y; } Point;

int main(void) {
    // code
}
```

**Clay Example** (from clay.h):
```c
// Clay is a single-header library
#define CLAY_IMPLEMENTATION
#include "clay.h"

// No other includes needed - zero dependencies!
```

---

## 15.3 #define - Simple Macros

Define constants and simple text replacements.

### Constant Macros

```c
#define PI 3.14159
#define MAX_SIZE 1000
#define BUFFER_SIZE 256
#define VERSION "1.0.0"

int main(void) {
    float radius = 5.0f;
    float area = PI * radius * radius;  // PI replaced with 3.14159

    char buffer[BUFFER_SIZE];
    return 0;
}
```

**Clay Example** (from clay.h:102):
```c
#define CLAY_VERSION_MAJOR 0
#define CLAY_VERSION_MINOR 12
#define CLAY_VERSION_PATCH 0

// Used to check version at compile time
#if CLAY_VERSION_MAJOR >= 1
    // Version 1.0 or higher
#endif
```

### Expression Macros

```c
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define IS_EVEN(n) (((n) & 1) == 0)

int main(void) {
    int sq = SQUARE(5);       // 25
    int max = MAX(10, 20);    // 20
    int even = IS_EVEN(4);    // 1 (true)
    return 0;
}
```

**Important:** Always use parentheses!

```c
// WRONG
#define SQUARE(x) x * x
int result = SQUARE(2 + 3);  // Expands to: 2 + 3 * 2 + 3 = 11 (wrong!)

// RIGHT
#define SQUARE(x) ((x) * (x))
int result = SQUARE(2 + 3);  // Expands to: ((2 + 3) * (2 + 3)) = 25 (correct!)
```

**Clay Example** (from clay.h:111):
```c
#define CLAY__MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CLAY__MIN(x, y) (((x) < (y)) ? (x) : (y))

// Usage in Clay's layout calculations
float width = CLAY__MAX(minWidth, calculatedWidth);
```

---

## 15.4 Conditional Compilation

Compile different code based on conditions.

### #ifdef and #ifndef

```c
#define DEBUG

int main(void) {
    #ifdef DEBUG
        printf("Debug mode enabled\n");
    #endif

    #ifndef RELEASE
        printf("Not in release mode\n");
    #endif

    return 0;
}
```

### #if, #elif, #else, #endif

```c
#define VERSION 2

#if VERSION == 1
    printf("Version 1\n");
#elif VERSION == 2
    printf("Version 2\n");
#else
    printf("Unknown version\n");
#endif
```

### defined() Operator

```c
#if defined(DEBUG) && defined(VERBOSE)
    printf("Debug and verbose mode\n");
#endif

// Equivalent to
#if defined(DEBUG)
    #if defined(VERBOSE)
        printf("Debug and verbose mode\n");
    #endif
#endif
```

**Clay Example** (from clay.h:88):
```c
// Platform-specific exports
#if defined(_MSC_VER)
    #define CLAY_WASM __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
    #define CLAY_WASM __attribute__((visibility("default")))
#else
    #define CLAY_WASM
#endif
```

### Platform Detection

```c
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
#elif defined(__linux__)
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms) * 1000)
#elif defined(__APPLE__)
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms) * 1000)
#endif

int main(void) {
    SLEEP(1000);  // Sleep 1 second on any platform
    return 0;
}
```

**Clay Example** (from clay.h):
```c
// SIMD optimization based on platform
#if !defined(CLAY_DISABLE_SIMD)
    #if defined(__x86_64__) || defined(_M_X64)
        #include <emmintrin.h>
        // Use SSE2 instructions
    #elif defined(__aarch64__)
        #include <arm_neon.h>
        // Use NEON instructions
    #endif
#endif
```

---

## 15.5 Header Guards

Prevent multiple inclusion of same header.

### Traditional Header Guards

```c
// myheader.h
#ifndef MYHEADER_H
#define MYHEADER_H

// Header contents
void myFunction(void);

typedef struct {
    int x, y;
} Point;

#endif // MYHEADER_H
```

**How it works:**
1. First inclusion: `MYHEADER_H` not defined, content is included
2. Second inclusion: `MYHEADER_H` already defined, content skipped

### #pragma once (Non-standard but widely supported)

```c
// myheader.h
#pragma once

// Header contents
void myFunction(void);
```

**Clay Example** (from clay.h:82):
```c
#ifndef CLAY_HEADER
#define CLAY_HEADER

// All Clay declarations (4000+ lines)

typedef struct {
    float x, y;
} Clay_Vector2;

// ... more declarations

#endif // CLAY_HEADER
```

---

## 15.6 Function-like Macros

Macros that look like functions.

### Basic Function-like Macros

```c
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define SQUARE(x) ((x) * (x))
#define SWAP(a, b, type) { type temp = a; a = b; b = temp; }

int main(void) {
    int abs_val = ABS(-5);        // 5
    int squared = SQUARE(4);      // 16

    int x = 10, y = 20;
    SWAP(x, y, int);
    printf("%d %d\n", x, y);      // 20 10

    return 0;
}
```

### Macros vs Functions

**Advantages of macros:**
- ✅ No function call overhead
- ✅ Type-generic (works with any type)
- ✅ Can access local variables

**Disadvantages:**
- ❌ Code bloat (expanded everywhere)
- ❌ No type checking
- ❌ Side effects with multiple evaluation
- ❌ Harder to debug

### Multiple Evaluation Problem

```c
#define SQUARE(x) ((x) * (x))

int main(void) {
    int n = 5;
    int result = SQUARE(n++);
    // Expands to: ((n++) * (n++))
    // n is incremented TWICE!
    printf("n = %d, result = %d\n", n, result);  // n = 7, result = 30
    return 0;
}
```

**Solution:** Use inline functions when possible:

```c
static inline int square(int x) {
    return x * x;
}

int main(void) {
    int n = 5;
    int result = square(n++);  // Only increments once
    printf("n = %d, result = %d\n", n, result);  // n = 6, result = 25
    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Clay uses inline functions when type safety matters
static inline float Clay__Min(float a, float b) {
    return a < b ? a : b;
}

static inline float Clay__Max(float a, float b) {
    return a > b ? a : b;
}

// But uses macros for compile-time constants
#define CLAY_PADDING_ALL(amount) \
    (Clay_Padding){amount, amount, amount, amount}
```

---

## 15.7 Multi-line Macros

Use backslash to continue lines.

### Basic Multi-line Macro

```c
#define PRINT_COORDS(x, y) \
    printf("X: %d\n", x); \
    printf("Y: %d\n", y);

int main(void) {
    PRINT_COORDS(10, 20);
    return 0;
}
```

### do-while(0) Trick

Makes macros behave like statements:

```c
// WRONG - breaks in if statements
#define SWAP(a, b, type) \
    type temp = a; \
    a = b; \
    b = temp;

if (condition)
    SWAP(x, y, int);  // Syntax error!
else
    other();

// RIGHT - works everywhere
#define SWAP(a, b, type) \
    do { \
        type temp = a; \
        a = b; \
        b = temp; \
    } while(0)

if (condition)
    SWAP(x, y, int);  // Works correctly!
else
    other();
```

**Why it works:**
- `do { } while(0)` is a single statement
- Can be used anywhere a statement is expected
- while(0) ensures it runs exactly once
- Semicolon at end required by user

**Clay Example** (from clay.h):
```c
#define CLAY__ASSERT(condition) \
    do { \
        if (!(condition)) { \
            Clay__currentContext->errorHandler.errorHandlerFunction( \
                CLAY__INIT(Clay_ErrorData) { \
                    .errorType = CLAY_ERROR_TYPE_ASSERTION_FAILED, \
                    .errorText = CLAY_STRING("Assertion failed: " #condition), \
                    .userData = Clay__currentContext->errorHandler.userData \
                } \
            ); \
        } \
    } while(0)

// Usage
CLAY__ASSERT(element != NULL);  // Works correctly
```

---

## 15.8 Stringification (#)

Convert macro argument to string.

### Basic Stringification

```c
#define TO_STRING(x) #x

int main(void) {
    printf("%s\n", TO_STRING(Hello));       // "Hello"
    printf("%s\n", TO_STRING(123));         // "123"
    printf("%s\n", TO_STRING(int x = 10));  // "int x = 10"
    return 0;
}
```

### Practical Example

```c
#define PRINT_VAR(var) printf(#var " = %d\n", var)

int main(void) {
    int age = 25;
    int count = 100;

    PRINT_VAR(age);    // Prints: age = 25
    PRINT_VAR(count);  // Prints: count = 100

    return 0;
}
```

### Double Stringification

```c
#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

#define VERSION_MAJOR 1
#define VERSION_MINOR 2
#define VERSION_PATCH 3

#define VERSION_STRING \
    TO_STRING(VERSION_MAJOR) "." \
    TO_STRING(VERSION_MINOR) "." \
    TO_STRING(VERSION_PATCH)

int main(void) {
    printf("Version: %s\n", VERSION_STRING);  // "Version: 1.2.3"
    return 0;
}
```

**Clay Example** (from clay.h):
```c
#define CLAY__STRINGIFY(x) #x
#define CLAY__VERSION_STRING \
    CLAY__STRINGIFY(CLAY_VERSION_MAJOR) "." \
    CLAY__STRINGIFY(CLAY_VERSION_MINOR) "." \
    CLAY__STRINGIFY(CLAY_VERSION_PATCH)

// Results in version string "0.12.0"
```

---

## 15.9 Token Pasting (##)

Concatenate tokens to create new identifiers.

### Basic Token Pasting

```c
#define CONCAT(a, b) a##b

int main(void) {
    int xy = 100;
    int value = CONCAT(x, y);  // Becomes: xy
    printf("%d\n", value);     // 100

    return 0;
}
```

### Generate Variable Names

```c
#define VAR(name, index) name##index

int main(void) {
    int VAR(value, 1) = 10;  // int value1 = 10;
    int VAR(value, 2) = 20;  // int value2 = 20;
    int VAR(value, 3) = 30;  // int value3 = 30;

    printf("%d %d %d\n", value1, value2, value3);
    return 0;
}
```

### Generate Function Names

```c
#define DEFINE_GETTER(type, name) \
    type get_##name(void) { \
        return global_##name; \
    }

int global_count = 10;
float global_value = 3.14f;

DEFINE_GETTER(int, count)    // Creates: int get_count(void)
DEFINE_GETTER(float, value)  // Creates: float get_value(void)

int main(void) {
    printf("%d\n", get_count());   // 10
    printf("%.2f\n", get_value()); // 3.14
    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Generate array types for different types
#define CLAY__ARRAY_DEFINE(typeName, arrayName) \
    typedef struct { \
        int32_t capacity; \
        int32_t length; \
        typeName *internalArray; \
    } arrayName;

// Creates multiple array types
CLAY__ARRAY_DEFINE(int32_t, Clay__int32_tArray)
CLAY__ARRAY_DEFINE(Clay_String, Clay__StringArray)
CLAY__ARRAY_DEFINE(Clay_RenderCommand, Clay__RenderCommandArray)
```

---

## 15.10 Predefined Macros

C provides built-in macros.

### Standard Predefined Macros

```c
#include <stdio.h>

int main(void) {
    printf("File: %s\n", __FILE__);       // Current file name
    printf("Line: %d\n", __LINE__);       // Current line number
    printf("Date: %s\n", __DATE__);       // Compilation date
    printf("Time: %s\n", __TIME__);       // Compilation time
    printf("Function: %s\n", __func__);   // Current function (C99)

    #ifdef __STDC__
        printf("Standard C: Yes\n");
    #endif

    #ifdef __STDC_VERSION__
        printf("C Version: %ld\n", __STDC_VERSION__);
    #endif

    return 0;
}
```

### Platform Predefined Macros

```c
int main(void) {
    #ifdef _WIN32
        printf("Windows\n");
    #endif

    #ifdef __linux__
        printf("Linux\n");
    #endif

    #ifdef __APPLE__
        printf("macOS\n");
    #endif

    #ifdef __x86_64__
        printf("64-bit x86\n");
    #endif

    #ifdef __aarch64__
        printf("64-bit ARM\n");
    #endif

    return 0;
}
```

### Compiler Predefined Macros

```c
int main(void) {
    #ifdef __GNUC__
        printf("GCC version: %d.%d.%d\n",
               __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #endif

    #ifdef _MSC_VER
        printf("MSVC version: %d\n", _MSC_VER);
    #endif

    #ifdef __clang__
        printf("Clang version: %s\n", __clang_version__);
    #endif

    return 0;
}
```

---

## 15.11 Macro Best Practices

### Rule 1: Always Use Parentheses

```c
// WRONG
#define MULTIPLY(a, b) a * b
int result = MULTIPLY(2 + 3, 4 + 5);  // 2 + 3 * 4 + 5 = 19 (wrong!)

// RIGHT
#define MULTIPLY(a, b) ((a) * (b))
int result = MULTIPLY(2 + 3, 4 + 5);  // (2 + 3) * (4 + 5) = 45 (correct!)
```

### Rule 2: Avoid Side Effects

```c
// DANGEROUS
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int x = 5;
int max = MAX(x++, 10);  // x incremented multiple times!

// SAFER - use inline function
static inline int max(int a, int b) {
    return a > b ? a : b;
}

int x = 5;
int max_val = max(x++, 10);  // x incremented once
```

### Rule 3: Use UPPERCASE for Macros

```c
// Makes it clear this is a macro
#define BUFFER_SIZE 256
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Not a macro
static inline int max(int a, int b) { return a > b ? a : b; }
```

### Rule 4: Prefer inline Functions

```c
// Macro (older style)
#define SQUARE(x) ((x) * (x))

// inline function (modern, preferred)
static inline int square(int x) {
    return x * x;
}
```

**When to use macros:**
- ✅ Constants
- ✅ Conditional compilation
- ✅ Type-generic operations
- ✅ Code generation

**When to use inline functions:**
- ✅ Type-safe operations
- ✅ Complex logic
- ✅ Avoid side effects

---

## 15.12 Clay's Macro Usage

### Configuration Macros

```c
// User can override before including
#define CLAY_MAX_ELEMENT_COUNT 8192
#define CLAY_MAX_MEASURETEXT_CACHE_SIZE 16384

#define CLAY_IMPLEMENTATION
#include "clay.h"
```

### Initialization Macros

```c
// Compound literal initialization
#define CLAY__INIT(type) (type)

Clay_Color red = CLAY__INIT(Clay_Color) {
    .r = 255, .g = 0, .b = 0, .a = 255
};
```

### String Macros

```c
// Create Clay_String from literal
#define CLAY_STRING(stringContents) \
    (Clay_String){ \
        .length = sizeof(stringContents) - 1, \
        .chars = (stringContents) \
    }

// Usage
Clay_String title = CLAY_STRING("My Application");
```

### Layout Macros

```c
// Convenient padding creation
#define CLAY_PADDING_ALL(amount) \
    (Clay_Padding){amount, amount, amount, amount}

#define CLAY_PADDING(left, right, top, bottom) \
    (Clay_Padding){left, right, top, bottom}

// Usage
Clay_Padding pad = CLAY_PADDING_ALL(16);
```

### ID Macros

```c
// Generate element IDs
#define CLAY_ID(label) \
    Clay__HashString(CLAY_STRING(label), 0, 0)

#define CLAY_IDI(label, index) \
    Clay__HashString(CLAY_STRING(label), index, 0)

// Usage
Clay_ElementId buttonId = CLAY_ID("SubmitButton");
Clay_ElementId itemId = CLAY_IDI("ListItem", i);
```

---

## 15.13 Complete Clay Macro Example

```c
#define CLAY_IMPLEMENTATION
#include "clay.h"

int main(void) {
    // String macro
    Clay_String title = CLAY_STRING("Clay Demo");

    // Color macro
    Clay_Color bgColor = CLAY__INIT(Clay_Color) {
        .r = 200, .g = 200, .b = 200, .a = 255
    };

    // Padding macro
    Clay_Padding padding = CLAY_PADDING_ALL(16);

    // ID macro
    Clay_ElementId rootId = CLAY_ID("Root");

    // Use in layout
    CLAY(CLAY_ID("Container"),
         CLAY_LAYOUT({
             .padding = CLAY_PADDING_ALL(8),
             .sizing = {
                 .width = CLAY_SIZING_GROW(0),
                 .height = CLAY_SIZING_GROW(0)
             }
         }),
         CLAY_RECTANGLE_CONFIG({
             .color = bgColor
         })) {
        CLAY_TEXT(title, CLAY_TEXT_CONFIG({
            .fontSize = 24,
            .textColor = CLAY__INIT(Clay_Color){0, 0, 0, 255}
        }));
    }

    return 0;
}
```

---

## 15.14 Key Concepts Learned

- ✅ Preprocessor runs before compilation
- ✅ #include inserts file contents
- ✅ #define creates macros
- ✅ Conditional compilation (#ifdef, #if)
- ✅ Header guards prevent multiple inclusion
- ✅ Function-like macros with parameters
- ✅ Multi-line macros with do-while(0)
- ✅ Stringification with #
- ✅ Token pasting with ##
- ✅ Predefined macros (__FILE__, __LINE__)
- ✅ Best practices and pitfalls
- ✅ Clay's effective macro usage

---

## Practice Exercises

1. Create a DEBUG macro that prints file, line, and function name
2. Write ARRAY_SIZE macro to get array length
3. Implement MIN3 and MAX3 macros for 3 values
4. Create a TYPEOF macro using _Generic (C11)
5. Build a FOR_EACH macro to iterate arrays
6. Design a BENCHMARK macro to time code execution
7. Create platform-specific file path macros
8. Implement a simple logging system with macros
