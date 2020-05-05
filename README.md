# CHIP-8 Emulator

A CHIP-8 interpreter written in C and using SDL2 for the frontend. Chip logic is emulated before being sent off to SDL to render it, so performance is slow -- but it works! Currently sound is _not_ implemented.

## Running the program
First, compile it using the included Makefile
```$ make```

Then, just run the `main` executable. Give a path to the program you want to run as an argument.
```./main path/to/program.c8```