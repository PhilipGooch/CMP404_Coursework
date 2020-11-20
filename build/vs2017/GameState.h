#pragma once

#include "State.h"

#include "Globals.h" // DELETE

#include <vector>
#include <maths/matrix44.h>

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
			  StateMachine* state_machine,
			  std::vector<gef::Mesh*> meshes);

protected:
	void DrawHUD();
	void SetupLights();

	gef::Matrix44 projection_matrix_;
	gef::Matrix44 view_matrix_;

	int number_of_markers_;
	std::vector<Marker*> markers_;
	Marker* cow_marker_;
	Marker* wolf_marker_;
	Marker* tree_marker_;
	Marker* targeted_marker_;
	Marker* selected_marker_;

	int number_of_cows_;
	std::vector<Boid*> cows_;

	int number_of_wolves_;
	std::vector<Boid*> wolves_;

	int number_of_trees_;
	std::vector<Boid*> trees_;

};

