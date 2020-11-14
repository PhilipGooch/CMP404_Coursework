#pragma once

#include "State.h"

class OptionsState : public State
{
public:
	OptionsState(gef::Platform* platform, gef::InputManager* input_manager, gef::AudioManager* audio_manager, gef::SpriteRenderer* sprite_renderer);

	void HandleInput();
	void Update(float delta_time);
	void Render();
};

