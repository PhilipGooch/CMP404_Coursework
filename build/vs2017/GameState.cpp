#include "GameState.h"

GameState::GameState(gef::Platform* platform, 
					 gef::InputManager* input_manager, 
					 gef::AudioManager* audio_manager, 
					 gef::SpriteRenderer* sprite_renderer,
					 gef::Renderer3D* renderer_3D,
					 gef::Font* font,
					 std::vector<gef::Mesh*> meshes) :
	State(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, meshes)
{
}

