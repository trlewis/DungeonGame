#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_

#include "StateManager.hpp"

class StateManager;

class GameState
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;
	virtual void pause() = 0;
	virtual void unpause() = 0;
	virtual void handleEvents(StateManager* stateManager) = 0;
	virtual void update(StateManager* stateManager) = 0;
	virtual void draw(StateManager* stateManager) = 0;
	inline void changeState(StateManager* stateManager, GameState* state);

	virtual ~GameState(){};
protected:
	bool paused;
};

#endif // !GAME_STATE_HPP_