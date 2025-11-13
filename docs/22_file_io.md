# Chapter 22: File I/O in C

## Complete Guide with Clay Library Examples

---

## 22.1 Introduction to File I/O

C provides file operations through `stdio.h`:
- Open files
- Read/write data
- Close files
- Navigate file position
- Check file status

**Clay's Approach:** Clay doesn't do file I/O - it's a UI layout library. But applications using Clay need file operations for saving/loading UI state, configurations, etc.

---

## 22.2 Opening and Closing Files

### Basic File Operations

```c
#include <stdio.h>

int main(void) {
    FILE *file;

    // Open file for writing
    file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Write to file
    fprintf(file, "Hello, File!\n");

    // Close file
    fclose(file);

    return 0;
}
```

### File Modes

| Mode | Description | Creates if not exists | Truncates existing |
|------|-------------|----------------------|-------------------|
| `"r"` | Read only | No | No |
| `"w"` | Write only | Yes | Yes |
| `"a"` | Append | Yes | No |
| `"r+"` | Read/Write | No | No |
| `"w+"` | Read/Write | Yes | Yes |
| `"a+"` | Read/Append | Yes | No |
| `"rb"` | Read binary | No | No |
| `"wb"` | Write binary | Yes | Yes |

```c
FILE *fr = fopen("input.txt", "r");      // Read text
FILE *fw = fopen("output.txt", "w");     // Write text
FILE *fa = fopen("log.txt", "a");        // Append text
FILE *fb = fopen("data.bin", "rb");      // Read binary
```

---

## 22.3 Writing to Files

### fprintf - Formatted Output

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("data.txt", "w");
    if (file == NULL) {
        return 1;
    }

    int age = 25;
    float height = 5.9f;
    char name[] = "John";

    // Write formatted data
    fprintf(file, "Name: %s\n", name);
    fprintf(file, "Age: %d\n", age);
    fprintf(file, "Height: %.1f\n", height);

    fclose(file);
    return 0;
}
```

### fputs - Write String

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) return 1;

    fputs("Line 1\n", file);
    fputs("Line 2\n", file);
    fputs("Line 3\n", file);

    fclose(file);
    return 0;
}
```

### fputc - Write Character

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) return 1;

    fputc('H', file);
    fputc('i', file);
    fputc('\n', file);

    fclose(file);
    return 0;
}
```

### fwrite - Write Binary Data

```c
#include <stdio.h>

typedef struct {
    int id;
    char name[50];
    float salary;
} Employee;

int main(void) {
    FILE *file = fopen("employees.dat", "wb");
    if (file == NULL) return 1;

    Employee emp = {1, "John Doe", 50000.0f};

    // Write struct to binary file
    size_t written = fwrite(&emp, sizeof(Employee), 1, file);
    if (written != 1) {
        printf("Write error!\n");
    }

    fclose(file);
    return 0;
}
```

---

## 22.4 Reading from Files

### fscanf - Formatted Input

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) return 1;

    char name[50];
    int age;
    float height;

    // Read formatted data
    fscanf(file, "Name: %s\n", name);
    fscanf(file, "Age: %d\n", &age);
    fscanf(file, "Height: %f\n", &height);

    printf("Name: %s, Age: %d, Height: %.1f\n", name, age, height);

    fclose(file);
    return 0;
}
```

### fgets - Read Line

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) return 1;

    char buffer[256];

    // Read line by line
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
    return 0;
}
```

### fgetc - Read Character

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) return 1;

    int ch;

    // Read character by character
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);
    return 0;
}
```

### fread - Read Binary Data

```c
#include <stdio.h>

typedef struct {
    int id;
    char name[50];
    float salary;
} Employee;

int main(void) {
    FILE *file = fopen("employees.dat", "rb");
    if (file == NULL) return 1;

    Employee emp;

    // Read struct from binary file
    size_t read = fread(&emp, sizeof(Employee), 1, file);
    if (read == 1) {
        printf("ID: %d, Name: %s, Salary: %.2f\n",
               emp.id, emp.name, emp.salary);
    }

    fclose(file);
    return 0;
}
```

---

## 22.5 File Position

### ftell - Get Position

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) return 1;

    // Get current position
    long pos = ftell(file);
    printf("Position: %ld\n", pos);  // 0 (at start)

    // Read some data
    char buffer[10];
    fgets(buffer, sizeof(buffer), file);

    // Get new position
    pos = ftell(file);
    printf("Position after read: %ld\n", pos);

    fclose(file);
    return 0;
}
```

### fseek - Set Position

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) return 1;

    // Seek to end
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    printf("File size: %ld bytes\n", size);

    // Seek to beginning
    fseek(file, 0, SEEK_SET);

    // Seek forward 10 bytes
    fseek(file, 10, SEEK_CUR);

    fclose(file);
    return 0;
}
```

**Seek origins:**
- `SEEK_SET` - Beginning of file
- `SEEK_CUR` - Current position
- `SEEK_END` - End of file

### rewind - Reset to Beginning

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) return 1;

    // Read file
    char buffer[100];
    fgets(buffer, sizeof(buffer), file);

    // Go back to start
    rewind(file);  // Same as fseek(file, 0, SEEK_SET)

    // Read again
    fgets(buffer, sizeof(buffer), file);

    fclose(file);
    return 0;
}
```

---

## 22.6 File Status

### feof - Check End of File

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) return 1;

    char ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch != EOF) {
            putchar(ch);
        }
    }

    fclose(file);
    return 0;
}
```

### ferror - Check Errors

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) return 1;

    fprintf(file, "Test\n");

    if (ferror(file)) {
        printf("Error writing to file!\n");
        clearerr(file);  // Clear error flag
    }

    fclose(file);
    return 0;
}
```

### File Existence

```c
#include <stdio.h>

int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

int main(void) {
    if (fileExists("config.txt")) {
        printf("Config file found!\n");
    } else {
        printf("Config file not found!\n");
    }
    return 0;
}
```

---

## 22.7 Practical Examples

### Copy File

```c
#include <stdio.h>

int copyFile(const char *src, const char *dest) {
    FILE *source = fopen(src, "rb");
    if (source == NULL) return 0;

    FILE *target = fopen(dest, "wb");
    if (target == NULL) {
        fclose(source);
        return 0;
    }

    // Copy buffer by buffer
    char buffer[1024];
    size_t bytes;

    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytes, target);
    }

    fclose(source);
    fclose(target);
    return 1;
}

int main(void) {
    if (copyFile("input.txt", "output.txt")) {
        printf("File copied successfully!\n");
    }
    return 0;
}
```

### Count Lines in File

```c
#include <stdio.h>

int countLines(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return -1;

    int lines = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }

    fclose(file);
    return lines;
}

int main(void) {
    int lines = countLines("input.txt");
    printf("Lines: %d\n", lines);
    return 0;
}
```

### Read Entire File

```c
#include <stdio.h>
#include <stdlib.h>

char* readFile(const char *filename, size_t *size) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return NULL;

    // Get file size
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    // Allocate buffer
    char *buffer = (char*)malloc(filesize + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }

    // Read entire file
    size_t read = fread(buffer, 1, filesize, file);
    buffer[read] = '\0';  // Null terminate

    if (size != NULL) {
        *size = read;
    }

    fclose(file);
    return buffer;
}

int main(void) {
    size_t size;
    char *content = readFile("input.txt", &size);

    if (content != NULL) {
        printf("Content (%zu bytes):\n%s\n", size, content);
        free(content);
    }

    return 0;
}
```

---

## 22.8 Configuration File (Clay Example)

### Save Clay UI Configuration

```c
#include <stdio.h>

typedef struct {
    float width;
    float height;
    int layoutDirection;
    int padding;
} Clay_LayoutConfig;

int saveLayoutConfig(const char *filename, Clay_LayoutConfig *config) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) return 0;

    fprintf(file, "width=%.2f\n", config->width);
    fprintf(file, "height=%.2f\n", config->height);
    fprintf(file, "layoutDirection=%d\n", config->layoutDirection);
    fprintf(file, "padding=%d\n", config->padding);

    fclose(file);
    return 1;
}

int loadLayoutConfig(const char *filename, Clay_LayoutConfig *config) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return 0;

    fscanf(file, "width=%f\n", &config->width);
    fscanf(file, "height=%f\n", &config->height);
    fscanf(file, "layoutDirection=%d\n", &config->layoutDirection);
    fscanf(file, "padding=%d\n", &config->padding);

    fclose(file);
    return 1;
}

int main(void) {
    Clay_LayoutConfig config = {
        .width = 800.0f,
        .height = 600.0f,
        .layoutDirection = 0,
        .padding = 16
    };

    // Save config
    saveLayoutConfig("layout.cfg", &config);

    // Load config
    Clay_LayoutConfig loaded = {0};
    if (loadLayoutConfig("layout.cfg", &loaded)) {
        printf("Loaded: %.0fx%.0f, dir=%d, pad=%d\n",
               loaded.width, loaded.height,
               loaded.layoutDirection, loaded.padding);
    }

    return 0;
}
```

---

## 22.9 Binary File Format

### Save/Load Array

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int count;
    float *values;
} FloatArray;

int saveFloatArray(const char *filename, FloatArray *arr) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) return 0;

    // Write count
    fwrite(&arr->count, sizeof(int), 1, file);

    // Write values
    fwrite(arr->values, sizeof(float), arr->count, file);

    fclose(file);
    return 1;
}

FloatArray* loadFloatArray(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return NULL;

    FloatArray *arr = (FloatArray*)malloc(sizeof(FloatArray));

    // Read count
    fread(&arr->count, sizeof(int), 1, file);

    // Allocate and read values
    arr->values = (float*)malloc(arr->count * sizeof(float));
    fread(arr->values, sizeof(float), arr->count, file);

    fclose(file);
    return arr;
}

int main(void) {
    // Create array
    FloatArray arr;
    arr.count = 5;
    arr.values = (float*)malloc(5 * sizeof(float));
    for (int i = 0; i < 5; i++) {
        arr.values[i] = i * 1.5f;
    }

    // Save
    saveFloatArray("data.bin", &arr);

    // Load
    FloatArray *loaded = loadFloatArray("data.bin");
    if (loaded != NULL) {
        printf("Loaded %d values:\n", loaded->count);
        for (int i = 0; i < loaded->count; i++) {
            printf("%.1f ", loaded->values[i]);
        }
        printf("\n");

        free(loaded->values);
        free(loaded);
    }

    free(arr.values);
    return 0;
}
```

---

## 22.10 Error Handling

### Robust File Operations

```c
#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef enum {
    FILE_OK,
    FILE_ERROR_OPEN,
    FILE_ERROR_READ,
    FILE_ERROR_WRITE
} FileError;

FileError writeData(const char *filename, const char *data) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Open error: %s\n", strerror(errno));
        return FILE_ERROR_OPEN;
    }

    if (fputs(data, file) == EOF) {
        printf("Write error: %s\n", strerror(errno));
        fclose(file);
        return FILE_ERROR_WRITE;
    }

    fclose(file);
    return FILE_OK;
}

int main(void) {
    FileError err = writeData("output.txt", "Hello!\n");
    if (err != FILE_OK) {
        printf("Error code: %d\n", err);
    }
    return 0;
}
```

---

## 22.11 Temporary Files

```c
#include <stdio.h>

int main(void) {
    // Create temporary file
    FILE *temp = tmpfile();
    if (temp == NULL) {
        printf("Error creating temp file!\n");
        return 1;
    }

    // Use temp file
    fprintf(temp, "Temporary data\n");
    rewind(temp);

    char buffer[100];
    fgets(buffer, sizeof(buffer), temp);
    printf("Read: %s", buffer);

    // File automatically deleted on close
    fclose(temp);

    return 0;
}
```

---

## 22.12 Buffer Control

### Set Buffer Mode

```c
#include <stdio.h>

int main(void) {
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) return 1;

    // Disable buffering
    setbuf(file, NULL);

    // Or set custom buffer
    char buffer[BUFSIZ];
    setvbuf(file, buffer, _IOFBF, BUFSIZ);  // Full buffering

    // _IONBF = No buffering
    // _IOLBF = Line buffering
    // _IOFBF = Full buffering

    fprintf(file, "Data\n");

    // Force write buffer
    fflush(file);

    fclose(file);
    return 0;
}
```

---

## 22.13 Key Concepts Learned
- ✅ Opening and closing files
- ✅ File modes (read, write, append, binary)
- ✅ Writing: fprintf, fputs, fputc, fwrite
- ✅ Reading: fscanf, fgets, fgetc, fread
- ✅ File position: ftell, fseek, rewind
- ✅ File status: feof, ferror
- ✅ Binary file operations
- ✅ Error handling
- ✅ Configuration files
- ✅ Buffer control

---

## Practice Exercises

1. Create a text editor that can save/load files
2. Build a CSV parser and writer
3. Implement a simple database using binary files
4. Create a log file system with timestamps
5. Write a file encryption/decryption program
6. Build a file comparison utility (diff)
7. Implement a config file parser (INI format)
8. Create a file compression utility (run-length encoding)
