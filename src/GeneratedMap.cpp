#include "GeneratedMap.hpp"

#include <algorithm>
#include <iostream> //cout, endl
#include <queue>
#include <string>
#include <time.h> //time()


GeneratedMap::GeneratedMap(const int width, const int height)
{
	myWidth = width;
	myHeight = height;
}

void GeneratedMap::generateMap()
{
	//create empty map
	myMapCells = std::vector<std::vector<char>>();
	for (int y = 0; y < myHeight; y++)
	{
		std::vector<char> row = std::vector<char>();
		for (int x = 0; x < myWidth; x++)
			row.push_back('.');
		
		myMapCells.push_back(row);
	}

	printMap("Empty map:");

	srand(time(0));

	//add in purposely blank cells, so there's some emptiness
	createEmptyCells();

	printMap("with empty cells:");

	//now to finally create rooms
	fillRooms();

	printMap("with rooms!");

	//add in some cycles to reduce backtracking
	fixTree();
} 

void GeneratedMap::printMap(std::string message)
{
	std::cout << message << std::endl;

	int element = 0;
	for (int y = 0; y < myHeight; y++)
	{
		for (int x = 0; x < myWidth; x++)
		{
			element = y * myWidth + x;
			std::cout << myMapCells[y][x];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool GeneratedMap::allUnassignedReachable()
{
	std::queue<Vector2i> queue;
	Vector2i start(0, 0);
	while (myMapCells[start.y][start.x] != '.')
	{
		start.x = rand() % myWidth;
		start.y = rand() % myHeight;
	}
	queue.push(start);

	while (queue.size() > 0)
	{
		Vector2i cell = queue.front();
		queue.pop();
		
		//left
		if (cell.x > 0 && myMapCells[cell.y][cell.x - 1] == '.')
		{
			Vector2i newCell(cell.x - 1, cell.y);
			myMapCells[newCell.y][newCell.x] = 'T';
			queue.push(newCell);
		}

		//right
		if (cell.x < myWidth - 1 && myMapCells[cell.y][cell.x + 1] == '.')
		{
			Vector2i newCell(cell.x + 1, cell.y);
			myMapCells[newCell.y][newCell.x] = 'T';
			queue.push(newCell);
		}

		//top
		if (cell.y > 0 && myMapCells[cell.y - 1][cell.x] == '.')
		{
			Vector2i newCell(cell.x, cell.y - 1);
			myMapCells[newCell.y][newCell.x] = 'T';
			queue.push(newCell);
		}

		//bottom
		if (cell.y < myHeight - 1 && myMapCells[cell.y + 1][cell.x] == '.')
		{
			Vector2i newCell(cell.x, cell.y + 1);
			myMapCells[newCell.y][newCell.x] = 'T';
			queue.push(newCell);
		}
	}

	//make sure no unassigned cells remain
	bool result = true;
	for (int y = 0; y < myHeight; y++)
	{
		for (int x = 0; x < myWidth; x++)
		{
			if (myMapCells[y][x] == '.')
				result = false;
			else if (myMapCells[y][x] == 'T')
				myMapCells[y][x] = '.';
		}
	}
	return result;
}

bool GeneratedMap::canPlaceRoom(Vector2i &topLeft, Vector2i &bottomRight)
{
	if (!isRoomInBounds(topLeft, bottomRight))
		return false;

	//assuming that the topLeft is indeed to the left and above bottomRight
	for (int y = topLeft.y; y <= bottomRight.y; y++)
	{
		for (int x = topLeft.x; x <= bottomRight.x; x++)
		{
			if (myMapCells[y][x] != '.')
				return false;
		}
	}
	return true;
}

void GeneratedMap::cleanupTest()
{
	for (int y = 0; y < myHeight; y++)
	{
		for (int x = 0; x < myWidth; x++)
		{
			char cell = myMapCells[y][x];
			if (cell == 'T')
				myMapCells[y][x] = '.';
		}
	}

	replacePeriods(startRoom->topLeft, startRoom->bottomRight, 'R');
	replacePeriods(endRoom->topLeft, endRoom->bottomRight, 'R');
}

void GeneratedMap::createEmptyCells()
{
	std::vector<Room> emptyRooms;
	int numEmpties = (myWidth * myHeight) * 0.20; //20% of cells are empty
	while (true)
	{
		//empty vector of rooms
		emptyRooms.clear();
		int emptyCellCount = 0;
		while (emptyCellCount < numEmpties)
		{
			//randomly pick a size
			int width = 0;
			int num = rand() % 100;
			if (num < 40)
				width = 0;
			else if (num >= 40 && num < 80)
				width = 1;
			else
				width = 2;

			int height = 0;
			num = rand() % 100;
			if (num < 40)
				height = 0;
			else if (num >= 40 && num < 80)
				height = 1;
			else
				height = 2;

			//randomly pick location
			int xpos = rand() % (myWidth - (width));
			int ypos = rand() % (myHeight - (height));

			//randomly pick a size
			Vector2i topLeft(xpos, ypos);
			Vector2i bottomRight(xpos + width, ypos + height);

			if (canPlaceRoom(topLeft, bottomRight))
			{
				//if it fits then add it to the list
				Room emptyRoom(topLeft, bottomRight);
				emptyRooms.push_back(emptyRoom);

				//fill cells in map
				replacePeriods(topLeft, bottomRight, 'X');

				//recalculate number of empty cells
				emptyCellCount = 0;
				for (int i = 0; i < emptyRooms.size(); i++)
					emptyCellCount += emptyRooms[i].getArea();
			}
		}

		if (allUnassignedReachable())
			break;
		else
			removeEmpties();
	}
}

Room* GeneratedMap::createRoom(Vector2i entrance, Vector2i from)
{
	//the entrance is the first cell of the room that the player enters from
	//the previous room, this cell MUST be in the room. The from cell is the
	//last cell of the previous room, adjacent to "entrance". It'll tell us
	//which directions we're allowed to slide the room we're creating in.

	if (myMapCells[entrance.y][entrance.x] != '.')
		return nullptr;

	int width = 0;
	int num = rand() % 100;
	if (num < 30)
		width = 0;
	else if (num >= 30 && num < 60)
		width = 1;
	else
		width = 2;

	int height = 0;
	num = rand() % 100;
	if (num < 30)
		height = 0;
	else if (num >= 30 && num < 60)
		height = 1;
	else
		height = 2;

	Vector2i topLeft, bottomRight;
	if (entrance.y != from.y)
	{
		// up/down transition

		//set left and right edges of room
		int xmin = (entrance.x - width < 0) ? 0 : entrance.x - width;
		int shiftHoriz = (width == 0 || entrance.x - xmin == 0) ? 0 : rand() % (entrance.x - xmin);// width;
		topLeft.x = xmin + shiftHoriz;
		bottomRight.x = topLeft.x + width;
		if (bottomRight.x >= myWidth)
			bottomRight.x = myWidth - 1;

		if (entrance.y < from.y)
		{
			//set top and bottom bounds
			bottomRight.y = entrance.y;
			int top = entrance.y - height;
			if (top < 0)
				top = 0;
			topLeft.y = top;
		}
		else
		{
			topLeft.y = entrance.y;
			int bottom = entrance.y + height;
			if (bottom >= myHeight)
				bottom = myHeight - 1;
			bottomRight.y = bottom;
		}
	}
	else
	{
		// left/right transition
		int ymin = (entrance.y - height < 0) ? 0 : entrance.y - height;
		int shiftVert = (height == 0 || entrance.y - ymin == 0) ? 0 : rand() % (entrance.y - ymin);//height;
		topLeft.y = ymin + shiftVert;
		bottomRight.y = topLeft.y + height;
		if (bottomRight.y >= myHeight)
			bottomRight.y = myHeight - 1;

		if (entrance.x < from.x)
		{
			bottomRight.x = entrance.x;
			int left = entrance.x - width;
			if (left < 0)
				left = 0;
			topLeft.x = left;
		}
		else
		{
			topLeft.x = entrance.x;
			int right = topLeft.x + width;
			if (right >= myWidth)
				right = myWidth - 1;
			bottomRight.x = right;
		}
	}

	if (canPlaceRoom(topLeft, bottomRight))
	{
		Room* r = new Room(topLeft, bottomRight, entrance);
		r->previousCell = from;
		return r;
	}
	return nullptr;
}


void GeneratedMap::fillRooms()
{
	Vector2i startEnter, startFrom;
	Room* seedRoom;
	do
	{
		startEnter.x = rand() % myWidth;
		startEnter.y = rand() % myHeight;

		int dir = rand() % 4;
		switch (dir)
		{
		case 0: //from left
			startFrom.x = startEnter.x - 1;
			startFrom.y = startEnter.y;
			break;
		case 1: //from right
			startFrom.x = startEnter.x + 1;
			startFrom.y = startEnter.y;
			break;
		case 2: //from top
			startFrom.x = startEnter.x;
			startFrom.y = startEnter.y - 1;
			break;
		case 3: //from bottom
			startFrom.x = startEnter.x;
			startFrom.y = startEnter.y + 1;
			break;
		}

		if (startFrom.x < 0 || startFrom.x >= myWidth || startFrom.y < 0 || startFrom.y >= myHeight)
			continue;

		seedRoom = createRoom(startEnter, startFrom);
		if (seedRoom != nullptr)
			break;
	} while (true);

	rooms.push_back(seedRoom);
	this->seedRoom = seedRoom;
	placeRoom(seedRoom);
	roomStack.push_back(seedRoom);

	while (roomStack.size() > 0)
	{
		//get the next room and remove it from the list/stack
		Room* room = *roomStack.begin();
		roomStack.erase(roomStack.begin());

		//try to generate another room in each direction
		if (room->topLeft.x > 0 && room->left == nullptr)
		{
			//to left
			std::vector<Vector2i> possibleStarts;
			for (int y = room->topLeft.y; y <= room->bottomRight.y; y++)
				possibleStarts.push_back(Vector2i(room->topLeft.x - 1, y));
			std::random_shuffle(possibleStarts.begin(), possibleStarts.end());

			//try to find an unassigned adjacent cell to the left
			Vector2i entrance;
			bool validStart = false;
			do
			{
				entrance = *possibleStarts.begin();
				possibleStarts.erase(possibleStarts.begin());
				if (myMapCells[entrance.y][entrance.x] == '.')
				{
					validStart = true;
					break;
				}
			} while (possibleStarts.size() > 0);

			if (validStart)
			{
				Vector2i from(entrance.x + 1, entrance.y);
				Room* newroom = nullptr;
				do
				{
					newroom = createRoom(entrance, from);
				} while (newroom == nullptr);

				if (newroom != nullptr)
				{
					placeRoom(newroom);
					room->left = newroom;
					roomStack.push_back(newroom);
					rooms.push_back(newroom);
				}
			}
		}

		if (room->bottomRight.x < myWidth - 1 && room->right == nullptr)
		{
			//to right
			std::vector<Vector2i> possibleStarts;
			for (int y = room->topLeft.y; y <= room->bottomRight.y; y++)
				possibleStarts.push_back(Vector2i(room->bottomRight.x + 1, y));
			std::random_shuffle(possibleStarts.begin(), possibleStarts.end());

			//try to find an unassigned adjacent cell to the right
			Vector2i entrance;
			bool validStart = false;
			do
			{
				entrance = *possibleStarts.begin();
				possibleStarts.erase(possibleStarts.begin());
				if (myMapCells[entrance.y][entrance.x] == '.')
				{
					validStart = true;
					break;
				}
			} while (possibleStarts.size() > 0);

			if (validStart)
			{
				Vector2i from(entrance.x - 1, entrance.y);
				Room* newroom = nullptr;
				do
				{
					newroom = createRoom(entrance, from);
				} while (newroom == nullptr);

				if (newroom != nullptr)
				{
					placeRoom(newroom);
					room->right = newroom;
					roomStack.push_back(newroom);
					rooms.push_back(newroom);
				}

			}
		}

		if (room->topLeft.y > 0 && room->top == nullptr)
		{
			//top
			std::vector<Vector2i> possibleStarts;
			for (int x = room->topLeft.x; x <= room->bottomRight.x; x++)
				possibleStarts.push_back(Vector2i(x, room->topLeft.y - 1));
			std::random_shuffle(possibleStarts.begin(), possibleStarts.end());

			//try to find an unassigned adjacent cell to the top
			Vector2i entrance;
			bool validStart = false;
			do
			{
				entrance = *possibleStarts.begin();
				possibleStarts.erase(possibleStarts.begin());
				if (myMapCells[entrance.y][entrance.x] == '.')
				{
					validStart = true;
					break;
				}
			} while (possibleStarts.size() > 0);

			if (validStart)
			{
				Vector2i from(entrance.x, entrance.y + 1);
				Room* newroom = nullptr;
				do
				{
					newroom = createRoom(entrance, from);
				} while (newroom == nullptr);

				if (newroom != nullptr)
				{
					placeRoom(newroom);
					room->top = newroom;
					roomStack.push_back(newroom);
					rooms.push_back(newroom);
				}

			}
		}

		if (room->bottomRight.y < myHeight - 1 && room->bottom == nullptr)
		{
			//bottom
			std::vector<Vector2i> possibleStarts;
			for (int x = room->topLeft.x; x <= room->bottomRight.x; x++)
				possibleStarts.push_back(Vector2i(x, room->bottomRight.y + 1));
			std::random_shuffle(possibleStarts.begin(), possibleStarts.end());

			//try to find an unassigned adjacent cell to the bottom
			Vector2i entrance;
			bool validStart = false;
			do
			{
				entrance = *possibleStarts.begin();
				possibleStarts.erase(possibleStarts.begin());
				if (myMapCells[entrance.y][entrance.x] == '.')
				{
					validStart = true;
					break;
				}
			} while (possibleStarts.size() > 0);

			if (validStart)
			{
				Vector2i from(entrance.x, entrance.y - 1);
				Room* newroom = nullptr;
				do
				{
					newroom = createRoom(entrance, from);
				} while (newroom == nullptr);

				if (newroom != nullptr)
				{
					placeRoom(newroom);
					room->bottom = newroom;
					roomStack.push_back(newroom);
					rooms.push_back(newroom);
				}
			}
		}

		//shuffle the remaining rooms
		if (roomStack.size() > 0)
			std::random_shuffle(roomStack.begin(), roomStack.end());
	}

	//pick a random two rooms as the start/finish rooms
	Room* start;
	Room* finish;
	do
	{
		start = rooms[rand() % rooms.size()];
		finish = rooms[rand() % rooms.size()];

		int mindistance = (myWidth > myHeight) ? myWidth * 0.75 : myHeight * 0.75;
		if (Vector2i::deltaDistance(start->topLeft, finish->topLeft) < mindistance)
			continue;
		if (start->isDeadEnd() || finish->isDeadEnd())
			continue;

		start->isStarting = true;
		finish->isEnding = true;
		startRoom = start;
		endRoom = finish;
		break;
	} while (true);

}

void GeneratedMap::fixTree()
{
	//get all dead ends
	std::vector<Room*> deadEnds;
	for (int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->isDeadEnd())
			deadEnds.push_back(rooms[i]);
	}
	std::random_shuffle(deadEnds.begin(), deadEnds.end());

	int j = 0;
	int addedCount = 0;
	int cyclesToAdd = (deadEnds.size() * CYCLE_PERCENT) / 100;
	//keep adding cycles until the quantity is met or we run out of
	//dead ends to try
	while (j < deadEnds.size() && addedCount < cyclesToAdd)
	{
		Room* de = deadEnds[j++];
		std::vector<Room*> adjacentRooms = getAdjacentRooms(de);
		if (adjacentRooms.size() == 0)
			continue;

		std::random_shuffle(adjacentRooms.begin(), adjacentRooms.end());

		int i2 = 0;
		Room* aj = nullptr;
		//find first non-connected room
		while (i2 < adjacentRooms.size() && aj == nullptr)
		{
			if (!de->areConnected(adjacentRooms[i2]))
				aj = adjacentRooms[i2];			
			i2++;
		}

		//if no non-connected room is found try the next dead-end
		if (aj == nullptr)
			continue;

		aj->hasSecondEntrance = true;

		//rooms are adjacent, but not connected, now we connect them... with
		//wizardry...
		int directionOfOther = de->directionOfOtherRoom(aj);
		int miny = 0, minx = 0, shift = 0;
		if (directionOfOther == Room::LEFT || directionOfOther == Room::RIGHT)
		{
			miny = de->topLeft.y >= aj->topLeft.y
				? de->topLeft.y : aj->topLeft.y;
			int maxy = de->bottomRight.y <= aj->bottomRight.y
				? de->bottomRight.y : aj->bottomRight.y;
			int dy = abs(maxy - miny);
			shift = dy <= 0 ? 0 : rand() % (dy + 1);
		}
		else if (directionOfOther == Room::UP || directionOfOther == Room::DOWN)
		{
			minx = de->topLeft.x >= aj->topLeft.x
				? de->topLeft.x : aj->topLeft.x;
			int maxx = de->bottomRight.x <= aj->bottomRight.x
				? de->bottomRight.x : aj->bottomRight.x;
			int dx = abs(maxx - minx);
			shift = dx <= 0 ? 0 : rand() % (dx + 1);
		}

		switch (directionOfOther)
		{
		case Room::LEFT:
			aj->startingCell2 = Vector2i(aj->bottomRight.x, miny + shift);
			aj->previousCell2 = Vector2i(de->topLeft.x, miny + shift);
			break;
		case Room::RIGHT:
			aj->startingCell2 = Vector2i(aj->topLeft.x, miny + shift);
			aj->previousCell2 = Vector2i(de->bottomRight.x, miny + shift);
			break;
		case Room::UP:
			aj->startingCell2 = Vector2i(minx + shift, aj->bottomRight.y);
			aj->previousCell2 = Vector2i(minx + shift, de->topLeft.y);
			break;
		case Room::DOWN:
			aj->startingCell2 = Vector2i(minx + shift, aj->topLeft.y);
			aj->previousCell2 = Vector2i(minx + shift, de->bottomRight.y);
			break;
		}

		addedCount++;
	}
	int x = 2;
}

std::vector<Room*> GeneratedMap::getAdjacentRooms(Room* room)
{
	std::vector<Room*> adjacentRooms;

	//a hopefully reusable lambda that will make the code below shorter
	//it checks if the given room is not null, if it isn't, adds the given
	//room to the roomsToCheck vector.
	auto addRoomToCollection = [](Room* room, std::vector<Room*>* adjacentRooms) 
	{
		if (room == nullptr)
			return;

		bool found = false;
		for (int i = 0; i < adjacentRooms->size(); i++)
		{
			if ((*adjacentRooms)[i] == room)
			{
				found = true;
				break;
			}
		}

		if (!found)
			adjacentRooms->push_back(room);
	};

	//rooms to left
	if (room->topLeft.x > 0)
	{
		int x = room->topLeft.x - 1;
		for (int y = room->topLeft.y; y <= room->bottomRight.y; y++)
			addRoomToCollection(getRoomFromCell(x, y), &adjacentRooms);
	}

	//rooms to right
	if (room->bottomRight.x < myWidth - 1)
	{
		int x = room->bottomRight.x + 1;
		for (int y = room->topLeft.y; y <= room->bottomRight.y; y++)
			addRoomToCollection(getRoomFromCell(x, y), &adjacentRooms);
	}

	//rooms to top
	if (room->topLeft.y > 0)
	{
		int y = room->topLeft.y - 1;
		for (int x = room->topLeft.x - 1; x <= room->bottomRight.x; x++)
			addRoomToCollection(getRoomFromCell(x, y), &adjacentRooms);
	}

	//rooms to bottom
	if (room->bottomRight.y < myHeight - 1)
	{
		int y = room->bottomRight.y + 1;
		for (int x = room->topLeft.x - 1; x <= room->bottomRight.x; x++)
			addRoomToCollection(getRoomFromCell(x, y), &adjacentRooms);
	}

	return adjacentRooms;
}

Room* GeneratedMap::getRoomFromCell(int x, int y)
{
	Room* room;
	if (x < 0 || x >= myWidth || y < 0 || y >= myHeight)
		return nullptr;
	if (myMapCells[y][x] == 'X')
		return nullptr;

	for (int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->containsCell(x, y))
			return rooms[i];
	}
	return nullptr;
}

bool GeneratedMap::isRoomInBounds(Vector2i &topLeft, Vector2i &bottomRight)
{
	if (topLeft.x < 0 || bottomRight.x < 0)
		return false;
	if (topLeft.x > myWidth || bottomRight.x >= myWidth)
		return false;
	if (topLeft.y < 0 || bottomRight.y < 0)
		return false;
	if (topLeft.y > myHeight || bottomRight.y >= myHeight)
		return false;

	return true;
}

void GeneratedMap::placeRoom(Room* room)
{
	for (int y = room->topLeft.y; y <= room->bottomRight.y; y++)
	{
		for (int x = room->topLeft.x; x <= room->bottomRight.x; x++)
		{
			myMapCells[y][x] = 'R';
		}
	}
}

void GeneratedMap::removeEmpties()
{
	for (int y = 0; y < myHeight; y++)
	{
		for (int x = 0; x < myWidth; x++)
		{
			if (myMapCells[y][x] == 'X')
				myMapCells[y][x] = '.';
		}
	}
}

void GeneratedMap::replacePeriods(Vector2i topLeft, Vector2i bottomRight, char replacement)
{
	for(int y = topLeft.y ; y <= bottomRight.y ; y++)
	{
		for(int x = topLeft.x ; x <= bottomRight.x ; x++)
		{
			if(myMapCells[y][x] == '.')			
				myMapCells[y][x] = replacement;			
		}
	}
}
