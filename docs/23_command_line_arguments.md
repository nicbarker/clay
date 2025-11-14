# Chapter 23: Command-Line Arguments in C

## Complete Guide with Clay Library Examples

---

## 23.1 Introduction to Command-Line Arguments

When you run a program from the terminal:
```bash
./program arg1 arg2 arg3
```

C provides access to these arguments through `main`:

```c
int main(int argc, char *argv[]) {
    // argc = argument count
    // argv = argument vector (array of strings)
    return 0;
}
```

**Parts:**
- `argc` - Number of arguments (including program name)
- `argv` - Array of argument strings
- `argv[0]` - Program name
- `argv[1]` to `argv[argc-1]` - Actual arguments
- `argv[argc]` - NULL pointer

---

## 23.2 Basic Example

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Program name: %s\n", argv[0]);
    printf("Argument count: %d\n", argc);

    printf("Arguments:\n");
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d] = %s\n", i, argv[i]);
    }

    return 0;
}
```

**Running:**
```bash
$ ./program hello world 123
Program name: ./program
Argument count: 4
Arguments:
  argv[0] = ./program
  argv[1] = hello
  argv[2] = world
  argv[3] = 123
```

---

## 23.3 Processing Arguments

### Check Argument Count

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <name> <age>\n", argv[0]);
        return 1;
    }

    char *name = argv[1];
    int age = atoi(argv[2]);  // Convert string to int

    printf("Name: %s, Age: %d\n", name, age);
    return 0;
}
```

**Running:**
```bash
$ ./program John 25
Name: John, Age: 25

$ ./program John
Usage: ./program <name> <age>
```

---

## 23.4 Parsing Numeric Arguments

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <a> <b> <c>\n", argv[0]);
        return 1;
    }

    // Convert arguments to numbers
    int a = atoi(argv[1]);           // Integer
    long b = atol(argv[2]);          // Long
    double c = atof(argv[3]);        // Double

    printf("a = %d\n", a);
    printf("b = %ld\n", b);
    printf("c = %.2f\n", c);

    return 0;
}
```

**Running:**
```bash
$ ./program 10 1000000 3.14
a = 10
b = 1000000
c = 3.14
```

---

## 23.5 Command-Line Flags

### Simple Flags

```c
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int verbose = 0;
    int debug = 0;

    // Check for flags
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 ||
            strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        }
        else if (strcmp(argv[i], "-d") == 0 ||
                 strcmp(argv[i], "--debug") == 0) {
            debug = 1;
        }
    }

    printf("Verbose: %s\n", verbose ? "ON" : "OFF");
    printf("Debug: %s\n", debug ? "ON" : "OFF");

    return 0;
}
```

**Running:**
```bash
$ ./program -v
Verbose: ON
Debug: OFF

$ ./program -v -d
Verbose: ON
Debug: ON

$ ./program --verbose --debug
Verbose: ON
Debug: ON
```

---

## 23.6 Flags with Values

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *output = "output.txt";  // Default
    int level = 1;                // Default

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output = argv[++i];  // Next argument is the value
        }
        else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            level = atoi(argv[++i]);
        }
    }

    printf("Output: %s\n", output);
    printf("Level: %d\n", level);

    return 0;
}
```

**Running:**
```bash
$ ./program -o result.txt -l 5
Output: result.txt
Level: 5

$ ./program
Output: output.txt
Level: 1
```

---

## 23.7 getopt for Option Parsing

POSIX provides `getopt` for standard option parsing:

```c
#include <stdio.h>
#include <unistd.h>  // POSIX

int main(int argc, char *argv[]) {
    int opt;
    int verbose = 0;
    char *output = NULL;
    int level = 1;

    // Parse options
    while ((opt = getopt(argc, argv, "vo:l:")) != -1) {
        switch (opt) {
            case 'v':
                verbose = 1;
                break;
            case 'o':
                output = optarg;  // Option argument
                break;
            case 'l':
                level = atoi(optarg);
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                return 1;
        }
    }

    // Process remaining arguments
    for (int i = optind; i < argc; i++) {
        printf("Non-option arg: %s\n", argv[i]);
    }

    printf("Verbose: %d\n", verbose);
    printf("Output: %s\n", output ? output : "none");
    printf("Level: %d\n", level);

    return 0;
}
```

**Running:**
```bash
$ ./program -v -o test.txt -l 3 file1 file2
Non-option arg: file1
Non-option arg: file2
Verbose: 1
Output: test.txt
Level: 3
```

---

## 23.8 Clay Example: UI Configuration

Imagine a Clay-based application with command-line options:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int width;
    int height;
    int fullscreen;
    char *title;
} AppConfig;

void printUsage(const char *programName) {
    printf("Usage: %s [options]\n", programName);
    printf("Options:\n");
    printf("  -w <width>     Window width (default: 800)\n");
    printf("  -h <height>    Window height (default: 600)\n");
    printf("  -f             Fullscreen mode\n");
    printf("  -t <title>     Window title\n");
    printf("  --help         Show this help\n");
}

int parseArgs(int argc, char *argv[], AppConfig *config) {
    // Defaults
    config->width = 800;
    config->height = 600;
    config->fullscreen = 0;
    config->title = "Clay Application";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-w") == 0 && i + 1 < argc) {
            config->width = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
            config->height = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-f") == 0) {
            config->fullscreen = 1;
        }
        else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            config->title = argv[++i];
        }
        else if (strcmp(argv[i], "--help") == 0) {
            return 0;  // Show help
        }
        else {
            printf("Unknown option: %s\n", argv[i]);
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[]) {
    AppConfig config;

    if (!parseArgs(argc, argv, &config)) {
        printUsage(argv[0]);
        return 1;
    }

    printf("Starting Clay application:\n");
    printf("  Size: %dx%d\n", config.width, config.height);
    printf("  Fullscreen: %s\n", config.fullscreen ? "Yes" : "No");
    printf("  Title: %s\n", config.title);

    // Initialize Clay with config
    // Clay_Dimensions screenSize = {config.width, config.height};
    // Clay_Initialize(...);

    return 0;
}
```

**Running:**
```bash
$ ./clay_app -w 1024 -h 768 -f -t "My UI"
Starting Clay application:
  Size: 1024x768
  Fullscreen: Yes
  Title: My UI

$ ./clay_app --help
Usage: ./clay_app [options]
Options:
  -w <width>     Window width (default: 800)
  -h <height>    Window height (default: 600)
  -f             Fullscreen mode
  -t <title>     Window title
  --help         Show this help
```

---

## 23.9 Subcommands (git-style)

```c
#include <stdio.h>
#include <string.h>

void cmd_init(int argc, char *argv[]) {
    printf("Initializing...\n");
}

void cmd_add(int argc, char *argv[]) {
    if (argc < 1) {
        printf("Error: add requires a file argument\n");
        return;
    }
    printf("Adding: %s\n", argv[0]);
}

void cmd_commit(int argc, char *argv[]) {
    char *message = "Default message";
    if (argc >= 2 && strcmp(argv[0], "-m") == 0) {
        message = argv[1];
    }
    printf("Committing: %s\n", message);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        printf("Commands: init, add, commit\n");
        return 1;
    }

    char *command = argv[1];
    int cmd_argc = argc - 2;
    char **cmd_argv = argv + 2;

    if (strcmp(command, "init") == 0) {
        cmd_init(cmd_argc, cmd_argv);
    }
    else if (strcmp(command, "add") == 0) {
        cmd_add(cmd_argc, cmd_argv);
    }
    else if (strcmp(command, "commit") == 0) {
        cmd_commit(cmd_argc, cmd_argv);
    }
    else {
        printf("Unknown command: %s\n", command);
        return 1;
    }

    return 0;
}
```

**Running:**
```bash
$ ./mygit init
Initializing...

$ ./mygit add file.txt
Adding: file.txt

$ ./mygit commit -m "Initial commit"
Committing: Initial commit
```

---

## 23.10 Environment Variables

Access environment variables alongside arguments:

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
    printf("Arguments:\n");
    for (int i = 0; i < argc; i++) {
        printf("  %s\n", argv[i]);
    }

    printf("\nEnvironment Variables:\n");
    for (int i = 0; envp[i] != NULL; i++) {
        printf("  %s\n", envp[i]);
    }

    // Or use getenv
    char *home = getenv("HOME");
    printf("\nHOME: %s\n", home);

    return 0;
}
```

---

## 23.11 Argument Validation

### Robust Argument Parser

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

int parseInteger(const char *str, int *out) {
    char *endptr;
    errno = 0;

    long val = strtol(str, &endptr, 10);

    if (errno == ERANGE || val > INT_MAX || val < INT_MIN) {
        return 0;  // Out of range
    }

    if (endptr == str || *endptr != '\0') {
        return 0;  // Not a valid number
    }

    *out = (int)val;
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int value;
    if (!parseInteger(argv[1], &value)) {
        printf("Error: '%s' is not a valid integer\n", argv[1]);
        return 1;
    }

    printf("Valid integer: %d\n", value);
    return 0;
}
```

---

## 23.12 Configuration Priority

Combine defaults, config files, and command-line arguments:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int port;
    char *host;
    int debug;
} ServerConfig;

void setDefaults(ServerConfig *config) {
    config->port = 8080;
    config->host = "localhost";
    config->debug = 0;
}

void loadConfigFile(ServerConfig *config, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "port=", 5) == 0) {
            config->port = atoi(line + 5);
        }
        else if (strncmp(line, "host=", 5) == 0) {
            config->host = strdup(line + 5);
            // Remove newline
            config->host[strcspn(config->host, "\n")] = 0;
        }
    }

    fclose(file);
}

void parseArgs(ServerConfig *config, int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            config->port = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-h") == 0 && i + 1 < argc) {
            config->host = argv[++i];
        }
        else if (strcmp(argv[i], "-d") == 0) {
            config->debug = 1;
        }
    }
}

int main(int argc, char *argv[]) {
    ServerConfig config;

    // Priority: defaults < config file < command line
    setDefaults(&config);
    loadConfigFile(&config, "server.cfg");
    parseArgs(&config, argc, argv);

    printf("Server configuration:\n");
    printf("  Host: %s\n", config.host);
    printf("  Port: %d\n", config.port);
    printf("  Debug: %s\n", config.debug ? "ON" : "OFF");

    return 0;
}
```

---

## 23.13 Wildcard Arguments

Handle wildcards expanded by shell:

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <files...>\n", argv[0]);
        return 1;
    }

    printf("Processing %d files:\n", argc - 1);
    for (int i = 1; i < argc; i++) {
        printf("  - %s\n", argv[i]);
    }

    return 0;
}
```

**Running:**
```bash
$ ./program *.txt
Processing 3 files:
  - file1.txt
  - file2.txt
  - file3.txt
```

---

## 23.14 Interactive vs Batch Mode

```c
#include <stdio.h>
#include <string.h>

int interactiveMode(void) {
    char input[100];

    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        // Remove newline
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "quit") == 0) {
            break;
        }

        printf("You entered: %s\n", input);
    }

    return 0;
}

int batchMode(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open %s\n", filename);
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        // Process line
        printf("Processing: %s", line);
    }

    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // No arguments: interactive mode
        printf("Interactive mode (type 'quit' to exit)\n");
        return interactiveMode();
    }
    else {
        // Arguments provided: batch mode
        for (int i = 1; i < argc; i++) {
            printf("Processing file: %s\n", argv[i]);
            batchMode(argv[i]);
        }
        return 0;
    }
}
```

---

## 23.15 Clay Application Example

Complete Clay application with arguments:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define CLAY_IMPLEMENTATION
// #include "clay.h"

typedef struct {
    int width;
    int height;
    int fps;
    char *layout_file;
    int benchmark;
} ClayAppConfig;

void printHelp(const char *program) {
    printf("Usage: %s [options] [layout_file]\n", program);
    printf("\nOptions:\n");
    printf("  -w, --width <N>      Window width (default: 1024)\n");
    printf("  -h, --height <N>     Window height (default: 768)\n");
    printf("  --fps <N>            Target FPS (default: 60)\n");
    printf("  --benchmark          Run in benchmark mode\n");
    printf("  --help               Show this help\n");
    printf("\nExamples:\n");
    printf("  %s --width 1920 --height 1080 layout.clay\n", program);
    printf("  %s --benchmark\n", program);
}

int parseArguments(int argc, char *argv[], ClayAppConfig *config) {
    // Defaults
    config->width = 1024;
    config->height = 768;
    config->fps = 60;
    config->layout_file = NULL;
    config->benchmark = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) {
            if (++i >= argc) return 0;
            config->width = atoi(argv[i]);
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0) {
            if (++i >= argc) return 0;
            config->height = atoi(argv[i]);
        }
        else if (strcmp(argv[i], "--fps") == 0) {
            if (++i >= argc) return 0;
            config->fps = atoi(argv[i]);
        }
        else if (strcmp(argv[i], "--benchmark") == 0) {
            config->benchmark = 1;
        }
        else if (strcmp(argv[i], "--help") == 0) {
            return 0;
        }
        else if (argv[i][0] != '-') {
            config->layout_file = argv[i];
        }
        else {
            printf("Unknown option: %s\n", argv[i]);
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[]) {
    ClayAppConfig config;

    if (!parseArguments(argc, argv, &config)) {
        printHelp(argv[0]);
        return 1;
    }

    printf("Clay Application Starting:\n");
    printf("  Resolution: %dx%d\n", config.width, config.height);
    printf("  Target FPS: %d\n", config.fps);
    printf("  Benchmark: %s\n", config.benchmark ? "Yes" : "No");
    if (config.layout_file) {
        printf("  Layout File: %s\n", config.layout_file);
    }

    // Initialize Clay
    // Clay_Dimensions windowSize = {config.width, config.height};
    // Clay_Initialize(...);

    // Main loop...

    return 0;
}
```

---

## 23.16 Key Concepts Learned
- ✅ argc and argv basics
- ✅ Processing command-line arguments
- ✅ Parsing numeric arguments
- ✅ Command-line flags
- ✅ Flags with values
- ✅ getopt for option parsing
- ✅ Subcommands
- ✅ Environment variables
- ✅ Argument validation
- ✅ Configuration priority
- ✅ Interactive vs batch mode
- ✅ Clay application with arguments

---

## Practice Exercises

1. Build a calculator that accepts expressions as arguments
2. Create a file search utility with filtering options
3. Implement a CSV processor with column selection
4. Build a simple HTTP server with configurable port
5. Create a code formatter with style options
6. Implement a task manager CLI (add, list, complete)
7. Build an image converter with format options
8. Create a benchmarking tool with various options
