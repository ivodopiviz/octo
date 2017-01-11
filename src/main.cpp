#include <stdio.h>
#include "SDL.h"
#include "nfd.h"
#include "Chip8.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

Chip8 chip8;
int zoom = 10;

// Window size
int display_width = SCREEN_WIDTH * zoom;
int display_height = SCREEN_HEIGHT * zoom;

int main(int argc, char* argv[]) 
{
	if (argc < 2)
	{
		printf("Usage: octo.exe rom\n");
		return 1;
	}

	// Load game
	if (!chip8.loadApplication(argv[1]))
		return 1;

	bool quit = false;
	SDL_Event e;

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(display_width, display_height, 0, &window, &renderer);
	SDL_SetWindowTitle(window, "Octo");
	SDL_RenderSetScale(renderer, zoom, zoom);
	
	if (window == NULL) 
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	if (renderer == NULL)
	{
		printf("Could not create renderer: %s\n", SDL_GetError());
		return 1;
	}

	while (!quit)
	{
		chip8.emulateCycle();
		SDL_RenderClear(renderer);

		while (SDL_PollEvent(&e) != 0) 
		{
			if(e.type == SDL_QUIT)
			{ 
				quit = true;
			}

			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
					case SDLK_1: chip8.key[0x1] = 1; break;
					case SDLK_2: chip8.key[0x2] = 1; break;
					case SDLK_3: chip8.key[0x3] = 1; break;
					case SDLK_4: chip8.key[0xC] = 1; break;
					
					case SDLK_q: chip8.key[0x4] = 1; break;
					case SDLK_w: chip8.key[0x5] = 1; break;
					case SDLK_e: chip8.key[0x6] = 1; break;
					case SDLK_r: chip8.key[0xD] = 1; break;

					case SDLK_a: chip8.key[0x7] = 1; break;
					case SDLK_s: chip8.key[0x8] = 1; break;
					case SDLK_d: chip8.key[0x9] = 1; break;
					case SDLK_f: chip8.key[0xE] = 1; break;

					case SDLK_z: chip8.key[0xA] = 1; break;
					case SDLK_x: chip8.key[0x0] = 1; break;
					case SDLK_c: chip8.key[0xB] = 1; break;
					case SDLK_v: chip8.key[0xF] = 1; break;
				}
			}

			if (e.type == SDL_KEYUP)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_1: chip8.key[0x1] = 0; break;
				case SDLK_2: chip8.key[0x2] = 0; break;
				case SDLK_3: chip8.key[0x3] = 0; break;
				case SDLK_4: chip8.key[0xC] = 0; break;

				case SDLK_q: chip8.key[0x4] = 0; break;
				case SDLK_w: chip8.key[0x5] = 0; break;
				case SDLK_e: chip8.key[0x6] = 0; break;
				case SDLK_r: chip8.key[0xD] = 0; break;

				case SDLK_a: chip8.key[0x7] = 0; break;
				case SDLK_s: chip8.key[0x8] = 0; break;
				case SDLK_d: chip8.key[0x9] = 0; break;
				case SDLK_f: chip8.key[0xE] = 0; break;

				case SDLK_z: chip8.key[0xA] = 0; break;
				case SDLK_x: chip8.key[0x0] = 0; break;
				case SDLK_c: chip8.key[0xB] = 0; break;
				case SDLK_v: chip8.key[0xF] = 0; break;
				}
			}
		}

		for (int y = 0; y < 32; ++y)
			for (int x = 0; x < 64; ++x)
			{
				if (chip8.gfx[(y * 64) + x] == 0)
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
				else
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

				SDL_RenderDrawPoint(renderer, x, y);
			}

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
