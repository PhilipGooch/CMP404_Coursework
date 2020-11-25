#include "Boid.h"
#include <math.h>

Boid::Boid() :
	perception_(100.f),
	max_force_(50.f),
	max_speed_(100.f),
	min_speed_(30.f),
	separation_weight_(3.f),
	alignment_weight_(0.7f),
	cohesion_weight_(1.5f),
	avoidance_weight_(1.f),
	flee_weight_(5.f),
	flee_radius_(100.f),
	predators_(nullptr),
	prey_(nullptr),
	trees_(nullptr)
{
	local_marker_matrix_.SetIdentity();
	marker_matrix_.SetIdentity();

	float x = (float)(rand() % ENVIRONMENT_HALF_WIDTH * 2) - (ENVIRONMENT_HALF_WIDTH);
	float z = (float)(rand() % ENVIRONMENT_HALF_DEPTH * 2) - (ENVIRONMENT_HALF_DEPTH);
	float a = (float)(rand() % 360);
	float vx = cosf(a * 3.1415f / 180.f) * max_speed_;
	float vz = sinf(a * 3.1415f / 180.f) * max_speed_;
	float ax = 0.f;
	float az = 0.f;
	position_ = gef::Vector4(x, 0.f, z);
	velocity_ = gef::Vector4(0, 0.f, 0);
	acceleration_ = gef::Vector4(ax, 0.f, az);

}

void Boid::Flock(std::vector<Boid*> boids, float delta_time)
{
	acceleration_ = gef::Vector4(0.f, 0.f, 0.f);

	//Edges();

	//acceleration_ += Separation(boids) * separation_weight_;
	//acceleration_ += Alignment(boids) * alignment_weight_;
	//acceleration_ += Cohesion(boids) * cohesion_weight_;
	//acceleration_ += Repel() * flee_weight_;
	//cceleration_ += Attract() * flee_weight_;
	acceleration_ += Flee(boids) * cohesion_weight_;
		
	velocity_ += acceleration_ * delta_time; 
	//velocity_ = vLimit(velocity_, m_max_speed);
	//velocity_ = vClamp(velocity_, min_speed_, max_speed_);
		
	position_ += velocity_ * delta_time;

}

gef::Vector4 Boid::Flee(std::vector<Boid*> boids)
{
	//// working
	//if (!trees_) return gef::Vector4(0.f, 0.f, 0.f);
	//
	//gef::Matrix44 inverse_marker_matrix_;
	//inverse_marker_matrix_.AffineInverse(marker_matrix_);
	//
	//gef::Vector4 tree_local_position = (trees_->front()->marker_matrix_ * inverse_marker_matrix_).GetTranslation();
	//tree_local_position.set_z(-tree_local_position.y());
	//tree_local_position.set_y(0);
	//tree_local_position *= 1000.f;
	//
	//gef::Vector4 steering = gef::Vector4(0.f, 0.f, 0.f);
	//gef::Vector4 desired = position_ - tree_local_position;
	//float distance = vDistance(position_, tree_local_position);
	//if (distance < flee_radius_)
	//{
	//	desired = vSetMagnitude(desired, max_speed_);
	//	steering = desired - velocity_;
	//	steering = vLimit(steering, max_force_);
	//}
	//return steering;



	if (!trees_) return gef::Vector4(0.f, 0.f, 0.f);

	gef::Matrix44 inverse_marker_matrix_;
	inverse_marker_matrix_.AffineInverse(marker_matrix_);

	gef::Matrix44 tree_local_transform;
	tree_local_transform.SetIdentity();
	tree_local_transform.SetTranslation(trees_->front()->position_);

	tree_local_transform = tree_local_transform * trees_->front()->marker_matrix_ * inverse_marker_matrix_;

	gef::Vector4 tree_marker_local_position = tree_local_transform.GetTranslation();

	tree_marker_local_position.set_z(-tree_marker_local_position.y());
	tree_marker_local_position.set_y(0);
	tree_marker_local_position *= 1000.f;

	gef::Vector4 steering = gef::Vector4(0.f, 0.f, 0.f);
	gef::Vector4 desired = position_ - tree_marker_local_position;
	float distance = vDistance(position_, tree_marker_local_position);
	if (distance < flee_radius_)
	{
		desired = vSetMagnitude(desired, max_speed_);
		steering = desired - velocity_;
		steering = vLimit(steering, max_force_);
	}
	return steering;
	
}

void Boid::SetPredatorLocalTransform(gef::Vector4 predator)
{
	// converting from sony coordinates to normal...
	// setting elevation to 0 as cows are in 2D space so not interested in elevation component.
	predator.set_z(-predator.y());
	predator.set_y(0);
	predator *= 1000.f;
	predator_ = predator;
}

void Boid::Edges()
{
	if (position_.x() < -ENVIRONMENT_HALF_WIDTH + 100)
	{
		gef::Vector4 steering = gef::Vector4(max_speed_, 0.f, 0.f);
		steering -= velocity_;
		steering = vLimit(steering, max_force_ * avoidance_weight_);
		acceleration_ += steering;
	}
	if (position_.x() > ENVIRONMENT_HALF_WIDTH - 100)
	{
		gef::Vector4 steering = gef::Vector4(-max_speed_, 0.f, 0.f);
		steering -= velocity_;
		steering = vLimit(steering, max_force_ * avoidance_weight_);
		acceleration_ += steering;
	}
	if (position_.z() < -ENVIRONMENT_HALF_DEPTH + 100)
	{
		gef::Vector4 steering = gef::Vector4(0.f, 0.f, max_speed_);
		steering -= velocity_;
		steering = vLimit(steering, max_force_ * avoidance_weight_);
		acceleration_ += steering;
	}
	if (position_.z() > ENVIRONMENT_HALF_DEPTH - 100)
	{
		gef::Vector4 steering = gef::Vector4(0.f, 0.f, -max_speed_);
		steering -= velocity_;
		steering = vLimit(steering, max_force_ * avoidance_weight_);
		acceleration_ += steering;
	}
}

gef::Vector4 Boid::Separation(std::vector<Boid*> boids)
{
	gef::Vector4 steering = gef::Vector4(0.f, 0.f, 0.f);
	int count = 0;
	for (Boid* boid : boids)
	{
		if (boid == this) continue;

		float distance = vDistance(position_, boid->position_); // why do i have access to boid->position_ ? it is protected. 
		if (distance < perception_)
		{
			gef::Vector4 difference = position_ - boid->position_;
			difference = vDivide(difference, pow(distance, 2));
			steering += difference;
			count++;
		}
	}
	if (count > 0)
	{
		steering = vDivide(steering, count);
		steering = vSetMagnitude(steering, max_speed_);
		steering -= velocity_;
		steering = vLimit(steering, max_force_);
	}

	return steering;
}

gef::Vector4 Boid::Alignment(std::vector<Boid*> boids)
{
	gef::Vector4 steering = gef::Vector4(0.f, 0.f, 0.f);
	int count = 0;
	for (Boid* boid : boids)
	{
		if (boid == this) continue;

		float distance = vDistance(position_, boid->position_); 
		if (distance < perception_)
		{
			steering += boid->velocity_;
			count++;
		}
	}
	if (count > 0)
	{
		steering = vDivide(steering, count);
		steering = vSetMagnitude(steering, max_speed_);
		steering -= velocity_;
		steering = vLimit(steering, max_force_);
	}

	return steering;

}

gef::Vector4 Boid::Cohesion(std::vector<Boid*> boids)
{
	gef::Vector4 steering = gef::Vector4(0.f, 0.f, 0.f);
	int count = 0;
	for (Boid* boid : boids)
	{
		if (boid == this) continue;

		float distance = vDistance(position_, boid->position_); // why do i have access to boid->position_ ? it is protected. 
		if (distance < perception_)
		{
			steering += boid->position_;
			count++;
		}
	}
	if (count > 0)
	{
		steering = vDivide(steering, count);
		steering -= position_;
		steering = vSetMagnitude(steering, max_speed_);
		steering -= velocity_;
		steering = vLimit(steering, max_force_);
	}

	return steering;
}

gef::Vector4 Boid::Repel()
{
	if (!predators_ && !trees_) return gef::Vector4(0.f, 0.f, 0.f);

	gef::Matrix44 inverse_marker_matrix_;
	inverse_marker_matrix_.AffineInverse(marker_matrix_);

	gef::Vector4 steering = gef::Vector4(0.f, 0.f, 0.f);
	gef::Vector4 desired = gef::Vector4(0.f, 0.f, 0.f);
	int count = 0;
	if (predators_)
	{
		for (Boid* predator : *predators_)
		{
			gef::Vector4 predator_local_position = (predator->GetWorldMatrix() * inverse_marker_matrix_).GetTranslation();
			predator_local_position.set_z(-predator_local_position.y());
			predator_local_position.set_y(0);
			predator_local_position *= 1000.f;
			float distance = vDistance(position_, predator_local_position);
			if (distance < flee_radius_)
			{
				desired += position_ - predator_local_position;
				count++;
			}
		}
	}
	for (Boid* tree : *trees_)
	{
		gef::Vector4 tree_local_position = (tree->GetWorldMatrix() * inverse_marker_matrix_).GetTranslation();
		float distance = vDistance(position_, tree_local_position);
		if (distance < flee_radius_)
		{
			desired += position_ - tree_local_position;
			count++;
		}
	}
	if (count > 0)
	{
		desired /= count;
		desired = vSetMagnitude(desired, max_speed_);
		steering = desired - velocity_;
		steering = vLimit(steering, max_force_);
	}

	return steering;
}

gef::Vector4 Boid::Attract()
{
	if (!prey_) return gef::Vector4(0.f, 0.f, 0.f);

	gef::Matrix44 inverse_marker_matrix_;
	inverse_marker_matrix_.AffineInverse(marker_matrix_);

	gef::Vector4 steering = gef::Vector4(0.f, 0.f, 0.f);
	gef::Vector4 desired = gef::Vector4(0.f, 0.f, 0.f);
	int count = 0;
	for (Boid* prey : *prey_)
	{
		gef::Vector4 prey_local_position = (prey->GetWorldMatrix() * inverse_marker_matrix_).GetTranslation();

		float distance = vDistance(position_, prey_local_position);

		if (distance < flee_radius_)
		{
			desired += prey_local_position - position_;
			count++;
		}

	}
	if (count > 0)
	{
		desired /= count;
		desired = vSetMagnitude(desired, max_speed_);
		steering = desired - velocity_;
		steering = vLimit(steering, max_force_);
	}

	return steering;
}

gef::Matrix44 Boid::GetWorldMatrix()
{
	gef::Matrix44 boid_matrix_;
	boid_matrix_.SetIdentity();
	boid_matrix_.SetTranslation(position_);
	return marker_matrix_ * local_marker_matrix_ * boid_matrix_;
}

// vector functions
///////////////////////////////////////

float Boid::vDistance(gef::Vector4 a, gef::Vector4 b)
{
	return sqrt(pow(b.x() - a.x(), 2) + pow(b.y() - a.y(), 2) + pow(b.z() - a.z(), 2));
}

float Boid::vMagnitudeSquared(gef::Vector4 vector)
{
	return pow(vector.x(), 2) + pow(vector.y(), 2) + pow(vector.z(), 2);
}

float Boid::vMagnitude(gef::Vector4 vector)
{
	return sqrt(pow(vector.x(), 2) + pow(vector.y(), 2) + pow(vector.z(), 2));
}

gef::Vector4 Boid::vDivide(gef::Vector4 vector, float divisor)
{
	vector /= divisor;
	return vector;
}

gef::Vector4 Boid::vMultiply(gef::Vector4 vector, float multiplier)
{
	vector *= multiplier;
	return vector;
}

gef::Vector4 Boid::vNormalize(gef::Vector4 vector)
{
	float length = vMagnitude(vector);
	if (length != 0)
	{
		vector = vMultiply(vector, 1.f / length);
	}
	return vector;
}

gef::Vector4 Boid::vSetMagnitude(gef::Vector4 vector, float magnitude)
{
	vector = vNormalize(vector);
	vector = vMultiply(vector, magnitude);
	return vector;
}

gef::Vector4 Boid::vLimit(gef::Vector4 vector, float max)
{
	float magnitude_squared = vMagnitudeSquared(vector);
	if (magnitude_squared > pow(max, 2))
	{
		vector = vDivide(vector, sqrt(magnitude_squared));
		vector = vMultiply(vector, max);
	}
	return vector;
}

gef::Vector4 Boid::vClamp(gef::Vector4 vector, float min, float max)
{
	float magnitude_squared = vMagnitudeSquared(vector);
	if (magnitude_squared < pow(min, 2))
	{
		vector = vDivide(vector, sqrt(magnitude_squared));
		vector = vMultiply(vector, min);
	}
	else if (magnitude_squared > pow(max, 2))
	{
		vector = vDivide(vector, sqrt(magnitude_squared));
		vector = vMultiply(vector, max);
	}
	return vector;
}

float Boid::vMap(float value, float min_A, float max_A, float min_B, float max_B) {
	return min_B + (max_B - min_B) * ((value - min_A) / (max_A - min_A));
}