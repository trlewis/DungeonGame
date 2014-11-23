#include "DungeonMap.hpp"

#include "ImageHelper.hpp"

DungeonMap::DungeonMap(GeneratedMap* genMap, std::string mapTilesLocation,
	SDL_Renderer* renderer)
{
	generatedMap = genMap;
	int heightPx = generatedMap->getHeight() * TILES_PER_CELL * TILE_SIZE;
	int widthPx = generatedMap->getWidth() * TILES_PER_CELL * TILE_SIZE;

	//destRect.x = destRect.y = 0;
	//destRect.w = widthPx;
	//destRect.h = heightPx;

	dungeonTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
		SDL_TEXTUREACCESS_TARGET, widthPx, heightPx);
	if (dungeonTex == NULL)
		return;
	SDL_Rect r;
	r.x = r.y = 0;
	r.w = widthPx;
	r.h = heightPx;
	SDL_SetRenderTarget(renderer, dungeonTex);
	SDL_SetRenderDrawColor(renderer, 0x00,0x00,0x00,0xFF);
	SDL_RenderFillRect(renderer, &r);


	tileSourceTex = loadTexture(mapTilesLocation, renderer);
	if (tileSourceTex == NULL)
		return;

	for (int i = 0; i < 9; i++)
	{
		SDL_Rect rekt;
		rekt.x = i * TILE_SIZE;
		rekt.y = 0;
		rekt.w = TILE_SIZE;
		rekt.h = TILE_SIZE;
		tileRects[i] = rekt;
	}

	//SDL_Rect r;
	//r.x = r.y = 10;
	//r.w = r.h = 32;

	//SDL_SetRenderTarget(renderer, dungeonTex);
	//SDL_RenderCopy(renderer, tileSourceTex, &tileRects[0], &r);
	createDungeonTexture(renderer);
}

void DungeonMap::createDungeonTexture(SDL_Renderer* renderer)
{
	SDL_SetRenderTarget(renderer, dungeonTex);

	//go through each room in the map...
	for(std::vector<Room*>::iterator it = generatedMap->rooms.begin(); 
		it != generatedMap->rooms.end(); ++it)
	{
		Room* room = *it;
		int startx = room->topLeft.x * TILES_PER_CELL + 1;
		int starty = room->topLeft.y * TILES_PER_CELL + 1;
		int endx = room->bottomRight.x * TILES_PER_CELL + TILES_PER_CELL - 2;
		int endy = room->bottomRight.y * TILES_PER_CELL + TILES_PER_CELL - 2;

		//draw top left corner
		SDL_Rect r;
		r.x = startx * TILE_SIZE;
		r.y = starty * TILE_SIZE;
		r.w = r.h = TILE_SIZE;
		SDL_RenderCopy(renderer, tileSourceTex, &tileRects[TOPLEFT_INSIDECORNER], &r);

		//bottom right corner
		r.x = endx * TILE_SIZE;
		r.y = endy * TILE_SIZE;
		SDL_RenderCopy(renderer, tileSourceTex, &tileRects[BOTTOMRIGHT_INSIDECORNER], &r);

		//bottom left corner
		r.x = startx * TILE_SIZE;
		r.y = endy * TILE_SIZE;
		SDL_RenderCopy(renderer, tileSourceTex, &tileRects[BOTTOMLEFT_INSIDECORNER], &r);

		//top right corner
		r.x = endx * TILE_SIZE;
		r.y = starty * TILE_SIZE;
		SDL_RenderCopy(renderer, tileSourceTex, &tileRects[TOPRIGHT_INSIDECORNER], &r); 

		//left/right wall
		for(int y = starty+1; y < endy; y++)
		{
			r.y = y * TILE_SIZE;
			r.x = startx * TILE_SIZE;
			SDL_RenderCopy(renderer, tileSourceTex, &tileRects[LEFT_WALL], &r);
			r.x = endx * TILE_SIZE;
			SDL_RenderCopy(renderer, tileSourceTex, &tileRects[RIGHT_WALL], &r);
		}

		//top/bottom wall
		for(int x = startx + 1; x < endx ;x++)
		{
			r.x = x * TILE_SIZE;
			r.y = starty * TILE_SIZE;
			SDL_RenderCopy(renderer, tileSourceTex, &tileRects[TOP_WALL], &r);
			r.y = endy * TILE_SIZE;
			SDL_RenderCopy(renderer, tileSourceTex, &tileRects[BOTTOM_WALL], &r);
		}

		//floor
		for(int y = starty + 1; y < endy; y++)
		{
			for(int x = startx + 1 ; x < endx; x++)
			{
				r.x = x * TILE_SIZE;
				r.y = y * TILE_SIZE;
				SDL_RenderCopy(renderer, tileSourceTex, &tileRects[FLOORX], &r);
			}
		}

		
	//	std::cout << "[" << startx << "," << starty << "] - ["
	//		<< room->bottomRight.x << "," << room->bottomRight.y
	//		<< "]" << std::endl;
	}


}
