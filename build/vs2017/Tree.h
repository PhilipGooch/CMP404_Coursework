#pragma once
#pragma once
#include "Model.h"
#include <graphics/mesh_instance.h>

namespace gef
{
	class Mesh;
}

class Tree : public Model
{
public:
	Tree(gef::Renderer3D* renderer_3d, std::vector<gef::Mesh*> meshes);

	void Update(float delta_time);
	void Render();

	void SetBlockTransform(gef::MeshInstance & mesh_instance, gef::Vector4 position);

protected:
	gef::MeshInstance leaves;
	gef::MeshInstance wood;
};


