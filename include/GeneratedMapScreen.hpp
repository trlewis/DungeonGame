#ifndef GENERATED_MAP_SCREEN_HPP_
#define GENERATED_MAP_SCREEN_HPP_

#include "GameState.hpp"

#include "GeneratedMap.hpp"

class GeneratedMapScreen : public GameState
{
public:
	GeneratedMapScreen(int cellsWide = DEFAULT_CELLS_WIDE,
		int cellsHigh = DEFAULT_CELLS_HIGH);

	void init();
	void cleanup();

	void pause();
	void unpause();

	void handleEvents(StateManager* stateManager);
	void update(StateManager* stateManager);
	void draw(StateManager* stateManager);

private:
	static const int ROOM_BUFFER = 1;
	static const int TILE_SIZE = 3;
	static const int TILES_PER_CELL = 8;

	static const int DEFAULT_CELLS_WIDE = 33;
	static const int DEFAULT_CELLS_HIGH = 25;

	int cellsWide;
	int cellsHigh;

	int roomBuffer;
	int tileSize;
	int tilesPerCell;

	bool regenerateMap;
	bool drawCorridors;

	GeneratedMap* map;

	void drawMap(sf::RenderWindow* win);
	void drawCorridor(sf::RenderWindow* win, Room* room, bool cycle);
};

#endif // !GENERATED_MAP_SCREEN_HPP_