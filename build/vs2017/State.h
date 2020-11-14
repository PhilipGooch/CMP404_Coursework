#pragma once

namespace gef
{
	class Platform;
	class InputManager;
	class SpriteRenderer;
	class AudioManager;
}

class State
{
public:
	State();

	enum TYPE
	{
		MENU,
		GAME,
		OPTIONS
	};

	// virtual functions use dynamic dispatch using a v-table.
	// v-tables map virtual functions to the correct overridden function at runtime.
	// they have some overhead as memory is needed to create the v-table and there is some extra processing time for look-up.
	// very convenient. allow for polymorphism.

	// = 0 indicates the function is a pure virtual function.
	// a class with a pure virual function makes it abstract. 
	// abstract classes can not be instantiated.
	// child classes MUST override these functions or they will also be abstract.

	virtual void HandleInput() = 0;
	virtual void Update(float delta_time) = 0;
	virtual void Render() = 0;

protected:
	gef::Platform* platform_;
	gef::InputManager* input_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::AudioManager* audio_manager_;

	TYPE type_;
};

