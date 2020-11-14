#include "App.h"

//#include <system/platform.h>
#include <input/input_manager.h>
//#include <input/sony_controller_input_manager.h>
#include <audio/audio_manager.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>

#include "StateMachine.h"

App::App(gef::Platform & platform) :
	Application(platform),
	sprite_renderer_(nullptr),
	input_manager_(nullptr),
	font_(nullptr)
{
}

void App::Init()
{
	input_manager_ = gef::InputManager::Create(platform_);
	audio_manager_ = gef::AudioManager::Create();
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	font_ = new gef::Font(platform_);
	font_->Load("");

	state_machine_ = new StateMachine();
}

void App::CleanUp()
{
	delete input_manager_;
	input_manager_ = nullptr;

	delete audio_manager_;
	audio_manager_ = nullptr;

	delete sprite_renderer_;
	sprite_renderer_ = nullptr;

	delete font_;
	font_ = nullptr;
}

bool App::Update(float delta_time) 
{
	fps_ = 1.0f / delta_time;

	input_manager_->Update();

	State* state = state_machine_->GetState();
	state->HandleInput();
	state->Update(delta_time);

	return true;
}

void App::Render()
{
	State* state = state_machine_->GetState();
	state->Render();
}

