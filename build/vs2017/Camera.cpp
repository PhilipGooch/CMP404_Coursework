#include "Camera.h"
#include <maths/math_utils.h>
#include <system/platform.h>
#include <input/sony_controller_input_manager.h>
#include <input/keyboard.h>
#include <input/input_manager.h>
#include <math.h>

Camera::Camera(gef::Platform& platform, gef::InputManager* input_manager) :
	platform_(platform),
	input_manager_(input_manager),
	eye_(gef::Vector4(0, 00, 0)),
	look_at_(gef::Vector4(0.0f, 0.0f, 0.0f)),
	up_(gef::Vector4(0.0f, 1.0f, 0.0f)),
	camera_fov_(gef::DegToRad(25.0f)),
	near_plane_(0.01f),
	far_plane_(10000.f),
	roll_(0.f),
	yaw_(0.f),
	pitch_(-45.f),
	rotateSpeed(52.f),
	speed_(1000.f),
	active_(false)
{
}

void Camera::Init(gef::Platform& platform)
{
}

void Camera::Cleanup()
{
}

void Camera::HandleInput() {
	// read input devices
	if (input_manager_)
	{
		//input_manager_->Update();

		// controller input
		gef::SonyControllerInputManager* controller_manager = input_manager_->controller_input();
		if (controller_manager)
		{
		}

		// keyboard input
		gef::Keyboard* keyboard = input_manager_->keyboard();
		if (keyboard)
		{
			moving_forward_ = keyboard->IsKeyDown(gef::Keyboard::KC_W);
			moving_back_ = keyboard->IsKeyDown(gef::Keyboard::KC_S);
			moving_left_ = keyboard->IsKeyDown(gef::Keyboard::KC_A);
			moving_right_ = keyboard->IsKeyDown(gef::Keyboard::KC_D);
			moving_up_ = keyboard->IsKeyDown(gef::Keyboard::KC_E);
			moving_down_ = keyboard->IsKeyDown(gef::Keyboard::KC_Q);

			looking_up_ = keyboard->IsKeyDown(gef::Keyboard::KC_UP);
			looking_down_ = keyboard->IsKeyDown(gef::Keyboard::KC_DOWN);
			looking_left_ = keyboard->IsKeyDown(gef::Keyboard::KC_LEFT);
			looking_right_ = keyboard->IsKeyDown(gef::Keyboard::KC_RIGHT);
		}
	}
}

void Camera::Update(float delta_time)
{
	if (moving_forward_) eye_ += forward_ * speed_ * delta_time;
	if (moving_back_) eye_ -= forward_ * speed_ * delta_time;
	if (moving_left_) eye_ -= right_ * speed_ * delta_time;
	if (moving_right_) eye_ += right_ * speed_ * delta_time;
	if (moving_up_) eye_ += up_ * speed_ * delta_time;
	if (moving_down_) eye_ -= up_ * speed_ * delta_time;

	if (looking_up_) pitch_ += rotateSpeed * delta_time;
	if (looking_down_) pitch_ -= rotateSpeed * delta_time;
	if (looking_left_) yaw_ -= rotateSpeed * delta_time;
	if (looking_right_) yaw_ += rotateSpeed * delta_time;

	if (pitch_ > 90) pitch_ = 90;
	if (pitch_ < -90) pitch_ = -90;

	float cos_roll, cos_pitch, cos_yaw;
	float sin_roll, sin_pitch, sin_yaw;

	cos_yaw = cosf(yaw_ * 3.1415 / 180);
	cos_pitch = cosf(pitch_ * 3.1415 / 180);
	cos_roll = cosf(roll_ * 3.1415 / 180);
	sin_yaw = sinf(yaw_ * 3.1415 / 180);
	sin_pitch = sinf(pitch_ * 3.1415 / 180);
	sin_roll = sinf(roll_ * 3.1415 / 180);

	forward_.set_x(sin_yaw * cos_pitch);
	forward_.set_y(sin_pitch);
	forward_.set_z(cos_pitch * -cos_yaw);

	up_.set_x(-cos_yaw * sin_roll - sin_yaw * sin_pitch * cos_roll);
	up_.set_y(cos_pitch * cos_roll);
	up_.set_z(-sin_yaw * sin_roll - sin_pitch * cos_roll * -cos_yaw);
	look_at_ = eye_ + forward_;

	right_ = forward_.CrossProduct(up_);

	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	projection_matrix_ = platform_.PerspectiveProjectionFov(camera_fov_, aspect_ratio, near_plane_, far_plane_);
	view_matrix_.LookAt(eye_, look_at_, up_);
}
