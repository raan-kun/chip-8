# CHIP-8 Emulator

A CHIP-8 interpreter for Unix-like systems - written in C and using SDL2 for the frontend. Chip logic is emulated before being sent off to SDL to render it. Currently sound is _not_ implemented.

## Prerequisites
This program depends only on SDL2. It links the headers using `sdl2-config` so you should use an installation that includes this program.

On Ubuntu:

```apt install libsdl2-dev```

On MacOS:

```brew install sdl2```

## Compiling
Use the included Makefile:

```$ make```

## Running
Just run the `main` executable and give a path to the program you want to run as the only argument:

```./main path/to/program.c8```
