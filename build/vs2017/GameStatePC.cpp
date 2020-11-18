#include "GameStatePC.h"

#include <input/input_manager.h>
#include <input/keyboard.h>
#include <graphics/renderer_3d.h>
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include "Boid.h"
#include "Cow.h"
#include "Wolf.h"
#include "Marker.h"
#include "Camera.h"
#include "StateMachine.h"

GameStatePC::GameStatePC(gef::Platform* platform,
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
	camera_ = new Camera(*platform, input_manager);
	camera_->eye_ = gef::Vector4(0, 1300.0f, 1300.f);
	camera_->look_at_ = gef::Vector4(0.f, 0.f, 0.f);
	projection_matrix_ = camera_->projection_matrix_;
	view_matrix_ = camera_->view_matrix_;


	// MARKERS
	std::vector<gef::Vector4> marker_translations
	{
		gef::Vector4(-400, 0, -300),
		gef::Vector4(0, 0, -300),
		gef::Vector4(400, 0, -300),
		gef::Vector4(-400, 0, 300),
		gef::Vector4(0, 0, 300),
		gef::Vector4(400, 0, 300)
	};
	for (int i = 0; i < number_of_markers_; i++)
	{
		gef::Matrix44 world_matrix;
		world_matrix.SetIdentity();
		world_matrix.SetTranslation(marker_translations[i]);
		markers_.push_back(new Marker(renderer_3D, meshes, world_matrix, i));
	}	

	targeted_marker_ = markers_[0];
	selected_marker_ = targeted_marker_;
	
	cow_marker_ = markers_[0];
	cow_marker_->child_ = Marker::CHILD::COW;
	cow_marker_->boids_ = &cows_;
	cow_marker_->targeted_ = true;
	for (Boid* boid : cows_)
	{
		Cow* cow = (Cow*)boid;
		cow->marker_matrix_ = markers_[0]->world_matrix_;
	}

	wolf_marker_ = markers_[5];
	wolf_marker_->child_ = Marker::CHILD::WOLF;
	wolf_marker_->boids_ = &wolves_;
	for (Boid* boid : wolves_)
	{
		Wolf* wolf = (Wolf*)boid;
		wolf->marker_matrix_ = markers_[5]->world_matrix_;
	}

	gef::Matrix44 tree_matrix;
	tree_matrix.SetIdentity();
	tree_matrix.SetTranslation(gef::Vector4(0, 0.f, -200));
	tree_->marker_matrix_ = tree_matrix;
}

bool GameStatePC::HandleInput()
{
	if (input_manager_)
	{
		input_manager_->Update();
		gef::Keyboard* keyboard = input_manager_->keyboard();
		if (keyboard)
		{
			if (keyboard->IsKeyDown(gef::Keyboard::KC_ESCAPE))
			{
				return false;
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_F))
			{
				camera_->active_ = true;
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_C))
			{
				camera_->active_ = false;
			}
			if (camera_->active_)
			{
				camera_->HandleInput();
			}
			else if (!camera_->active_)
			{
				if (keyboard->IsKeyDown(gef::Keyboard::KC_1))
				{
					SwapTargetMarker(0);
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_2))
				{
					SwapTargetMarker(1);
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_3))
				{
					SwapTargetMarker(2);
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_4))
				{
					SwapTargetMarker(3);
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_5))
				{
					SwapTargetMarker(4);
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_6))
				{
					SwapTargetMarker(5);
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_A))
				{
					MoveMarker(gef::Vector4(-8.f, 0.f, 0.f));
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_D))
				{
					MoveMarker(gef::Vector4(8.f, 0.f, 0.f));
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_W))
				{
					MoveMarker(gef::Vector4(0.f, 0.f, -8.f));
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_S))
				{
					MoveMarker(gef::Vector4(0.f, 0.f, 8.f));
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_E))
				{
					RotateMarker(-0.02f);
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_Q))
				{
					RotateMarker(0.02f);
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_SPACE))
				{
					if (targeted_marker_->child_ == Marker::CHILD::NONE)
					{
						gef::Matrix44 previous_marker_matrix = selected_marker_->world_matrix_;

						SwapMarker();

						gef::Matrix44 selected_marker_inverse_world_matrix;
						selected_marker_inverse_world_matrix.AffineInverse(selected_marker_->world_matrix_);

						for (Boid* boid : cows_)
						{
							Cow* cow = (Cow*)boid;
							cow->marker_matrix_ = selected_marker_->world_matrix_;
							cow->local_matrix_ = cow->local_matrix_ * previous_marker_matrix * selected_marker_inverse_world_matrix;
						}
					}
					
				}
			}
		}
	}
	return true;
}

void GameStatePC::Update(float delta_time)
{
	fps_ = 1.f / delta_time;

	camera_->Update(delta_time);
	projection_matrix_ = camera_->projection_matrix_;
	view_matrix_ = camera_->view_matrix_;

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

void GameStatePC::Render()
{
	renderer_3D_->set_projection_matrix(projection_matrix_);
	renderer_3D_->set_view_matrix(view_matrix_);
	renderer_3D_->Begin(true);

	for (Marker* marker : markers_)
	{
		marker->Render();
	}

	for (Boid* boid : cows_)
	{
		Cow* cow = (Cow*)boid;
		cow->Render();
	}

	for (Boid* boid : wolves_)
	{
		Wolf* wolf = (Wolf*)boid;
		wolf->Render();
	}

	tree_->Render();

	renderer_3D_->End();

	DrawHUD();
}

void GameStatePC::MoveMarker(gef::Vector4 velocity)
{
	gef::Vector4 position = targeted_marker_->world_matrix_.GetTranslation();
	position += velocity;
	targeted_marker_->world_matrix_.SetTranslation(position);
}

void GameStatePC::RotateMarker(float delta_angle)
{
	targeted_marker_->angle_ += delta_angle;
	gef::Matrix44 matrix = targeted_marker_->world_matrix_;
	targeted_marker_->world_matrix_.SetTranslation(gef::Vector4(0.f, 0.f, 0.f));
	targeted_marker_->world_matrix_.RotationY(targeted_marker_->angle_);
	targeted_marker_->world_matrix_.SetTranslation(matrix.GetTranslation());
}

void GameStatePC::SwapTargetMarker(int target)
{
	targeted_marker_->targeted_ = false;
	targeted_marker_ = markers_[target];
	targeted_marker_->targeted_ = true;
}

void GameStatePC::SwapMarker()
{
	Marker::CHILD previous_anchor = selected_marker_->child_;
	selected_marker_->child_ = Marker::CHILD::NONE;
	selected_marker_ = targeted_marker_;
	selected_marker_->child_ = previous_anchor;
}
