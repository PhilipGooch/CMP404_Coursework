#include "Wolf.h"

#include <math.h>

Wolf::Wolf(gef::Renderer3D* renderer_3d, std::vector<gef::Mesh*> meshes) :
	Model(renderer_3d, meshes)
{
	body_dimensions = gef::Vector4(6.f, 6.f, 9.f);
	mane_dimensions = gef::Vector4(8.f, 7.f, 6.f);
	head_dimensions = gef::Vector4(6.f, 6.f, 4.f);
	nose_dimensions = gef::Vector4(3.f, 3.f, 3.f);
	ears_dimensions = gef::Vector4(2.f, 2.f, 1.f);
	legs_dimensions = gef::Vector4(2.f, 8.f, 2.f);
	tail_dimensions = gef::Vector4(2.f, 8.f, 2.f);

	body_offset = gef::Vector4(0.f, body_dimensions.y() + legs_dimensions.y() * 2.f, 0);
	mane_offset = gef::Vector4(0.f, 1.f, body_dimensions.z() + mane_dimensions.z());
	head_offset = gef::Vector4(0.f, -1.f, mane_dimensions.z() + head_dimensions.z() - head_dimensions.z() / 2.f);
	nose_offset = gef::Vector4(0.f, -3.f, head_dimensions.z() + nose_dimensions.z());
	left_ear_offset = gef::Vector4(head_dimensions.x() / 2.f + ears_dimensions.x() / 2.f, head_dimensions.y() + ears_dimensions.y(), -ears_dimensions.z());
	right_ear_offset = gef::Vector4(-head_dimensions.x() / 2.f - ears_dimensions.x() / 2.f, head_dimensions.y() + ears_dimensions.y(), -ears_dimensions.z());
	front_left_leg_offset = gef::Vector4(body_dimensions.x() - legs_dimensions.x(), -body_dimensions.y() - legs_dimensions.y(), body_dimensions.z() + mane_dimensions.z() * 2.f - legs_dimensions.z());
	front_right_leg_offset = gef::Vector4(-body_dimensions.x() + legs_dimensions.x(), -body_dimensions.y() - legs_dimensions.y(), body_dimensions.z() + mane_dimensions.z() * 2.f - legs_dimensions.z());
	back_left_leg_offset = gef::Vector4(body_dimensions.x() - legs_dimensions.x(), -body_dimensions.y() - legs_dimensions.y(), -body_dimensions.z() + legs_dimensions.z());
	back_right_leg_offset = gef::Vector4(-body_dimensions.x() + legs_dimensions.x(), -body_dimensions.y() - legs_dimensions.y(), -body_dimensions.z() + legs_dimensions.z());
	tail_offset = gef::Vector4(0.f, -tail_dimensions.y(), -body_dimensions.z());

	body.set_mesh(meshes[6]);
	mane.set_mesh(meshes[7]);
	head.set_mesh(meshes[8]);
	nose.set_mesh(meshes[9]);
	legs.set_mesh(meshes[10]);
	tail.set_mesh(meshes[11]);
	left_ear.set_mesh(meshes[12]);
	right_ear.set_mesh(meshes[13]);

	front_left_leg_direction = -1.f;
	front_right_leg_direction = 1.f;
	back_left_leg_direction = -1.f;
	back_right_leg_direction = 1.f;

	front_left_leg_angle  = 0.f;
	front_right_leg_angle = 0.f;
	back_left_leg_angle   = 0.f;
	back_right_leg_angle  = 0.f;

	leg_rotation_speed = 3.f;
	max_leg_rotation = 3.142f / 4;

	leg_joint_offset = gef::Vector4(0.f, legs_dimensions.y(), 0.f);
	tail_joint_offset = gef::Vector4(0.f, tail_dimensions.y(), 0.f);


	tail_angle = 3.141592f / 8.f * 3.f;
}

void Wolf::Update(float delta_time)
{
	Animate(delta_time);
}

void Wolf::Animate(float delta_time)
{
	leg_rotation_speed = vMap(vMagnitudeSquared(velocity_), 0, max_speed_ * max_speed_, 0.8f, 3.9f);
	max_leg_rotation = vMap(vMagnitudeSquared(velocity_), 0, max_speed_ * max_speed_, 0.3f, 0.9f);

	float* leg_angles[4]{ &front_left_leg_angle, &front_right_leg_angle, &back_left_leg_angle, &back_right_leg_angle };
	float* leg_directions[4]{ &front_left_leg_direction, &front_right_leg_direction, &back_left_leg_direction, &back_right_leg_direction };

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

void Wolf::Render()
{
	PushMatrix();
	{
#if VITA_MODE
		Scale(gef::Vector4(0.001f, 0.001f, 0.001f, 1.f)); // scale down to sony unit sizes
		RotateX(3.141592 / 2); // rotate to sony coordinate system
#endif
		Translate(position_);
		Translate(body_offset);
		RotateY(-atan2f(velocity_.z(), velocity_.x()) + (3.1415f / 2));	// direction vector
		Draw(body);
		PushMatrix();
		{
			Translate(mane_offset);
			Draw(mane);
			PushMatrix();
			{
				Translate(head_offset);
				Draw(head);
				PushMatrix();
				{
					Translate(nose_offset);
					Draw(nose);
				}
				PopMatrix();
				PushMatrix();
				{
					Translate(left_ear_offset);
					Draw(left_ear);
				}
				PopMatrix();
				PushMatrix();
				{
					Translate(right_ear_offset);
					Draw(right_ear);
				}
				PopMatrix();
			}
			PopMatrix();
		}
		PopMatrix();
		PushMatrix();
		{
			Translate(front_left_leg_offset);
			Translate(leg_joint_offset);
			RotateX(front_left_leg_angle);
			Translate(-leg_joint_offset);
			Draw(legs);
		}
		PopMatrix();
		PushMatrix();
		{
			Translate(front_right_leg_offset);
			Translate(leg_joint_offset);
			RotateX(front_right_leg_angle);
			Translate(-leg_joint_offset);
			Draw(legs);
		}
		PopMatrix();
		PushMatrix();
		{
			Translate(back_left_leg_offset);
			Translate(leg_joint_offset);
			RotateX(back_left_leg_angle);
			Translate(-leg_joint_offset);
			Draw(legs);
		}
		PopMatrix();
		PushMatrix();
		{
			Translate(back_right_leg_offset);
			Translate(leg_joint_offset);
			RotateX(back_right_leg_angle);
			Translate(-leg_joint_offset);
			Draw(legs);
		}
		PopMatrix();
		PushMatrix();
		{
			Translate(tail_offset);
			Translate(tail_joint_offset);
			RotateX(tail_angle);
			Translate(-tail_joint_offset);
			Draw(tail);
		}
		PopMatrix();
	}
	PopMatrix();
}
