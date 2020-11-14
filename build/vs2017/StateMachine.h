#pragma once

#include "State.h"

namespace gef
{
	class Platform;
	class InputManager;
	class SpriteRenderer;
	class AudioManager;
	class Renderer3D;
}
class MenuState;
class GameState;
class OptionsState;

class StateMachine
{
public:
	StateMachine(gef::Platform* platform, 
				 gef::InputManager* input_manager, 
				 gef::AudioManager* audio_manager, 
				 gef::SpriteRenderer* sprite_renderer,
				 gef::Renderer3D* renderer_3D,
				 gef::Font* font,
				 std::vector<gef::Mesh*> meshes);

	~StateMachine();

	enum STATE
	{
		MENU,
		GAME,
		OPTIONS
	};

	void SetState(STATE type);

	inline State* GetState() { return state_; }

private:

	gef::Platform* platform_;
	gef::InputManager* input_manager_;
	gef::AudioManager* audio_manager_;
	gef::SpriteRenderer* sprite_renderer_;

	GameState* game_state_;
	MenuState* menu_state_;
	OptionsState* options_state_;

	State* state_;
};

