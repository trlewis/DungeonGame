#include "StateManager.hpp"

bool StateManager::init(sf::RenderWindow* win, const char* title, int width, int height, bool fullScreen)
{
	fullscreen = fullScreen;//that's not confusing...

	sf::Uint32 flags;
	if (fullScreen)
		flags = sf::Style::Fullscreen;
	else
		flags = sf::Style::Titlebar | sf::Style::Close;

	win->create(sf::VideoMode(width, height), title, flags);

	window = win;

	running = true;

	return true;
}

void StateManager::changeState(GameState* state)
{
	if (!states.empty())
	{
		states.top()->cleanup();
		states.pop();
	}
	states.push(state);
}

void StateManager::cleanup()
{
	while (!states.empty())
	{
		states.top()->cleanup();
		delete states.top();
		states.pop();
	}

	window->close();
}

void StateManager::draw()
{
	window->clear();

	if (!states.empty())
		states.top()->draw(this);

	window->display();
}

void StateManager::handleEvents()
{
	if (!states.empty())
		states.top()->handleEvents(this);
}

void StateManager::popState()
{
	if (!states.empty())
	{
		states.top()->cleanup();
		delete states.top();
		states.pop();
	}

	if (!states.empty())
		states.top()->unpause();
}

void StateManager::pushState(GameState* state)
{
	if (!states.empty())
		states.top()->pause();

	states.push(state);
}

void StateManager::update()
{
	if (!states.empty())
		states.top()->update(this);
}