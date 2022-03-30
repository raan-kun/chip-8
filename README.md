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

## Usage
Run the `main` executable and give a path to the program you want to run as the only argument:

```./main path/to/program.c8```

The program will be loaded and started, and will display its output in a new window. The console window will display the contents of the virtual machine's registers. Some programs will make use of CHIP-8's keypad; this is emulated using the top-left corner of a standard QWERTY keyboard, with the following mapping:

```
CHIP-8 keypad layout
1 2 3 c
4 5 6 d
7 8 9 e
a 0 b f

QWERTY mapping
1 2 3 4
 q w e r
  a s d f
   z x c v
```

To close, just click the X button on the window border.
