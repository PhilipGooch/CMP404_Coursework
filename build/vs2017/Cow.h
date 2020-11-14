#pragma once
#include "Model.h"

class Cow : public Model
{
public:
	Cow(gef::Renderer3D& renderer_3d, PrimitiveBuilder& primitive_builder, std::vector<gef::Material*> materials);

	void Update(float delta_time);
	void Animate(float dt);
	void Render();

protected:
	float scale;

	gef::Mesh* body_mesh;
	gef::Mesh* head_mesh;
	gef::Mesh* left_leg_mesh;
	gef::Mesh* right_leg_mesh;
	gef::Mesh* horn_mesh;
	gef::Mesh* udders_mesh;

	gef::MeshInstance body;
	gef::MeshInstance head;
	gef::MeshInstance left_leg;
	gef::MeshInstance right_leg;
	gef::MeshInstance horn;
	gef::MeshInstance udders;

	gef::Vector4 body_half_size;
	gef::Vector4 head_half_size;
	gef::Vector4 leg_half_size;
	gef::Vector4 horn_half_size;
	gef::Vector4 udders_half_size;

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

