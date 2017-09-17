#ifndef CHIP8_H
#define CHIP8_H

#include "SDL.h"

class Chip8 {
public:
	Chip8();
	~Chip8();

	bool drawFlag;
	bool beepFlag;

	void emulateCycle();
	void debugRender();
	bool loadApplication(const char * filename);

	unsigned char gfx[64 * 32];
	unsigned char key[16];

private:
	unsigned short _pc;
	unsigned short _opcode;
	unsigned short _i;
	unsigned short _sp;

	unsigned char _v[16];
	unsigned short _stack[16];
	unsigned char _memory[4096];

	unsigned char _delayTimer;
	unsigned char _soundTimer;

	void init();
};

#endif
