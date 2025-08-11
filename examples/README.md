# Building the Examples

This guide describes how to build and run the examples in this project.

---

## 1. Prerequisites

- **CMake** (3.25 or newer recommended)
- **C Compiler with Wasm32-target support** (GCC, Clang, or MSVC)
- **Specific SDKs or Libraries** for some examples:
    - [Playdate SDK](https://play.date/dev/) for Playdate examples
    - Raylib, Sokol, stb, termbox2: fetched automatically by CMake

---

## 2. Clone the Repository

```sh
git clone https://github.com/nicbarker/clay.git
cd clay
```

---

## 3. Build All Examples

To build all examples (except Playdate by default):

```sh
cmake -DCLAY_INCLUDE_ALL_EXAMPLES=ON -B cmake-build-debug
cmake --build cmake-build-debug
```

---

## 4. Build Specific Example Sets

You can selectively enable examples:

- **Raylib Examples:**  
  `-DCLAY_INCLUDE_RAYLIB_EXAMPLES=ON`
- **SDL2 Example:**  
  `-DCLAY_INCLUDE_SDL2_EXAMPLES=ON`
- **Sokol Examples:**  
  `-DCLAY_INCLUDE_SOKOL_EXAMPLES=ON`
- **Terminal Examples:**  
  `-DCLAY_INCLUDE_DEMOS=ON`

Example (for Raylib only):

```sh
cmake -DCLAY_INCLUDE_RAYLIB_EXAMPLES=ON -B cmake-build-raylib
cmake --build cmake-build-raylib
```

---

## 5. Building the Playdate Example

**Requires the [Playdate SDK](https://play.date/dev/) to be installed.**

To build for the Playdate simulator:

```sh
cmake -DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON -B cmake-build-playdate
cmake --build cmake-build-playdate
```

The output `.pdx` file will be:
```
examples/playdate-project-example/clay_playdate_example.pdx
```
You can open this file using the Playdate simulator.

**To build for Playdate hardware:**  
Replace `/path/to/PlaydateSDK` with your Playdate SDK path.

```sh
cmake -DTOOLCHAIN=armgcc \
      -DCMAKE_TOOLCHAIN_FILE=/path/to/PlaydateSDK/C_API/buildsupport/arm.cmake \
      -DCLAY_INCLUDE_ALL_EXAMPLES=OFF \
      -DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON \
      -B cmake-build-release
cmake --build cmake-build-release
```

---

## 6. Building the Web Example (WASM)

To build the official website demo as a WebAssembly app:

```sh
cd examples/clay-official-website
./build.sh
```

> [!WARNING]
> If you are on Mac OS X and see the following error:
> ```logs
> error: unable to create target: 'No available targets are compatible with triple "wasm32"'
> ```
> ... then you may need to install `llvm` using homebrew and add it to your PATH, resulting in it being used instead of `clang` which comes bundled with Mac OS X (which does not support WASM). An alternative is using `emscripten` but this requires modifying the `build.sh` script.

Resulting files are found in the `./build/clay` subfolder:
- `build/clay/index.html`
- `build/clay/index.wasm`

In order to view the page correctly, you will need to serve the `build` folder via a webserver, and then navigate to `index.html` in a web browser.

Using vscode, open the `build` directory in own's workspace then use the Live Preview extension.

---

## 7. Running Examples

After building, executables are found in `cmake-build-*` directories or in their respective subfolders.  
Run them directly, e.g.:

```sh
./cmake-build-debug/examples/introducing-clay-video-demo/clay_examples_introducing_clay_video_demo
```

---

## Note

- Many examples automatically fetch dependencies using CMake's `FetchContent` – no manual installs needed for Raylib, Sokol, stb, or termbox2.
- For example-specific instructions, check the README files inside each `examples/` folder.

---
