cp ../../clay.h clay.c;
clang -c -o clay.o -static clay.c -fPIC && ar r clay-odin/clay.a clay.o;
rm clay.o;
rm clay.c;