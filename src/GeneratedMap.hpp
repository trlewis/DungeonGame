#ifndef GENERATED_MAP_HPP_
#define GENERATED_MAP_HPP_

#include <vector>

//on a high level, a map is divided into "cells", many cells can make a "room"
//rooms have a size in "tiles"

class Vector2i
{
public:
	Vector2i();
	Vector2i(int xVal, int yVal);

	int x;
	int y;

	// gets the distance between two points by deltaX + deltaY, this
	// does NOT get the pythagorean distance!
	static int deltaDistance(const Vector2i& a, const Vector2i& b);
};


class Room
{
	//y axis goes top to bottom, higher values are further down
	//x axis goes left to right, pretty standard
public:
	Room(Vector2i start);
	Room(Vector2i topLeft, Vector2i bottomRight);
	Room(Vector2i topLeft, Vector2i bottomRight, Vector2i start);

	Vector2i startingCell; // the first cell of the room that is walked into from the prev room
	Vector2i previousCell; // the exit cell from the previous room

	// these set the bounds of the room
	Vector2i topLeft;
	Vector2i bottomRight;

	bool isStarting; // if this room is the "start" room of the map
	bool isEnding; // if this room is the "end" room of the map
	bool branchesCreatd; // if the rooms this room goes to have been created

	// these are the potential rooms that are "ahead" of this one.
	// that the flood fill algorithm will flood to. Delete these after
	// map generation is finished... memory leaks yo.
	Room* left;
	Room* right;
	Room* bottom;
	Room* top;

	// It would be nice if the map didn't consist entirely of a minimum
	// spanning tree, so some dead-ends will want to be connected to
	// neighboring rooms.
	bool isDeadEnd();

	// an easy way to check if the given cell is within (inclusive)
	// the bounds of the room.
	bool containsCell(Vector2i &cell);

	// the number of cells this room takes up
	int getHeight();
	int getWidth();
	int getArea();
	
private:
	void init();
};


class GeneratedMap
{
public:
	GeneratedMap(const int width, const int height);
	
	//static const int DEFAULT_ROOM_SIZE = 3;
	static const int MAX_ROOM_WIDTH = 3;
	static const int MAX_ROOM_HEIGHT = 3;
	static const int DEFAULT_MAP_SIZE = 20;

	static const int SHIFT_BOTH = 0;
	static const int SHIFT_HORIZONTAL = 1;
	static const int SHIFT_VERTICAL = 2;


	void generateMap();
	int getHeight();
	int getWidth();
	void printMap(std::string message);

	Room* startRoom;
	Room* endRoom;

	std::vector<Room*> rooms;
	void fillRooms();
	
	std::vector<Room*> roomStack;
private:
	int myHeight;
	std::vector<std::vector<char>> myMapCells;
	int myWidth;	

	//bool isEndReachable();
	void cleanupTest();
	void removeEmpties();
	bool isRoomInBounds(Vector2i &topLeft, Vector2i &bottomRight);
	bool canPlaceRoom(Vector2i &topLeft, Vector2i &bottomRight);// , bool placingEmpty);//, bool placingStartFinish);
	void createEmptyCells();
	//void createStartEndRooms();
	void replacePeriods(Vector2i topLeft, Vector2i bottomRight, char replacement);
	void placeRoom(Room* room);
	bool allUnassignedReachable();
	
	Room* createRoom(Vector2i entrance, Vector2i from);
};






#endif //GAME_MAP_HPP_
