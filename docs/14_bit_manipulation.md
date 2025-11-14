# Chapter 14: Bit Manipulation in C

## Complete Guide with Clay Library Examples

---

## 14.1 Binary Number System

Understanding binary is essential for bit manipulation:

```c
// Decimal vs Binary
0 = 0000
1 = 0001
2 = 0010
3 = 0011
4 = 0100
5 = 0101
6 = 0110
7 = 0111
8 = 1000
```

### Converting Binary to Decimal

```c
#include <stdio.h>

int binaryToDecimal(int binary) {
    int decimal = 0, base = 1;
    while (binary > 0) {
        int lastDigit = binary % 10;
        decimal += lastDigit * base;
        base *= 2;
        binary /= 10;
    }
    return decimal;
}

int main(void) {
    printf("%d\n", binaryToDecimal(1010));  // 10
    printf("%d\n", binaryToDecimal(1111));  // 15
    return 0;
}
```

---

## 14.2 Bitwise Operators

### AND Operator (&)

```c
// Both bits must be 1
int a = 5;   // 0101
int b = 3;   // 0011
int c = a & b;  // 0001 = 1

// Check if number is even
int isEven(int n) {
    return (n & 1) == 0;  // Check if last bit is 0
}
```

**Clay Example:**
```c
// Check if corner radius flag is set
typedef enum {
    CLAY_CORNER_RADIUS_TOP_LEFT = 1 << 0,      // 0001
    CLAY_CORNER_RADIUS_TOP_RIGHT = 1 << 1,     // 0010
    CLAY_CORNER_RADIUS_BOTTOM_LEFT = 1 << 2,   // 0100
    CLAY_CORNER_RADIUS_BOTTOM_RIGHT = 1 << 3   // 1000
} Clay_CornerRadiusSet;

// Check if top-left corner has radius
if (flags & CLAY_CORNER_RADIUS_TOP_LEFT) {
    // Top left corner has radius
}
```

### OR Operator (|)

```c
// At least one bit must be 1
int a = 5;   // 0101
int b = 3;   // 0011
int c = a | b;  // 0111 = 7

// Set specific bit
int setBit(int num, int pos) {
    return num | (1 << pos);
}
```

**Clay Example:**
```c
// Combine multiple corner radius flags
int flags = CLAY_CORNER_RADIUS_TOP_LEFT |
            CLAY_CORNER_RADIUS_TOP_RIGHT;  // 0011 = both top corners

// Add another flag
flags |= CLAY_CORNER_RADIUS_BOTTOM_LEFT;  // Now 0111 = three corners
```

### XOR Operator (^)

```c
// Bits must be different
int a = 5;   // 0101
int b = 3;   // 0011
int c = a ^ b;  // 0110 = 6

// Swap two numbers without temp variable
void swap(int *a, int *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}
```

### NOT Operator (~)

```c
// Inverts all bits
int a = 5;      // 00000101
int b = ~a;     // 11111010 (in 8-bit)

// Clear specific bit
int clearBit(int num, int pos) {
    return num & ~(1 << pos);
}
```

### Left Shift (<<)

```c
// Multiply by 2^n
int a = 5;      // 0101
int b = a << 1; // 1010 = 10 (multiply by 2)
int c = a << 2; // 10100 = 20 (multiply by 4)

// Fast power of 2
int powerOf2(int n) {
    return 1 << n;  // 2^n
}
```

**Clay Example:**
```c
// Define bit flags with left shift
typedef enum {
    CLAY_CORNER_RADIUS_TOP_LEFT = 1 << 0,      // 1
    CLAY_CORNER_RADIUS_TOP_RIGHT = 1 << 1,     // 2
    CLAY_CORNER_RADIUS_BOTTOM_LEFT = 1 << 2,   // 4
    CLAY_CORNER_RADIUS_BOTTOM_RIGHT = 1 << 3   // 8
} Clay_CornerRadiusSet;
```

### Right Shift (>>)

```c
// Divide by 2^n
int a = 20;     // 10100
int b = a >> 1; // 01010 = 10 (divide by 2)
int c = a >> 2; // 00101 = 5 (divide by 4)

// Extract specific bits
int getBits(int num, int start, int count) {
    return (num >> start) & ((1 << count) - 1);
}
```

---

## 14.3 Common Bit Operations

### Check if Bit is Set

```c
int isBitSet(int num, int pos) {
    return (num & (1 << pos)) != 0;
}

int main(void) {
    int num = 5;  // 0101
    printf("%d\n", isBitSet(num, 0));  // 1 (bit 0 is set)
    printf("%d\n", isBitSet(num, 1));  // 0 (bit 1 is not set)
    printf("%d\n", isBitSet(num, 2));  // 1 (bit 2 is set)
    return 0;
}
```

### Set a Bit

```c
int setBit(int num, int pos) {
    return num | (1 << pos);
}

int main(void) {
    int num = 5;  // 0101
    num = setBit(num, 1);  // 0111 = 7
    printf("%d\n", num);
    return 0;
}
```

### Clear a Bit

```c
int clearBit(int num, int pos) {
    return num & ~(1 << pos);
}

int main(void) {
    int num = 7;  // 0111
    num = clearBit(num, 1);  // 0101 = 5
    printf("%d\n", num);
    return 0;
}
```

### Toggle a Bit

```c
int toggleBit(int num, int pos) {
    return num ^ (1 << pos);
}

int main(void) {
    int num = 5;  // 0101
    num = toggleBit(num, 1);  // 0111 = 7
    num = toggleBit(num, 1);  // 0101 = 5 (back to original)
    printf("%d\n", num);
    return 0;
}
```

---

## 14.4 Bit Masks

Masks select specific bits:

```c
// Extract RGB components from 32-bit color
typedef struct {
    uint32_t color;  // 0xAARRGGBB
} Color;

uint8_t getRed(Color c) {
    return (c.color >> 16) & 0xFF;  // Mask: 0x00FF0000
}

uint8_t getGreen(Color c) {
    return (c.color >> 8) & 0xFF;   // Mask: 0x0000FF00
}

uint8_t getBlue(Color c) {
    return c.color & 0xFF;           // Mask: 0x000000FF
}

uint8_t getAlpha(Color c) {
    return (c.color >> 24) & 0xFF;  // Mask: 0xFF000000
}

uint32_t makeColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((uint32_t)a << 24) |
           ((uint32_t)r << 16) |
           ((uint32_t)g << 8) |
            (uint32_t)b;
}
```

**Clay Example (conceptual):**
```c
// Clay uses float colors, but shows similar concepts
typedef struct {
    float r, g, b, a;
} Clay_Color;

// If Clay used packed colors:
uint32_t packColor(Clay_Color c) {
    uint8_t r = (uint8_t)(c.r * 255);
    uint8_t g = (uint8_t)(c.g * 255);
    uint8_t b = (uint8_t)(c.b * 255);
    uint8_t a = (uint8_t)(c.a * 255);
    return (a << 24) | (r << 16) | (g << 8) | b;
}
```

---

## 14.5 Bit Fields in Structs

Pack multiple values efficiently:

```c
struct Flags {
    unsigned int isVisible : 1;   // 1 bit
    unsigned int isEnabled : 1;   // 1 bit
    unsigned int priority : 3;    // 3 bits (0-7)
    unsigned int id : 11;         // 11 bits
};  // Total: 16 bits = 2 bytes

int main(void) {
    struct Flags f = {0};
    f.isVisible = 1;
    f.isEnabled = 1;
    f.priority = 5;
    f.id = 1024;

    printf("Size: %zu bytes\n", sizeof(f));  // 2 or 4 (compiler dependent)
    printf("Priority: %u\n", f.priority);
    return 0;
}
```

**Clay Example (conceptual):**
```c
// Clay could use bit fields for layout flags
typedef struct {
    uint32_t layoutDirection : 1;     // 0 = horizontal, 1 = vertical
    uint32_t wrapChildren : 1;        // Wrap to next line
    uint32_t alignmentX : 2;          // 0-3 for alignment options
    uint32_t alignmentY : 2;
    uint32_t reserved : 26;           // Future use
} Clay_LayoutFlags;
```

---

## 14.6 Counting Set Bits

### Method 1: Loop

```c
int countSetBits(int n) {
    int count = 0;
    while (n > 0) {
        count += n & 1;  // Add last bit
        n >>= 1;         // Shift right
    }
    return count;
}
```

### Method 2: Brian Kernighan's Algorithm

```c
int countSetBits(int n) {
    int count = 0;
    while (n > 0) {
        n &= (n - 1);  // Clear rightmost set bit
        count++;
    }
    return count;
}

// Example: n = 12 (1100)
// 1100 & 1011 = 1000 (count = 1)
// 1000 & 0111 = 0000 (count = 2)
```

### Method 3: Lookup Table

```c
// Precomputed table for 4-bit numbers
int table[16] = {
    0, 1, 1, 2, 1, 2, 2, 3,
    1, 2, 2, 3, 2, 3, 3, 4
};

int countSetBits(int n) {
    int count = 0;
    while (n > 0) {
        count += table[n & 0xF];  // Count 4 bits at a time
        n >>= 4;
    }
    return count;
}
```

---

## 14.7 Power of Two

### Check if Power of 2

```c
int isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// Examples:
// 4 (100) & 3 (011) = 0 → true
// 5 (101) & 4 (100) = 4 → false
// 8 (1000) & 7 (0111) = 0 → true
```

### Next Power of 2

```c
int nextPowerOfTwo(int n) {
    if (n <= 0) return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

int main(void) {
    printf("%d\n", nextPowerOfTwo(5));   // 8
    printf("%d\n", nextPowerOfTwo(17));  // 32
    return 0;
}
```

---

## 14.8 Swap Values

### Without Temporary Variable

```c
void swap(int *a, int *b) {
    if (a != b) {  // Must check for same address
        *a ^= *b;
        *b ^= *a;
        *a ^= *b;
    }
}
```

### Swap Nibbles (4-bit)

```c
int swapNibbles(int n) {
    return ((n & 0x0F) << 4) | ((n & 0xF0) >> 4);
}

// Example: 0x5A → 0xA5
```

---

## 14.9 Reverse Bits

```c
uint32_t reverseBits(uint32_t n) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
        result <<= 1;           // Shift result left
        result |= (n & 1);      // Add rightmost bit of n
        n >>= 1;                // Shift n right
    }
    return result;
}
```

---

## 14.10 Parity

Check if number of set bits is even or odd:

```c
int getParity(int n) {
    int parity = 0;
    while (n > 0) {
        parity ^= (n & 1);
        n >>= 1;
    }
    return parity;  // 0 = even, 1 = odd
}
```

---

## 14.11 Gray Code

Convert between binary and Gray code:

```c
// Binary to Gray
int binaryToGray(int n) {
    return n ^ (n >> 1);
}

// Gray to Binary
int grayToBinary(int n) {
    int binary = 0;
    while (n > 0) {
        binary ^= n;
        n >>= 1;
    }
    return binary;
}
```

---

## 14.12 Position of Rightmost Set Bit

```c
int rightmostSetBit(int n) {
    return n & ~(n - 1);
}

// Example: 12 (1100)
// Returns: 4 (0100) - position of rightmost 1
```

---

## 14.13 Toggle All Bits After Rightmost Set Bit

```c
int toggleAfterRightmost(int n) {
    return n ^ (n - 1);
}

// Example: 12 (1100)
// Returns: 15 (1111)
```

---

## 14.14 Extract and Set Bit Ranges

```c
// Extract bits from position p to p+n
int extractBits(int num, int p, int n) {
    return (num >> p) & ((1 << n) - 1);
}

// Set bits from position p to p+n
int setBits(int num, int p, int n, int value) {
    int mask = ((1 << n) - 1) << p;
    return (num & ~mask) | ((value << p) & mask);
}
```

---

## 14.15 Practical Applications in Clay

### Bit Flags for UI State

```c
typedef enum {
    CLAY_ELEMENT_VISIBLE    = 1 << 0,  // 0x01
    CLAY_ELEMENT_ENABLED    = 1 << 1,  // 0x02
    CLAY_ELEMENT_FOCUSED    = 1 << 2,  // 0x04
    CLAY_ELEMENT_HOVERED    = 1 << 3,  // 0x08
    CLAY_ELEMENT_PRESSED    = 1 << 4,  // 0x10
    CLAY_ELEMENT_SELECTED   = 1 << 5,  // 0x20
    CLAY_ELEMENT_DISABLED   = 1 << 6,  // 0x40
    CLAY_ELEMENT_HIDDEN     = 1 << 7   // 0x80
} Clay_ElementFlags;

typedef struct {
    uint32_t flags;
} Clay_Element;

// Check state
int isVisible(Clay_Element *el) {
    return (el->flags & CLAY_ELEMENT_VISIBLE) != 0;
}

// Set state
void setVisible(Clay_Element *el, int visible) {
    if (visible) {
        el->flags |= CLAY_ELEMENT_VISIBLE;  // Set bit
    } else {
        el->flags &= ~CLAY_ELEMENT_VISIBLE; // Clear bit
    }
}

// Toggle state
void toggleVisible(Clay_Element *el) {
    el->flags ^= CLAY_ELEMENT_VISIBLE;
}

// Multiple states at once
void setInteractive(Clay_Element *el) {
    el->flags |= (CLAY_ELEMENT_VISIBLE |
                  CLAY_ELEMENT_ENABLED);
}
```

### Optimized Hash Functions

```c
// Clay uses hashing for element lookup
uint32_t hashString(const char *str, size_t length) {
    uint32_t hash = 0;
    for (size_t i = 0; i < length; i++) {
        hash = (hash << 5) - hash + str[i];  // hash * 31 + c
        // Using bit shift instead of multiply
    }
    return hash;
}
```

### Memory Alignment

```c
// Align size to power of 2
size_t alignSize(size_t size, size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

// Example: align to 16 bytes
// size = 25
// (25 + 15) & ~15 = 40 & 0xFFFFFFF0 = 32
```

---

## 14.16 Performance Tips

**Use bit operations for:**
- ✅ Powers of 2 multiplication/division
- ✅ Flag checks (faster than booleans)
- ✅ Color packing/unpacking
- ✅ Hash functions
- ✅ Alignment calculations

**Avoid for:**
- ❌ Regular arithmetic (compiler optimizes)
- ❌ Readability-critical code
- ❌ Floating-point operations

---

## 14.17 Key Concepts Learned
- ✅ Bitwise operators: &, |, ^, ~, <<, >>
- ✅ Bit manipulation: set, clear, toggle, check
- ✅ Bit masks and extraction
- ✅ Bit fields in structs
- ✅ Counting set bits
- ✅ Power of 2 operations
- ✅ Practical applications in Clay
- ✅ Performance considerations

---

## Practice Exercises

1. Write a function to count trailing zeros in a number
2. Implement bitwise rotation (rotate left/right)
3. Find the only non-duplicate number in an array (all others appear twice)
4. Swap all odd and even bits in a number
5. Add two numbers without using + operator
6. Implement a bit vector data structure
7. Create a compact permission system using bit flags
8. Write efficient popcount for 64-bit numbers
