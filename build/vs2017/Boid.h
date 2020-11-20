#pragma once
#include <maths/Vector4.h>
#include <vector>
#include <maths/matrix44.h>
#include "Globals.h" // DELETE

class Boid
{
public:
	Boid();

	void Flock(std::vector<Boid*> boids, float delta_time);
	gef::Matrix44 GetWorldMatrix();

	std::vector<Boid*>* predators_;
	std::vector<Boid*>* prey_;
	std::vector<Boid*>* trees_;

	gef::Vector4 position_;
	gef::Vector4 velocity_;

	gef::Matrix44 local_marker_matrix_;		// local matrix of the marker if parented to another marker
	gef::Matrix44 marker_matrix_;
protected:
	gef::Vector4 acceleration_;

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

	float vDistance(gef::Vector4 a, gef::Vector4 b); // these functions should be in their own library.
	float vMagnitudeSquared(gef::Vector4 vector);
	float vMagnitude(gef::Vector4 vector);
	gef::Vector4 vDivide(gef::Vector4 vector, float divisor);
	gef::Vector4 vMultiply(gef::Vector4 vector, float multiplier);
	gef::Vector4 vNormalize(gef::Vector4 vector);
	gef::Vector4 vSetMagnitude(gef::Vector4 vector, float magnitude);
	gef::Vector4 vLimit(gef::Vector4 vector, float max);
	gef::Vector4 vClamp(gef::Vector4 vector, float min, float max);
	float vMap(float value, float min_A, float max_A, float min_B, float max_B); // not vector math but should be in personal math library with vector functions.

private:
	void Edges();
	gef::Vector4 Separation(std::vector<Boid*> boids);
	gef::Vector4 Alignment(std::vector<Boid*> boids);
	gef::Vector4 Cohesion(std::vector<Boid*> boids);
	gef::Vector4 Repel();
	gef::Vector4 Attract();
};

