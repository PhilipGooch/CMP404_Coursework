#pragma once

#include "GameState.h"

#include <camera.h>
#include <graphics/sprite.h>

namespace gef
{
	class TextureVita;
}



namespace gef
{
	class Sprite;
}

class GameStateVITA : public GameState
{
public:
	GameStateVITA(gef::Platform* platform,
				  gef::InputManager* input_manager,
				  gef::AudioManager* audio_manager,
				  gef::SpriteRenderer* sprite_renderer,
				  gef::Renderer3D* renderer_3D,
				  gef::Font* font,
				  StateMachine* state_machine,
				  std::vector<gef::Mesh*> meshes);
	
	~GameStateVITA();

	bool HandleInput() override;
	void Update(float delta_time) override;
	void Render() override;

private:
	void SwapTargetMarker(int target);
	void SwapMarker();

	gef::Matrix44 ortho_matrix_;
	gef::Sprite camera_feed_sprite_;
	gef::TextureVita* camera_feed_texture_;
};

