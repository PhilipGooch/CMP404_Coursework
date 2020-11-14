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

class Model : public Boid
{
public:
	Model(gef::Renderer3D* renderer_3D, std::vector<gef::Mesh*> meshes);

	//gef::Matrix44 GetOriginMarker() { return origin_marker_; }
	gef::Matrix44 GetCurrentMarker() { return local_matrix_; }

	//void SetOriginMarker(gef::Matrix44 origin_marker) { origin_marker_ = origin_marker; }
	void SetMarkerMatrix(gef::Matrix44 marker_matrix) { local_matrix_ = marker_matrix; }
	
	gef::Matrix44 local_matrix_;
	gef::Matrix44 marker_matrix_;
protected:
	gef::Renderer3D* renderer_3D_;

	std::vector<gef::Mesh*> meshes_;


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


