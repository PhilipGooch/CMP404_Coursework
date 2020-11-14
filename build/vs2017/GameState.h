#pragma once

#include "State.h"

class GameState : public State
{
public:
	GameState();

	void HandleInput();
	void Update(float delta_time);
	void Render();
};

