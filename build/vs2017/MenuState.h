#pragma once

#include "State.h"

#include <maths/matrix44.h>
#include <graphics/mesh_instance.h>

class Cow;
class Camera;
class Wolf;
class Tree;

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

	Wolf* wolf_;

	Tree* tree_;
	Tree* tree_2_;
	Tree* tree_3_;

	gef::MeshInstance plane_mesh_instance_;
};

