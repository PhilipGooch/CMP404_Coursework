#pragma once

#include <vector>

namespace gef
{
	class Platform;
	class InputManager;
	class AudioManager;
	class SpriteRenderer;
	class Renderer3D;
	class Font;
	class Mesh;
}
class StateMachine;

class State
{
public:
	State(gef::Platform* platform, 
		  gef::InputManager* input_manager, 
		  gef::AudioManager* audio_manager, 
		  gef::SpriteRenderer* sprite_renderer,
		  gef::Renderer3D* renderer_3D,
		  gef::Font* font,
		  StateMachine* state_machine,
		  std::vector<gef::Mesh*> meshes);

	// virtual functions use dynamic dispatch using a v-table.
	// v-tables map virtual functions to the correct overridden function at runtime.
	// they have some overhead as memory is needed to create the v-table and there is some extra processing time for look-up.
	// very convenient. allow for polymorphism.

	// = 0 indicates the function is a pure virtual function.
	// a class with a pure virual function makes it abstract. 
	// abstract classes can not be instantiated.
	// child classes MUST override these functions or they will also be abstract.

	void Init();
	virtual bool HandleInput() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Render() = 0;

	StateMachine* GetStateMachine() { return state_machine_; }

protected:
	gef::Platform* platform_;
	gef::InputManager* input_manager_;
	gef::AudioManager* audio_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::Renderer3D* renderer_3D_;
	gef::Font* font_;

	std::vector<gef::Mesh*> meshes_;

	float fps_;

	StateMachine* state_machine_;
};

