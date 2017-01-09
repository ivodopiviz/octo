#include "chip8.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned char chip8Font[80] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	0x20, 0x60, 0x20, 0x20, 0x70, //1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	0x90, 0x90, 0xF0, 0x10, 0x10, //4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	0xF0, 0x10, 0x20, 0x40, 0x40, //7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

Chip8::Chip8()
{
	// empty
}

Chip8::~Chip8()
{
	// empty
}

void Chip8::init()
{
	_pc = 0x200;
	_opcode = 0;
	_i = 0;
	_sp = 0;

	for (int i = 0; i < 2048; ++i)
		gfx[i] = 0;

	for (int i = 0; i < 16; ++i)
		_stack[i] = 0;

	for (int i = 0; i < 16; ++i)
		key[i] = _v[i] = 0;

	for (int i = 0; i < 4096; ++i)
		_memory[i] = 0;

	for (int i = 0; i < 80; ++i)
		_memory[i] = chip8Font[i];

	_delayTimer = 0;
	_soundTimer = 0;

	drawFlag = true;
	srand(time(NULL));
}

void Chip8::emulateCycle()
{
	_opcode = _memory[_pc] << 8 | _memory[_pc + 1];

	switch (_opcode & 0xF000)
	{
	case 0x0000:
		switch (_opcode & 0x000F)
		{
		case 0x0000:
			for (int i = 0; i < 2048; ++i)
				gfx[i] = 0x0;
			drawFlag = true;
			_pc += 2;
			break;

		case 0x000E:
			--_sp;
			_pc = _stack[_sp];
			_pc += 2;
			break;

		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", _opcode);
		}
		break;

	case 0x1000:
		_pc = _opcode & 0x0FFF;
		break;

	case 0x2000:
		_stack[_sp] = _pc;
		++_sp;
		_pc = _opcode & 0x0FFF;
		break;

	case 0x3000:
		if (_v[(_opcode & 0x0F00) >> 8] == (_opcode & 0x00FF))
			_pc += 4;
		else
			_pc += 2;
		break;

	case 0x4000:
		if (_v[(_opcode & 0x0F00) >> 8] != (_opcode & 0x00FF))
			_pc += 4;
		else
			_pc += 2;
		break;

	case 0x5000:
		if (_v[(_opcode & 0x0F00) >> 8] == _v[(_opcode & 0x00F0) >> 4])
			_pc += 4;
		else
			_pc += 2;
		break;

	case 0x6000:
		_v[(_opcode & 0x0F00) >> 8] = _opcode & 0x00FF;
		_pc += 2;
		break;

	case 0x7000:
		_v[(_opcode & 0x0F00) >> 8] += _opcode & 0x00FF;
		_pc += 2;
		break;

	case 0x8000:
		switch (_opcode & 0x000F)
		{
		case 0x0000:
			_v[(_opcode & 0x0F00) >> 8] = _v[(_opcode & 0x00F0) >> 4];
			_pc += 2;
			break;

		case 0x0001:
			_v[(_opcode & 0x0F00) >> 8] |= _v[(_opcode & 0x00F0) >> 4];
			_pc += 2;
			break;

		case 0x0002:
			_v[(_opcode & 0x0F00) >> 8] &= _v[(_opcode & 0x00F0) >> 4];
			_pc += 2;
			break;

		case 0x0003:
			_v[(_opcode & 0x0F00) >> 8] ^= _v[(_opcode & 0x00F0) >> 4];
			_pc += 2;
			break;

		case 0x0004:
			if (_v[(_opcode & 0x00F0) >> 4] > (0xFF - _v[(_opcode & 0x0F00) >> 8]))
				_v[0xF] = 1;
			else
				_v[0xF] = 0;
			_v[(_opcode & 0x0F00) >> 8] += _v[(_opcode & 0x00F0) >> 4];
			_pc += 2;
			break;

		case 0x0005:
			if (_v[(_opcode & 0x00F0) >> 4] > _v[(_opcode & 0x0F00) >> 8])
				_v[0xF] = 0;
			else
				_v[0xF] = 1;
			_v[(_opcode & 0x0F00) >> 8] -= _v[(_opcode & 0x00F0) >> 4];
			_pc += 2;
			break;

		case 0x0006:
			_v[0xF] = _v[(_opcode & 0x0F00) >> 8] & 0x1;
			_v[(_opcode & 0x0F00) >> 8] >>= 1;
			_pc += 2;
			break;

		case 0x0007:
			if (_v[(_opcode & 0x0F00) >> 8] > _v[(_opcode & 0x00F0) >> 4])	// VY-VX
				_v[0xF] = 0;
			else
				_v[0xF] = 1;
			_v[(_opcode & 0x0F00) >> 8] = _v[(_opcode & 0x00F0) >> 4] - _v[(_opcode & 0x0F00) >> 8];
			_pc += 2;
			break;

		case 0x000E:
			_v[0xF] = _v[(_opcode & 0x0F00) >> 8] >> 7;
			_v[(_opcode & 0x0F00) >> 8] <<= 1;
			_pc += 2;
			break;

		default:
			printf("Unknown opcode [0x8000]: 0x%X\n", _opcode);
		}
		break;

	case 0x9000:
		if (_v[(_opcode & 0x0F00) >> 8] != _v[(_opcode & 0x00F0) >> 4])
			_pc += 4;
		else
			_pc += 2;
		break;

	case 0xA000:
		_i = _opcode & 0x0FFF;
		_pc += 2;
		break;

	case 0xB000:
		_pc = (_opcode & 0x0FFF) + _v[0];
		break;

	case 0xC000:
		_v[(_opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (_opcode & 0x00FF);
		_pc += 2;
		break;

	case 0xD000: 
	{
		unsigned short x = _v[(_opcode & 0x0F00) >> 8];
		unsigned short y = _v[(_opcode & 0x00F0) >> 4];
		unsigned short height = _opcode & 0x000F;
		unsigned short pixel;

		_v[0xF] = 0;
		for (int yline = 0; yline < height; yline++)
		{
			pixel = _memory[_i + yline];
			for (int xline = 0; xline < 8; xline++)
			{
				if ((pixel & (0x80 >> xline)) != 0)
				{
					if (gfx[(x + xline + ((y + yline) * 64))] == 1)
					{
						_v[0xF] = 1;
					}
					gfx[x + xline + ((y + yline) * 64)] ^= 1;
				}
			}
		}

		drawFlag = true;
		_pc += 2;
	}
	break;

	case 0xE000:
		switch (_opcode & 0x00FF)
		{
		case 0x009E:
			if (key[_v[(_opcode & 0x0F00) >> 8]] != 0)
				_pc += 4;
			else
				_pc += 2;
			break;

		case 0x00A1:
			if (key[_v[(_opcode & 0x0F00) >> 8]] == 0)
				_pc += 4;
			else
				_pc += 2;
			break;

		default:
			printf("Unknown opcode [0xE000]: 0x%X\n", _opcode);
		}
		break;

	case 0xF000:
		switch (_opcode & 0x00FF)
		{
		case 0x0007:
			_v[(_opcode & 0x0F00) >> 8] = _delayTimer;
			_pc += 2;
			break;

		case 0x000A:
		{
			bool keyPress = false;

			for (int i = 0; i < 16; ++i)
			{
				if (key[i] != 0)
				{
					_v[(_opcode & 0x0F00) >> 8] = i;
					keyPress = true;
				}
			}

			if (!keyPress)
				return;

			_pc += 2;
		}
		break;

		case 0x0015:
			_delayTimer = _v[(_opcode & 0x0F00) >> 8];
			_pc += 2;
			break;

		case 0x0018:
			_soundTimer = _v[(_opcode & 0x0F00) >> 8];
			_pc += 2;
			break;

		case 0x001E:
			if (_i + _v[(_opcode & 0x0F00) >> 8] > 0xFFF)
				_v[0xF] = 1;
			else
				_v[0xF] = 0;
			_i += _v[(_opcode & 0x0F00) >> 8];
			_pc += 2;
			break;

		case 0x0029:
			_i = _v[(_opcode & 0x0F00) >> 8] * 0x5;
			_pc += 2;
			break;

		case 0x0033:
			_memory[_i] = _v[(_opcode & 0x0F00) >> 8] / 100;
			_memory[_i + 1] = (_v[(_opcode & 0x0F00) >> 8] / 10) % 10;
			_memory[_i + 2] = (_v[(_opcode & 0x0F00) >> 8] % 100) % 10;
			_pc += 2;
			break;

		case 0x0055:
			for (int i = 0; i <= ((_opcode & 0x0F00) >> 8); ++i)
				_memory[_i + i] = _v[i];

			_i += ((_opcode & 0x0F00) >> 8) + 1;
			_pc += 2;
			break;

		case 0x0065:
			for (int i = 0; i <= ((_opcode & 0x0F00) >> 8); ++i)
				_v[i] = _memory[_i + i];

			_i += ((_opcode & 0x0F00) >> 8) + 1;
			_pc += 2;
			break;

		default:
			printf("Unknown opcode [0xF000]: 0x%X\n", _opcode);
		}
		break;

	default:
		printf("Unknown opcode: 0x%X\n", _opcode);
	}

	if (_delayTimer > 0)
		--_delayTimer;

	if (_soundTimer > 0)
	{
		if (_soundTimer == 1)
			printf("Beep!\n");
		--_soundTimer;
	}
}

void Chip8::debugRender()
{
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 64; ++x)
		{
			if (gfx[(y * 64) + x] == 0)
				printf("O");
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

bool Chip8::loadApplication(const char * filename)
{
	init();
	printf("Loading: %s\n", filename);

	FILE * pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		return false;
	}

	fseek(pFile, 0, SEEK_END);
	long lSize = ftell(pFile);
	rewind(pFile);
	printf("Filesize: %d\n", (int)lSize);

	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		return false;
	}

	size_t result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		return false;
	}

	if ((4096 - 512) > lSize)
	{
		for (int i = 0; i < lSize; ++i)
			_memory[i + 512] = buffer[i];
	}
	else
		printf("Error: ROM too big for memory");

	fclose(pFile);
	free(buffer);

	return true;
}