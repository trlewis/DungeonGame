#ifndef GENERATED_MAP_HPP_
#define GENERATED_MAP_HPP_

#include <string>
#include <vector>

#include "Room.hpp"
#include "Vector2i.hpp"

/*! This class will generate a dungeon made of rooms and their connections.
 * It does not do anything involving graphics, this is purely data about
 * the rooms and their relationships.*/
class GeneratedMap
{
public:
	static const int MAX_ROOM_WIDTH = 3;
	static const int MAX_ROOM_HEIGHT = 3;
	static const int DEFAULT_MAP_SIZE = 20;

	GeneratedMap(const int width, const int height);
	void generateMap();
	inline int getHeight() { return myHeight; }
	inline int getWidth() { return myWidth; }

	/*! This prints the map to the console with the given string above it.*/
	void printMap(std::string message);

	Room* startRoom; //!< The "entrance" room to the dungeon.
	Room* endRoom; //!< The "exit" room of the dungeon.
	Room* seedRoom; //!< The room from which all others spawn.
	std::vector<Room*> rooms; //!< All rooms, including start/end.

private:
	/*! What percent of dead-ends we will attempt to add connections to.*/
	static const int CYCLE_PERCENT = 50;
	static const int EMPTY_PERCENT = 20;

	int myHeight;
	int myWidth;

	/*! This is the 2D grid of chars that represents the cells
	 * in the dungeon.*/
	std::vector<std::vector<char>> myMapCells;
	std::vector<Room*> roomStack;

	/*! Checks to see that all unassigned spaces can be reached using
	* flood fill*/
	bool allUnassignedReachable();

	/*! Checks if the given room bounds fit within the map bounds and that all
	* the cells are unassigned. */
	bool canPlaceRoom(Vector2i &topLeft, Vector2i &bottomRight);

	/*! Replaces all 'T' characters with '.' characters. Called after testing
	 * to see if the end is reachable. */
	void cleanupTest();

	/*! Creates the 'X' characters that are reserved empty cells where no rooms
	 * will be placed. */
	void createEmptyCells();

	/*! Tries to create a room that contains the "entrance" cell, and is
	* directly adjacent to the "from" cell. This method will attempt to
	* create a room with dimensions between 1x1 and 3x3 cells. If it is not
	*possible to create a room with these constraints it will return null
	* instead.*/
	Room* createRoom(Vector2i entrance, Vector2i from);

	/*! This does the actual flood filling of the rooms into the dungeon. */
	void fillRooms();

	/*! Adds in cycles so that the dungeon is no longer a tree. */
	void fixTree();

	/*! Gets all the rooms to the top/left/right/bottom of the given one. */
	std::vector<Room*> getAdjacentRooms(Room* room);

	/*! Gets the room that is within the */
	Room* getRoomFromCell(int x, int y);

	/*! Checks to see that the given boundaries are within the dungeon. */
	bool isRoomInBounds(Vector2i &topLeft, Vector2i &bottomRight);

	/*! Adds the room to the 2D character array of cells, thus reserving
	 * those cells.*/
	void placeRoom(Room* room);

	/*! Replaces all 'X' characters with '.' characters. Called when
	 * createEmptyCells() fails to make a usable map.*/
	void removeEmpties();

	/*! Replaces all the unassigned cells in the map with the given
	* character. */
	void replacePeriods(Vector2i topLeft, Vector2i bottomRight,
		char replacement);
};

#endif //GAME_MAP_HPP_