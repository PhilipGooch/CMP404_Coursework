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
#if VITA_MODE
	gef::Matrix44 rotate;
	rotate.SetIdentity();
	rotate.RotationX(3.141592 / 2);

	gef::Matrix44 scale;
	scale.SetIdentity();
	scale.Scale(gef::Vector4(0.001f, 0.001f, 0.001f, 1.f));

	mesh_instance_.set_transform(rotate * scale * mesh_instance_.transform());
#endif
	renderer_3D_->DrawMesh(mesh_instance_);
}
