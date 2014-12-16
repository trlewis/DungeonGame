//Using standard IO, and strings

#include <SFML/Graphics.hpp>

#include "StateManager.hpp"
#include "GeneratedMapScreen.hpp"

int main(int argc, char* args[])
{
	sf::RenderWindow rw;

	StateManager sm;
	sm.init(&rw, "G: generate | C: cycles | ESC: quit");
	GeneratedMapScreen* mapScreen = new GeneratedMapScreen();
	sm.pushState((GameState*)mapScreen);

	while (sm.isRunning())
	{
		sm.handleEvents();
		sm.update();
		sm.draw();
	}

	sm.cleanup();

	return 0;
}