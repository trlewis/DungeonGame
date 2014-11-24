#ifndef GENERATED_ROOM_HPP_
#define GENERATED_ROOM_HPP

#include "Vector2i.hpp"

class Room
{
	//y axis goes top to bottom, higher values are further down
	//x axis goes left to right, pretty standard
public:
	Room(Vector2i start);
	Room(Vector2i topLeft, Vector2i bottomRight);
	Room(Vector2i topLeft, Vector2i bottomRight, Vector2i start);

	static const int LEFT = 1;
	static const int UP = 2;
	static const int RIGHT = 3;
	static const int DOWN = 4;

	/*! The first cell of the room that is walked into from the prev room */
	Vector2i startingCell; 
	/*! The exit cell from the previous room */
	Vector2i previousCell; 

	//hacky solution to add cycles into the dungeon...
	bool hasSecondEntrance;
	Vector2i startingCell2;
	Vector2i previousCell2;

	/*! Top left corner of the room, defines boundary/size.*/
	Vector2i topLeft;
	/*! Bottom right corner of the room, defines boundary/size.*/
	Vector2i bottomRight;

	bool isStarting; //!< if this room is the "start" room of the map
	bool isEnding; //!< if this room is the "end" room of the map
	bool branchesCreatd; //!< if the rooms this room goes to have been created

	// Delete these after map generation is finished... memory leaks yo.
	Room* left; //!< The room to the left of this one.
	Room* right; //!< The room to the right of this one.
	Room* bottom; //!< The room below this one.
	Room* top; //!< The room above this one.

	/*! Checks if this room or the other room are connected to each other. */
	bool areConnected(Room* other);

	int directionOfOtherRoom(Room* other);

	/*! Returns true if the room has no neighbors. */
	bool isDeadEnd();

	// an easy way to check if the given cell is within (inclusive)
	// the bounds of the room.

	/*! Checks to see if the given x/y coordinate lies in the space defined by
	 * The topLeft and bottomRight Vector2i fields.*/
	bool containsCell(Vector2i &cell);

	bool containsCell(int x, int y);

	// the number of cells this room takes up
	int getHeight(); //!< How tall this room is.
	int getWidth(); //!< How wide this room is.
	int getArea(); //!< Width * height of this room.

private:
	void init();
};

#endif