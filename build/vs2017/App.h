#pragma once

#include <system/application.h>

namespace gef
{
	class Platform;
	class InputManager;
	class AudioManager;
	class SpriteRenderer;
	class Font;
}
class StateMachine;

class App : public gef::Application
{
public:
	App(gef::Platform& platform);

	void Init();
	void CleanUp();
	bool Update(float delta_time) override;
	void Render() override;

private:

	gef::InputManager* input_manager_;
	gef::AudioManager* audio_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;

	float fps_;

	StateMachine* state_machine_;
};

