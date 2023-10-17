CC = clang
CFLAGS = -std=c89 -fuse-ld=mold -Wall -O3
all: build run

build: lowfetch.c
	$(CC) lowfetch.c -o lowfetch $(CFLAGS)

run: lowfetch
	./lowfetch

clean:
	rm -f lowfetch
