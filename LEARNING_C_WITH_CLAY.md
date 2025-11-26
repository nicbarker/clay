# Learning C Programming with Clay: A Complete Step-by-Step Guide

## Table of Contents
1. [Introduction to C and Clay](#introduction)
2. [Chapter 1: C Basics - Your First Program](#chapter-1)
3. [Chapter 2: Variables and Data Types](#chapter-2)
4. [Chapter 3: Operators](#chapter-3)
5. [Chapter 4: Control Flow](#chapter-4)
6. [Chapter 5: Loops](#chapter-5)
7. [Chapter 6: Functions](#chapter-6)
8. [Chapter 7: Pointers - The Heart of C](#chapter-7)
9. [Chapter 8: Structs and Typedef](#chapter-8)
10. [Chapter 9: Arrays and Memory](#chapter-9)
11. [Chapter 10: Strings](#chapter-10)
12. [Chapter 11: Type Casting and Qualifiers](#chapter-11)
13. [Chapter 12: Storage Classes and Scope](#chapter-12)
14. [Chapter 13: Recursion](#chapter-13)
15. [Chapter 14: Bit Manipulation](#chapter-14)
16. [Chapter 15: Preprocessor and Macros](#chapter-15)
17. [Chapter 16: Advanced Macros and Metaprogramming](#chapter-16)
18. [Chapter 17: Memory Management](#chapter-17)
19. [Chapter 18: Header Files and Project Organization](#chapter-18)
20. [Chapter 19: Enums and Unions](#chapter-19)
21. [Chapter 20: Function Pointers and Callbacks](#chapter-20)
22. [Chapter 21: Standard Library Basics](#chapter-21)
23. [Chapter 22: File I/O](#chapter-22)
24. [Chapter 23: Command-Line Arguments](#chapter-23)
25. [Chapter 24: Building Complete Programs](#chapter-24)

---

## Introduction to C and Clay {#introduction}

### What is C?

C is a powerful, low-level programming language created in 1972. It's:
- **Fast**: Compiles to native machine code
- **Portable**: Runs on almost any hardware
- **Simple**: Small set of keywords and features
- **Powerful**: Direct memory access and hardware control
- **Foundation**: Many languages (C++, Java, Python internals) are built in C

### What is Clay?

Clay is a high-performance 2D UI layout library that demonstrates professional C programming:
- **Single-header library**: Entire implementation in one file
- **Zero dependencies**: No standard library required
- **Microsecond performance**: Extremely fast
- **Production-ready**: Real-world, well-designed code

### Why Learn C with Clay?

Clay shows you:
- Professional C code patterns
- Real-world memory management
- Advanced macro techniques
- API design principles
- Performance optimization

Let's begin!

---

## Chapter 1: C Basics - Your First Program {#chapter-1}

### 1.1 The Simplest C Program

Every C program starts with a `main` function:

```c
int main(void) {
    return 0;
}
```

**Breaking it down:**
- `int` - The function returns an integer (0 = success)
- `main` - Special function name (program entry point)
- `void` - Takes no parameters
- `return 0` - Exit code (0 means success)

### 1.2 Including Headers

To use functions from other files, we include headers:

```c
#include <stdio.h>  // Standard Input/Output

int main(void) {
    printf("Hello, World!\n");
    return 0;
}
```

**Clay Example:**
```c
#define CLAY_IMPLEMENTATION
#include "clay.h"  // Include Clay library

int main(void) {
    // Clay initialization code here
    return 0;
}
```

### 1.3 Comments

Two types of comments in C:

```c
// Single-line comment

/*
   Multi-line comment
   Can span multiple lines
*/
```

**Clay Example** (from clay.h:1):
```c
/*
    Clay 0.12 - A High Performance UI Layout Library in C

    Features:
    - Flexbox-style responsive layout
    - Single header library
    - Microsecond layout performance
*/
```

### 1.4 Key Concepts Learned
- ✅ Basic program structure
- ✅ The main() function
- ✅ Including headers with #include
- ✅ Comments

---

## Chapter 2: Variables and Data Types {#chapter-2}

### 2.1 Basic Data Types

C has several built-in types:

```c
int main(void) {
    // Integer types
    int age = 25;              // Signed integer (usually 32 bits)
    unsigned int count = 100;  // Unsigned (only positive)

    // Floating-point types
    float pi = 3.14f;          // Single precision
    double precise = 3.14159;  // Double precision

    // Character type
    char letter = 'A';         // Single character

    // Boolean (C99+)
    _Bool isTrue = 1;          // 0 = false, 1 = true

    return 0;
}
```

### 2.2 Fixed-Width Integer Types

Modern C uses fixed-width types for portability:

```c
#include <stdint.h>

int main(void) {
    int8_t   small = 127;      // 8-bit signed (-128 to 127)
    uint8_t  byte = 255;       // 8-bit unsigned (0 to 255)
    int16_t  medium = 32767;   // 16-bit signed
    uint16_t umedium = 65535;  // 16-bit unsigned
    int32_t  large = 2147483647;   // 32-bit signed
    uint32_t ularge = 4294967295;  // 32-bit unsigned
    int64_t  huge = 9223372036854775807; // 64-bit signed

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Clay uses fixed-width types for precision
typedef struct {
    int32_t capacity;   // Exactly 32 bits
    int32_t length;
    uint32_t *internalArray;  // Unsigned 32-bit
} Clay__int32_tArray;
```

### 2.3 Type Sizes

```c
#include <stdio.h>

int main(void) {
    printf("int size: %zu bytes\n", sizeof(int));
    printf("float size: %zu bytes\n", sizeof(float));
    printf("double size: %zu bytes\n", sizeof(double));
    printf("char size: %zu bytes\n", sizeof(char));

    return 0;
}
```

### 2.4 Constants

Make values unchangeable:

```c
const int MAX_ITEMS = 100;  // Cannot be changed
const float PI = 3.14159f;
```

**Clay Example** (from clay.h):
```c
const Clay_Color CLAY_COLOR_WHITE = {255, 255, 255, 255};
const Clay_Color CLAY_COLOR_BLACK = {0, 0, 0, 255};
```

### 2.5 Key Concepts Learned
- ✅ Basic types: int, float, double, char
- ✅ Fixed-width types: int32_t, uint32_t, etc.
- ✅ sizeof operator
- ✅ const keyword

---

## Chapter 3: Operators {#chapter-3}

### 3.1 Arithmetic Operators

Basic math operations:

```c
int main(void) {
    int a = 10, b = 3;

    int sum = a + b;         // 13 (addition)
    int diff = a - b;        // 7 (subtraction)
    int product = a * b;     // 30 (multiplication)
    int quotient = a / b;    // 3 (integer division)
    int remainder = a % b;   // 1 (modulo/remainder)

    // Floating-point division
    float result = 10.0f / 3.0f;  // 3.333...

    return 0;
}
```

### 3.2 Increment and Decrement

```c
int main(void) {
    int x = 5;

    // Post-increment (use then increment)
    int a = x++;  // a = 5, x = 6

    // Pre-increment (increment then use)
    int b = ++x;  // x = 7, b = 7

    // Post-decrement
    int c = x--;  // c = 7, x = 6

    // Pre-decrement
    int d = --x;  // x = 5, d = 5

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Pre-increment used for index management
Clay__currentContext->layoutElements.length++;

// Post-increment to get current then move
element = array->internalArray[index++];
```

### 3.3 Relational Operators

Compare values:

```c
int main(void) {
    int a = 10, b = 20;

    int equal = (a == b);        // 0 (false)
    int notEqual = (a != b);     // 1 (true)
    int less = (a < b);          // 1 (true)
    int lessOrEqual = (a <= b);  // 1 (true)
    int greater = (a > b);       // 0 (false)
    int greaterOrEqual = (a >= b); // 0 (false)

    return 0;
}
```

**Clay Example** (from clay.h):
```c
static inline float Clay__Max(float a, float b) {
    return a > b ? a : b;  // Using > operator
}

if (config->type == CLAY_SIZING_TYPE_FIXED) {
    // Using == operator
}
```

### 3.4 Logical Operators

Boolean logic:

```c
int main(void) {
    int a = 1, b = 0;  // 1 = true, 0 = false

    int andResult = a && b;  // 0 (both must be true)
    int orResult = a || b;   // 1 (at least one true)
    int notResult = !a;      // 0 (logical NOT)

    // Short-circuit evaluation
    if (ptr != NULL && *ptr > 0) {
        // Safe: *ptr only evaluated if ptr != NULL
    }

    return 0;
}
```

**Clay Example** (from clay.h):
```c
if (element->layoutConfig &&
    element->layoutConfig->sizing.width.type == CLAY_SIZING_TYPE_GROW) {
    // Short-circuit: second condition only checked if first is true
}
```

### 3.5 Bitwise Operators

Work on individual bits:

```c
int main(void) {
    unsigned int a = 5;   // 0101 in binary
    unsigned int b = 3;   // 0011 in binary

    unsigned int and = a & b;   // 0001 = 1 (AND)
    unsigned int or = a | b;    // 0111 = 7 (OR)
    unsigned int xor = a ^ b;   // 0110 = 6 (XOR)
    unsigned int not = ~a;      // 1010 (NOT, inverts all bits)

    // Bit shifts
    unsigned int leftShift = a << 1;   // 1010 = 10 (multiply by 2)
    unsigned int rightShift = a >> 1;  // 0010 = 2 (divide by 2)

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Bit flags for corner radius
typedef enum {
    CLAY_CORNER_RADIUS_TOP_LEFT = 1 << 0,      // 0001
    CLAY_CORNER_RADIUS_TOP_RIGHT = 1 << 1,     // 0010
    CLAY_CORNER_RADIUS_BOTTOM_LEFT = 1 << 2,   // 0100
    CLAY_CORNER_RADIUS_BOTTOM_RIGHT = 1 << 3   // 1000
} Clay_CornerRadiusSet;

// Combine flags with OR
int flags = CLAY_CORNER_RADIUS_TOP_LEFT | CLAY_CORNER_RADIUS_TOP_RIGHT;

// Check if flag is set with AND
if (flags & CLAY_CORNER_RADIUS_TOP_LEFT) {
    // Top left corner has radius
}
```

### 3.6 Assignment Operators

```c
int main(void) {
    int x = 10;

    x += 5;   // x = x + 5  (15)
    x -= 3;   // x = x - 3  (12)
    x *= 2;   // x = x * 2  (24)
    x /= 4;   // x = x / 4  (6)
    x %= 4;   // x = x % 4  (2)

    // Bitwise compound assignments
    x &= 3;   // x = x & 3
    x |= 4;   // x = x | 4
    x ^= 1;   // x = x ^ 1
    x <<= 1;  // x = x << 1
    x >>= 1;  // x = x >> 1

    return 0;
}
```

### 3.7 Ternary Operator

Compact if-else:

```c
int main(void) {
    int a = 10, b = 20;

    // condition ? value_if_true : value_if_false
    int max = (a > b) ? a : b;

    // Equivalent to:
    int max2;
    if (a > b) {
        max2 = a;
    } else {
        max2 = b;
    }

    return 0;
}
```

**Clay Example** (from clay.h):
```c
static inline float Clay__Min(float a, float b) {
    return a < b ? a : b;  // Ternary operator
}

// Nested ternary (less readable)
int result = (x > 0) ? 1 : (x < 0) ? -1 : 0;
```

### 3.8 sizeof Operator

Get size of type or variable:

```c
#include <stdio.h>

int main(void) {
    printf("int: %zu bytes\n", sizeof(int));
    printf("float: %zu bytes\n", sizeof(float));
    printf("double: %zu bytes\n", sizeof(double));
    printf("char: %zu bytes\n", sizeof(char));
    printf("pointer: %zu bytes\n", sizeof(void*));

    int arr[10];
    printf("array: %zu bytes\n", sizeof(arr));  // 40 bytes (10 * 4)

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Get size of struct
size_t size = sizeof(Clay_LayoutElement);

// Allocate memory
void *mem = malloc(count * sizeof(Clay_RenderCommand));

// Calculate array length
int length = sizeof(array) / sizeof(array[0]);
```

### 3.9 Comma Operator

Evaluate multiple expressions:

```c
int main(void) {
    int a, b, c;

    // Comma operator: evaluates left to right, returns rightmost
    a = (b = 5, c = 10, b + c);  // a = 15

    // Common in for loops
    for (int i = 0, j = 10; i < j; i++, j--) {
        // i increments, j decrements
    }

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Used in macro for-loop trick
#define CLAY(...) \
    for ( \
        CLAY__ELEMENT_DEFINITION_LATCH = ( \
            Clay__OpenElement(),              /* First */ \
            Clay__ConfigureOpenElement(...),  /* Second */ \
            0                                 /* Result */ \
        ); \
        /* ... */ \
    )
```

### 3.10 Operator Precedence

Order of evaluation (high to low):

```c
int main(void) {
    // Precedence matters!
    int x = 2 + 3 * 4;      // 14, not 20 (* before +)
    int y = (2 + 3) * 4;    // 20 (parentheses first)

    // Common precedence
    // 1. () [] -> .
    // 2. ! ~ ++ -- + - * & sizeof
    // 3. * / %
    // 4. + -
    // 5. << >>
    // 6. < <= > >=
    // 7. == !=
    // 8. &
    // 9. ^
    // 10. |
    // 11. &&
    // 12. ||
    // 13. ?:
    // 14. = += -= etc.
    // 15. ,

    return 0;
}
```

**Tip**: When in doubt, use parentheses for clarity!

### 3.11 Key Concepts Learned
- ✅ Arithmetic operators (+, -, *, /, %)
- ✅ Increment/decrement (++, --)
- ✅ Relational operators (==, !=, <, >, <=, >=)
- ✅ Logical operators (&&, ||, !)
- ✅ Bitwise operators (&, |, ^, ~, <<, >>)
- ✅ Assignment operators (=, +=, -=, etc.)
- ✅ Ternary operator (?:)
- ✅ sizeof operator
- ✅ Operator precedence

---

## Chapter 4: Control Flow {#chapter-4}

### 4.1 if Statement

Basic conditional execution:

```c
int main(void) {
    int age = 18;

    if (age >= 18) {
        printf("Adult\n");
    }

    return 0;
}
```

**Clay Example** (from clay.h):
```c
if (layoutElement->layoutConfig) {
    // Process layout config
}

if (element == NULL) {
    return;  // Early return
}
```

### 4.2 if-else Statement

```c
int main(void) {
    int score = 75;

    if (score >= 60) {
        printf("Pass\n");
    } else {
        printf("Fail\n");
    }

    return 0;
}
```

### 4.3 else-if Chains

```c
int main(void) {
    int grade = 85;

    if (grade >= 90) {
        printf("A\n");
    } else if (grade >= 80) {
        printf("B\n");
    } else if (grade >= 70) {
        printf("C\n");
    } else if (grade >= 60) {
        printf("D\n");
    } else {
        printf("F\n");
    }

    return 0;
}
```

**Clay Example** (from clay.h):
```c
if (config->type == CLAY_SIZING_TYPE_FIT) {
    // Handle FIT sizing
} else if (config->type == CLAY_SIZING_TYPE_GROW) {
    // Handle GROW sizing
} else if (config->type == CLAY_SIZING_TYPE_FIXED) {
    // Handle FIXED sizing
}
```

### 4.4 Nested if Statements

```c
int main(void) {
    int age = 25;
    int hasLicense = 1;

    if (age >= 18) {
        if (hasLicense) {
            printf("Can drive\n");
        } else {
            printf("Need license\n");
        }
    } else {
        printf("Too young\n");
    }

    return 0;
}
```

### 4.5 switch Statement

Multiple conditions based on value:

```c
int main(void) {
    int day = 3;

    switch (day) {
        case 1:
            printf("Monday\n");
            break;
        case 2:
            printf("Tuesday\n");
            break;
        case 3:
            printf("Wednesday\n");
            break;
        case 4:
            printf("Thursday\n");
            break;
        case 5:
            printf("Friday\n");
            break;
        case 6:
        case 7:
            printf("Weekend\n");
            break;
        default:
            printf("Invalid day\n");
            break;
    }

    return 0;
}
```

**Important**: `break` is required to exit switch, otherwise execution "falls through"!

**Clay Example** (from clay.h):
```c
switch (commandType) {
    case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
        // Render rectangle
        break;
    }
    case CLAY_RENDER_COMMAND_TYPE_TEXT: {
        // Render text
        break;
    }
    case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
        // Render image
        break;
    }
    default: {
        // Unknown command
        break;
    }
}
```

### 4.6 Fall-through in switch

Intentional fall-through can be useful:

```c
int main(void) {
    char ch = 'a';

    switch (ch) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            printf("Vowel\n");
            break;
        default:
            printf("Consonant\n");
            break;
    }

    return 0;
}
```

### 4.7 goto Statement

Jump to a label (use sparingly):

```c
int main(void) {
    int error = processData();

    if (error) {
        goto cleanup;
    }

    error = processMoreData();

    if (error) {
        goto cleanup;
    }

    return 0;

cleanup:
    freeResources();
    return 1;
}
```

**When goto is acceptable:**
- Error handling with cleanup
- Breaking out of nested loops
- State machines

**Clay doesn't use goto** - demonstrates clean error handling without it!

### 4.8 Key Concepts Learned
- ✅ if, else, else-if statements
- ✅ Nested conditionals
- ✅ switch-case statements
- ✅ break in switch
- ✅ Fall-through behavior
- ✅ goto statement (use carefully)

---

## Chapter 5: Loops {#chapter-5}

### 5.1 while Loop

Repeat while condition is true:

```c
int main(void) {
    int count = 0;

    while (count < 5) {
        printf("%d\n", count);
        count++;
    }
    // Output: 0 1 2 3 4

    return 0;
}
```

**Clay Example** (from clay.h):
```c
while (currentElement != NULL) {
    // Process element
    currentElement = currentElement->next;
}
```

### 5.2 do-while Loop

Execute at least once, then check condition:

```c
int main(void) {
    int count = 0;

    do {
        printf("%d\n", count);
        count++;
    } while (count < 5);
    // Output: 0 1 2 3 4

    // Even runs once if condition is false
    int x = 10;
    do {
        printf("Runs once\n");
    } while (x < 5);  // False, but body already executed

    return 0;
}
```

### 5.3 for Loop

Most common loop for counting:

```c
int main(void) {
    // for (initialization; condition; increment)
    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }
    // Output: 0 1 2 3 4

    // Multiple variables
    for (int i = 0, j = 10; i < j; i++, j--) {
        printf("i=%d, j=%d\n", i, j);
    }

    return 0;
}
```

**Clay Example** (from clay.h):
```c
for (int i = 0; i < elementCount; i++) {
    Clay_LayoutElement *element = &elements[i];
    // Process element
}

// Iterate backwards
for (int i = array->length - 1; i >= 0; i--) {
    // Process in reverse
}
```

### 5.4 break Statement

Exit loop early:

```c
int main(void) {
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            break;  // Exit loop when i is 5
        }
        printf("%d\n", i);
    }
    // Output: 0 1 2 3 4

    return 0;
}
```

**Clay Example** (from clay.h):
```c
for (int i = 0; i < maxIterations; i++) {
    if (found) {
        break;  // Stop searching
    }
    // Continue searching...
}
```

### 5.5 continue Statement

Skip to next iteration:

```c
int main(void) {
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            continue;  // Skip even numbers
        }
        printf("%d\n", i);
    }
    // Output: 1 3 5 7 9 (odd numbers only)

    return 0;
}
```

### 5.6 Nested Loops

Loops inside loops:

```c
int main(void) {
    // Print multiplication table
    for (int i = 1; i <= 5; i++) {
        for (int j = 1; j <= 5; j++) {
            printf("%3d ", i * j);
        }
        printf("\n");
    }

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Iterate through 2D structure
for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
        Clay_LayoutElement *cell = GetCell(row, col);
        // Process cell
    }
}
```

### 5.7 Breaking from Nested Loops

```c
int main(void) {
    int found = 0;

    for (int i = 0; i < 10 && !found; i++) {
        for (int j = 0; j < 10; j++) {
            if (i * j == 42) {
                found = 1;
                break;  // Only breaks inner loop
            }
        }
    }

    // Alternative with goto (acceptable here)
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i * j == 42) {
                goto found_it;  // Breaks both loops
            }
        }
    }
found_it:
    printf("Found it!\n");

    return 0;
}
```

### 5.8 Infinite Loops

```c
int main(void) {
    // Infinite loop - use break to exit
    while (1) {  // or for(;;) or while(true)
        int input = getInput();
        if (input == -1) {
            break;
        }
        processInput(input);
    }

    return 0;
}
```

### 5.9 Loop Patterns

**Count up:**
```c
for (int i = 0; i < n; i++) { }
```

**Count down:**
```c
for (int i = n - 1; i >= 0; i--) { }
```

**Step by 2:**
```c
for (int i = 0; i < n; i += 2) { }
```

**Iterate array:**
```c
for (int i = 0; i < arraySize; i++) {
    printf("%d\n", array[i]);
}
```

**Iterate with pointer:**
```c
for (int *p = array; p < array + arraySize; p++) {
    printf("%d\n", *p);
}
```

### 5.10 Key Concepts Learned
- ✅ while loops
- ✅ do-while loops
- ✅ for loops
- ✅ break statement
- ✅ continue statement
- ✅ Nested loops
- ✅ Infinite loops
- ✅ Common loop patterns

---

## Chapter 6: Functions {#chapter-6}

### 3.1 Function Basics

Functions organize code into reusable blocks:

```c
// Function declaration (prototype)
int add(int a, int b);

int main(void) {
    int result = add(5, 3);  // Call the function
    return 0;
}

// Function definition
int add(int a, int b) {
    return a + b;
}
```

**Parts of a function:**
- `int` - Return type
- `add` - Function name
- `(int a, int b)` - Parameters
- `{ ... }` - Function body

### 3.2 Void Functions

Functions that don't return a value:

```c
void printMessage(void) {
    printf("Hello!\n");
}

int main(void) {
    printMessage();  // No return value
    return 0;
}
```

**Clay Example** (from clay.h):
```c
void Clay_BeginLayout(void) {
    // Starts a new layout frame
    Clay__currentContext->layoutElementsHashMapInternal.length = 0;
    Clay__treeNodeVisited.length = 0;
    // ... more initialization
}
```

### 3.3 Function with Multiple Parameters

```c
float calculateArea(float width, float height) {
    return width * height;
}

int main(void) {
    float area = calculateArea(10.5f, 5.2f);
    return 0;
}
```

**Clay Example** (from clay.h):
```c
Clay_Dimensions Clay__MeasureTextCached(
    Clay_String *text,
    Clay_TextElementConfig *config
) {
    // Custom text measurement function
    Clay_Dimensions dimensions = {0};
    // ... measurement logic
    return dimensions;
}
```

### 3.4 Static Functions (Internal Linkage)

`static` makes functions private to a file:

```c
// Only visible in this file
static int helperFunction(int x) {
    return x * 2;
}

int publicFunction(int x) {
    return helperFunction(x);  // Can use it here
}
```

**Clay Example** (from clay.h):
```c
// Internal function, not exposed to users
static inline Clay_BoundingBox Clay__HashMapElementBoundingBox(
    int32_t index
) {
    // Implementation details hidden from API users
}
```

### 3.5 Inline Functions

`inline` suggests the compiler to insert code directly (faster):

```c
static inline int max(int a, int b) {
    return (a > b) ? a : b;
}
```

**Clay Example** (from clay.h):
```c
static inline float Clay__Min(float a, float b) {
    return a < b ? a : b;
}

static inline float Clay__Max(float a, float b) {
    return a > b ? a : b;
}
```

### 3.6 Key Concepts Learned
- ✅ Function declaration vs definition
- ✅ Parameters and return values
- ✅ void functions
- ✅ static keyword for internal functions
- ✅ inline optimization hint

---

## Chapter 4: Pointers - The Heart of C {#chapter-4}

### 4.1 What is a Pointer?

A pointer stores a memory address:

```c
int main(void) {
    int age = 25;        // Regular variable
    int *ptr = &age;     // Pointer to age

    // ptr holds the address of age
    // *ptr accesses the value at that address

    printf("age = %d\n", age);      // 25
    printf("&age = %p\n", &age);    // Address (e.g., 0x7fff5fbff5ac)
    printf("ptr = %p\n", ptr);      // Same address
    printf("*ptr = %d\n", *ptr);    // 25 (dereferencing)

    return 0;
}
```

**Key operators:**
- `&` - Address-of operator (get address)
- `*` - Dereference operator (get value at address)

### 4.2 Pointer Syntax

```c
int x = 10;
int *p;      // Declare pointer to int
p = &x;      // p now points to x
*p = 20;     // Changes x to 20 through pointer

float y = 3.14f;
float *fp = &y;  // Pointer to float

char c = 'A';
char *cp = &c;   // Pointer to char
```

### 4.3 Pointers as Function Parameters

Pass by reference to modify variables:

```c
// Pass by value (doesn't modify original)
void incrementValue(int x) {
    x = x + 1;  // Only modifies local copy
}

// Pass by pointer (modifies original)
void incrementPointer(int *x) {
    *x = *x + 1;  // Modifies original through pointer
}

int main(void) {
    int num = 5;

    incrementValue(num);
    printf("%d\n", num);  // Still 5

    incrementPointer(&num);
    printf("%d\n", num);  // Now 6

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Takes pointer to modify the element
void Clay__OpenElement(void) {
    Clay_LayoutElement *openLayoutElement =
        Clay__LayoutElementPointerArray_Get(
            &Clay__currentContext->layoutElements,
            Clay__currentContext->layoutElements.length++
        );
    // Modifies element through pointer
}
```

### 4.4 NULL Pointers

A pointer to nothing:

```c
int *p = NULL;  // Points to nothing (address 0)

if (p == NULL) {
    printf("Pointer is null\n");
}

// Don't dereference NULL pointers!
// *p = 5;  // CRASH!
```

**Clay Example** (from clay.h):
```c
typedef struct {
    Clay_ElementId elementId;
    Clay_LayoutElement *layoutElement;  // Can be NULL
    Clay_BoundingBox boundingBox;
} Clay__LayoutElementTreeNode;

// Check before use
if (layoutElement != NULL) {
    // Safe to use
}
```

### 4.5 Pointer Arithmetic

Pointers can be incremented/decremented:

```c
int arr[5] = {10, 20, 30, 40, 50};
int *p = arr;  // Points to first element

printf("%d\n", *p);      // 10
p++;                     // Move to next element
printf("%d\n", *p);      // 20
p += 2;                  // Move 2 elements forward
printf("%d\n", *p);      // 40
```

**Clay Example** (from clay.h):
```c
// Iterate through array with pointer
char *chars = string.chars;
for (int i = 0; i < string.length; i++) {
    char current = chars[i];  // or *(chars + i)
    // Process character
}
```

### 4.6 Key Concepts Learned
- ✅ Pointers store memory addresses
- ✅ & (address-of) and * (dereference) operators
- ✅ Pass by pointer to modify variables
- ✅ NULL pointers
- ✅ Pointer arithmetic

---

## Chapter 5: Structs and Typedef {#chapter-5}

### 5.1 Structs - Grouping Related Data

Structs group multiple variables together:

```c
struct Person {
    char name[50];
    int age;
    float height;
};

int main(void) {
    struct Person john;
    john.age = 30;
    john.height = 5.9f;

    printf("Age: %d\n", john.age);
    return 0;
}
```

### 5.2 Typedef - Creating Type Aliases

`typedef` creates shorter names:

```c
// Without typedef
struct Person {
    char name[50];
    int age;
};
struct Person john;  // Must write "struct"

// With typedef
typedef struct {
    char name[50];
    int age;
} Person;

Person john;  // Cleaner!
```

**Clay Example** (from clay.h):
```c
typedef struct {
    float x, y;
} Clay_Vector2;

typedef struct {
    float width, height;
} Clay_Dimensions;

typedef struct {
    float r, g, b, a;
} Clay_Color;
```

### 5.3 Nested Structs

Structs can contain other structs:

```c
typedef struct {
    float x, y;
} Point;

typedef struct {
    Point topLeft;
    Point bottomRight;
} Rectangle;

int main(void) {
    Rectangle rect;
    rect.topLeft.x = 0.0f;
    rect.topLeft.y = 0.0f;
    rect.bottomRight.x = 100.0f;
    rect.bottomRight.y = 50.0f;

    return 0;
}
```

**Clay Example** (from clay.h):
```c
typedef struct {
    Clay_Vector2 x, y;  // Nested struct
} Clay_BoundingBox;

typedef struct {
    Clay_BoundingBox boundingBox;  // Nested
    Clay_Dimensions dimensions;    // Nested
    Clay_LayoutConfig config;      // Nested
} Clay_LayoutElement;
```

### 5.4 Struct Initialization

Multiple ways to initialize structs:

```c
typedef struct {
    int x;
    int y;
    int z;
} Point3D;

// Method 1: Member by member
Point3D p1;
p1.x = 1;
p1.y = 2;
p1.z = 3;

// Method 2: Initializer list (order matters)
Point3D p2 = {1, 2, 3};

// Method 3: Designated initializers (C99+)
Point3D p3 = {.x = 1, .y = 2, .z = 3};
Point3D p4 = {.z = 3, .x = 1, .y = 2};  // Order doesn't matter!

// Partial initialization (rest = 0)
Point3D p5 = {.x = 1};  // y=0, z=0
```

**Clay Example** (from clay.h):
```c
// Designated initializers for clean API
Clay_Color white = {.r = 255, .g = 255, .b = 255, .a = 255};

Clay_Dimensions size = {.width = 100, .height = 50};

Clay_BoundingBox box = {
    .x = {.min = 0, .max = 100},
    .y = {.min = 0, .max = 50}
};
```

### 5.5 Pointers to Structs

Use `->` to access members through pointers:

```c
typedef struct {
    int x, y;
} Point;

int main(void) {
    Point p = {10, 20};
    Point *ptr = &p;

    // Two ways to access through pointer:
    (*ptr).x = 30;   // Dereference then access
    ptr->y = 40;     // Arrow operator (cleaner)

    printf("(%d, %d)\n", p.x, p.y);  // (30, 40)
    return 0;
}
```

**Clay Example** (from clay.h):
```c
Clay_LayoutElement *element = GetLayoutElement();

// Access members with ->
element->dimensions.width = 100;
element->dimensions.height = 50;
element->childrenOrTextContent.children.length = 0;
```

### 5.6 Forward Declarations

Declare struct name before definition:

```c
// Forward declaration
typedef struct Clay_Context Clay_Context;

// Can now use Clay_Context* in other structs
typedef struct {
    Clay_Context *context;
} SomeStruct;

// Full definition later
struct Clay_Context {
    int32_t maxElementCount;
    // ... other members
};
```

**Clay Example** (from clay.h:287):
```c
typedef struct Clay_Context Clay_Context;

// Used before full definition
Clay_Context* Clay_GetCurrentContext(void);
```

### 5.7 Key Concepts Learned
- ✅ struct for grouping data
- ✅ typedef for type aliases
- ✅ Nested structs
- ✅ Designated initializers
- ✅ -> operator for pointer member access
- ✅ Forward declarations

---

## Chapter 6: Arrays and Memory {#chapter-6}

### 6.1 Static Arrays

Fixed-size arrays declared at compile time:

```c
int main(void) {
    int numbers[5];           // Array of 5 ints
    numbers[0] = 10;          // First element
    numbers[4] = 50;          // Last element

    // Initialize on declaration
    int values[5] = {1, 2, 3, 4, 5};

    // Partial initialization (rest = 0)
    int zeros[10] = {0};  // All zeros

    // Size inferred from initializer
    int items[] = {10, 20, 30};  // Size = 3

    return 0;
}
```

### 6.2 Array and Pointer Relationship

**Important**: Array names decay to pointers!

```c
int main(void) {
    int arr[5] = {10, 20, 30, 40, 50};

    int *p = arr;  // arr decays to pointer to first element

    printf("%d\n", arr[0]);   // 10
    printf("%d\n", *arr);     // 10 (same)
    printf("%d\n", *(arr+1)); // 20
    printf("%d\n", arr[1]);   // 20 (same)

    return 0;
}
```

### 6.3 Passing Arrays to Functions

Arrays are always passed as pointers:

```c
// These are equivalent:
void processArray1(int arr[], int size) { }
void processArray2(int *arr, int size) { }

int main(void) {
    int numbers[5] = {1, 2, 3, 4, 5};

    // Must pass size separately!
    processArray1(numbers, 5);
    processArray2(numbers, 5);

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Takes pointer and length
static inline void Clay__MeasureTextCached(
    Clay_String *text,  // Pointer to string (char array)
    Clay_TextElementConfig *config
) {
    // text->chars is char array
    // text->length is size
}
```

### 6.4 Multidimensional Arrays

```c
int main(void) {
    // 2D array: 3 rows, 4 columns
    int matrix[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    printf("%d\n", matrix[0][0]);  // 1
    printf("%d\n", matrix[1][2]);  // 7
    printf("%d\n", matrix[2][3]);  // 12

    return 0;
}
```

### 6.5 Flexible Array Members

Last member of struct can be flexible:

```c
typedef struct {
    int length;
    int items[];  // Flexible array (must be last)
} DynamicArray;

// Allocate with specific size
DynamicArray *arr = malloc(sizeof(DynamicArray) + 10 * sizeof(int));
arr->length = 10;
arr->items[0] = 100;
```

**Clay Example** (from clay.h):
```c
typedef struct {
    int32_t capacity;
    int32_t length;
    Clay_ElementId *internalArray;  // Pointer acts like flexible array
} Clay__ElementIdArray;
```

### 6.6 String Arrays

Strings in C are char arrays ending with '\0':

```c
int main(void) {
    // String literal
    char *str1 = "Hello";  // Points to read-only memory

    // Char array
    char str2[] = "Hello";  // Mutable, size = 6 (includes \0)

    // Explicit size
    char str3[10] = "Hello";  // Rest filled with \0

    // Character by character
    char str4[6] = {'H', 'e', 'l', 'l', 'o', '\0'};

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Clay uses explicit length instead of \0
typedef struct {
    int32_t length;
    const char *chars;  // Not null-terminated!
} Clay_String;

// Macro to create from string literal
#define CLAY_STRING(stringContents) \
    (Clay_String) { .length = sizeof(stringContents) - 1, .chars = stringContents }
```

### 6.7 sizeof with Arrays

```c
int main(void) {
    int arr[10];

    size_t arrayBytes = sizeof(arr);        // 40 (10 * 4)
    size_t elementBytes = sizeof(arr[0]);   // 4
    size_t arrayLength = sizeof(arr) / sizeof(arr[0]);  // 10

    // Warning: This doesn't work with pointers!
    int *p = arr;
    size_t pointerSize = sizeof(p);  // 8 (on 64-bit), not 40!

    return 0;
}
```

### 6.8 Key Concepts Learned
- ✅ Static array declaration
- ✅ Array initialization
- ✅ Arrays decay to pointers
- ✅ Passing arrays to functions
- ✅ Multidimensional arrays
- ✅ C strings (null-terminated char arrays)
- ✅ sizeof with arrays

---

## Chapter 7: Preprocessor and Macros {#chapter-7}

### 7.1 What is the Preprocessor?

The preprocessor runs BEFORE compilation and performs text substitution:

```c
#include <stdio.h>   // Insert file contents
#define MAX 100      // Text replacement

int main(void) {
    int arr[MAX];    // Becomes: int arr[100];
    return 0;
}
```

### 7.2 #define - Simple Macros

```c
#define PI 3.14159
#define MAX_SIZE 1000
#define PROGRAM_NAME "MyApp"

int main(void) {
    float radius = 5.0f;
    float area = PI * radius * radius;  // PI replaced with 3.14159
    return 0;
}
```

**Clay Example** (from clay.h:102):
```c
#define CLAY_VERSION_MAJOR 0
#define CLAY_VERSION_MINOR 12
#define CLAY_VERSION_PATCH 0
```

### 7.3 #ifdef and Conditional Compilation

```c
#define DEBUG

int main(void) {
    #ifdef DEBUG
        printf("Debug mode enabled\n");
    #endif

    #ifndef RELEASE
        printf("Not release mode\n");
    #endif

    return 0;
}
```

**Clay Example** (from clay.h:82):
```c
#ifndef CLAY_HEADER
#define CLAY_HEADER

// Header contents here...

#endif // CLAY_HEADER
```

This prevents multiple inclusion of the same header.

### 7.4 Function-like Macros

Macros can take arguments:

```c
#define SQUARE(x) ((x) * (x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void) {
    int result = SQUARE(5);     // (5) * (5) = 25
    int max = MAX(10, 20);      // 20

    // Warning: Be careful!
    int bad = SQUARE(2 + 3);    // (2 + 3) * (2 + 3) = 25 ✓
    // Without parentheses would be: 2 + 3 * 2 + 3 = 11 ✗

    return 0;
}
```

**Clay Example** (from clay.h:111):
```c
#define CLAY__MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CLAY__MIN(x, y) (((x) < (y)) ? (x) : (y))
```

### 7.5 Multi-line Macros

Use backslash to continue lines:

```c
#define SWAP(a, b) \
    do { \
        typeof(a) temp = a; \
        a = b; \
        b = temp; \
    } while(0)

int main(void) {
    int x = 5, y = 10;
    SWAP(x, y);
    printf("%d %d\n", x, y);  // 10 5
    return 0;
}
```

**Why `do { } while(0)`?** It ensures the macro behaves like a statement:

```c
if (condition)
    SWAP(x, y);  // Works correctly
else
    other();
```

**Clay Example** (from clay.h):
```c
#define CLAY__ASSERT(condition) \
    if (!(condition)) { \
        Clay__currentContext->errorHandler.errorHandlerFunction( \
            CLAY__INIT(Clay_ErrorData) { \
                .errorType = CLAY_ERROR_TYPE_ASSERTION_FAILED, \
                .errorText = CLAY_STRING("Assertion failed") \
            } \
        ); \
    }
```

### 7.6 Stringification (#)

Convert macro argument to string:

```c
#define TO_STRING(x) #x

int main(void) {
    printf("%s\n", TO_STRING(Hello));  // "Hello"
    printf("%s\n", TO_STRING(123));    // "123"
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
// Results in "0.12.0"
```

### 7.7 Token Pasting (##)

Concatenate tokens:

```c
#define CONCAT(a, b) a##b

int main(void) {
    int xy = 100;
    int value = CONCAT(x, y);  // Becomes: xy
    printf("%d\n", value);     // 100
    return 0;
}
```

**Clay Example** (from clay.h):
```c
#define CLAY__ARRAY_DEFINE(typeName, arrayName) \
    typedef struct { \
        int32_t capacity; \
        int32_t length; \
        typeName *internalArray; \
    } arrayName;

// Creates type: Clay__int32_tArray
CLAY__ARRAY_DEFINE(int32_t, Clay__int32_tArray)
```

### 7.8 Predefined Macros

C provides built-in macros:

```c
#include <stdio.h>

int main(void) {
    printf("File: %s\n", __FILE__);      // Current file name
    printf("Line: %d\n", __LINE__);      // Current line number
    printf("Date: %s\n", __DATE__);      // Compilation date
    printf("Time: %s\n", __TIME__);      // Compilation time
    printf("Function: %s\n", __func__);  // Current function name (C99)

    return 0;
}
```

### 7.9 Header Guards

Prevent multiple inclusion:

```c
// myheader.h
#ifndef MYHEADER_H
#define MYHEADER_H

// Header contents here

#endif // MYHEADER_H
```

**Clay Example** (from clay.h:82):
```c
#ifndef CLAY_HEADER
#define CLAY_HEADER

// All Clay declarations

#endif // CLAY_HEADER
```

### 7.10 Key Concepts Learned
- ✅ Preprocessor runs before compilation
- ✅ #define for constants and macros
- ✅ #ifdef, #ifndef for conditional compilation
- ✅ Function-like macros with parameters
- ✅ Multi-line macros with backslash
- ✅ # for stringification
- ✅ ## for token pasting
- ✅ Header guards

---

## Chapter 8: Advanced Macros and Metaprogramming {#chapter-8}

### 8.1 Variadic Macros

Macros that accept variable number of arguments:

```c
#define LOG(format, ...) \
    printf("[LOG] " format "\n", __VA_ARGS__)

int main(void) {
    LOG("Value: %d", 42);
    LOG("X: %d, Y: %d", 10, 20);
    return 0;
}
```

**`__VA_ARGS__`** represents all extra arguments.

**Clay Example** (from clay.h):
```c
#define CLAY__INIT(type) \
    (type)

// Usage with designated initializers
Clay_Color color = CLAY__INIT(Clay_Color) {
    .r = 255, .g = 0, .b = 0, .a = 255
};
```

### 8.2 X-Macros Pattern

Generate code from data:

```c
// Define data once
#define COLOR_LIST \
    X(RED, 0xFF0000) \
    X(GREEN, 0x00FF00) \
    X(BLUE, 0x0000FF)

// Generate enum
enum Colors {
    #define X(name, value) COLOR_##name,
    COLOR_LIST
    #undef X
};

// Generate array
const char* colorNames[] = {
    #define X(name, value) #name,
    COLOR_LIST
    #undef X
};

int main(void) {
    printf("%s\n", colorNames[COLOR_RED]);  // "RED"
    return 0;
}
```

### 8.3 Generic Macros with _Generic (C11)

Type-based selection at compile time:

```c
#define max(a, b) _Generic((a), \
    int: max_int, \
    float: max_float, \
    double: max_double \
)(a, b)

int max_int(int a, int b) { return a > b ? a : b; }
float max_float(float a, float b) { return a > b ? a : b; }
double max_double(double a, double b) { return a > b ? a : b; }

int main(void) {
    int i = max(5, 10);          // Calls max_int
    float f = max(5.5f, 10.2f);  // Calls max_float
    return 0;
}
```

### 8.4 Compound Literals

Create temporary struct values:

```c
typedef struct {
    int x, y;
} Point;

void printPoint(Point p) {
    printf("(%d, %d)\n", p.x, p.y);
}

int main(void) {
    // Compound literal
    printPoint((Point){.x = 10, .y = 20});

    // Another example
    Point *p = &(Point){.x = 5, .y = 15};

    return 0;
}
```

**Clay Example** (from clay.h):
```c
// Macro uses compound literals for clean API
#define CLAY_COLOR(r, g, b, a) \
    (Clay_Color) {.r = r, .g = b, .b = b, .a = a}

// Usage
Clay_Color red = CLAY_COLOR(255, 0, 0, 255);
```

### 8.5 Statement Expressions (GCC Extension)

Create macros that return values:

```c
#define MAX(a, b) ({ \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    _a > _b ? _a : _b; \
})

int main(void) {
    int x = MAX(5 + 2, 3 + 4);  // Evaluates each expression once
    return 0;
}
```

### 8.6 The For-Loop Macro Trick

Create scope-based macros:

```c
#define WITH_LOCK(mutex) \
    for (int _i = (lock(mutex), 0); _i < 1; _i++, unlock(mutex))

// Usage
WITH_LOCK(&myMutex) {
    // Critical section
    // mutex automatically unlocked when block exits
}
```

**Clay Example** (from clay.h:2016) - This is ADVANCED:
```c
#define CLAY(...) \
    for ( \
        CLAY__ELEMENT_DEFINITION_LATCH = ( \
            Clay__OpenElement(), \
            Clay__ConfigureOpenElement(__VA_ARGS__), \
            0 \
        ); \
        CLAY__ELEMENT_DEFINITION_LATCH < 1; \
        CLAY__ELEMENT_DEFINITION_LATCH = 1, Clay__CloseElement() \
    )

// Usage creates automatic open/close pairs
CLAY({ .layout = { .padding = CLAY_PADDING_ALL(8) } }) {
    // Child elements
}
// Automatically closes element
```

**How it works:**
1. **Init**: `Clay__OpenElement()`, configure, set latch to 0
2. **Condition**: `latch < 1` is true (0 < 1), enter loop
3. **Body**: User code executes
4. **Increment**: Set latch to 1, `Clay__CloseElement()`
5. **Condition**: `latch < 1` is false (1 < 1), exit loop

This ensures `CloseElement` is always called!

### 8.7 Designated Initializers in Macros

```c
#define CREATE_RECT(w, h) \
    (Rectangle) { \
        .width = (w), \
        .height = (h), \
        .x = 0, \
        .y = 0 \
    }

int main(void) {
    Rectangle r = CREATE_RECT(100, 50);
    return 0;
}
```

**Clay Example** (from clay.h):
```c
#define CLAY_LAYOUT(...) \
    (Clay_LayoutConfig) { __VA_ARGS__ }

#define CLAY_SIZING_FIT(min, max) \
    (Clay_Sizing) { \
        .type = CLAY_SIZING_TYPE_FIT, \
        .size = { .minMax = { .min = (min), .max = (max) } } \
    }

// Usage
Clay_LayoutConfig layout = CLAY_LAYOUT(
    .sizing = { .width = CLAY_SIZING_FIT(100, 500) }
);
```

### 8.8 Macro Debugging Tips

```c
// Use #pragma to see macro expansion
#define COMPLEX_MACRO(x) ((x) * 2 + 1)

int main(void) {
    #pragma message "COMPLEX_MACRO(5) expands to:"
    int result = COMPLEX_MACRO(5);
    return 0;
}

// Compile with gcc -E to see preprocessor output
```

### 8.9 Key Concepts Learned
- ✅ Variadic macros (__VA_ARGS__)
- ✅ X-Macros for code generation
- ✅ _Generic for type-based selection
- ✅ Compound literals
- ✅ Statement expressions
- ✅ For-loop macro trick
- ✅ Designated initializers in macros

---

## Chapter 9: Memory Management {#chapter-9}

### 9.1 Stack vs Heap

**Stack Memory:**
- Automatic allocation/deallocation
- Fast
- Limited size
- Variables disappear when function returns

```c
void function(void) {
    int x = 10;        // On stack
    char str[100];     // On stack
}  // x and str are destroyed here
```

**Heap Memory:**
- Manual allocation (malloc) and deallocation (free)
- Slower
- Large size available
- Persists until freed

```c
#include <stdlib.h>

void function(void) {
    int *p = malloc(sizeof(int));  // On heap
    *p = 10;
    free(p);  // Must manually free!
}
```

### 9.2 malloc, calloc, realloc, free

```c
#include <stdlib.h>

int main(void) {
    // malloc - allocate uninitialized memory
    int *arr1 = malloc(10 * sizeof(int));
    if (arr1 == NULL) {
        // Allocation failed!
        return 1;
    }

    // calloc - allocate zero-initialized memory
    int *arr2 = calloc(10, sizeof(int));  // All zeros

    // realloc - resize allocated memory
    arr1 = realloc(arr1, 20 * sizeof(int));

    // free - deallocate memory
    free(arr1);
    free(arr2);

    return 0;
}
```

### 9.3 Common Memory Errors

```c
// 1. Memory leak - allocated but never freed
void leak(void) {
    int *p = malloc(sizeof(int));
    // Forgot to free(p)!
}

// 2. Use after free
void useAfterFree(void) {
    int *p = malloc(sizeof(int));
    free(p);
    *p = 10;  // DANGEROUS! Undefined behavior
}

// 3. Double free
void doubleFree(void) {
    int *p = malloc(sizeof(int));
    free(p);
    free(p);  // CRASH!
}

// 4. Accessing uninitialized memory
void uninit(void) {
    int *p = malloc(sizeof(int));
    printf("%d\n", *p);  // Random value!
    free(p);
}
```

### 9.4 Arena Allocators

Instead of many malloc/free calls, allocate one large block:

```c
typedef struct {
    char *memory;
    size_t size;
    size_t used;
} Arena;

void Arena_Init(Arena *arena, size_t size) {
    arena->memory = malloc(size);
    arena->size = size;
    arena->used = 0;
}

void* Arena_Alloc(Arena *arena, size_t size) {
    if (arena->used + size > arena->size) {
        return NULL;  // Out of memory
    }
    void *ptr = arena->memory + arena->used;
    arena->used += size;
    return ptr;
}

void Arena_Free(Arena *arena) {
    free(arena->memory);  // Free everything at once
}

int main(void) {
    Arena arena;
    Arena_Init(&arena, 1024 * 1024);  // 1MB

    int *arr1 = Arena_Alloc(&arena, 100 * sizeof(int));
    float *arr2 = Arena_Alloc(&arena, 50 * sizeof(float));

    // Use allocations...

    Arena_Free(&arena);  // Free everything
    return 0;
}
```

**Clay Example** (from clay.h:185):
```c
typedef struct {
    uintptr_t nextAllocation;
    size_t capacity;
    char *memory;
} Clay_Arena;

// Clay allocates everything from arenas - no malloc in hot path!
void* Clay__Array_Allocate_Arena(
    int32_t capacity,
    uint32_t itemSize,
    Clay_Arena *arena
) {
    size_t totalSizeBytes = capacity * itemSize;
    uintptr_t nextAllocation = arena->nextAllocation + totalSizeBytes;

    if (nextAllocation <= arena->capacity) {
        void *allocation = (void*)(arena->memory + arena->nextAllocation);
        arena->nextAllocation = nextAllocation;
        return allocation;
    }

    return NULL;  // Out of memory
}
```

### 9.5 Memory Alignment

CPUs prefer aligned memory access:

```c
#include <stdint.h>

// Proper alignment
struct Aligned {
    uint64_t a;  // 8 bytes, aligned to 8
    uint32_t b;  // 4 bytes
    uint32_t c;  // 4 bytes
};  // Total: 16 bytes

// Poor alignment - compiler adds padding
struct Unaligned {
    uint8_t a;   // 1 byte
    // 7 bytes padding
    uint64_t b;  // 8 bytes
    uint8_t c;   // 1 byte
    // 7 bytes padding
};  // Total: 24 bytes instead of 10!
```

**Clay Example** (from clay.h):
```c
// Careful struct layout for performance
typedef struct {
    float width, height;  // 8 bytes total
} Clay_Dimensions;  // Aligned to 4 bytes

typedef struct {
    Clay_Vector2 x, y;    // 16 bytes total
} Clay_BoundingBox;  // Aligned to 4 bytes
```

### 9.6 Memory Pools

Pre-allocate objects of same size:

```c
#define POOL_SIZE 100

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node nodes[POOL_SIZE];
    Node *freeList;
} NodePool;

void Pool_Init(NodePool *pool) {
    // Chain all nodes into free list
    for (int i = 0; i < POOL_SIZE - 1; i++) {
        pool->nodes[i].next = &pool->nodes[i + 1];
    }
    pool->nodes[POOL_SIZE - 1].next = NULL;
    pool->freeList = &pool->nodes[0];
}

Node* Pool_Alloc(NodePool *pool) {
    if (pool->freeList == NULL) return NULL;

    Node *node = pool->freeList;
    pool->freeList = node->next;
    return node;
}

void Pool_Free(NodePool *pool, Node *node) {
    node->next = pool->freeList;
    pool->freeList = node;
}
```

### 9.7 Key Concepts Learned
- ✅ Stack vs heap memory
- ✅ malloc, calloc, realloc, free
- ✅ Common memory errors
- ✅ Arena allocators
- ✅ Memory alignment
- ✅ Memory pools

---

## Chapter 10: Header Files and Project Organization {#chapter-10}

### 10.1 Header Files Basics

**Header file (.h)**: Declarations (interface)
**Source file (.c)**: Definitions (implementation)

```c
// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

int add(int a, int b);        // Declaration only
int multiply(int a, int b);

#endif
```

```c
// math_utils.c
#include "math_utils.h"

int add(int a, int b) {       // Definition
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}
```

```c
// main.c
#include "math_utils.h"

int main(void) {
    int result = add(5, 3);
    return 0;
}
```

### 10.2 Header Guards

Prevent multiple inclusion:

```c
// myheader.h
#ifndef MYHEADER_H
#define MYHEADER_H

// Declarations

#endif // MYHEADER_H
```

Alternative (non-standard but widely supported):
```c
#pragma once

// Declarations
```

### 10.3 Include Order

Best practice:

```c
// In myfile.c:

// 1. Corresponding header
#include "myfile.h"

// 2. System headers
#include <stdio.h>
#include <stdlib.h>

// 3. Third-party headers
#include "external_lib.h"

// 4. Project headers
#include "project_utils.h"
```

### 10.4 Single-Header Library Pattern

Entire library in one header file:

```c
// mylib.h
#ifndef MYLIB_H
#define MYLIB_H

// Declarations (always included)
void myFunction(void);

// Implementation (included only once)
#ifdef MYLIB_IMPLEMENTATION

void myFunction(void) {
    // Implementation here
}

#endif // MYLIB_IMPLEMENTATION
#endif // MYLIB_H
```

**Usage:**
```c
// In ONE .c file:
#define MYLIB_IMPLEMENTATION
#include "mylib.h"

// In other files:
#include "mylib.h"
```

**Clay Example** (clay.h structure):
```c
#ifndef CLAY_HEADER
#define CLAY_HEADER

// ===== PUBLIC API DECLARATIONS =====
typedef struct { /* ... */ } Clay_Dimensions;
void Clay_BeginLayout(void);
// ... more declarations

// ===== IMPLEMENTATION =====
#ifdef CLAY_IMPLEMENTATION

// All implementation code here
void Clay_BeginLayout(void) {
    // ...
}

#endif // CLAY_IMPLEMENTATION
#endif // CLAY_HEADER
```

### 10.5 Forward Declarations

Declare types before full definition:

```c
// Forward declaration
typedef struct Node Node;

typedef struct {
    Node *next;  // Can use pointer to Node
} List;

// Full definition later
struct Node {
    int value;
    Node *next;
};
```

**Clay Example** (from clay.h:287):
```c
typedef struct Clay_Context Clay_Context;

// Can now use Clay_Context* in functions
Clay_Context* Clay_GetCurrentContext(void);

// Full definition comes later (line 1900+)
struct Clay_Context {
    // ...
};
```

### 10.6 Opaque Pointers

Hide implementation details:

```c
// widget.h
typedef struct Widget Widget;  // Opaque type

Widget* Widget_Create(void);
void Widget_Destroy(Widget *w);
void Widget_SetValue(Widget *w, int value);
```

```c
// widget.c
struct Widget {  // Full definition only in .c file
    int value;
    int internal_state;
};

Widget* Widget_Create(void) {
    Widget *w = malloc(sizeof(Widget));
    w->value = 0;
    w->internal_state = 0;
    return w;
}
```

Users can't access internal members directly!

### 10.7 Conditional Compilation for Platforms

```c
#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms) * 1000)
#endif

int main(void) {
    SLEEP(1000);  // Sleep 1 second on any platform
    return 0;
}
```

**Clay Example** (from clay.h:88):
```c
#if defined(_MSC_VER)
    #define CLAY_WASM __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
    #define CLAY_WASM __attribute__((visibility("default")))
#else
    #define CLAY_WASM
#endif
```

### 10.8 Key Concepts Learned
- ✅ Header vs source files
- ✅ Header guards
- ✅ Include order best practices
- ✅ Single-header library pattern
- ✅ Forward declarations
- ✅ Opaque pointers
- ✅ Platform-specific code

---

## Chapter 11: Enums and Unions {#chapter-11}

### 11.1 Enums - Named Constants

```c
enum Color {
    COLOR_RED,      // 0
    COLOR_GREEN,    // 1
    COLOR_BLUE      // 2
};

int main(void) {
    enum Color myColor = COLOR_RED;

    if (myColor == COLOR_RED) {
        printf("Red!\n");
    }

    return 0;
}
```

### 11.2 Enums with Typedef

```c
typedef enum {
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE
} Color;

Color myColor = COLOR_RED;  // Cleaner syntax
```

### 11.3 Custom Enum Values

```c
typedef enum {
    ERROR_NONE = 0,
    ERROR_FILE_NOT_FOUND = 1,
    ERROR_PERMISSION_DENIED = 2,
    ERROR_OUT_OF_MEMORY = 100,
    ERROR_UNKNOWN = -1
} ErrorCode;
```

**Clay Example** (from clay.h:208):
```c
typedef enum {
    CLAY_SIZING_TYPE_FIT,
    CLAY_SIZING_TYPE_GROW,
    CLAY_SIZING_TYPE_PERCENT,
    CLAY_SIZING_TYPE_FIXED
} Clay_SizingType;

typedef enum {
    CLAY_LAYOUT_DIRECTION_LEFT_TO_RIGHT,
    CLAY_LAYOUT_DIRECTION_TOP_TO_BOTTOM
} Clay_LayoutDirection;
```

### 11.4 Enum Flags (Bit Flags)

```c
typedef enum {
    FLAG_NONE     = 0,      // 0000
    FLAG_READ     = 1 << 0, // 0001
    FLAG_WRITE    = 1 << 1, // 0010
    FLAG_EXECUTE  = 1 << 2, // 0100
    FLAG_ADMIN    = 1 << 3  // 1000
} Permissions;

int main(void) {
    Permissions perms = FLAG_READ | FLAG_WRITE;  // Combine flags

    if (perms & FLAG_READ) {
        printf("Can read\n");
    }

    perms |= FLAG_EXECUTE;   // Add execute
    perms &= ~FLAG_WRITE;    // Remove write

    return 0;
}
```

**Clay Example** (from clay.h:252):
```c
typedef enum {
    CLAY_CORNER_RADIUS_NONE = 0,
    CLAY_CORNER_RADIUS_TOP_LEFT = 1 << 0,
    CLAY_CORNER_RADIUS_TOP_RIGHT = 1 << 1,
    CLAY_CORNER_RADIUS_BOTTOM_LEFT = 1 << 2,
    CLAY_CORNER_RADIUS_BOTTOM_RIGHT = 1 << 3,
    CLAY_CORNER_RADIUS_ALL =
        CLAY_CORNER_RADIUS_TOP_LEFT |
        CLAY_CORNER_RADIUS_TOP_RIGHT |
        CLAY_CORNER_RADIUS_BOTTOM_LEFT |
        CLAY_CORNER_RADIUS_BOTTOM_RIGHT
} Clay_CornerRadiusSet;
```

### 11.5 Unions - Same Memory, Different Types

```c
union Data {
    int i;
    float f;
    char str[20];
};

int main(void) {
    union Data data;

    data.i = 10;
    printf("%d\n", data.i);    // 10

    data.f = 3.14f;            // Overwrites i
    printf("%f\n", data.f);    // 3.14
    // data.i is now garbage!

    printf("Size: %zu\n", sizeof(data));  // 20 (largest member)

    return 0;
}
```

### 11.6 Tagged Unions

Track which union member is active:

```c
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING
} ValueType;

typedef struct {
    ValueType type;  // Tag
    union {
        int i;
        float f;
        char *s;
    } data;
} Value;

int main(void) {
    Value v;

    v.type = TYPE_INT;
    v.data.i = 42;

    // Safe access based on tag
    switch (v.type) {
        case TYPE_INT:
            printf("Int: %d\n", v.data.i);
            break;
        case TYPE_FLOAT:
            printf("Float: %f\n", v.data.f);
            break;
        case TYPE_STRING:
            printf("String: %s\n", v.data.s);
            break;
    }

    return 0;
}
```

**Clay Example** (from clay.h:213):
```c
typedef struct {
    Clay_SizingType type;  // Tag
    union {
        float sizeMinMax;
        struct {
            float min;
            float max;
        } minMax;
        float sizePercent;
    } size;
} Clay_Sizing;

// Usage
Clay_Sizing sizing;
sizing.type = CLAY_SIZING_TYPE_FIXED;
sizing.size.sizeMinMax = 100.0f;  // Safe because type is FIXED
```

### 11.7 Anonymous Unions and Structs (C11)

```c
typedef struct {
    enum { TYPE_INT, TYPE_FLOAT } type;
    union {  // Anonymous union
        int i;
        float f;
    };  // No name!
} Value;

int main(void) {
    Value v;
    v.type = TYPE_INT;
    v.i = 42;  // Access directly, not v.data.i

    return 0;
}
```

### 11.8 Key Concepts Learned
- ✅ Enums for named constants
- ✅ Typedef with enums
- ✅ Bit flags with enums
- ✅ Unions for memory-efficient storage
- ✅ Tagged unions for type safety
- ✅ Anonymous unions

---

## Chapter 12: Function Pointers and Callbacks {#chapter-12}

### 12.1 Function Pointer Basics

Functions have addresses too!

```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main(void) {
    // Function pointer syntax: return_type (*name)(params)
    int (*operation)(int, int);

    operation = add;
    printf("%d\n", operation(5, 3));  // 8

    operation = multiply;
    printf("%d\n", operation(5, 3));  // 15

    return 0;
}
```

### 12.2 Typedef with Function Pointers

Make syntax cleaner:

```c
// Without typedef
int (*callback)(int, int);

// With typedef
typedef int (*BinaryOperation)(int, int);

BinaryOperation callback;  // Much cleaner!
```

**Clay Example** (from clay.h:166):
```c
typedef void (*Clay_ErrorHandler)(Clay_ErrorData errorData);

typedef struct {
    Clay_ErrorHandler errorHandlerFunction;
    void *userData;
} Clay_ErrorHandlerFunction;
```

### 12.3 Callbacks

Pass functions as parameters:

```c
#include <stdio.h>

typedef void (*Callback)(int);

void processArray(int *arr, int size, Callback func) {
    for (int i = 0; i < size; i++) {
        func(arr[i]);  // Call the callback
    }
}

void printDouble(int x) {
    printf("%d ", x * 2);
}

void printSquare(int x) {
    printf("%d ", x * x);
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5};

    processArray(arr, 5, printDouble);  // 2 4 6 8 10
    printf("\n");
    processArray(arr, 5, printSquare);  // 1 4 9 16 25
    printf("\n");

    return 0;
}
```

**Clay Example** (from clay.h:1037):
```c
typedef Clay_Dimensions (*Clay_MeasureTextFunction)(
    Clay_String *text,
    Clay_TextElementConfig *config,
    void *userData
);

// Store in config
typedef struct {
    Clay_MeasureTextFunction measureTextFunction;
    void *userData;
} Clay_TextMeasureFunction;

// Usage
Clay_Dimensions measureText(
    Clay_String *text,
    Clay_TextElementConfig *config,
    void *userData
) {
    // Custom measurement
    return (Clay_Dimensions){100, 20};
}

// Set callback
Clay_SetMeasureTextFunction(measureText, NULL);
```

### 12.4 Function Pointer Arrays

```c
#include <stdio.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div(int a, int b) { return a / b; }

int main(void) {
    // Array of function pointers
    int (*operations[4])(int, int) = {add, sub, mul, div};

    printf("%d\n", operations[0](10, 5));  // 15 (add)
    printf("%d\n", operations[1](10, 5));  // 5 (sub)
    printf("%d\n", operations[2](10, 5));  // 50 (mul)
    printf("%d\n", operations[3](10, 5));  // 2 (div)

    return 0;
}
```

### 12.5 Callbacks with User Data

```c
typedef void (*Callback)(int value, void *userData);

void forEach(int *arr, int size, Callback func, void *userData) {
    for (int i = 0; i < size; i++) {
        func(arr[i], userData);
    }
}

void printWithPrefix(int value, void *userData) {
    char *prefix = (char*)userData;
    printf("%s%d\n", prefix, value);
}

int main(void) {
    int arr[] = {1, 2, 3};
    forEach(arr, 3, printWithPrefix, "Number: ");
    // Output:
    // Number: 1
    // Number: 2
    // Number: 3

    return 0;
}
```

**Clay Example** (from clay.h):
```c
typedef void (*Clay_QueryScrollCallback)(
    Clay_ScrollContainerData *scrollData,
    void *userData
);

// Usage with user data
void handleScroll(Clay_ScrollContainerData *data, void *userData) {
    MyContext *ctx = (MyContext*)userData;
    // Use ctx...
}

Clay_SetQueryScrollCallback(handleScroll, &myContext);
```

### 12.6 Key Concepts Learned
- ✅ Function pointers store function addresses
- ✅ Typedef for cleaner function pointer syntax
- ✅ Callbacks for customizable behavior
- ✅ Function pointer arrays
- ✅ User data pattern for context

---

## Chapter 13: Building Complete Programs {#chapter-13}

### 13.1 Simple Clay Example

Let's build a complete UI program:

```c
// example.c
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include <stdio.h>
#include <stdlib.h>

// Memory for Clay
Clay_Arena arena;
char arenaMemory[1024 * 1024];  // 1MB

// Text measurement (simplified)
Clay_Dimensions MeasureText(
    Clay_String *text,
    Clay_TextElementConfig *config,
    void *userData
) {
    // Simple: 10 pixels per character, 20 pixels high
    return (Clay_Dimensions){
        .width = text->length * 10,
        .height = 20
    };
}

int main(void) {
    // Initialize Clay
    arena.memory = arenaMemory;
    arena.capacity = sizeof(arenaMemory);

    Clay_Initialize(arena, (Clay_Dimensions){1024, 768});
    Clay_SetMeasureTextFunction(MeasureText, NULL);

    // Begin layout
    Clay_BeginLayout();

    // Create UI hierarchy
    CLAY({
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0)
            },
            .padding = CLAY_PADDING_ALL(16),
            .childGap = 16,
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .backgroundColor = CLAY_COLOR(200, 200, 200, 255)
    }) {
        // Header
        CLAY_TEXT(
            CLAY_STRING("My Application"),
            CLAY_TEXT_CONFIG({
                .fontSize = 24,
                .textColor = CLAY_COLOR(0, 0, 0, 255)
            })
        );

        // Content
        CLAY({
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_FIXED(200)
                },
                .padding = CLAY_PADDING_ALL(8)
            },
            .backgroundColor = CLAY_COLOR(255, 255, 255, 255)
        }) {
            CLAY_TEXT(
                CLAY_STRING("This is my UI content!"),
                CLAY_TEXT_CONFIG({
                    .fontSize = 16,
                    .textColor = CLAY_COLOR(0, 0, 0, 255)
                })
            );
        }
    }

    // End layout and get render commands
    Clay_RenderCommandArray commands = Clay_EndLayout();

    // Render (simplified - just print)
    printf("Generated %d render commands\n", commands.length);
    for (int i = 0; i < commands.length; i++) {
        Clay_RenderCommand *cmd = &commands.internalArray[i];
        printf("Command %d: type=%d, bounds=(%.0f,%.0f,%.0f,%.0f)\n",
            i, cmd->commandType,
            cmd->boundingBox.x, cmd->boundingBox.y,
            cmd->boundingBox.width, cmd->boundingBox.height
        );
    }

    return 0;
}
```

### 13.2 Compiling

```bash
# GCC
gcc -o example example.c -lm

# Clang
clang -o example example.c -lm

# With warnings
gcc -Wall -Wextra -o example example.c -lm

# With optimization
gcc -O2 -o example example.c -lm
```

### 13.3 Makefiles

Automate compilation:

```makefile
# Makefile
CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBS = -lm

example: example.c clay.h
	$(CC) $(CFLAGS) -o example example.c $(LIBS)

clean:
	rm -f example

run: example
	./example
```

Usage:
```bash
make           # Build
make run       # Build and run
make clean     # Remove executable
```

### 13.4 Multi-File Project

```
project/
├── Makefile
├── clay.h
├── src/
│   ├── main.c
│   ├── ui.c
│   ├── ui.h
│   └── utils.c
│   └── utils.h
```

**ui.h:**
```c
#ifndef UI_H
#define UI_H

#include "clay.h"

void UI_CreateLayout(void);

#endif
```

**ui.c:**
```c
#include "ui.h"

void UI_CreateLayout(void) {
    CLAY({
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0)
            }
        }
    }) {
        // UI code
    }
}
```

**main.c:**
```c
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "ui.h"

int main(void) {
    // Initialize Clay

    Clay_BeginLayout();
    UI_CreateLayout();
    Clay_RenderCommandArray commands = Clay_EndLayout();

    // Render...

    return 0;
}
```

### 13.5 Common C Patterns Recap

**1. Initialization Pattern:**
```c
typedef struct {
    int *data;
    size_t size;
} Array;

Array* Array_Create(size_t size) {
    Array *arr = malloc(sizeof(Array));
    arr->data = malloc(size * sizeof(int));
    arr->size = size;
    return arr;
}

void Array_Destroy(Array *arr) {
    free(arr->data);
    free(arr);
}
```

**2. Error Handling:**
```c
typedef enum {
    STATUS_OK,
    STATUS_ERROR_NULL_POINTER,
    STATUS_ERROR_OUT_OF_MEMORY
} Status;

Status doSomething(void *data) {
    if (data == NULL) {
        return STATUS_ERROR_NULL_POINTER;
    }

    void *mem = malloc(100);
    if (mem == NULL) {
        return STATUS_ERROR_OUT_OF_MEMORY;
    }

    // Do work...

    free(mem);
    return STATUS_OK;
}
```

**3. Iterator Pattern:**
```c
typedef struct {
    int *current;
    int *end;
} Iterator;

Iterator Array_Begin(Array *arr) {
    return (Iterator){arr->data, arr->data + arr->size};
}

bool Iterator_HasNext(Iterator *it) {
    return it->current < it->end;
}

int Iterator_Next(Iterator *it) {
    return *(it->current++);
}

// Usage
Iterator it = Array_Begin(&myArray);
while (Iterator_HasNext(&it)) {
    int value = Iterator_Next(&it);
    printf("%d\n", value);
}
```

### 13.6 Key Concepts Learned
- ✅ Complete program structure
- ✅ Compiling with gcc/clang
- ✅ Makefiles for automation
- ✅ Multi-file project organization
- ✅ Common C patterns

---

## Conclusion and Next Steps

### What You've Learned

You now understand:

1. **C Fundamentals**: Variables, types, functions, control flow
2. **Memory Management**: Pointers, stack vs heap, arena allocators
3. **Advanced Types**: Structs, unions, enums, typedefs
4. **Preprocessor**: Macros, conditional compilation, code generation
5. **Project Organization**: Headers, source files, compilation
6. **Real-World Patterns**: As demonstrated by Clay library

### Clay Demonstrates

- **Professional C code**: Clean, well-documented, performant
- **Single-header pattern**: Easy distribution and integration
- **Zero dependencies**: Portable, minimal C programming
- **Memory efficiency**: Arena allocators, no malloc in hot path
- **API design**: Clear, consistent, user-friendly
- **Macro DSL**: Declarative UI in C

### Continue Learning

**Read Clay Source:**
- `clay.h` - Study the implementation
- `examples/` - See real usage
- `renderers/` - Integration with graphics libraries

**Build Projects:**
- Simple calculator UI
- File browser interface
- Game menu system
- Settings panel

**Explore More:**
- SDL2/Raylib for graphics
- stb_image for image loading
- Other single-header libraries

**Advanced Topics:**
- SIMD optimization
- Platform-specific code
- Custom memory allocators
- Performance profiling

### Resources

- **Clay Website**: https://nicbarker.com/clay
- **C Reference**: https://en.cppreference.com/w/c
- **The C Programming Language** by Kernighan & Ritchie
- **Modern C** by Jens Gustedt

---

## Practice Exercises

### Exercise 1: Modify Clay Layout
Create a 3-column layout with different colors.

### Exercise 2: Custom Text Measurement
Implement proper text measurement using a font library.

### Exercise 3: Handle Input
Add mouse click detection to Clay elements.

### Exercise 4: Scrolling Container
Create a scrollable list of items.

### Exercise 5: Build a Calculator
Create a calculator UI using Clay.

---

**Happy Coding!** 🚀

You now have a solid foundation in C programming. Practice regularly, read professional C code like Clay, and build projects to reinforce your learning.