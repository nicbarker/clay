cp ../../clay.h clay.c;
# Intel Mac
clang -c -o clay.o -static -target x86_64-apple-darwin clay.c -fPIC && ar r clay-odin/macos/clay.a clay.o;
# ARM Mac
clang -c -o clay.o -static clay.c -fPIC && ar r clay-odin/macos-arm64/clay.a clay.o;
# x64 Windows
clang -c -o clay-odin/windows/clay.lib -target x86_64-pc-windows-msvc -fuse-ld=llvm-lib -static clay.c;
# Linux
clang -c -o clay.o -static -target x86_64-unknown-linux-gnu clay.c -fPIC && ar r clay-odin/linux/clay.a clay.o;
# WASM
clang -c -o clay-odin/wasm/clay.o -target wasm32 -nostdlib -static clay.c;
rm clay.o;
rm clay.c;
