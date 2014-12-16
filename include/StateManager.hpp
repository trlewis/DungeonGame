#ifndef STATE_MANAGER_HPP_
#define STATE_MANAGER_HPP_

#include <stack>
#include <SFML/Graphics.hpp>

#include "GameState.hpp"

class GameState;

class StateManager
{
public:
	bool init(sf::RenderWindow* win, const char* title, int width = DEFAULT_SCREEN_WIDTH,
		int height = DEFAULT_SCREEN_HEIGHT, bool fullScreen = false);

	/*! Replaces the active state with the given state. */
	void changeState(GameState* state);

	/*! Calls cleanup() on all states in the stack and deletes them. */
	void cleanup();

	/*! Tells the active state to draw its contents. */
	void draw();

	/*! Calls handelEvents() on the active state. */
	void handleEvents();

	/*! Removes the active (top state in the stack) state. */
	void popState();

	/*! Pushes the given state onto the stack, becoming the new active state.*/
	void pushState(GameState* state);

	/*! Calls update() on the active state. */
	void update();

	inline bool isRunning() { return running; }
	inline void quit() { running = false; }

	sf::RenderWindow* window;
private:
	static const int DEFAULT_SCREEN_WIDTH = 800;
	static const int DEFAULT_SCREEN_HEIGHT = 600;

	/*! The stack of game states (screens), the top one is the "active state"*/
	std::stack<GameState* > states;
	bool running;
	bool fullscreen;
};

#endif // !STATE_MANAGER_HPP_