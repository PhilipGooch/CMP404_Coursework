#pragma once
#include "Model.h"
#include <graphics/mesh_instance.h>

namespace gef
{
	class Mesh;
}

class Cow : public Model
{
public:
	Cow(gef::Renderer3D* renderer_3d, std::vector<gef::Mesh*> meshes);

	void Update(float delta_time);
	void Animate(float dt);
	void Render();

protected:
	gef::MeshInstance body;
	gef::MeshInstance head;
	gef::MeshInstance left_leg;
	gef::MeshInstance right_leg;
	gef::MeshInstance horn;
	gef::MeshInstance udders;

	gef::Vector4 body_dimensions;
	gef::Vector4 head_dimensions;
	gef::Vector4 leg_dimensions;
	gef::Vector4 horn_dimensions;
	gef::Vector4 udders_dimensions;

	gef::Vector4 body_offset;
	gef::Vector4 head_offset;
	gef::Vector4 front_left_leg_offset;
	gef::Vector4 front_right_leg_offset;
	gef::Vector4 back_left_leg_offset;
	gef::Vector4 back_right_leg_offset;
	gef::Vector4 left_horn_offset;
	gef::Vector4 right_horn_offset;
	gef::Vector4 udders_offset;

	gef::Vector4 leg_joint_offset;

	float front_left_leg_direction;
	float front_right_leg_direction;
	float back_left_leg_direction;
	float back_right_leg_direction;

	float front_left_leg_angle;
	float front_right_leg_angle;
	float back_left_leg_angle;
	float back_right_leg_angle;

	float leg_rotation_speed;
	float max_leg_rotation;
};

