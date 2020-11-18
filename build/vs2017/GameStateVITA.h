#pragma once

#include "GameState.h"

#include <camera.h>
#include <graphics/sprite.h>

namespace gef
{
	class TextureVita;
}

//#include <gxm.h>
//#include <motion.h>
//#include <libdbg.h>
//#include <libsmart.h>

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

	bool HandleInput() override;
	void Update(float delta_time) override;

private:
	void SwapTargetMarker(int target);
	void SwapMarker();

	gef::Matrix44 ortho_matrix_;
	gef::Sprite camera_feed_sprite_;
	gef::TextureVita* camera_feed_texture_;
};

