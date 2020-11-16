#pragma once

#include "State.h"

#include <maths/matrix44.h>

class Cow;
class Camera;

class MenuState : public State
{
public:
	MenuState(gef::Platform* platform,
			  gef::InputManager* input_manager,
			  gef::AudioManager* audio_manager,
			  gef::SpriteRenderer* sprite_renderer,
			  gef::Renderer3D* renderer_3D,
			  gef::Font* font,
			  StateMachine* state_machine,
			  std::vector<gef::Mesh*> meshes);
	~MenuState();

	bool HandleInput() override;
	void Update(float delta_time) override;
	void Render() override;

	void DrawHUD();

	void SetupLights();

private:
	gef::Matrix44 projection_matrix_;
	gef::Matrix44 view_matrix_;

	Camera* camera_;

	Cow* cow_;
};

