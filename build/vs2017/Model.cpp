#include "Model.h"

#include <graphics/mesh_instance.h>
#include <graphics/renderer_3d.h>
#include "Marker.h"

Model::Model(gef::Renderer3D* renderer_3D, std::vector<gef::Mesh*> meshes) :
	renderer_3D_(renderer_3D)
{
	local_matrix_.SetIdentity();
}

void Model::PushMatrix()
{
	gef::Matrix44 matrix;
	matrix.SetIdentity();
	matrix_stack_.push_back(matrix);
}

void Model::PopMatrix()
{
	matrix_stack_.pop_back();
}

void Model::Translate(gef::Vector4 translation)
{
	gef::Matrix44 matrix;
	matrix.SetIdentity();
	matrix.SetTranslation(translation);
	matrix_stack_.back() = matrix * matrix_stack_.back();
}

void Model::RotateX(float angle)
{
	gef::Matrix44 matrix;
	matrix.SetIdentity();
	matrix.RotationX(angle);
	matrix_stack_.back() = matrix * matrix_stack_.back();
}

void Model::RotateY(float angle)
{
	gef::Matrix44 matrix;
	matrix.SetIdentity();
	matrix.RotationY(angle);
	matrix_stack_.back() = matrix * matrix_stack_.back();
}

void Model::RotateZ(float angle)
{
	gef::Matrix44 matrix;
	matrix.SetIdentity();
	matrix.RotationZ(angle);
	matrix_stack_.back() = matrix * matrix_stack_.back();
}

void Model::Scale(gef::Vector4 scale)
{
	gef::Matrix44 matrix;
	matrix.SetIdentity();
	matrix.Scale(scale);
	matrix_stack_.back() = matrix * matrix_stack_.back();
}

void Model::Draw(gef::MeshInstance& mesh_instance)
{
	gef::Matrix44 matrix;
	matrix.SetIdentity();

	for (int i = matrix_stack_.size() - 1; i >= 0; i--)
	{
		matrix = matrix * matrix_stack_[i];
	}

	matrix = matrix * local_matrix_ * marker_matrix_;

	mesh_instance.set_transform(matrix);
	renderer_3D_->DrawMesh(mesh_instance);
}
