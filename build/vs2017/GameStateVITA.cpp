#include "GameStateVITA.h"

//#include <input/touch_input_manager.h>
//#include <platform/vita/graphics/texture_vita.h>

#include <input/input_manager.h>
#include <graphics/renderer_3d.h>
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include "Boid.h"
#include "Cow.h"
#include "Wolf.h"
#include "Marker.h"
#include "StateMachine.h"

GameStateVITA::GameStateVITA(gef::Platform* platform,
	gef::InputManager* input_manager,
	gef::AudioManager* audio_manager,
	gef::SpriteRenderer* sprite_renderer,
	gef::Renderer3D* renderer_3D,
	gef::Font* font,
	StateMachine* state_machine,
	std::vector<gef::Mesh*> meshes) :
	GameState(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, state_machine, meshes)
{
	// PROJECTION and VIEW MATRICES
	
	//projection_matrix_ = camera_->projection_matrix_;
	//view_matrix_ = camera_->view_matrix_;


	// MARKERS
	
}

bool GameStateVITA::HandleInput()
{
	if (input_manager_)
	{
		input_manager_->Update();
	}
	return true;
}

void GameStateVITA::Update(float delta_time)
{
	fps_ = 1.f / delta_time;

	//projection_matrix_ = camera_->projection_matrix_;
	//view_matrix_ = camera_->view_matrix_;

	for (Marker* marker : markers_)
	{
		marker->Update();
	}

	for (Boid* boid : cows_)
	{
		Cow* cow = (Cow*)boid;
		cow->marker_matrix_ = selected_marker_->world_matrix_;
		cow->Flock(cows_, delta_time);
		cow->Update(delta_time);
	}

	for (Boid* boid : wolves_)
	{
		Wolf* wolf = (Wolf*)boid;
		wolf->marker_matrix_ = markers_[5]->world_matrix_;
		wolf->Flock(wolves_, delta_time);
		wolf->Update(delta_time);
	}
}

void GameStateVITA::SwapTargetMarker(int target)
{
	targeted_marker_->targeted_ = false;
	targeted_marker_ = markers_[target];
	targeted_marker_->targeted_ = true;
}

void GameStateVITA::SwapMarker()
{
	Marker::CHILD previous_anchor = selected_marker_->child_;
	selected_marker_->child_ = Marker::CHILD::NONE;
	selected_marker_ = targeted_marker_;
	selected_marker_->child_ = previous_anchor;
}
