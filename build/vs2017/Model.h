#pragma once
#include <maths/matrix44.h>
#include <vector>
#include <graphics/mesh.h>
#include <graphics/mesh_instance.h>
#include "primitive_builder.h"
#include <graphics/renderer_3d.h>
#include "Boid.h"

class Model : public Boid
{
public:
	Model(gef::Renderer3D& renderer_3d, PrimitiveBuilder& primitive_builder, std::vector<gef::Material*> materials);

	//gef::Matrix44 GetOriginMarker() { return origin_marker_; }
	gef::Matrix44 GetCurrentMarker() { return local_matrix_; }

	//void SetOriginMarker(gef::Matrix44 origin_marker) { origin_marker_ = origin_marker; }
	void SetMarkerMatrix(gef::Matrix44 marker_matrix) { local_matrix_ = marker_matrix; }
	
	gef::Matrix44 local_matrix_;
	gef::Matrix44 marker_matrix_;
protected:
	gef::Renderer3D& renderer_3d_;
	PrimitiveBuilder& primitive_builder_;


	std::vector<gef::Matrix44> matrix_stack_;

	std::vector<gef::Material*> materials_;

	void PushMatrix();
	void PopMatrix();
	void Translate(gef::Vector4 translate);
	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);
	void Scale(gef::Vector4 scale);
	void Draw(gef::MeshInstance& mesh_instance);

	

};


