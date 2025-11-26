# Chapter 17: Memory Management in C

## Complete Guide with Clay Library Examples

---

## Table of Contents
1. Stack vs Heap Memory
2. malloc, calloc, realloc, free
3. Common Memory Errors
4. Memory Leaks and Detection
5. Arena Allocators
6. Memory Pools
7. Memory Alignment
8. Custom Allocators
9. Clay's Memory Strategy
10. Best Practices

---

## 17.1 Stack vs Heap Memory

### Stack Memory

**Characteristics:**
- ✅ Automatic allocation/deallocation
- ✅ Very fast (just moving stack pointer)
- ✅ Limited size (typically 1-8 MB)
- ✅ LIFO (Last In, First Out)
- ✅ Variables destroyed when function returns

```c
void function(void) {
    int x = 10;              // Stack
    char buffer[100];        // Stack
    float arr[50];           // Stack
}  // All memory automatically freed here
```

### Heap Memory

**Characteristics:**
- Manual allocation with malloc/calloc
- Manual deallocation with free
- Slower than stack
- Large size available (GBs)
- Persists until explicitly freed
- Risk of memory leaks

```c
#include <stdlib.h>

void function(void) {
    int *p = malloc(sizeof(int));  // Heap
    *p = 10;
    // ...
    free(p);  // Must manually free!
}
```

### Comparison

```c
void stackExample(void) {
    int arr[1000];  // Stack: fast, automatic cleanup
    // Use arr
}  // Automatically freed

void heapExample(void) {
    int *arr = malloc(1000 * sizeof(int));  // Heap: manual management
    if (arr == NULL) {
        // Handle error
        return;
    }
    // Use arr
    free(arr);  // Must free manually
}
```

---

## 17.2 malloc, calloc, realloc, free

### malloc - Allocate Memory

```c
#include <stdlib.h>

int main(void) {
    // Allocate 10 integers
    int *arr = (int*)malloc(10 * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Memory is UNINITIALIZED (contains garbage)
    for (int i = 0; i < 10; i++) {
        arr[i] = i * 2;
    }

    free(arr);  // Always free!
    return 0;
}
```

### calloc - Allocate and Zero

```c
#include <stdlib.h>

int main(void) {
    // Allocate 10 integers, initialized to 0
    int *arr = (int*)calloc(10, sizeof(int));

    if (arr == NULL) {
        return 1;
    }

    // All elements are 0
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);  // 0 0 0 0 0 0 0 0 0 0
    }

    free(arr);
    return 0;
}
```

**malloc vs calloc:**
- `malloc(n)`: Allocates n bytes, uninitialized
- `calloc(count, size)`: Allocates count*size bytes, zero-initialized

### realloc - Resize Memory

```c
#include <stdlib.h>

int main(void) {
    // Allocate 10 integers
    int *arr = malloc(10 * sizeof(int));
    if (arr == NULL) return 1;

    // Initialize
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    // Need more space - resize to 20 integers
    int *temp = realloc(arr, 20 * sizeof(int));
    if (temp == NULL) {
        // Realloc failed, original arr still valid
        free(arr);
        return 1;
    }

    arr = temp;  // Update pointer
    // First 10 elements preserved, last 10 uninitialized

    free(arr);
    return 0;
}
```

**realloc behavior:**
- May move data to new location
- Original contents preserved up to old size
- Returns NULL on failure (old pointer still valid)
- Can shrink or grow allocation

### free - Deallocate Memory

```c
int main(void) {
    int *p = malloc(sizeof(int));

    if (p != NULL) {
        *p = 42;
        // Use p
        free(p);  // Release memory
        p = NULL;  // Good practice: prevent dangling pointer
    }

    return 0;
}
```

---

## 17.3 Common Memory Errors

### 1. Memory Leak

```c
void leak(void) {
    int *p = malloc(sizeof(int));
    *p = 42;
    // Forgot to free(p)!
}  // Memory leaked!

int main(void) {
    for (int i = 0; i < 1000; i++) {
        leak();  // Leaks 4 bytes per iteration = 4KB total
    }
    return 0;
}
```

### 2. Use After Free

```c
int main(void) {
    int *p = malloc(sizeof(int));
    *p = 42;

    free(p);

    *p = 100;  // UNDEFINED BEHAVIOR! Memory no longer owned
    printf("%d\n", *p);  // May crash or print garbage

    return 0;
}
```

### 3. Double Free

```c
int main(void) {
    int *p = malloc(sizeof(int));

    free(p);
    free(p);  // CRASH! Double free

    return 0;
}
```

### 4. Invalid Free

```c
int main(void) {
    int x = 10;
    int *p = &x;  // Points to stack variable

    free(p);  // CRASH! Can only free heap memory

    return 0;
}
```

### 5. Buffer Overflow

```c
int main(void) {
    int *arr = malloc(10 * sizeof(int));

    arr[15] = 100;  // OUT OF BOUNDS! Undefined behavior

    free(arr);
    return 0;
}
```

### 6. Uninitialized Memory

```c
int main(void) {
    int *p = malloc(sizeof(int));
    printf("%d\n", *p);  // Garbage value!

    free(p);
    return 0;
}
```

---

## 17.4 Memory Leaks and Detection

### Detecting Leaks with Valgrind

```bash
# Compile with debug symbols
gcc -g program.c -o program

# Run with Valgrind
valgrind --leak-check=full ./program
```

**Output example:**
```
HEAP SUMMARY:
    in use at exit: 40 bytes in 1 blocks
  total heap usage: 2 allocs, 1 frees, 1,064 bytes allocated

40 bytes in 1 blocks are definitely lost in loss record 1 of 1
   at 0x: malloc
   by 0x: main (program.c:10)
```

### Manual Leak Tracking

```c
#ifdef DEBUG
    static size_t allocations = 0;
    static size_t deallocations = 0;

    #define MALLOC(size) \
        (allocations++, malloc(size))

    #define FREE(ptr) \
        (deallocations++, free(ptr))

    #define REPORT_LEAKS() \
        printf("Allocations: %zu, Frees: %zu, Leaked: %zu\n", \
               allocations, deallocations, allocations - deallocations)
#else
    #define MALLOC(size) malloc(size)
    #define FREE(ptr) free(ptr)
    #define REPORT_LEAKS()
#endif

int main(void) {
    int *p1 = MALLOC(sizeof(int));
    int *p2 = MALLOC(sizeof(int));

    FREE(p1);
    // Forgot to free p2!

    REPORT_LEAKS();  // Shows 1 leak
    return 0;
}
```

---

## 17.5 Arena Allocators

Allocate from a pre-allocated buffer - fast and predictable.

### Basic Arena

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
    // Align to 8 bytes
    size = (size + 7) & ~7;

    if (arena->used + size > arena->size) {
        return NULL;  // Out of memory
    }

    void *ptr = arena->memory + arena->used;
    arena->used += size;
    return ptr;
}

void Arena_Free(Arena *arena) {
    free(arena->memory);  // Free everything at once
    arena->used = 0;
}

void Arena_Reset(Arena *arena) {
    arena->used = 0;  // Reset without freeing
}

int main(void) {
    Arena arena;
    Arena_Init(&arena, 1024 * 1024);  // 1MB

    // Allocate from arena
    int *arr1 = Arena_Alloc(&arena, 100 * sizeof(int));
    float *arr2 = Arena_Alloc(&arena, 50 * sizeof(float));

    // Use allocations...

    // Free everything at once
    Arena_Free(&arena);

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

// Clay initializes arena once
Clay_Arena arena = {
    .nextAllocation = 0,
    .capacity = CLAY_MAX_ELEMENT_COUNT * sizeof(Clay_LayoutElement),
    .memory = arenaMemory
};

// All allocations from arena - no malloc in hot path!
```

**Benefits:**
- ✅ Very fast allocation (just increment pointer)
- ✅ No fragmentation
- ✅ Bulk deallocation (free everything at once)
- ✅ Cache-friendly (linear memory)
- ✅ Predictable memory usage

---

## 17.6 Memory Pools

Pre-allocate objects of same size.

### Basic Pool

```c
#define POOL_SIZE 100

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node nodes[POOL_SIZE];
    Node *freeList;
    int allocated;
} NodePool;

void Pool_Init(NodePool *pool) {
    // Chain all nodes into free list
    for (int i = 0; i < POOL_SIZE - 1; i++) {
        pool->nodes[i].next = &pool->nodes[i + 1];
    }
    pool->nodes[POOL_SIZE - 1].next = NULL;
    pool->freeList = &pool->nodes[0];
    pool->allocated = 0;
}

Node* Pool_Alloc(NodePool *pool) {
    if (pool->freeList == NULL) {
        return NULL;  // Pool exhausted
    }

    Node *node = pool->freeList;
    pool->freeList = node->next;
    pool->allocated++;

    return node;
}

void Pool_Free(NodePool *pool, Node *node) {
    node->next = pool->freeList;
    pool->freeList = node;
    pool->allocated--;
}

int main(void) {
    NodePool pool;
    Pool_Init(&pool);

    // Allocate nodes
    Node *n1 = Pool_Alloc(&pool);
    Node *n2 = Pool_Alloc(&pool);
    n1->value = 10;
    n2->value = 20;

    // Free nodes
    Pool_Free(&pool, n1);
    Pool_Free(&pool, n2);

    return 0;
}
```

**Benefits:**
- ✅ Constant-time allocation/deallocation
- ✅ No fragmentation
- ✅ Good cache locality
- ✅ Predictable performance

---

## 17.7 Memory Alignment

CPUs prefer aligned memory access.

### Alignment Basics

```c
#include <stddef.h>

typedef struct {
    char a;    // 1 byte
    // 3 bytes padding
    int b;     // 4 bytes (aligned to 4)
    char c;    // 1 byte
    // 3 bytes padding
} Unaligned;  // Total: 12 bytes

typedef struct {
    int b;     // 4 bytes
    char a;    // 1 byte
    char c;    // 1 byte
    // 2 bytes padding
} Optimized;  // Total: 8 bytes

int main(void) {
    printf("Unaligned: %zu\n", sizeof(Unaligned));  // 12
    printf("Optimized: %zu\n", sizeof(Optimized));  // 8

    printf("Alignment of int: %zu\n", _Alignof(int));  // 4
    printf("Alignment of double: %zu\n", _Alignof(double));  // 8

    return 0;
}
```

### Manual Alignment

```c
// Align size to power of 2
size_t alignSize(size_t size, size_t alignment) {
    return (size + alignment - 1) & ~(alignment - 1);
}

int main(void) {
    size_t size = 25;

    printf("Align to 8: %zu\n", alignSize(size, 8));   // 32
    printf("Align to 16: %zu\n", alignSize(size, 16)); // 32
    printf("Align to 32: %zu\n", alignSize(size, 32)); // 32

    return 0;
}
```

### aligned_alloc (C11)

```c
#include <stdlib.h>

int main(void) {
    // Allocate 1024 bytes aligned to 64-byte boundary
    void *p = aligned_alloc(64, 1024);

    if (p != NULL) {
        // p is guaranteed to be 64-byte aligned
        free(p);
    }

    return 0;
}
```

**Clay Example:**
```c
// Clay carefully orders struct members for optimal alignment
typedef struct {
    float width, height;  // 8 bytes total, 4-byte aligned
} Clay_Dimensions;

typedef struct {
    Clay_Vector2 x, y;    // 16 bytes total, 4-byte aligned
} Clay_BoundingBox;

// Compact and cache-friendly!
```

---

## 17.8 Custom Allocators

Create application-specific allocators.

### Debug Allocator

```c
typedef struct {
    void *ptr;
    size_t size;
    const char *file;
    int line;
} AllocationInfo;

#define MAX_ALLOCATIONS 1000
AllocationInfo allocations[MAX_ALLOCATIONS];
int allocationCount = 0;

void* debug_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);

    if (ptr != NULL && allocationCount < MAX_ALLOCATIONS) {
        allocations[allocationCount++] = (AllocationInfo){
            .ptr = ptr,
            .size = size,
            .file = file,
            .line = line
        };
    }

    return ptr;
}

void debug_free(void *ptr) {
    // Remove from tracking
    for (int i = 0; i < allocationCount; i++) {
        if (allocations[i].ptr == ptr) {
            allocations[i] = allocations[--allocationCount];
            break;
        }
    }
    free(ptr);
}

void report_leaks(void) {
    printf("Memory leaks: %d allocations\n", allocationCount);
    for (int i = 0; i < allocationCount; i++) {
        printf("  %zu bytes at %s:%d\n",
               allocations[i].size,
               allocations[i].file,
               allocations[i].line);
    }
}

#define MALLOC(size) debug_malloc(size, __FILE__, __LINE__)
#define FREE(ptr) debug_free(ptr)
```

### Scratch Allocator

```c
typedef struct {
    char buffer[4096];
    size_t used;
} ScratchAllocator;

ScratchAllocator scratch = {0};

void* scratch_alloc(size_t size) {
    if (scratch.used + size > sizeof(scratch.buffer)) {
        return NULL;
    }
    void *ptr = scratch.buffer + scratch.used;
    scratch.used += size;
    return ptr;
}

void scratch_reset(void) {
    scratch.used = 0;
}

// Use for temporary allocations
void process_frame(void) {
    char *temp = scratch_alloc(1000);
    // Use temp
    scratch_reset();  // Free everything
}
```

---

## 17.9 Clay's Memory Strategy

**Key principles:**

1. **Arena Allocation**: All memory from pre-allocated arenas
2. **No malloc in Hot Path**: Allocations done at initialization
3. **Predictable Memory**: Know exactly how much is needed
4. **Cache-Friendly**: Linear memory layout

```c
// User provides memory
char arenaMemory[CLAY_MAX_ELEMENT_COUNT * sizeof(Clay_LayoutElement)];

Clay_Arena arena = {
    .nextAllocation = 0,
    .capacity = sizeof(arenaMemory),
    .memory = arenaMemory
};

// Initialize Clay with arena
Clay_Initialize(arena, screenSize);

// Layout calculation uses only arena memory
Clay_BeginLayout();
// ... build UI
Clay_EndLayout();  // No allocations, just arena usage

// Can reset arena each frame if needed
arena.nextAllocation = 0;
```

---

## 17.10 Best Practices

### 1. Always Check malloc Return

```c
int *p = malloc(sizeof(int));
if (p == NULL) {
    // Handle error!
    return;
}
```

### 2. Free What You Allocate

```c
void function(void) {
    int *p = malloc(sizeof(int));
    // Use p
    free(p);  // Always free!
}
```

### 3. Set Pointers to NULL After Free

```c
free(p);
p = NULL;  // Prevent use-after-free
```

### 4. Use sizeof on Variables

```c
int *p = malloc(sizeof(*p));  // Safer than sizeof(int)
```

### 5. Consider Arena Allocators

```c
// Instead of many small allocations
int *a = malloc(sizeof(int));
int *b = malloc(sizeof(int));
// ...lots of malloc/free

// Use arena for related allocations
Arena arena;
Arena_Init(&arena, 1024);
int *a = Arena_Alloc(&arena, sizeof(int));
int *b = Arena_Alloc(&arena, sizeof(int));
// Free all at once
Arena_Free(&arena);
```

### 6. Profile Memory Usage

```bash
valgrind --tool=massif ./program
ms_print massif.out.12345
```

---

## 17.11 Key Concepts Learned

- ✅ Stack vs heap memory
- ✅ malloc, calloc, realloc, free
- ✅ Common memory errors and prevention
- ✅ Memory leak detection
- ✅ Arena allocators for fast bulk allocation
- ✅ Memory pools for same-sized objects
- ✅ Memory alignment and optimization
- ✅ Custom allocators
- ✅ Clay's efficient memory strategy
- ✅ Best practices for memory management

---

## Practice Exercises

1. Implement a dynamic array that grows automatically
2. Create a garbage collector using reference counting
3. Build a memory debugger that tracks all allocations
4. Implement a slab allocator for kernel-style allocation
5. Create a stack allocator with save/restore points
6. Build a ring buffer allocator
7. Implement buddy allocation system
8. Create thread-safe memory pool

