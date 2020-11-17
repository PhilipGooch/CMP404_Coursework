#include "GameState.h"

#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include <graphics/font.h>
#include "Boid.h"
#include "Cow.h"
#include "Wolf.h"
#include "Marker.h"

GameState::GameState(gef::Platform* platform, 
					 gef::InputManager* input_manager, 
					 gef::AudioManager* audio_manager, 
					 gef::SpriteRenderer* sprite_renderer,
					 gef::Renderer3D* renderer_3D,
					 gef::Font* font,
					 StateMachine* state_machine,
					 std::vector<gef::Mesh*> meshes) :
	State(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, state_machine, meshes)
{
	SetupLights();

	number_of_cows_ = 4;
	number_of_wolves_ = 4;
	number_of_markers_ = 6;

	for (int i = 0; i < number_of_cows_; i++)
	{
		cows_.push_back(new Cow(renderer_3D_, meshes));
	}

	for (int i = 0; i < number_of_wolves_; i++)
	{
		wolves_.push_back(new Wolf(renderer_3D_, meshes));
	}
}

void GameState::Render()
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

void GameState::DrawHUD()
{
	gef::Matrix44 proj_matrix2d;
	proj_matrix2d = platform_->OrthographicFrustum(0.0f, (float)platform_->width(), 0.0f, (float)platform_->height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	if (font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
	sprite_renderer_->End();
}

void GameState::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(450.f, 100.0f, 250.f));
	gef::Default3DShaderData& default_shader_data = renderer_3D_->default_shader_data();
	float light = 0.5f;
	default_shader_data.set_ambient_light_colour(gef::Colour(light, light, light, 1.0f));
	//default_shader_data.AddPointLight(default_point_light);
}

