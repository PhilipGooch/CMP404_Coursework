#include "GameStatePC.h"

#include "Boid.h"
#include "Cow.h"
#include "Wolf.h"
#include "Camera.h"
#include <input/input_manager.h>
#include <input/keyboard.h>
#include <graphics/renderer_3d.h>
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

GameStatePC::GameStatePC(gef::Platform* platform,
	gef::InputManager* input_manager,
	gef::AudioManager* audio_manager,
	gef::SpriteRenderer* sprite_renderer,
	gef::Renderer3D* renderer_3D,
	gef::Font* font,
	std::vector<gef::Mesh*> meshes) :
	GameState(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, meshes)
{
	camera_ = new Camera(*platform, input_manager);
	camera_active_ = true;

	number_of_cows_ = 10;

	for (int i = 0; i < number_of_cows_; i++)
	{
		cows_.push_back(new Cow(renderer_3D_, meshes));
	}

	wolf_ = new Wolf(renderer_3D_, meshes);

	// LIGHTING
	
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(450.f, 100.0f, 250.f));
	gef::Default3DShaderData& default_shader_data = renderer_3D_->default_shader_data();
	float light = 0.5f;
	default_shader_data.set_ambient_light_colour(gef::Colour(light, light, light, 1.0f));
	//default_shader_data.AddPointLight(default_point_light);

	// MARKERS
	targeted_marker_ID_ = 0;
	selected_marker_ID_ = 0;
	for (int i = 0; i < 6; i++)
	{
		marker_matrices_[i].SetIdentity();
	}
	marker_matrices_[0].SetTranslation(gef::Vector4(-400, 0, -300));
	marker_matrices_[1].SetTranslation(gef::Vector4(0, 0, -300));
	marker_matrices_[2].SetTranslation(gef::Vector4(400, 0, -300));
	marker_matrices_[3].SetTranslation(gef::Vector4(-400, 0, 300));
	marker_matrices_[4].SetTranslation(gef::Vector4(0, 0, 300));
	marker_matrices_[5].SetTranslation(gef::Vector4(400, 0, 300));
	anchorX = marker_matrices_[targeted_marker_ID_].GetRow(3).x();
	anchorZ = marker_matrices_[targeted_marker_ID_].GetRow(3).z();
	for (int i = 0; i < 6; i++)
	{
		angles[i] = 0.f;
	}
	UpdateMarkers();
	
	for (int i = 0; i < 6; i++)
	{
		marker_mesh_instances_[i].set_mesh(meshes[6 + i]);
	}
	
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
				camera_active_ = true;
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_C))
			{
				camera_active_ = false;
			}
			if (camera_active_)
			{
				camera_->HandleInput();
			}
			else
			{
				if (keyboard->IsKeyDown(gef::Keyboard::KC_1))
				{
					targeted_marker_ID_ = 0;
					anchorX = marker_matrices_[targeted_marker_ID_].GetRow(3).x();
					anchorZ = marker_matrices_[targeted_marker_ID_].GetRow(3).z();
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_2))
				{
					targeted_marker_ID_ = 1;
					anchorX = marker_matrices_[targeted_marker_ID_].GetRow(3).x();
					anchorZ = marker_matrices_[targeted_marker_ID_].GetRow(3).z();
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_3))
				{
					targeted_marker_ID_ = 2;
					anchorX = marker_matrices_[targeted_marker_ID_].GetRow(3).x();
					anchorZ = marker_matrices_[targeted_marker_ID_].GetRow(3).z();
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_4))
				{
					targeted_marker_ID_ = 3;
					anchorX = marker_matrices_[targeted_marker_ID_].GetRow(3).x();
					anchorZ = marker_matrices_[targeted_marker_ID_].GetRow(3).z();
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_5))
				{
					targeted_marker_ID_ = 4;
					anchorX = marker_matrices_[targeted_marker_ID_].GetRow(3).x();
					anchorZ = marker_matrices_[targeted_marker_ID_].GetRow(3).z();
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_6))
				{
					targeted_marker_ID_ = 5;
					anchorX = marker_matrices_[targeted_marker_ID_].GetRow(3).x();
					anchorZ = marker_matrices_[targeted_marker_ID_].GetRow(3).z();
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_A))
				{
					anchorX -= 10;
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_D))
				{
					anchorX += 10;
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_W))
				{
					anchorZ -= 10;
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_S))
				{
					anchorZ += 10;
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_E))
				{
					angles[targeted_marker_ID_] -= 0.01f;
					gef::Matrix44 matrix = marker_matrices_[targeted_marker_ID_];
					marker_matrices_[targeted_marker_ID_].SetTranslation(gef::Vector4(0.f, 0.f, 0.f));
					marker_matrices_[targeted_marker_ID_].RotationY(angles[targeted_marker_ID_]);
					marker_matrices_[targeted_marker_ID_].SetTranslation(matrix.GetTranslation());
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_Q))
				{
					angles[targeted_marker_ID_] += 0.01f;
					gef::Matrix44 matrix = marker_matrices_[targeted_marker_ID_];
					marker_matrices_[targeted_marker_ID_].SetTranslation(gef::Vector4(0.f, 0.f, 0.f));
					marker_matrices_[targeted_marker_ID_].RotationY(angles[targeted_marker_ID_]);
					marker_matrices_[targeted_marker_ID_].SetTranslation(matrix.GetTranslation());
				}
				if (keyboard->IsKeyDown(gef::Keyboard::KC_SPACE))
				{
					if (selected_marker_ID_ != targeted_marker_ID_ && targeted_marker_ID_ != 5)
					{
						int previous_marker_ID = selected_marker_ID_;
						selected_marker_ID_ = targeted_marker_ID_;

						anchorX = marker_matrices_[targeted_marker_ID_].GetRow(3).x();
						anchorZ = marker_matrices_[targeted_marker_ID_].GetRow(3).z();

						gef::Matrix44 previous_marker = marker_matrices_[previous_marker_ID];
						gef::Matrix44 selected_marker = marker_matrices_[selected_marker_ID_];

						gef::Matrix44 selected_marker_inverse;
						selected_marker_inverse.AffineInverse(selected_marker);

						for (Boid* boid : cows_)
						{
							Cow* cow = (Cow*)boid;
							cow->marker_matrix_ = selected_marker;
							cow->local_matrix_ = cow->local_matrix_ * previous_marker * selected_marker_inverse;
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

	for (Boid* boid : cows_)
	{
		Cow* cow = (Cow*)boid; 
		if (targeted_marker_ID_ == selected_marker_ID_ && selected_marker_ID_ != 5)
		{
			cow->marker_matrix_ = marker_matrices_[selected_marker_ID_];
		}
		cow->Flock(cows_, delta_time);
		cow->Update(delta_time);
	}

	wolf_->marker_matrix_ = marker_matrices_[5];

	UpdateMarkers();
}

void GameStatePC::Render()
{
	// 3D scene
	renderer_3D_->set_projection_matrix(projection_matrix_);
	renderer_3D_->set_view_matrix(view_matrix_);

	renderer_3D_->Begin(true);
	
	selected_marker_mesh_instance_.set_mesh(meshes_[6 + selected_marker_ID_ + 6]);
	targeted_marker_mesh_instance_.set_mesh(meshes_[6 + targeted_marker_ID_ + 12]);
	for (int i = 0; i < 6; i++)
	{
		if (i == selected_marker_ID_) continue;
		if (i == targeted_marker_ID_) continue;
		renderer_3D_->DrawMesh(marker_mesh_instances_[i]);
	}
	renderer_3D_->DrawMesh(targeted_marker_mesh_instance_);
	renderer_3D_->DrawMesh(selected_marker_mesh_instance_);

	for (Boid* boid : cows_)
	{
		Cow* cow = (Cow*)boid;
		cow->Render();
	}

	wolf_->Render();

	renderer_3D_->End();

	// HUD
	gef::Matrix44 proj_matrix2d;
	proj_matrix2d = platform_->OrthographicFrustum(0.0f, platform_->width(), 0.0f, platform_->height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	if (font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
	sprite_renderer_->End();

}

void GameStatePC::UpdateMarkers()
{
	marker_matrices_[targeted_marker_ID_].SetTranslation(gef::Vector4(anchorX, 0, anchorZ));

	for (int i = 0; i < 6; i++)
	{
		marker_mesh_instances_[i].set_transform(marker_matrices_[i]);
	}
	selected_marker_mesh_instance_.set_transform(marker_matrices_[selected_marker_ID_]);
	targeted_marker_mesh_instance_.set_transform(marker_matrices_[targeted_marker_ID_]);
}
