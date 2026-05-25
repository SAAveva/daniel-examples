CC = cc
FLAGS = 
LIBS = -lSDL2 -lm

#run: str-demo
#	./str-demo

#str-demo: str-demo.c
#	$(CC) $(FLAGS) str-demo.c -o str-demo $(LIBS)

run: image
	./image

image: image.c
	$(CC) $(FLAGS) image.c -o image $(LIBS)

