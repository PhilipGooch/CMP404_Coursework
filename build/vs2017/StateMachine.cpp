#include "StateMachine.h"

#if VITA_MODE
#include "GameStateVITA.h"
#else
#include "GameStatePC.h"
#endif
#include "MenuState.h"
#include "OptionsState.h"

StateMachine::StateMachine(gef::Platform* platform, 
						   gef::InputManager* input_manager,
						   gef::AudioManager* audio_manager,
						   gef::SpriteRenderer* sprite_renderer,
						   gef::Renderer3D* renderer_3D,
						   gef::Font* font,
						   std::vector<gef::Mesh*> meshes) :
	state_(nullptr)
{
#if VITA_MODE
	game_state_ = new GameStateVITA(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, this, meshes);
#else
	game_state_ = new GameStatePC(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, this, meshes);
#endif
	menu_state_ = new MenuState(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, this, meshes);
	options_state_ = new OptionsState(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, this, meshes);

	SetState(STATE::MENU); 
}

StateMachine::~StateMachine()
{
}

void StateMachine::SetState(STATE state)
{
	
	switch (state)
	{
	case STATE::GAME:
		state_ = game_state_;
		break;
	case STATE::MENU:
		state_ = menu_state_;
		break;
	case STATE::OPTIONS:
		state_ = options_state_;
		break;
	}

	
}
