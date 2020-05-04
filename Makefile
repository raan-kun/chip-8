SDL2FLAGS = `sdl2-config --cflags --libs`
CFLAGS = -Wall -pedantic $(SDL2FLAGS) -g
OBJ = main.o chip8.o
EXEC = main

$(EXEC): $(OBJ)
	gcc $(OBJ) $(SDL2FLAGS) -o $(EXEC)

test_chip8: test_chip8.o chip8.o
	gcc test_chip8.o chip8.o -o test_chip8

test_chip8.o: test_chip8.c
	gcc $(CFLAGS) -c test_chip8.c

main.o: main.c
	gcc $(CFLAGS) -c main.c

chip8.o: chip8.c chip8.h
	gcc $(CFLAGS) -c chip8.c

clean:
	rm -f main test_chip8 $(OBJ) test_chip8.o