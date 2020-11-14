#pragma once
#include <maths/Vector4.h>
#include <vector>
#include "Globals.h" // DELETE

class Boid
{
public:
	Boid();

	void Flock(std::vector<Boid*> boids, float delta_time);
	gef::Vector4 Flee(std::vector<Boid*> boids);
	void SetPredatorLocalTransform(gef::Vector4 predator);

	gef::Vector4 predator_;
protected:
	gef::Vector4 position_;
	gef::Vector4 velocity_;
	gef::Vector4 acceleration_;

private:
	float perception_;
	float max_force_;
	float max_speed_;
	float min_speed_;
	float separation_weight_;
	float alignment_weight_;
	float cohesion_weight_;
	float avoidance_weight_;
	float flee_weight_;
	float flee_radius_;

	
	void Edges();
	gef::Vector4 Separation(std::vector<Boid*> boids);
	gef::Vector4 Alignment(std::vector<Boid*> boids);
	gef::Vector4 Cohesion(std::vector<Boid*> boids);

	float vDistance(gef::Vector4 a, gef::Vector4 b);
	float vMagnitudeSquared(gef::Vector4 vector);
	float vMagnitude(gef::Vector4 vector);
	gef::Vector4 vDivide(gef::Vector4 vector, float divisor);
	gef::Vector4 vMultiply(gef::Vector4 vector, float multiplier);
	gef::Vector4 vNormalize(gef::Vector4 vector);
	gef::Vector4 vSetMagnitude(gef::Vector4 vector, float magnitude);
	gef::Vector4 vLimit(gef::Vector4 vector, float max);
	gef::Vector4 vClamp(gef::Vector4 vector, float min, float max);
};

