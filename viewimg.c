#include "viewimg.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

int isExit;
SDL_Surface *screen = NULL;
SDL_Surface *image = NULL;
SDL_Joystick *joy;
SDL_Event event;

int initSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK))
	{
		fprintf(stderr, "ERROR (initSDL): Failed to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	SDL_JoystickEventState(SDL_ENABLE);

	joy = SDL_JoystickOpen(0);

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	SDL_ShowCursor(SDL_DISABLE);

	return 0;
}

void cleanSDL()
{
	if(image != NULL)
	{
		SDL_FreeSurface(image);
	}
	SDL_JoystickClose(joy);
	SDL_Quit();
}

SDL_Surface *loadImage(char *fileName)
{
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;
	
	if(fileName == NULL)
	{
		fprintf(stderr, "ERROR (loadImage): File name not specified!\n");
		return NULL;
	}
	
	loadedImage = IMG_Load(fileName);
	
	if(loadedImage == NULL)
	{
		fprintf(stderr, "ERROR (loadImage): Failed to load image file: %s\n", fileName);
		return NULL;
	}
	else
	{
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
	}

	if(optimizedImage == NULL)
	{
		fprintf(stderr, "ERROR (loadImage): Failed to optimize image file: %s\n", fileName);
		return NULL;
	}

	return optimizedImage;
}

void applySurface(SDL_Surface *source, SDL_Surface *destination, int x, int y, SDL_Rect *clip)
{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

int updateScreen()
{
	if(SDL_Flip(screen) == -1)
	{
		fprintf(stderr, "ERROR (updateScreen): Failed to flip screen surface!\n");
		return 1;
	}

	return 0;
}

void input()
{
	if(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			//case SDL_JOYAXISMOTION:
				isExit = 1;
				break;
			default:
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	isExit = 0;

	if(argc < 2)
	{
		fprintf(stderr, "USAGE:\nviewimg [IMAGE_FILE]\n");
		return 1;
	}
	else
	{
		int posX = 0;
		int posY = 0;

		if(initSDL())
		{
			return 1;
		}
		image = loadImage(argv[1]);
		if(image == NULL)
		{
			cleanSDL();
			return 1;
		}

		if(image->w < screen->w)
		{
			posX = screen->w / 2 - image->w / 2;
		}
		if(image->h < screen->h)
		{
			posY = screen->h / 2 - image->h / 2;
		}

		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));
		applySurface(image, screen, posX, posY, NULL);
		updateScreen();
	}

	while(!isExit)
	{
		input();
		SDL_Delay(1);
	}

	cleanSDL();

	return 0;
}
