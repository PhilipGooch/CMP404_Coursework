#include "MenuState.h"

MenuState::MenuState(gef::Platform* platform, 
					 gef::InputManager* input_manager, 
					 gef::AudioManager* audio_manager, 
					 gef::SpriteRenderer* sprite_renderer,
					 gef::Renderer3D* renderer_3D) :
	State(platform, input_manager, audio_manager, sprite_renderer, renderer_3D)
{
}

void MenuState::HandleInput()
{
}

void MenuState::Update(float frame_time)
{
}

void MenuState::Render()
{
}
