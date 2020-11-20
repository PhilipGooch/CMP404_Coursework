#include "MenuState.h"

#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include <graphics/font.h>
#include <audio/audio_manager.h>
#include <input/input_manager.h>
#include <input/keyboard.h>
#include "StateMachine.h"
#include "Camera.h"
#include "Cow.h"
#include "Wolf.h"
#include "Tree.h"
#include "primitive_builder.h"

MenuState::MenuState(gef::Platform* platform, 
					 gef::InputManager* input_manager, 
					 gef::AudioManager* audio_manager, 
					 gef::SpriteRenderer* sprite_renderer,
					 gef::Renderer3D* renderer_3D,
					 gef::Font* font, 
					 StateMachine* state_machine,
					 std::vector<gef::Mesh*> meshes) :
	State(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, state_machine, meshes)
{
	float aspect_ratio = (float)platform_->width() / (float)platform_->height();
	float field_of_view = 3.141592f / 4;
	float near_plane_ = 0.01f;
	float far_plane_ = 1000.f;
	projection_matrix_ = platform_->PerspectiveProjectionFov(field_of_view, aspect_ratio, near_plane_, far_plane_);
	view_matrix_.SetIdentity();

	SetupLights();

	cow_ = new Cow(renderer_3D, meshes);
	cow_->position_ = gef::Vector4(0.f, 0.f, 0.f);
	cow_->velocity_ = gef::Vector4(0.f, 0.f, 0.f);
	gef::Matrix44 cow_matrix;
	cow_matrix.SetIdentity();
	cow_matrix.RotationY(-0.77);
	cow_matrix.SetTranslation(gef::Vector4(-50.f, -50.f, -100.f));
	cow_->marker_matrix_ = cow_matrix;

	wolf_ = new Wolf(renderer_3D, meshes);
	wolf_->position_ = gef::Vector4(0.f, 0.f, 0.f);
	wolf_->velocity_ = gef::Vector4(0.f, 0.f, 0.f);
	gef::Matrix44 wolf_matrix;
	wolf_matrix.SetIdentity();
	wolf_matrix.RotationY(-2.37f);
	wolf_matrix.SetTranslation(gef::Vector4(50.f, -45.f, -100.f));
	wolf_->marker_matrix_ = wolf_matrix;

	gef::Matrix44 plane_matrix;
	plane_matrix.SetIdentity();
	plane_matrix.SetTranslation(gef::Vector4(0, -50.f, -1000));
	plane_mesh_instance_.set_mesh(meshes[46]);
	plane_mesh_instance_.set_transform(plane_matrix);

	tree_ = new Tree(renderer_3D, meshes);
	gef::Matrix44 tree_matrix;
	tree_matrix.SetIdentity();
	tree_matrix.SetTranslation(gef::Vector4(320, -34.f, -500));
	tree_->marker_matrix_ = tree_matrix;

	tree_2_ = new Tree(renderer_3D, meshes);
	gef::Matrix44 tree_2_matrix;
	tree_2_matrix.SetIdentity();
	tree_2_matrix.SetTranslation(gef::Vector4(-270, -34.f, -600));
	tree_2_->marker_matrix_ = tree_2_matrix;

	tree_3_ = new Tree(renderer_3D, meshes);
	gef::Matrix44 tree_3_matrix;
	tree_3_matrix.SetIdentity();
	tree_3_matrix.SetTranslation(gef::Vector4(250, -34.f, -850));
	tree_3_->marker_matrix_ = tree_3_matrix;
}

MenuState::~MenuState()
{
}

bool MenuState::HandleInput()
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
				state_machine_->SetState(StateMachine::STATE::GAME);
				audio_manager_->PlaySample(0, false);
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_2)) 			
			{
				state_machine_->SetState(StateMachine::STATE::OPTIONS);
				audio_manager_->PlaySample(0, false);
			}
		}
	}
	return true;
}

void MenuState::Update(float delta_time)
{
	fps_ = 1.f / delta_time;

	//cow_->Update(delta_time);
}

void MenuState::Render()
{
	renderer_3D_->set_projection_matrix(projection_matrix_);
	renderer_3D_->set_view_matrix(view_matrix_);
	renderer_3D_->Begin(true);

	renderer_3D_->DrawMesh(plane_mesh_instance_);
	cow_->Render();
	wolf_->Render();
	tree_->Render(false);
	tree_2_->Render(false);
	tree_3_->Render(false);

	renderer_3D_->End();

	DrawHUD();
}

void MenuState::DrawHUD()
{
	gef::Matrix44 proj_matrix2d;
	proj_matrix2d = platform_->OrthographicFrustum(0.0f, platform_->width(), 0.0f, platform_->height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	if (font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(480.f, 50.f, -0.9f), 3.5f, 0xff000000, gef::TJ_CENTRE, "TITLE");
		font_->RenderText(sprite_renderer_, gef::Vector4(480.f, 200.f, -0.9f), 2.0f, 0xff000000, gef::TJ_CENTRE, "PLAY");
		font_->RenderText(sprite_renderer_, gef::Vector4(480.f, 275.f, -0.9f), 2.0f, 0xff000000, gef::TJ_CENTRE, "OPTIONS");
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
	sprite_renderer_->End();
}

void MenuState::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(450.f, 100.0f, 250.f));
	gef::Default3DShaderData& default_shader_data = renderer_3D_->default_shader_data();
	float light = 0.5f;
	default_shader_data.set_ambient_light_colour(gef::Colour(light, light, light, 1.0f));
	//default_shader_data.AddPointLight(default_point_light);
}
