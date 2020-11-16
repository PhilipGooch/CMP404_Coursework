#include "Marker.h"
#include <graphics/renderer_3d.h>

Marker::Marker(gef::Renderer3D * renderer_3D, std::vector<gef::Mesh*> meshes, gef::Matrix44 world_matrix, int ID) :
	renderer_3D_(renderer_3D),
	meshes_(meshes),
	world_matrix_(world_matrix),
	ID_(ID),
	//occupied_(false),
	child_(CHILD::NONE),
	boids_(nullptr),
	targeted_(false),
	//selected_(false),
	angle_(0.f)
{
	Update();
}

Marker::~Marker()
{
}

void Marker::Update()
{
	
	if (child_ == CHILD::COW)
	{
		mesh_instance_.set_mesh(meshes_[20 + ID_]);
	}
	else if (child_ == CHILD::WOLF)
	{
		mesh_instance_.set_mesh(meshes_[32 + ID_]);
	}
	else if (targeted_)
	{
		mesh_instance_.set_mesh(meshes_[26 + ID_]);
	}
	else
	{
		mesh_instance_.set_mesh(meshes_[14 + ID_]);
	}
	mesh_instance_.set_transform(world_matrix_);
}

void Marker::Render()
{
	renderer_3D_->DrawMesh(mesh_instance_);
}
