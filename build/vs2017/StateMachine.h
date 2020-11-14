#pragma once

#include "State.h"

namespace gef
{
	class Platform;
	class InputManager;
	class SpriteRenderer;
	class AudioManager;
}
class MenuState;
class GameState;
class OptionsState;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void SetState(State::TYPE type);

	inline State* GetState() { return state_; }

private:
	gef::Platform* platform_;
	gef::InputManager* input_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::AudioManager* audio_manager_;

	GameState* game_state_;
	MenuState* menu_state_;
	OptionsState* options_state_;

	State* state_;
};

