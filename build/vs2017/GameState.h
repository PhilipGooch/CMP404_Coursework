#pragma once

#include "State.h"

#include "Globals.h" // DELETE

#include <vector>
#include <maths/matrix44.h>
#include <graphics/mesh_instance.h>

class Boid;
class Wolf;

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
	virtual void Render() = 0;

protected:
	virtual void UpdateMarkers() = 0;

	gef::Matrix44 projection_matrix_;
	gef::Matrix44 view_matrix_;

	gef::Matrix44 marker_matrices_[6];
	gef::MeshInstance marker_mesh_instances_[6];

	int selected_marker_ID_;
	int targeted_marker_ID_;

	gef::MeshInstance selected_marker_mesh_instance_;
	gef::MeshInstance targeted_marker_mesh_instance_;

	int number_of_cows_;
	std::vector<Boid*> cows_;

	Wolf* wolf_;
};

