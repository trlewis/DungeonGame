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

	//not having this line in was causing all the screwed up cycle connections
	//lesson learned: instatiate your variables...
	hasSecondEntrance = false;
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

bool Room::areConnected(Room* other)
{
	bool lefta = left != nullptr && left == other;
	bool righta = right != nullptr && right == other;
	bool topa = top != nullptr && top == other;
	bool bottoma = bottom != nullptr && bottom == other;
	if (lefta || righta || topa || bottoma)
		return true;

	bool leftb = other->left != nullptr && other->left == this;
	bool rightb = other->right != nullptr && other->right == this;
	bool topb = other->top != nullptr && other->top == this;
	bool bottomb = other->bottom != nullptr && other->bottom == this;
	if (leftb || rightb || topb || bottomb)
		return true;

	return false;
}

int Room::directionOfOtherRoom(Room* other)
{
	if (other->bottomRight.x < topLeft.x)
		return LEFT;

	if (other->bottomRight.y < topLeft.y)
		return UP;

	if (other->topLeft.x > bottomRight.x)
		return RIGHT;

	if (other->topLeft.y > bottomRight.y)
		return DOWN;

	return 0;
}

bool Room::isDeadEnd()
{
	return !left && !right && !bottom && !top;
}

bool Room::containsCell(Vector2i &cell)
{
	if (cell.x >= topLeft.x && cell.x <= bottomRight.x)
	{
		if (cell.y >= topLeft.y && cell.y <= bottomRight.y)
			return true;
	}
	return false;
}

bool Room::containsCell(int x, int y)
{
	Vector2i v(x, y);
	return containsCell(v);
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
