#include <iostream>
#include "GeneratedMapScreen.hpp"

GeneratedMapScreen::GeneratedMapScreen(const int width, const int height)
{
	cellsWide = width;
	cellsHigh = height;
	init();
}

void GeneratedMapScreen::init()
{
	map = new GeneratedMap(cellsWide, cellsHigh);
	regenerateMap = false;
	drawCorridors = false;
}

void GeneratedMapScreen::cleanup()
{
	delete map;
}

void GeneratedMapScreen::pause() {}
void GeneratedMapScreen::unpause() {}

void GeneratedMapScreen::handleEvents(StateManager* stateManager)
{
	sf::Event e;

	while (stateManager->window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			stateManager->quit();

		if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::G)
				regenerateMap = true;
			if (e.key.code == sf::Keyboard::C)
				drawCorridors = true;
			if (e.key.code == sf::Keyboard::Escape)
				stateManager->quit();
		}
		if (e.type == sf::Event::KeyReleased)
		{
			if (e.key.code == sf::Keyboard::C)
				drawCorridors = false;
		}
	}
}

void GeneratedMapScreen::update(StateManager* stateManager)
{
	if (regenerateMap)
	{
		regenerateMap = false;
		delete map;
		map = new GeneratedMap(cellsWide, cellsHigh);
	}
}

void GeneratedMapScreen::draw(StateManager* stateManager)
{
	drawMap(stateManager->window);
}

void GeneratedMapScreen::drawMap(sf::RenderWindow* win)
{
	for (unsigned int i = 0; i < map->rooms.size(); i++)
	{
		Room* room = map->rooms[i];

		//calculate position and size
		int startx = room->topLeft.x * TILE_SIZE * TILES_PER_CELL;
		startx += TILE_SIZE * ROOM_BUFFER;
		int starty = room->topLeft.y * TILE_SIZE * TILES_PER_CELL;
		starty += TILE_SIZE * ROOM_BUFFER;
		int width = TILE_SIZE * TILES_PER_CELL * (room->getWidth() + 1);
		width -= TILE_SIZE * ROOM_BUFFER * 2;
		int height = TILE_SIZE * TILES_PER_CELL * (room->getHeight() + 1);
		height -= TILE_SIZE * ROOM_BUFFER * 2;

		//create and position the rectangle
		sf::RectangleShape roomRekt(sf::Vector2f(width, height));
		roomRekt.setPosition(sf::Vector2f(startx, starty));

		//different room colors for different types of rooms
		if (room == map->startRoom)
			roomRekt.setFillColor(sf::Color(0, 0, 255));
		else if (room == map->endRoom)
			roomRekt.setFillColor(sf::Color(0x04, 0xC9, 0xC0));
		else if (room == map->seedRoom)
			roomRekt.setFillColor(sf::Color(0xF2, 0x9E, 0x0C));
		else
			roomRekt.setFillColor(sf::Color(255, 0, 0));

		//if any rooms branched from this one, draw their connections
		if (room->left != nullptr)
			drawCorridor(win, room->left, false);
		if (room->right != nullptr)
			drawCorridor(win, room->right, false);
		if (room->bottom != nullptr)
			drawCorridor(win, room->bottom, false);
		if (room->top != nullptr)
			drawCorridor(win, room->top, false);

		//if a cycle was added to this room, draw that too
		if (drawCorridors && room->hasSecondEntrance)
			drawCorridor(win, room, true);

		win->draw(roomRekt);
	}
}

void GeneratedMapScreen::drawCorridor(sf::RenderWindow* win, Room* room, bool cycle)
{
	//get starting and previous cells x and y values, depending on if we're drawing a cycle
	int sx = cycle ? room->startingCell2.x : room->startingCell.x;
	int sy = cycle ? room->startingCell2.y : room->startingCell.y;
	int px = cycle ? room->previousCell2.x : room->previousCell.x;
	int py = cycle ? room->previousCell2.y : room->previousCell.y;

	//calculate location and size
	bool vertical = px - sx == 0;
	int startx, starty, width, height;
	if (vertical)
	{
		starty = std::max(sy, py) * TILE_SIZE * TILES_PER_CELL;
		starty -= TILE_SIZE * ROOM_BUFFER; //connections straddle two cells

		startx = sx * TILE_SIZE * TILES_PER_CELL;
		startx += (TILE_SIZE * TILES_PER_CELL / 2);
		width = TILE_SIZE * ROOM_BUFFER;
		height = TILE_SIZE * ROOM_BUFFER * 2;
	}
	else
	{
		startx = std::max(sx, px) * TILE_SIZE * TILES_PER_CELL;
		startx -= TILE_SIZE * ROOM_BUFFER; //connections straddle two cells

		starty = sy * TILE_SIZE * TILES_PER_CELL;
		starty += (TILE_SIZE * TILES_PER_CELL) / 2;
		width = TILE_SIZE * ROOM_BUFFER * 2;
		height = TILE_SIZE * ROOM_BUFFER;
	}

	sf::RectangleShape rekt(sf::Vector2f(width, height));
	rekt.setPosition(startx, starty);
	rekt.setFillColor(cycle ? sf::Color(0, 255, 0) : sf::Color(0xAA, 0x00, 0xAA));

	win->draw(rekt);
}
