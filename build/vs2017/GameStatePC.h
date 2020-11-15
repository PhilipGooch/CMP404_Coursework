#pragma once

#include "GameState.h"

class Camera;

class GameStatePC : public GameState
{
public:
	GameStatePC(gef::Platform* platform,
		gef::InputManager* input_manager,
		gef::AudioManager* audio_manager,
		gef::SpriteRenderer* sprite_renderer,
		gef::Renderer3D* renderer_3D,
		gef::Font* font,
		std::vector<gef::Mesh*> meshes);

	bool HandleInput() override;
	void Update(float delta_time) override;
	void Render() override;

private:
	void UpdateMarkers() override;

	Camera* camera_;
	bool camera_active_;

	float anchorX;
	float anchorZ;
	float angles[6];
};

