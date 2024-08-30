cp ../../clay.h clay.c;
clang -c -o clay.o -static -target x86_64-apple-darwin clay.c -fPIC && ar r clay-odin/macos/clay.a clay.o;
clang -c -o clay.o -static clay.c -fPIC && ar r clay-odin/macos-arm64/clay.a clay.o;
clang -c -o clay-odin/windows/clay.lib -target x86_64-pc-windows-msvc -fuse-ld=llvm-lib -static clay.c;
rm clay.o;
rm clay.c;