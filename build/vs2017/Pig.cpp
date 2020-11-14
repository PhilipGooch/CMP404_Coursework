#include "Pig.h"

//body = new Box(&textures[0], 10, 8, 16, size);
//head = new Box(&textures[1], 8, 8, 8, size);
//leftArm = new Box(&textures[2], 4, 6, 4, size);
//rightArm = new Box(&textures[3], 4, 6, 4, size);
//leftLeg = new Box(&textures[4], 4, 6, 4, size);
//rightLeg = new Box(&textures[5], 4, 6, 4, size);
//nose = new Box(&textures[6], 4, 3, 1, size);

Pig::Pig(gef::Renderer3D& renderer_3d, PrimitiveBuilder& primitive_builder, std::vector<gef::Material*> materials) :
	Model(renderer_3d, primitive_builder, materials)
{
	body_half_size = gef::Vector4(10.f, 8.f, 16.f) / 2;
	head_half_size = gef::Vector4(8.f, 8.f, 8.f) / 2;
	leg_half_size = gef::Vector4(4.f, 6.f, 4.f) / 2;
	nose_half_size = gef::Vector4(4.f, 3.f, 1.f) / 2;

	gef::Mesh* body_mesh = primitive_builder_.CreateBoxMesh(body_half_size);
	gef::Mesh* head_mesh = primitive_builder_.CreateBoxMesh(head_half_size);
	gef::Mesh* leg_mesh = primitive_builder_.CreateBoxMesh(leg_half_size);
	gef::Mesh* nose_mesh = primitive_builder_.CreateBoxMesh(nose_half_size);

	body_offset = gef::Vector4(0, body_half_size.y() + leg_half_size.y() * 2, 0);
	head_offset = gef::Vector4(0, body_half_size.y() / 2, - body_half_size.z());
	front_left_leg_offset = gef::Vector4(-body_half_size.x() + leg_half_size.x(), -body_half_size.y() - leg_half_size.y(), -body_half_size.z() + leg_half_size.z());
	front_right_leg_offset = gef::Vector4(body_half_size.x() - leg_half_size.x(), -body_half_size.y() - leg_half_size.y(), -body_half_size.z() + leg_half_size.z());
	back_left_leg_offset = gef::Vector4(-body_half_size.x() + leg_half_size.x(), -body_half_size.y() - leg_half_size.y(), body_half_size.z() - leg_half_size.z());
	back_right_leg_offset = gef::Vector4(body_half_size.x() - leg_half_size.x(), -body_half_size.y() - leg_half_size.y(), body_half_size.z() - leg_half_size.z());
	nose_offset = gef::Vector4(0, -nose_half_size.y(), -head_half_size.z());

	body.set_mesh(body_mesh);
	head.set_mesh(head_mesh);
	leg.set_mesh(leg_mesh);
	nose.set_mesh(nose_mesh);
}

void Pig::update(float delta_time)
{
}

void Pig::render()
{
	PushMatrix();
		Translate(body_offset);
		Draw(body);
		PushMatrix();
			Translate(head_offset);
			Draw(head);
			PushMatrix();
				Translate(nose_offset);
				Draw(nose);
			PopMatrix();
		PopMatrix();
		PushMatrix();
			Translate(front_left_leg_offset);
			Draw(leg);
		PopMatrix();
		PushMatrix();
			Translate(front_right_leg_offset);
			Draw(leg);
		PopMatrix();
		PushMatrix();
			Translate(back_left_leg_offset);
			Draw(leg);
		PopMatrix();
		PushMatrix();
			Translate(back_right_leg_offset);
			Draw(leg);
		PopMatrix();
	PopMatrix();
}
