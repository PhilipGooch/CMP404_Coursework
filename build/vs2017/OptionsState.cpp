#include "OptionsState.h"

OptionsState::OptionsState(gef::Platform* platform, 
						   gef::InputManager* input_manager, 
						   gef::AudioManager* audio_manager, 
						   gef::SpriteRenderer* sprite_renderer,
						   gef::Renderer3D* renderer_3D) :
	State(platform, input_manager, audio_manager, sprite_renderer, renderer_3D)
{
}

void OptionsState::HandleInput()
{
}

void OptionsState::Update(float frame_time)
{
}

void OptionsState::Render()
{
}
