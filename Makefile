SDL2FLAGS = `sdl2-config --cflags --libs`
CFLAGS = -Wall -pedantic $(SDL2FLAGS) -g
OBJ = main.o chip8.o
EXEC = main

$(EXEC): $(OBJ)
	gcc $(OBJ) $(SDL2FLAGS) -o $(EXEC)

main.o: main.c
	gcc $(CFLAGS) -c main.c

chip8.o: chip8.c chip8.h
	gcc $(CFLAGS) -c chip8.c

clean:
	rm -f main $(OBJ)