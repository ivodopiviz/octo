#include <stdio.h>
#include "SDL.h"
#include "Chip8.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

Chip8 chip8;
//int zoom = 10;
int zoom = 1;

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
			if( e.type == SDL_QUIT )
			{ 
				quit = true;
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
