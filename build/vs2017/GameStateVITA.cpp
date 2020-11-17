#include "GameStateVITA.h"

#include "Boid.h"
#include "Cow.h"
#include "Camera.h"
#include <input/input_manager.h>
#include <input/keyboard.h>
#include <graphics/renderer_3d.h>
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

GameStateVITA::GameStateVITA(gef::Platform* platform,
	gef::InputManager* input_manager,
	gef::AudioManager* audio_manager,
	gef::SpriteRenderer* sprite_renderer,
	gef::Renderer3D* renderer_3D,
	gef::Font* font,
	std::vector<gef::Mesh*> meshes) :
	GameState(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, meshes),
	number_of_cows_(10)
{
	float camera_image_scale_factor = (960.f / 544.f) / (640.f / 480.f);

	for (int i = 0; i < number_of_cows_; i++)
	{
		cows_.push_back(new Cow(renderer_3D_, meshes));
	}

	// lighting
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(0.f, 40.0f, 0.f));
	gef::Default3DShaderData& default_shader_data = renderer_3D_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);

	// markers
	targeted_marker_ID = 0;
	selected_marker_ID = 0;
	previous_marker_ID = 0;

	for (int i = 0; i < 6; i++)
	{
		markers_[i].SetIdentity();
	}
	markers_[0].RotationY(3.1415f / 4.0f);
	markers_[0].SetTranslation(gef::Vector4(-400, 0, 300));
	markers_[1].SetTranslation(gef::Vector4(-400, 0, -300));
	markers_[2].SetTranslation(gef::Vector4(0, 0, 300));
	markers_[3].SetTranslation(gef::Vector4(0, 0, -300));
	markers_[4].SetTranslation(gef::Vector4(400, 0, 300));
	markers_[5].SetTranslation(gef::Vector4(400, 0, -300));
	previous_marker_ = markers_[previous_marker_ID];
	selected_marker_ = markers_[selected_marker_ID];

	for (int i = 0; i < 6; i++)
	{
		marker_mesh_instances_[i].set_mesh(meshes[6]);
		marker_mesh_instances_[i].set_transform(markers_[i]);
	}
	selected_marker_mesh_instance.set_mesh(meshes[7]);
	targeted_marker_mesh_instance.set_mesh(meshes[8]);

	for (Boid* boid : cows_)
	{
		Cow* cow = (Cow*)boid;
		//cow->marker_matrix_ = selected_marker_;
	}

	anchorX = markers_[targeted_marker_ID].GetRow(3).x();
	anchorZ = markers_[targeted_marker_ID].GetRow(3).z();
}

bool GameStateVITA::HandleInput()
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
			if (keyboard->IsKeyDown(gef::Keyboard::KC_1))
			{
				targeted_marker_ID = 0;
				anchorX = markers_[targeted_marker_ID].GetRow(3).x();
				anchorZ = markers_[targeted_marker_ID].GetRow(3).z();
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_2))
			{
				targeted_marker_ID = 1;
				anchorX = markers_[targeted_marker_ID].GetRow(3).x();
				anchorZ = markers_[targeted_marker_ID].GetRow(3).z();
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_3))
			{
				targeted_marker_ID = 2;
				anchorX = markers_[targeted_marker_ID].GetRow(3).x();
				anchorZ = markers_[targeted_marker_ID].GetRow(3).z();
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_4))
			{
				targeted_marker_ID = 3;
				anchorX = markers_[targeted_marker_ID].GetRow(3).x();
				anchorZ = markers_[targeted_marker_ID].GetRow(3).z();
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_5))
			{
				targeted_marker_ID = 4;
				anchorX = markers_[targeted_marker_ID].GetRow(3).x();
				anchorZ = markers_[targeted_marker_ID].GetRow(3).z();
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_6))
			{
				targeted_marker_ID = 5;
				anchorX = markers_[targeted_marker_ID].GetRow(3).x();
				anchorZ = markers_[targeted_marker_ID].GetRow(3).z();
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
			if (keyboard->IsKeyDown(gef::Keyboard::KC_SPACE))
			{
				if (selected_marker_ID != targeted_marker_ID)
				{



					previous_marker_ID = selected_marker_ID;
					selected_marker_ID = targeted_marker_ID;

					gef::Matrix44 previous_marker = markers_[previous_marker_ID];
					gef::Matrix44 selected_marker = markers_[selected_marker_ID];

					gef::Matrix44 selected_marker_inverse;
					selected_marker_inverse.AffineInverse(selected_marker);

					for (Boid* boid : cows_)
					{
						Cow* cow = (Cow*)boid;
						//cow->marker_matrix_ = selected_marker;
						cow->local_matrix_ = cow->local_matrix_ * previous_marker * selected_marker_inverse;
					}

				}
			}
		}
	}
	return true;
}

void GameStateVITA::Update(float delta_time)
{
	fps_ = 1.f / delta_time;

	// marker tracking
	//AppData* dat = sampleUpdateBegin();

	//smartUpdate(dat->currentImage);

	//if (sampleIsMarkerFound(5))
	//{
	//	sampleGetTransform(5, &markers_[5]);

	//	gef::Matrix44 selected_marker_inverse;
	//	selected_marker_inverse.AffineInverse(selected_marker_);

	//	for (Boid* boid : cows_)
	//	{
	//		Cow* cow = (Cow*)boid;
	//		cow->SetPredatorLocalTransform((markers_[5] * selected_marker_inverse).GetTranslation());
	//	}
	//}

	//// swapping markers if no longer visible
	//if (!sampleIsMarkerFound(selected_marker_ID))
	//{
	//	for (int i = 0; i < 2; i++)
	//	{
	//		if (i == selected_marker_ID) continue;

	//		if (sampleIsMarkerFound(i))
	//		{
	//			previous_marker_ID = selected_marker_ID;
	//			selected_marker_ID = i;
	//			previous_marker_ = selected_marker_;

	//			sampleGetTransform(selected_marker_ID, &selected_marker_);

	//			gef::Matrix44 selected_marker_inverse;
	//			selected_marker_inverse.AffineInverse(selected_marker_);

	//			for (Boid* boid : cows_)
	//			{
	//				Cow* cow = (Cow*)boid;
	//				cow->local_matrix_ = cow->local_matrix_ * previous_marker_ * selected_marker_inverse;
	//			}


	//			break;
	//		}
	//	}
	//}
	//if (sampleIsMarkerFound(selected_marker_ID))
	//{
	//	sampleGetTransform(selected_marker_ID, &selected_marker_);

	//}
	//for (Boid* boid : cows_)
	//{
	//	Cow* cow = (Cow*)boid;
	//	cow->marker_matrix_ = selected_marker_;
	//	cow->Flock(cows_, delta_time);
	//	cow->Update(delta_time);
	//}

	//sampleUpdateEnd(dat);

	///////

	markers_[targeted_marker_ID].SetTranslation(gef::Vector4(anchorX, 0, anchorZ));
	previous_marker_ = markers_[previous_marker_ID];
	selected_marker_ = markers_[selected_marker_ID];

	for (int i = 0; i < 6; i++)
	{
		marker_mesh_instances_[i].set_transform(markers_[i]);
	}
	targeted_marker_mesh_instance.set_transform(markers_[targeted_marker_ID]);
	selected_marker_mesh_instance.set_transform(markers_[selected_marker_ID]);

	for (Boid* boid : cows_)
	{
		Cow* cow = (Cow*)boid;
		if (targeted_marker_ID == selected_marker_ID)
		{
			//cow->marker_matrix_ = selected_marker_;
		}
		cow->Flock(cows_, delta_time);
		cow->Update(delta_time);
	}
}

void GameStateVITA::Initialize()
{
}
