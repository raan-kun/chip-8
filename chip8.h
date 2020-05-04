typedef struct chip8 {

    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16]; // 8-bit registers
    unsigned short index; // index register
    unsigned short pc; // program counter
    unsigned char display[64 * 32];
    unsigned char delay_timer;
    unsigned char sound_timer;

    // call stack
    unsigned short stack[16];
    unsigned short sp;

    unsigned char key[16];
} chip8;