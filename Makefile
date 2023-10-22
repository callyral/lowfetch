CC = clang
CFLAGS = -std=c99 -fuse-ld=mold -Wall -O3
DEBUGFLAGS = -std=c99 -fuse-ld=mold -Wall -g -O0
SRC = lowfetch.c include/lowfetch_base.c include/package_amount.c include/kernel_version.c include/argument_parsing.c

all: build run

build: lowfetch.c
	$(CC) $(SRC) -o lowfetch $(CFLAGS)

debug: lowfetch.c
	$(CC) $(SRC) -o debug $(DEBUGFLAGS)
	gdb debug

run: lowfetch
	./lowfetch

clean:
	rm -f lowfetch debug
