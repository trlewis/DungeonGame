#ifndef DUNGEON_MAP_HPP_
#define DUNGEON_MAP_HPP_

#include <SDL.h>

#include "GeneratedMap.hpp"


class DungeonMap
{
public:
	DungeonMap(GeneratedMap generatedMap, std::string mapTilesLocation);
private:
	SDL_Surface* tileSourceImg;
};

#endif //DUNGEON_MAP_HPP_
