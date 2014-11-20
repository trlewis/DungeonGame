#ifndef DUNGEON_MAP_HPP_
#define DUNGEON_MAP_HPP_

#include <string>

//SDL include.
#ifdef _WIN32
	#include <SDL.h>
#elif
	#include <SDL2/SDL.h>
#endif

#include "GeneratedMap.hpp"

class DungeonMap
{
public:
	DungeonMap(GeneratedMap* generatedMap, std::string mapTilesLocation, 
		SDL_Renderer* renderer);
	
	SDL_Texture* dungeonTex;
	SDL_Texture* tileSourceTex;
	SDL_Rect destRect;
private:
	
	
	//all if this stuff should hopefully be replaced by a more
	//sophisticated method of keeping track of tiles. especially
	//since hopefully there can be several tiles of the same type...
	//probably need a metadata file to help out with it
	SDL_Rect tileRects[9];

	static const int TILES_PER_CELL = 10;
	static const int TILE_SIZE = 32;

	static const int FLOORX = 0;

	static const int LEFT_WALL = 1;
	static const int TOP_WALL = 2;
	static const int RIGHT_WALL = 3;
	static const int BOTTOM_WALL = 4;

	static const int TOPLEFT_INSIDECORNER = 5;
	static const int TOPRIGHT_INSIDECORNER = 6;
	static const int BOTTOMRIGHT_INSIDECORNER = 7;
	static const int BOTTOMLEFT_INSIDECORNER = 8;

	static const int TOPLEFT_OUTSIDECORNER = 9;
	static const int TOPRIGHT_OUTSIDECORNER = 10;
	static const int BOTTOMRIGHT_OUTSIDECORNER = 11;
	static const int BOTTOMLEFT_OUTSIDECORNER = 12;
};

#endif //DUNGEON_MAP_HPP_
