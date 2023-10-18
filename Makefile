CC = clang
CFLAGS = -std=c99 -fuse-ld=mold -Wall -O3
DEBUGFLAGS = -std=c99 -fuse-ld=mold -Wall -g -O0
all: build run

build: lowfetch.c
	$(CC) lowfetch.c -o lowfetch $(CFLAGS)

debug: lowfetch.c
	$(CC) lowfetch.c -o debug $(DEBUGFLAGS)
	gdb debug

run: lowfetch
	./lowfetch

clean:
	rm -f lowfetch debug
