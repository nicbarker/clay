# Chapter 21: C Standard Library Basics

## Complete Guide with Clay Library Examples

---

## 21.1 What is the C Standard Library?

The C Standard Library provides essential functions for:
- Input/Output (`stdio.h`)
- String manipulation (`string.h`)
- Memory management (`stdlib.h`)
- Math operations (`math.h`)
- Character handling (`ctype.h`)
- Time/Date (`time.h`)

**Clay's Approach:** Clay is **zero-dependency** - it doesn't use the standard library! This shows how to write portable C without stdlib.

---

## 21.2 stdio.h - Input/Output

### Printf Family

```c
#include <stdio.h>

int main(void) {
    // Basic printf
    printf("Hello, World!\n");

    // Format specifiers
    int age = 25;
    float height = 5.9f;
    char grade = 'A';

    printf("Age: %d\n", age);           // int
    printf("Height: %.2f\n", height);   // float with 2 decimals
    printf("Grade: %c\n", grade);       // char
    printf("Hex: %x\n", 255);           // hexadecimal (ff)
    printf("Pointer: %p\n", (void*)&age); // pointer address

    // Width and padding
    printf("%5d\n", 42);      // "   42" (right-aligned)
    printf("%-5d\n", 42);     // "42   " (left-aligned)
    printf("%05d\n", 42);     // "00042" (zero-padded)

    return 0;
}
```

### Sprintf - Format to String

```c
#include <stdio.h>

int main(void) {
    char buffer[100];

    int x = 10, y = 20;
    sprintf(buffer, "Point: (%d, %d)", x, y);
    printf("%s\n", buffer);  // "Point: (10, 20)"

    // Safer version with size limit
    snprintf(buffer, sizeof(buffer), "Value: %d", 42);

    return 0;
}
```

### Scanf Family

```c
#include <stdio.h>

int main(void) {
    int age;
    float height;
    char name[50];

    printf("Enter age: ");
    scanf("%d", &age);  // Note: address-of operator!

    printf("Enter height: ");
    scanf("%f", &height);

    printf("Enter name: ");
    scanf("%49s", name);  // Limit input to avoid overflow

    printf("Name: %s, Age: %d, Height: %.1f\n", name, age, height);

    return 0;
}
```

### File Operations

```c
#include <stdio.h>

int main(void) {
    FILE *file;

    // Write to file
    file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fprintf(file, "Hello, File!\n");
    fclose(file);

    // Read from file
    file = fopen("output.txt", "r");
    if (file != NULL) {
        char buffer[100];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
        }
        fclose(file);
    }

    return 0;
}
```

**Clay doesn't use stdio.h** - it's renderer-agnostic and doesn't handle file I/O internally.

---

## 21.3 stdlib.h - General Utilities

### Memory Allocation

```c
#include <stdlib.h>

int main(void) {
    // Allocate memory
    int *arr = (int*)malloc(10 * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Use array
    for (int i = 0; i < 10; i++) {
        arr[i] = i * 2;
    }

    // Resize array
    arr = (int*)realloc(arr, 20 * sizeof(int));

    // Free memory
    free(arr);

    return 0;
}
```

**Clay's Approach:** Uses custom arena allocators instead of malloc/free!

```c
// Clay's arena allocator (from clay.h)
typedef struct {
    size_t capacity;
    size_t nextAllocation;
    char *memory;
} Clay_Arena;

void* Clay__AllocateArena(Clay_Arena *arena, size_t size) {
    if (arena->nextAllocation + size <= arena->capacity) {
        void *ptr = arena->memory + arena->nextAllocation;
        arena->nextAllocation += size;
        return ptr;
    }
    return NULL;  // Out of memory
}
```

### String to Number Conversion

```c
#include <stdlib.h>

int main(void) {
    // String to integer
    int num1 = atoi("123");              // 123
    long num2 = atol("123456789");       // 123456789
    long long num3 = atoll("9999999999"); // 9999999999

    // String to float
    double num4 = atof("3.14159");       // 3.14159

    // More robust conversion with error checking
    char *endptr;
    long value = strtol("123abc", &endptr, 10);
    if (*endptr != '\0') {
        printf("Invalid number: stopped at '%s'\n", endptr);
    }

    return 0;
}
```

### Random Numbers

```c
#include <stdlib.h>
#include <time.h>

int main(void) {
    // Seed random number generator
    srand(time(NULL));

    // Generate random numbers
    for (int i = 0; i < 5; i++) {
        int random = rand();  // 0 to RAND_MAX
        printf("%d\n", random);
    }

    // Random in range [min, max]
    int min = 1, max = 100;
    int randomInRange = min + rand() % (max - min + 1);
    printf("Random (1-100): %d\n", randomInRange);

    return 0;
}
```

### Program Termination

```c
#include <stdlib.h>

void cleanup(void) {
    printf("Cleaning up...\n");
}

int main(void) {
    // Register cleanup function
    atexit(cleanup);

    // Normal exit
    // exit(0);  // Success
    // exit(1);  // Failure

    // Abnormal termination
    // abort();  // Immediate termination

    return 0;
}  // cleanup() called automatically
```

### Environment Variables

```c
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    // Get environment variable
    char *path = getenv("PATH");
    if (path != NULL) {
        printf("PATH: %s\n", path);
    }

    char *home = getenv("HOME");
    if (home != NULL) {
        printf("HOME: %s\n", home);
    }

    return 0;
}
```

---

## 21.4 string.h - String Manipulation

### String Length

```c
#include <string.h>

int main(void) {
    char str[] = "Hello";
    size_t len = strlen(str);  // 5
    printf("Length: %zu\n", len);

    return 0;
}
```

**Clay's Implementation** (from clay.h):
```c
// Clay uses explicit length, no strlen needed
typedef struct {
    int32_t length;      // Length stored directly!
    const char *chars;
} Clay_String;

// O(1) length access
int getLength(Clay_String *str) {
    return str->length;  // No need to count!
}
```

### String Copy

```c
#include <string.h>

int main(void) {
    char src[] = "Hello";
    char dest[20];

    // Copy string
    strcpy(dest, src);

    // Copy with size limit (safer)
    strncpy(dest, src, sizeof(dest) - 1);
    dest[sizeof(dest) - 1] = '\0';  // Ensure null termination

    printf("%s\n", dest);

    return 0;
}
```

### String Concatenation

```c
#include <string.h>

int main(void) {
    char str[50] = "Hello";

    // Concatenate
    strcat(str, " World");      // "Hello World"

    // Concatenate with limit (safer)
    strncat(str, "!", sizeof(str) - strlen(str) - 1);

    printf("%s\n", str);

    return 0;
}
```

### String Comparison

```c
#include <string.h>

int main(void) {
    char str1[] = "Apple";
    char str2[] = "Banana";

    // Compare strings
    int result = strcmp(str1, str2);
    if (result < 0) {
        printf("%s comes before %s\n", str1, str2);
    } else if (result > 0) {
        printf("%s comes after %s\n", str1, str2);
    } else {
        printf("Strings are equal\n");
    }

    // Compare n characters
    if (strncmp(str1, str2, 3) == 0) {
        printf("First 3 characters match\n");
    }

    return 0;
}
```

### Memory Operations

```c
#include <string.h>

int main(void) {
    char buffer[20];

    // Set memory to value
    memset(buffer, 'A', 10);
    buffer[10] = '\0';
    printf("%s\n", buffer);  // "AAAAAAAAAA"

    // Copy memory
    char src[] = "Hello";
    char dest[20];
    memcpy(dest, src, strlen(src) + 1);  // Include \0

    // Move memory (handles overlapping regions)
    memmove(dest + 2, dest, strlen(dest) + 1);  // Shift right
    printf("%s\n", dest);  // "HeHello"

    // Compare memory
    if (memcmp(src, dest, 5) == 0) {
        printf("Memory regions match\n");
    }

    return 0;
}
```

**Clay's Approach:**
```c
// Clay uses custom memory functions when needed
static inline void Clay__MemoryCopy(
    void *dest,
    const void *src,
    size_t size
) {
    char *d = (char*)dest;
    const char *s = (const char*)src;
    for (size_t i = 0; i < size; i++) {
        d[i] = s[i];
    }
}
```

---

## 21.5 math.h - Mathematical Functions

```c
#include <math.h>
#include <stdio.h>

int main(void) {
    // Power and roots
    printf("2^3 = %.0f\n", pow(2, 3));          // 8
    printf("sqrt(16) = %.0f\n", sqrt(16));      // 4
    printf("cbrt(27) = %.0f\n", cbrt(27));      // 3

    // Trigonometry
    printf("sin(π/2) = %.1f\n", sin(M_PI / 2)); // 1.0
    printf("cos(0) = %.1f\n", cos(0));          // 1.0
    printf("tan(π/4) = %.1f\n", tan(M_PI / 4)); // 1.0

    // Rounding
    printf("ceil(3.2) = %.0f\n", ceil(3.2));    // 4
    printf("floor(3.8) = %.0f\n", floor(3.8));  // 3
    printf("round(3.5) = %.0f\n", round(3.5));  // 4

    // Absolute value
    printf("fabs(-5.5) = %.1f\n", fabs(-5.5));  // 5.5
    printf("abs(-5) = %d\n", abs(-5));          // 5

    // Logarithms
    printf("log(e) = %.1f\n", log(M_E));        // 1.0
    printf("log10(100) = %.0f\n", log10(100));  // 2

    // Exponential
    printf("exp(1) = %.2f\n", exp(1));          // 2.72 (e)

    return 0;
}
```

**Clay's Implementation:**
```c
// Clay implements its own min/max (no stdlib needed)
static inline float Clay__Min(float a, float b) {
    return a < b ? a : b;
}

static inline float Clay__Max(float a, float b) {
    return a > b ? a : b;
}

static inline float Clay__Clamp(float value, float min, float max) {
    return Clay__Max(min, Clay__Min(value, max));
}
```

---

## 21.6 ctype.h - Character Handling

```c
#include <ctype.h>
#include <stdio.h>

int main(void) {
    char ch = 'A';

    // Character testing
    if (isalpha(ch))  printf("Letter\n");
    if (isdigit(ch))  printf("Digit\n");
    if (isalnum(ch))  printf("Alphanumeric\n");
    if (isupper(ch))  printf("Uppercase\n");
    if (islower(ch))  printf("Lowercase\n");
    if (isspace(ch))  printf("Whitespace\n");
    if (ispunct(ch))  printf("Punctuation\n");

    // Character conversion
    printf("Upper: %c\n", toupper('a'));  // 'A'
    printf("Lower: %c\n", tolower('A'));  // 'a'

    // Practical example: validate identifier
    int isValidIdentifier(const char *str) {
        if (!isalpha(str[0]) && str[0] != '_') {
            return 0;  // Must start with letter or _
        }
        for (int i = 1; str[i] != '\0'; i++) {
            if (!isalnum(str[i]) && str[i] != '_') {
                return 0;
            }
        }
        return 1;
    }

    printf("valid_name: %d\n", isValidIdentifier("valid_name"));  // 1
    printf("123invalid: %d\n", isValidIdentifier("123invalid"));  // 0

    return 0;
}
```

---

## 21.7 time.h - Time and Date

```c
#include <time.h>
#include <stdio.h>

int main(void) {
    // Current time
    time_t now = time(NULL);
    printf("Seconds since epoch: %ld\n", now);

    // Convert to readable format
    struct tm *timeinfo = localtime(&now);
    printf("Current time: %s", asctime(timeinfo));

    // Format time
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    printf("Formatted: %s\n", buffer);

    // Measure execution time
    clock_t start = clock();

    // Do some work
    for (int i = 0; i < 1000000; i++) {
        // Busy work
    }

    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU time: %.6f seconds\n", cpu_time);

    // Sleep (POSIX)
    // sleep(1);  // Sleep 1 second

    return 0;
}
```

---

## 21.8 assert.h - Debugging

```c
#include <assert.h>
#include <stdio.h>

int divide(int a, int b) {
    assert(b != 0);  // Crash if b is 0 (in debug builds)
    return a / b;
}

int main(void) {
    int result = divide(10, 2);  // OK
    printf("Result: %d\n", result);

    // result = divide(10, 0);  // Crashes with error message

    // Disable asserts in release
    // Compile with: gcc -DNDEBUG program.c

    return 0;
}
```

**Clay's Approach:**
```c
// Clay has custom error handling
#define CLAY__ASSERT(condition) \
    if (!(condition)) { \
        Clay__ErrorHandler(...); \
    }
```

---

## 21.9 stdint.h - Fixed-Width Integers

```c
#include <stdint.h>
#include <stdio.h>

int main(void) {
    // Exact-width integers
    int8_t   i8 = 127;           // -128 to 127
    uint8_t  u8 = 255;           // 0 to 255
    int16_t  i16 = 32767;        // -32768 to 32767
    uint16_t u16 = 65535;        // 0 to 65535
    int32_t  i32 = 2147483647;
    uint32_t u32 = 4294967295;
    int64_t  i64 = 9223372036854775807;
    uint64_t u64 = 18446744073709551615ULL;

    // Pointer-sized integers
    intptr_t  iptr;   // Can hold pointer
    uintptr_t uptr;

    // Fast types (at least N bits)
    int_fast32_t fast32;  // Fast 32-bit (may be 64-bit)
    int_least32_t least32; // At least 32-bit (may be 32+)

    // Print with proper format specifiers
    printf("int32: %" PRId32 "\n", i32);
    printf("uint64: %" PRIu64 "\n", u64);

    return 0;
}
```

**Clay uses fixed-width types extensively:**
```c
typedef struct {
    int32_t capacity;   // Exactly 32 bits
    int32_t length;
    uint32_t *internalArray;
} Clay__int32_tArray;
```

---

## 21.10 stdbool.h - Boolean Type

```c
#include <stdbool.h>
#include <stdio.h>

int main(void) {
    bool isActive = true;
    bool isValid = false;

    if (isActive) {
        printf("Active!\n");
    }

    // Boolean operations
    bool result = isActive && !isValid;  // true

    return 0;
}
```

**Pre-C99 approach:**
```c
typedef enum {
    false = 0,
    true = 1
} bool;
```

---

## 21.11 limits.h - Implementation Limits

```c
#include <limits.h>
#include <stdio.h>

int main(void) {
    printf("CHAR_BIT: %d\n", CHAR_BIT);      // 8
    printf("CHAR_MIN: %d\n", CHAR_MIN);
    printf("CHAR_MAX: %d\n", CHAR_MAX);
    printf("INT_MIN: %d\n", INT_MIN);        // -2147483648
    printf("INT_MAX: %d\n", INT_MAX);        // 2147483647
    printf("LONG_MAX: %ld\n", LONG_MAX);

    // Check for overflow
    int x = INT_MAX;
    if (x + 1 < x) {
        printf("Overflow detected!\n");
    }

    return 0;
}
```

---

## 21.12 Key Concepts Learned
- ✅ stdio.h for I/O operations
- ✅ stdlib.h for memory and utilities
- ✅ string.h for string manipulation
- ✅ math.h for mathematical functions
- ✅ ctype.h for character handling
- ✅ time.h for time/date operations
- ✅ assert.h for debugging
- ✅ stdint.h for fixed-width types
- ✅ stdbool.h for boolean type
- ✅ Clay's zero-dependency approach

---

## Practice Exercises

1. Write a program to read a file and count words
2. Implement your own strlen without using stdlib
3. Create a simple calculator using scanf and math.h
4. Build a random password generator
5. Make a function to format time as "X days, Y hours, Z minutes"
6. Implement case-insensitive string comparison
7. Create a memory pool allocator (like Clay's arena)
8. Write a benchmark utility using clock()
