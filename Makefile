CC = clang
CFLAGS = -std=c99 -fuse-ld=mold -Wall -O3
DEBUGFLAGS = -std=c99 -fuse-ld=mold -Wall -g -O0
SRC = src/lowfetch.c src/include/lowfetch_base.c src/include/package_amount.c src/include/kernel_version.c src/include/argument_parsing.c

all: build run

build: $(SRC)
	mkdir build/
	$(CC) $(SRC) -o build/lowfetch $(CFLAGS)

debug: $(SRC)
	mkdir debug/
	$(CC) $(SRC) -o debug/lowfetch $(DEBUGFLAGS)
	gdb debug/lowfetch

run: build/lowfetch
	./build/lowfetch

clean:
	rm -rf build/ debug/
