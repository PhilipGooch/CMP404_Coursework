#pragma once
#include "Model.h"
#include <graphics/mesh_instance.h>

namespace gef
{
	class Mesh;
}

class Wolf : public Model
{
public:
	Wolf(gef::Renderer3D* renderer_3d, std::vector<gef::Mesh*> meshes);

	void Update(float delta_time);
	void Animate(float dt);
	void Render();

protected:
	gef::MeshInstance body;
	gef::MeshInstance mane;
	gef::MeshInstance head;
	gef::MeshInstance nose;
	gef::MeshInstance legs;
	gef::MeshInstance tail;
	gef::MeshInstance left_ear;
	gef::MeshInstance right_ear;

	gef::Vector4 body_dimensions;
	gef::Vector4 mane_dimensions;
	gef::Vector4 head_dimensions;
	gef::Vector4 nose_dimensions;
	gef::Vector4 ears_dimensions;
	gef::Vector4 legs_dimensions;
	gef::Vector4 tail_dimensions;

	gef::Vector4 body_offset;
	gef::Vector4 mane_offset;
	gef::Vector4 head_offset;
	gef::Vector4 nose_offset;
	gef::Vector4 left_ear_offset;
	gef::Vector4 right_ear_offset;
	gef::Vector4 front_left_leg_offset;
	gef::Vector4 front_right_leg_offset;
	gef::Vector4 back_left_leg_offset;
	gef::Vector4 back_right_leg_offset;
	gef::Vector4 tail_offset;

	gef::Vector4 leg_joint_offset;
	gef::Vector4 tail_joint_offset;

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

	float tail_angle;
};

