#include <SDL_image.h>

#include "DungeonMap.hpp"
#include "GeneratedMap.hpp"
#include "ImageHelper.hpp"

DungeonMap::DungeonMap(GeneratedMap* generatedMap, std::string mapTilesLocation,
	SDL_Renderer* renderer)
{
	int heightPx = generatedMap->getHeight() * TILES_PER_CELL * TILE_SIZE;
	int widthPx = generatedMap->getWidth() * TILES_PER_CELL * TILE_SIZE;

	destRect.x = destRect.y = 0;
	destRect.w = widthPx;
	destRect.h = heightPx;

	//dungeonTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, 
	//	SDL_TEXTUREACCESS_STATIC, widthPx, heightPx);

	//tileSourceImg = loadOptimizedSurface(mapTilesLocation, format);
	tileSourceTex = loadTexture(mapTilesLocation, renderer);
	if (tileSourceTex == NULL)
		return;

//	Uint32 rmask, gmask, bmask, amask;
//#if SDL_BYTEORDER == SDL_BIG_ENDIAN
//	rmask = 0xff000000;
//	gmask = 0x00ff0000;
//	bmask = 0x0000ff00;
//	amask = 0x000000ff;
//#else
//	rmask = 0x000000ff;
//	gmask = 0x0000ff00;
//	bmask = 0x00ff0000;
//	amask = 0xff000000;
//#endif
//	dungeonImg = SDL_CreateRGBSurface(0, widthPx, heightPx, 32, rmask, gmask, bmask, amask);
//

	//dungeonTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
	//	SDL_TEXTUREACCESS_TARGET, widthPx, heightPx);

	dungeonTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_TARGET, 1280, 800);
	if (dungeonTex == NULL)
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

	SDL_Rect r;
	r.x = r.y = 10;
	r.w = r.h = 32;

	SDL_SetRenderTarget(renderer, dungeonTex);
	SDL_RenderCopy(renderer, tileSourceTex, &tileRects[0], &r);
}

