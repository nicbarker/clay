# Building the Examples

This guide describes how to build and run the examples in this project.

---

## 1. Prerequisites

- **CMake** (3.25 or newer recommended)
- **C Compiler with WASM support** (GCC, Clang, or MSVC)
- **Specific SDKs or Libraries** for some examples:
    - [Playdate SDK](https://play.date/dev/) for Playdate examples
    - Raylib, Sokol, stb, termbox2: which are fetched automatically by CMake

---

## 2. Clone the Repository

```sh
git clone https://github.com/nicbarker/clay.git
cd clay
```

---

## 3. Build All Examples (except Playdate)

To build all examples (except the Playdate example which is not built by default):

```sh
cmake -B cmake-build
cmake --build cmake-build
```

---

## 4. Build Other Examples

If you want more control over which examples you want to build, you can use the following `cmake` options, which can be combined together:

You can selectively build a subset of the examples:

| `cmake` Option | Explanation |
|---|---|
| No options | Builds all examples except the Playdate examples |
| `-DCLAY_INCLUDE_ALL_EXAMPLES=OFF` | Does not build any examples unless told to through other options below |
| `-DCLAY_INCLUDE_DEMOS=ON` | Builds video demo and website |
| `-DCLAY_INCLUDE_CPP_EXAMPLE=ON` | Builds C++ example |
| `-DCLAY_INCLUDE_RAYLIB_EXAMPLES=ON` | Builds raylib examples |
| `-DCLAY_INCLUDE_SDL2_EXAMPLES=ON` | Builds SDL 2 examples |
| `-DCLAY_INCLUDE_SDL3_EXAMPLES=ON` | Builds SDL 3 examples |
| `-DCLAY_INCLUDE_WIN32_GDI_EXAMPLES=ON` | Builds Win32 GDI examples |
| `-DCLAY_INCLUDE_SOKOL_EXAMPLES=ON` | Builds Sokol examples |
| `-DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON` | Builds Playdate examples |

For example, to building the Playdate examples only we can use:

```bash
cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON -B cmake-build
cmake --build cmake-build
```

Alternatively, here are some cmake invocations to build a specific set of examples:

| Build intention | `cmake` Options |
|---|---|
| Raylib Examples Only | `cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_RAYLIB_EXAMPLES=ON -B cmake-build` |
| SDL2 Example Only | `cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_SDL2_EXAMPLES=ON -B cmake-build` |
| Sokol Examples Only | `cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_SOKOL_EXAMPLES=ON-B cmake-build` |
| Terminal Examples Only | `cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_DEMOS=ON-B cmake-build` |

> [!IMPORTANT]
> The Playdate examples equires the [Playdate SDK](https://play.date/dev/) to be installed.

---

## 5. Playdate Examples

The output `.pdx` file will be at:

```
cmake-build/examples/playdate-project-example/clay_playdate_example.pdx
```

You can open this file using the Playdate simulator.

### Building for the Playdate Hardware

Replace `/path/to/PlaydateSDK` with your Playdate SDK path:

```sh
cmake -DTOOLCHAIN=armgcc \
      -DCMAKE_TOOLCHAIN_FILE=/path/to/PlaydateSDK/C_API/buildsupport/arm.cmake \
      -DCLAY_INCLUDE_ALL_EXAMPLES=OFF \
      -DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON \
      -B cmake-release-playdate
cmake --build cmake-release-playdate
```

---

## 6. Web Example

To build the official website demo as a WebAssembly app, in order for you to be able to view it in the browser. An additional step is needed:

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

After building, the examples can be found in their respective subfolders within the output folder `cmake-build/examples`:

```sh
./cmake-build/examples/introducing-clay-video-demo/clay_examples_introducing_clay_video_demo
```

If the executable is missing, run the command `make` in the respective subfolder.

---

## Note

- Many examples automatically fetch dependencies using CMake's `FetchContent` – no manual installs needed for Raylib, Sokol, stb, or termbox2.
- For example-specific instructions, check the README files inside each `examples/` folder.

---
