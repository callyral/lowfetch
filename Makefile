CC = clang
CFLAGS = -std=c99 -fuse-ld=mold -Wall -O3
DEBUGFLAGS = -std=c99 -fuse-ld=mold -Wall -g -O0
SRC = src/main.c src/include/argument_parsing/argument_parsing.c src/include/definitions/colors.c src/include/distro/distro.c src/include/general/file_read.c src/include/kernel/kernel.c src/include/package_amount/package_amount.c src/include/shell/shell.c src/include/uptime/uptime.c

all: build run

build: $(SRC)
	rm -rf build/
	mkdir build/
	$(CC) $(SRC) -o build/lowfetch $(CFLAGS)

debug: $(SRC)
	rm -rf debug/
	mkdir debug/
	$(CC) $(SRC) -o debug/lowfetch $(DEBUGFLAGS)
	gdb debug/lowfetch

run: build/lowfetch
	./build/lowfetch

clean:
	rm -rf build/ debug/
