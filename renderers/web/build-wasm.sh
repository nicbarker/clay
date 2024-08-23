cp ../../clay.h clay.c && 		\
clang 							\
-Os 							\
-DCLAY_WASM 					\
-mbulk-memory 					\
--target=wasm32 				\
-nostdlib 						\
-Wl,--strip-all 				\
-Wl,--export-dynamic 			\
-Wl,--no-entry 					\
-Wl,--export=__heap_base 		\
-Wl,--initial-memory=6553600 	\
-o clay.wasm clay.c; rm clay.c;