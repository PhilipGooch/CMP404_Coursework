#pragma once

#include <graphics/mesh_instance.h>
#include <maths/matrix44.h>
#include <vector>

namespace gef
{
	class Renderer3D;
	class Mesh;
	class MeshInstance;
}

class Marker
{
public:
	Marker(gef::Renderer3D* renderer_3D, std::vector<gef::Mesh*> meshes, gef::Matrix44 world_matrix, int ID);
	~Marker();

	void SetWorldMatrix(gef::Matrix44 world_matrix) { world_matrix_ = world_matrix; }
	void SetWorldMatrix(gef::Vector4 translation) { world_matrix_.SetTranslation(translation); }

	void Update();
	void Render();

	gef::Matrix44 world_matrix_;
	gef::Matrix44 local_matrix_;
	gef::MeshInstance mesh_instance_;
	int ID_;
	bool in_use_;
	bool targeted_;
	bool selected_;

	float angle_;	// only used on PC
private:
	gef::Renderer3D* renderer_3D_;

	std::vector<gef::Mesh*> meshes_;

};

