#include "Cow.h"
#include <math.h> // atan2

Cow::Cow(gef::Renderer3D* renderer_3d, std::vector<gef::Mesh*> meshes) :
	Model(renderer_3d, meshes)
{
	body_dimensions = gef::Vector4(12.f, 10.f, 18.f);
	head_dimensions = gef::Vector4(8.f, 8.f, 6.f);
	leg_dimensions = gef::Vector4(4.f, 12.f, 4.f);
	horn_dimensions = gef::Vector4(1.f, 3.f, 1.f);
	udders_dimensions = gef::Vector4(4.f, 1.f, 6.f);

	body_offset = gef::Vector4(0, body_dimensions.y() + leg_dimensions.y() * 2, 0);
	head_offset = gef::Vector4(0, body_dimensions.y() - head_dimensions.y() / 2, body_dimensions.z() + head_dimensions.z());
	front_left_leg_offset = gef::Vector4(body_dimensions.x() - leg_dimensions.x(), -body_dimensions.y() - leg_dimensions.y(), body_dimensions.z() - leg_dimensions.z());
	back_left_leg_offset = gef::Vector4(body_dimensions.x() - leg_dimensions.x(), -body_dimensions.y() - leg_dimensions.y(), -body_dimensions.z() + leg_dimensions.z());
	front_right_leg_offset = gef::Vector4(-body_dimensions.x() + leg_dimensions.x(), -body_dimensions.y() - leg_dimensions.y(), body_dimensions.z() - leg_dimensions.z());
	back_right_leg_offset = gef::Vector4(-body_dimensions.x() + leg_dimensions.x(), -body_dimensions.y() - leg_dimensions.y(), -body_dimensions.z() + leg_dimensions.z());
	left_horn_offset = gef::Vector4(-head_dimensions.x() - horn_dimensions.x(), head_dimensions.y() - horn_dimensions.z(), horn_dimensions.z());
	right_horn_offset = gef::Vector4(head_dimensions.x() + horn_dimensions.x(), head_dimensions.y() - horn_dimensions.z(), horn_dimensions.z());
	udders_offset = gef::Vector4(0, -body_dimensions.y() - udders_dimensions.y(), -body_dimensions.z() + udders_dimensions.z());

	body.set_mesh(meshes[0]);
	head.set_mesh(meshes[1]);
	left_leg.set_mesh(meshes[2]);
	right_leg.set_mesh(meshes[3]);
	horn.set_mesh(meshes[4]);
	udders.set_mesh(meshes[5]);

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

	leg_joint_offset = gef::Vector4(0.f, leg_dimensions.y(), 0.f);
}

void Cow::Update(float delta_time)
{
	Animate(delta_time);
}

void Cow::Animate(float delta_time)
{
	float* leg_angles[4] { &front_left_leg_angle, &front_right_leg_angle, &back_left_leg_angle, &back_right_leg_angle };
	float* leg_directions[4] { &front_left_leg_direction, &front_right_leg_direction, &back_left_leg_direction, &back_right_leg_direction };

	for (int i = 0; i < 4; i++) 
	{
		*leg_angles[i] += *leg_directions[i] * leg_rotation_speed * delta_time;
		if (*leg_angles[i] > max_leg_rotation) 
		{
			*leg_angles[i] = max_leg_rotation;
			*leg_directions[i] = -*leg_directions[i];
		}
		if (*leg_angles[i] < -max_leg_rotation) 
		{
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
		RotateY(-atan2f(velocity_.z(), velocity_.x()) + (3.1415f / 2));	// direction vector
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
