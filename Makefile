name := lowfetch
CXX = clang
CSTD = c89
LD = mold
all: build run

build: $(name).c
	$(CXX) $(name).c -o $(name) -std=$(CSTD) -fuse-ld=$(LD) -Wall

run: $(name)
	./$(name)

clean: $(name)
	rm $(name)
