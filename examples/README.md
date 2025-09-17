# Building the Examples

This guide explains how to build and run the examples in this project.

## Prerequisites

Before building the examples, ensure you have:

- **CMake** (version 3.25 or newer recommended)
- **C compiler with WebAssembly support** (GCC, Clang, or MSVC)
- **Platform-specific dependencies** for certain examples:
    - [Playdate SDK](https://play.date/dev/) for Playdate examples

Most dependencies (Raylib, Sokol, stb, termbox2) are automatically downloaded by CMake.

## Quick Start

1. **Clone the repository:**
   ```sh
   git clone https://github.com/nicbarker/clay.git
   cd clay
   ```

2. **Build all examples (except Playdate):**
   ```sh
   cmake -B cmake-build
   cmake --build cmake-build
   ```

## Selective Building

For more control over which examples to build, use these CMake options:

| CMake Option | Description |
|---|---|
| (no options) | Builds all examples except Playdate |
| `-DCLAY_INCLUDE_ALL_EXAMPLES=OFF` | Builds no examples unless specified |
| `-DCLAY_INCLUDE_DEMOS=ON` | Builds video demo and website |
| `-DCLAY_INCLUDE_CPP_EXAMPLE=ON` | Builds C++ example |
| `-DCLAY_INCLUDE_RAYLIB_EXAMPLES=ON` | Builds Raylib examples |
| `-DCLAY_INCLUDE_SDL2_EXAMPLES=ON` | Builds SDL2 examples |
| `-DCLAY_INCLUDE_SDL3_EXAMPLES=ON` | Builds SDL3 examples |
| `-DCLAY_INCLUDE_WIN32_GDI_EXAMPLES=ON` | Builds Win32 GDI examples |
| `-DCLAY_INCLUDE_SOKOL_EXAMPLES=ON` | Builds Sokol examples |
| `-DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON` | Builds Playdate examples |

### Example Commands

**Build only Playdate examples:**
```sh
cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON -B cmake-build
cmake --build cmake-build
```

**Build specific example types:**

| Example Type | Command |
|---|---|
| Raylib only | `cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_RAYLIB_EXAMPLES=ON -B cmake-build` |
| SDL2 only | `cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_SDL2_EXAMPLES=ON -B cmake-build` |
| Sokol only | `cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_SOKOL_EXAMPLES=ON -B cmake-build` |
| Terminal demos only | `cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_DEMOS=ON -B cmake-build` |

## Platform-Specific Instructions

### Playdate Examples

> **Note:** The [Playdate SDK](https://play.date/dev/) must be installed before building Playdate examples.

The compiled `.pdx` file will be located at:
```
cmake-build/examples/playdate-project-example/clay_playdate_example.pdx
```

Open this file in the Playdate simulator to run it.

**Building for Playdate hardware:**
```sh
cmake -DTOOLCHAIN=armgcc \
      -DCMAKE_TOOLCHAIN_FILE=/path/to/PlaydateSDK/C_API/buildsupport/arm.cmake \
      -DCLAY_INCLUDE_ALL_EXAMPLES=OFF \
      -DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON \
      -B cmake-release-playdate
cmake --build cmake-release-playdate
```

Replace `/path/to/PlaydateSDK` with your actual Playdate SDK installation path.

### WebAssembly Example

To build the official website demo for browsers:

```sh
cd examples/clay-official-website
./build.sh
```

**macOS WebAssembly Build Issues:**
If you encounter this error on macOS:
```
error: unable to create target: 'No available targets are compatible with triple "wasm32"'
```

**Solution 1 (Recommended):** Install LLVM via Homebrew:
```sh
brew install llvm
export PATH="/opt/homebrew/bin:$PATH"
```

**Solution 2:** Use Emscripten (requires modifying the `build.sh` script):
```sh
brew install emscripten
```

**Viewing the WebAssembly example:**
1. The build creates files in `./build/clay/`:
   - `build/clay/index.html`
   - `build/clay/index.wasm`

2. Serve the `build` folder using a web server (the files won't work when opened directly in a browser)

3. Navigate to `index.html` in your browser

**Using VS Code:** Open the `build` directory and use the Live Preview extension to serve the files.

## Running Examples

After building, find executables in their respective subfolders within `cmake-build/examples`:

```sh
./cmake-build/examples/introducing-clay-video-demo/clay_examples_introducing_clay_video_demo
```

If an executable is missing, run `make` in the respective example subfolder.

## Additional Notes

- Dependencies like Raylib, Sokol, stb, and termbox2 are automatically fetched by CMake
- For example-specific instructions, check the README files in individual `examples/` folders
- Most examples work out of the box once built, with no additional setup required