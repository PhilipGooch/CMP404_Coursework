#pragma once
#include "Boid.h"

#include <maths/matrix44.h>
#include <vector>

namespace gef
{
	class Renderer3D;
	class Mesh;
	class MeshInstance;
}
class Marker;

class Model : public Boid
{
public:
	Model(gef::Renderer3D* renderer_3D, std::vector<gef::Mesh*> meshes);
	
	//gef::Matrix44 local_matrix_;
	//gef::Matrix44 marker_matrix_;
	//Marker* marker_;			// <---- this could potentially be causing performance issues. pointer chasing. used multiple times each frame. could store marker_matrix_;
protected:
	gef::Renderer3D* renderer_3D_;

	std::vector<gef::Matrix44> matrix_stack_;

	void PushMatrix();
	void PopMatrix();
	void Translate(gef::Vector4 translate);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
	void Scale(gef::Vector4 scale);
	void Draw(gef::MeshInstance& mesh_instance);
};


