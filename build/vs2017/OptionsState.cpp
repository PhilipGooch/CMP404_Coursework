#include "OptionsState.h"

#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include <graphics/font.h>
#include <audio/audio_manager.h>
#include <input/input_manager.h>
#include <input/keyboard.h>
#include "StateMachine.h"

OptionsState::OptionsState(gef::Platform* platform,
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
	float field_of_view = 0.436332f;
	float near_plane_ = 0.01f;
	float far_plane_ = 1000.f;
	projection_matrix_ = platform_->PerspectiveProjectionFov(field_of_view, aspect_ratio, near_plane_, far_plane_);
	view_matrix_.SetIdentity();

	SetupLights();
}

bool OptionsState::HandleInput()
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
			if (keyboard->IsKeyDown(gef::Keyboard::KC_3))	
			{
				state_machine_->SetState(StateMachine::STATE::MENU);
				audio_manager_->PlaySample(0, false);
			}

		}
	}
	return true;
}

void OptionsState::Update(float delta_time)
{
	fps_ = 1.f / delta_time;
}

void OptionsState::Render()
{
	renderer_3D_->set_projection_matrix(projection_matrix_);
	renderer_3D_->set_view_matrix(view_matrix_);
	renderer_3D_->Begin(true);

	renderer_3D_->End();

	DrawHUD();
}

void OptionsState::DrawHUD()
{
	gef::Matrix44 proj_matrix2d;
	proj_matrix2d = platform_->OrthographicFrustum(0.0f, platform_->width(), 0.0f, platform_->height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	if (font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(480.f, 50.f, -0.9f), 3.5f, 0xff000000, gef::TJ_CENTRE, "OPTIONS");
		font_->RenderText(sprite_renderer_, gef::Vector4(480.f, 200.f, -0.9f), 2.0f, 0xff000000, gef::TJ_CENTRE, "3. BACK");
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
	sprite_renderer_->End();
}

void OptionsState::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(450.f, 100.0f, 250.f));
	gef::Default3DShaderData& default_shader_data = renderer_3D_->default_shader_data();
	float light = 0.5f;
	default_shader_data.set_ambient_light_colour(gef::Colour(light, light, light, 1.0f));
	//default_shader_data.AddPointLight(default_point_light);
}
