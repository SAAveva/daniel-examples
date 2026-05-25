CC = cc
FLAGS = 
LIBS = -lSDL2 -lm

run: build/image
	./build/image

build/image: image.c
	mkdir -p build
	$(CC) $(FLAGS) image.c -o build/image $(LIBS)

clean: build/
	rm build -rf
