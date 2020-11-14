#pragma once
#include "Model.h"

class Pig : public Model
{
public:
	Pig(gef::Renderer3D& renderer_3d, PrimitiveBuilder& primitive_builder, std::vector<gef::Material*> materials);

	void update(float delta_time);
	void render();

protected:
	gef::MeshInstance body;
	gef::MeshInstance head;
	gef::MeshInstance leg;
	gef::MeshInstance nose;

	gef::Vector4 body_half_size;
	gef::Vector4 head_half_size;
	gef::Vector4 leg_half_size;
	gef::Vector4 nose_half_size;

	gef::Vector4 body_offset;
	gef::Vector4 head_offset;
	gef::Vector4 front_left_leg_offset;
	gef::Vector4 front_right_leg_offset;
	gef::Vector4 back_left_leg_offset;
	gef::Vector4 back_right_leg_offset;
	gef::Vector4 nose_offset;
};

