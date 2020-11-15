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
				StateMachine* state_machine,
				std::vector<gef::Mesh*> meshes);

	bool HandleInput() override;
	void Update(float delta_time) override;

private:
	void MoveMarker(gef::Vector4 velocity);
	void RotateMarker(float delta_angle);
	void SwapTargetMarker(int target);
	void SwapSelectedMarker();

	Camera* camera_;

	std::vector<float> angles_;
};

