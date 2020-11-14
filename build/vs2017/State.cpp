#include "State.h"

State::State(gef::Platform* platform, 
			 gef::InputManager* input_manager, 
			 gef::AudioManager* audio_manager, 
			 gef::SpriteRenderer* sprite_renderer,
			 gef::Renderer3D* renderer_3D,
			 gef::Font* font,
			 std::vector<gef::Mesh*> meshes) :
	platform_(platform),
	input_manager_(input_manager),
	audio_manager_(audio_manager),
	sprite_renderer_(sprite_renderer),
	renderer_3D_(renderer_3D),
	font_(font),
	meshes_(meshes),
	fps_(-1.f)
{
}
