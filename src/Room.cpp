#include "Room.hpp"

void Room::init()
{
	isStarting = false;
	isEnding = false;
	branchesCreatd = false;
	left = nullptr;
	right = nullptr;
	top = nullptr;
	bottom = nullptr;
}

Room::Room(Vector2i start)
{
	init();
	startingCell = start;
}


Room::Room(Vector2i topLeft, Vector2i bottomRight)
{
	init();
	this->topLeft = topLeft;
	this->bottomRight = bottomRight;
}

Room::Room(Vector2i topLeft, Vector2i bottomRight, Vector2i start)
{
	init();
	this->topLeft = topLeft;
	this->bottomRight = bottomRight;
	startingCell = start;
}

bool Room::isDeadEnd()
{
	// is the same as (generally... depending on what NULL is defined as)
	// left == NULL
	return !left && !right && !bottom && !top;
}

bool Room::containsCell(Vector2i &cell)
{
	if (topLeft.x >= cell.x && bottomRight.x <= cell.y)
	{
		if (topLeft.y <= cell.y && bottomRight.x >= cell.x)
			return true;
	}
	return false;
}

int Room::getHeight()
{
	return bottomRight.y - topLeft.y;
}

int Room::getWidth()
{
	return bottomRight.x - topLeft.x;
}

int Room::getArea()
{
	return (getHeight() + 1) * (getWidth() + 1);
}
