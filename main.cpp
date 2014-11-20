//Using SDL, standard IO, and strings

#include <stdio.h>
#include <string>

#include <iostream>
#include <vector>

//SDL include
#ifdef _WIN32
	#include <SDL.h>
#elif
	#include <SDL2/SDL.h>
#endif

#include "DungeonMap.hpp"
#include "GeneratedMap.hpp"

const int SCREEN_WIDTH = 1250;
const int SCREEN_HEIGHT = 800;

bool init();
void close();

void DrawMap(GeneratedMap&);

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* renderer = NULL;

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	
	gWindow = SDL_CreateWindow("Press G to generate new dungeon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	gScreenSurface = SDL_GetWindowSurface(gWindow);
	return true;
}

void close()
{
	SDL_DestroyRenderer(renderer);

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* args[])
{
	int mapwidth = 25;
	int mapheight = 16;

	GeneratedMap map(mapwidth, mapheight);
	map.generateMap();
	
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
		close();
		return 0;
	}
	
	SDL_Texture* tx =  SDL_GetRenderTarget(renderer);
	DungeonMap dm(&map, "Resources/Images/combined2.png", renderer);
	SDL_SetRenderTarget(renderer, tx);

	bool quit = false;
	SDL_Event e;

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			const Uint8* currentKeyStates = NULL;
			currentKeyStates = SDL_GetKeyboardState(NULL);

			if (currentKeyStates[SDL_SCANCODE_ESCAPE])
				quit = true;
			if (currentKeyStates[SDL_SCANCODE_G])
			{
				map = GeneratedMap(mapwidth, mapheight);
				map.generateMap();
			}

			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		//clear screen
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);

		//SDL_BlitSurface(dm.dungeonImg, NULL, gScreenSurface, &dm.destRect);
		//SDL_UpdateWindowSurface(gWindow);

		// SDL_Rect screenRect;
		// screenRect.x = screenRect.y = 0;
		// screenRect.w =  SCREEN_WIDTH;
		// screenRect.h = SCREEN_HEIGHT;
		//SDL_RenderCopy(renderer, dm.dungeonTex, &screenRect, NULL);// &dm.destRect);

		//SDL_RenderCopyEx(renderer, dm.dungeonTex, NULL, NULL, 0, NULL, SDL_FLIP_NONE);

		
		DrawMap(map);

		SDL_RenderPresent(renderer);
		SDL_Delay(15);
	}

	//Free resources and close SDL
	close();
	return 0;
}

void DrawMap(GeneratedMap& map)
{

	Uint8 red = 80;

	int tilesize = 5;
	int tilesPerRoom = 10;
	int roomBuffer = 1;//each room is 2 tiles from the edge of the cell

	for (int i = 0; i < map.rooms.size(); i++)
	{
		Room* room = map.rooms[i];

		if (room == map.startRoom)
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
		else if (room == map.endRoom)
			SDL_SetRenderDrawColor(renderer, 0x99, 0x00, 0xFF, 0xFF);
		else
		{
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);//0xFF, 0x00, 0x00, 0xFF);
			red += 2;
			if (red > 0xFF)
			{
				red = 0xFF;
			}
		}
		
		int horizCellCount = room->getWidth() + 1;
		horizCellCount *= tilesPerRoom;
		horizCellCount -= 2 * roomBuffer;

		int vertCellCount = room->getHeight() + 1;
		vertCellCount *= tilesPerRoom;
		vertCellCount -= 2 * roomBuffer;

		int startx = (room->topLeft.x * tilesize * tilesPerRoom);
		startx += roomBuffer * tilesize;

		int starty = (room->topLeft.y * tilesize * tilesPerRoom);
		starty += roomBuffer * tilesize;

		for (int y = 0; y < vertCellCount; y++)
		{
			for (int x = 0; x < horizCellCount; x++)
			{
				SDL_Rect rekt;
				rekt.x = startx + (x * tilesize);
				rekt.y = starty + (y * tilesize);
				rekt.h = tilesize;
				rekt.w = tilesize;
				SDL_RenderFillRect(renderer, &rekt);
			}
		}

		//draw connections
		if (room->left != nullptr)
		{
			Vector2i startingCell = room->left->startingCell;
			int startx = (startingCell.x + 1) * tilesize * tilesPerRoom;
			startx -= (tilesize * roomBuffer);
			int starty = (startingCell.y * tilesize * tilesPerRoom) + ((tilesPerRoom * tilesize) / 2);

			for (int y = 0; y < 2; y++)
			{
				for (int x = 0; x < roomBuffer * 2; x++)
				{
					SDL_Rect rekt;
					rekt.x = startx + (x * tilesize);
					rekt.y = starty + (y * tilesize);
					rekt.w = tilesize;
					rekt.h = tilesize;
					SDL_RenderFillRect(renderer, &rekt);
				}
			}
		}

		if (room->right != nullptr)
		{
			Vector2i startingCell = room->right->startingCell;
			int startx = startingCell.x * tilesize * tilesPerRoom;
			startx -= (tilesize * roomBuffer);
			int starty = (startingCell.y * tilesize * tilesPerRoom) + ((tilesPerRoom * tilesize) / 2);

			for (int y = 0; y < 2; y++)
			{
				for (int x = 0; x < roomBuffer * 2; x++)
				{
					SDL_Rect rekt;
					rekt.x = startx + (x * tilesize);
					rekt.y = starty + (y * tilesize);
					rekt.w = tilesize;
					rekt.h = tilesize;
					SDL_RenderFillRect(renderer, &rekt);
				}
			}
		}

		if (room->top != nullptr)
		{
			Vector2i thisTopLeft = room->topLeft;
			Room top = *map.rooms[i]->top;

			Vector2i startingCell = room->top->startingCell;
			int starty = (startingCell.y + 1) * tilesize * tilesPerRoom;
			starty -= (tilesize * roomBuffer);

			int startx = (startingCell.x * tilesize * tilesPerRoom) + ((tilesPerRoom * tilesize) / 2);

			for (int y = 0; y < roomBuffer * 2; y++)
			{
				for (int x = 0; x < 2; x++)
				{
					SDL_Rect rekt;
					rekt.x = startx + (x * tilesize);
					rekt.y = starty + (y * tilesize);
					rekt.w = tilesize;
					rekt.h = tilesize;
					SDL_RenderFillRect(renderer, &rekt);
				}
			}
		}

		if (room->bottom != nullptr)
		{
			Vector2i thisBottomRight = room->bottomRight;
			Room* top = map.rooms[i]->top;

			Vector2i startCell = room->bottom->startingCell;
			int starty = startCell.y * tilesize * tilesPerRoom;
			starty -= tilesize * roomBuffer;

			int startx = (startCell.x * tilesize * tilesPerRoom) + ((tilesPerRoom * tilesize) / 2);

			for (int y = 0; y < roomBuffer * 2; y++)
			{
				for (int x = 0; x < 2; x++)
				{
					SDL_Rect rekt;
					rekt.x = startx + (x * tilesize);
					rekt.y = starty + (y * tilesize);
					rekt.w = tilesize;
					rekt.h = tilesize;
					SDL_RenderFillRect(renderer, &rekt);
				}
			}
		}

	}
}
