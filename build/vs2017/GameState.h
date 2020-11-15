#pragma once

#include "State.h"

#include "Globals.h" // DELETE

#include <vector>
#include <maths/matrix44.h>
#include <graphics/mesh_instance.h>

class Boid;
class Wolf;
class Marker;

class GameState : public State
{
public:
	GameState(gef::Platform* platform, 
			  gef::InputManager* input_manager, 
			  gef::AudioManager* audio_manager, 
			  gef::SpriteRenderer* sprite_renderer,
			  gef::Renderer3D* renderer_3D,
			  gef::Font* font,
			  std::vector<gef::Mesh*> meshes);

	virtual bool HandleInput() = 0;
	virtual void Update(float delta_time) = 0;
	void Render();

protected:
	void DrawHUD();
	void SetupLights();

	gef::Matrix44 projection_matrix_;
	gef::Matrix44 view_matrix_;

	int number_of_markers_;
	std::vector<Marker*> markers_;
	Marker* selected_marker_;
	Marker* targeted_marker_;

	int number_of_cows_;
	std::vector<Boid*> cows_;

	int number_of_wolves_;
	std::vector<Boid*> wolves_;
};

