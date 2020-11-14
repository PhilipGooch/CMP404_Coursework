#pragma once
#include <maths/vector4.h>
#include <maths/matrix44.h>
#include "Globals.h" // DELETE

namespace gef
{
	class Platform;
	class InputManager;
}

class Camera
{
public:
	Camera(gef::Platform& platform, gef::InputManager* input_manager);

	void Init(gef::Platform& platform);
	void Cleanup();
	void HandleInput();
	void Update(float delta_time);



public:

	gef::Platform& platform_;
	gef::InputManager* input_manager_;

	float roll_;
	float yaw_;
	float pitch_;
	float rotateSpeed;
	float speed_;
	gef::Vector4 forward_;
	gef::Vector4 right_;
	gef::Vector4 eye_;
	gef::Vector4 look_at_;
	gef::Vector4 up_;
	float camera_fov_;
	float near_plane_;
	float far_plane_;

	bool moving_forward, moving_back, moving_up, moving_down, moving_left, moving_right;
	bool looking_up, looking_down, looking_left, looking_right;

	gef::Matrix44 projection_matrix_;
	gef::Matrix44 view_matrix_;
};

