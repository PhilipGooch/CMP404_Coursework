#include "Cow.h"
#include <math.h> // atan2

Cow::Cow(gef::Renderer3D& renderer_3d, PrimitiveBuilder& primitive_builder, std::vector<gef::Material*> materials) :
	Model(renderer_3d, primitive_builder, materials),
	scale(1.f),
	body_half_size(gef::Vector4(12.f, 10.f, 18.f) / scale),
	head_half_size(gef::Vector4(8.f, 8.f, 6.f) / scale),
	leg_half_size(gef::Vector4(4.f, 12.f, 4.f) / scale),
	horn_half_size(gef::Vector4(1.f, 3.f, 1.f) / scale),
	udders_half_size(gef::Vector4(4.f, 1.f, 6.f) / scale)
{
	
	body_mesh = primitive_builder_.CreateBoxMesh(body_half_size, gef::Vector4(0.f, 0.f, 0.f), &materials[0]);
	head_mesh = primitive_builder_.CreateBoxMesh(head_half_size, gef::Vector4(0.f, 0.f, 0.f), &materials[6]);
	left_leg_mesh = primitive_builder_.CreateBoxMesh(leg_half_size, gef::Vector4(0.f, 0.f, 0.f), &materials[12]);
	right_leg_mesh = primitive_builder_.CreateBoxMesh(leg_half_size, gef::Vector4(0.f, 0.f, 0.f), &materials[18]);
	horn_mesh = primitive_builder_.CreateBoxMesh(horn_half_size, gef::Vector4(0.f, 0.f, 0.f), &materials[24]);
	udders_mesh = primitive_builder_.CreateBoxMesh(udders_half_size, gef::Vector4(0.f, 0.f, 0.f), &materials[30]);

	//body_mesh = primitive_builder_.CreateBoxMesh(body_half_size);
	//head_mesh = primitive_builder_.CreateBoxMesh(head_half_size);
	//left_leg_mesh = primitive_builder_.CreateBoxMesh(leg_half_size);
	//right_leg_mesh = primitive_builder_.CreateBoxMesh(leg_half_size);
	//horn_mesh = primitive_builder_.CreateBoxMesh(horn_half_size);
	//udders_mesh = primitive_builder_.CreateBoxMesh(udders_half_size);

	body_offset = gef::Vector4(0, body_half_size.y() + leg_half_size.y() * 2, 0);
	head_offset = gef::Vector4(0, body_half_size.y() - head_half_size.y() / 2, body_half_size.z() + head_half_size.z());
	front_left_leg_offset = gef::Vector4(body_half_size.x() - leg_half_size.x(), -body_half_size.y() - leg_half_size.y(), body_half_size.z() - leg_half_size.z());
	back_left_leg_offset = gef::Vector4(body_half_size.x() - leg_half_size.x(), -body_half_size.y() - leg_half_size.y(), -body_half_size.z() + leg_half_size.z());
	front_right_leg_offset = gef::Vector4(-body_half_size.x() + leg_half_size.x(), -body_half_size.y() - leg_half_size.y(), body_half_size.z() - leg_half_size.z());
	back_right_leg_offset = gef::Vector4(-body_half_size.x() + leg_half_size.x(), -body_half_size.y() - leg_half_size.y(), -body_half_size.z() + leg_half_size.z());
	left_horn_offset = gef::Vector4(-head_half_size.x() - horn_half_size.x(), head_half_size.y() - horn_half_size.z(), horn_half_size.z());
	right_horn_offset = gef::Vector4(head_half_size.x() + horn_half_size.x(), head_half_size.y() - horn_half_size.z(), horn_half_size.z());
	udders_offset = gef::Vector4(0, -body_half_size.y() - udders_half_size.y(), -body_half_size.z() + udders_half_size.z());

	body.set_mesh(body_mesh);
	head.set_mesh(head_mesh);
	left_leg.set_mesh(left_leg_mesh);
	right_leg.set_mesh(right_leg_mesh);
	horn.set_mesh(horn_mesh);
	udders.set_mesh(udders_mesh);

	front_left_leg_direction = -1.f;
	front_right_leg_direction = 1.f;
	back_left_leg_direction = -1.f;
	back_right_leg_direction = 1.f;

	front_left_leg_angle = 0;
	front_right_leg_angle = 0;
	back_left_leg_angle = 0;
	back_right_leg_angle = 0;

	leg_rotation_speed = 3.f;
	max_leg_rotation = 3.142f / 4;

	leg_joint_offset = gef::Vector4(0.f, leg_half_size.y(), 0.f);
}

void Cow::Update(float delta_time)
{
	Animate(delta_time);
}

void Cow::Animate(float delta_time)
{
	float* leg_angles[4] { &front_left_leg_angle, &front_right_leg_angle, &back_left_leg_angle, &back_right_leg_angle };
	float* leg_directions[4] { &front_left_leg_direction, &front_right_leg_direction, &back_left_leg_direction, &back_right_leg_direction };

	for (int i = 0; i < 4; i++) {
		*leg_angles[i] += *leg_directions[i] * leg_rotation_speed * delta_time;
		if (*leg_angles[i] > max_leg_rotation) {
			*leg_angles[i] = max_leg_rotation;
			*leg_directions[i] = -*leg_directions[i];
		}
		if (*leg_angles[i] < -max_leg_rotation) {
			*leg_angles[i] = -max_leg_rotation;
			*leg_directions[i] = -*leg_directions[i];
		}
	}
}

void Cow::Render()
{
	PushMatrix();
#if VITA_MODE
		Scale(gef::Vector4(0.001f, 0.001f, 0.001f, 1.f)); // scale down to sony unit sizes
		RotateX(3.141592 / 2); // rotate to sony coordinate system
#endif
		Translate(position_);
		Translate(body_offset);
		RotateY(-atan2f(velocity_.z(), velocity_.x()) + (3.1415f / 2));	// refactor...
		Draw(body);
		PushMatrix();
			Translate(head_offset);
			Draw(head);
			PushMatrix();
				Translate(left_horn_offset);
				Draw(horn);
			PopMatrix();
			PushMatrix();
				Translate(right_horn_offset);
				Draw(horn);
			PopMatrix();
		PopMatrix();
		PushMatrix();
			Translate(front_left_leg_offset);
			Translate(leg_joint_offset);
			RotateX(front_left_leg_angle);
			Translate(-leg_joint_offset);
			Draw(left_leg);
		PopMatrix();
		PushMatrix();
			Translate(front_right_leg_offset);
			Translate(leg_joint_offset);
			RotateX(front_right_leg_angle);
			Translate(-leg_joint_offset);
			//Scale(gef::Vector4(-1.f, 1.f, 1.f));
			Draw(right_leg);
		PopMatrix();
		PushMatrix();
			Translate(back_left_leg_offset);
			Translate(leg_joint_offset);
			RotateX(back_left_leg_angle);
			Translate(-leg_joint_offset);
			Draw(left_leg);
		PopMatrix();
		PushMatrix();
			Translate(back_right_leg_offset);
			Translate(leg_joint_offset);
			RotateX(back_right_leg_angle);
			Translate(-leg_joint_offset);
			Draw(right_leg);
		PopMatrix();
		PushMatrix();
			Translate(udders_offset);
			Draw(udders);
		PopMatrix();
	PopMatrix();
}
