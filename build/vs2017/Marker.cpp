#include "Marker.h"
#include <graphics/renderer_3d.h>

Marker::Marker(gef::Renderer3D * renderer_3D, std::vector<gef::Mesh*> meshes, gef::Matrix44 world_matrix, int ID) :
	renderer_3D_(renderer_3D),
	meshes_(meshes),
	world_matrix_(world_matrix),
	ID_(ID),
	in_use_(false),
	targeted_(false),
	selected_(false),
	angle_(0.f)
{
	local_matrix_.SetIdentity();
	Update();
}

Marker::~Marker()
{
}

void Marker::Update()
{
	if (selected_)
	{
		mesh_instance_.set_mesh(meshes_[12 + ID_]);
	}
	else if (targeted_)
	{
		mesh_instance_.set_mesh(meshes_[18 + ID_]);
	}
	else
	{
		mesh_instance_.set_mesh(meshes_[6 + ID_]);
	}
	mesh_instance_.set_transform(local_matrix_ * world_matrix_);

	// VITA

	/*AppData* dat = sampleUpdateBegin();

	smartUpdate(dat->currentImage);

	for (int i = 0; i < 6; i++)
	{
		if (sampleIsMarkerFound(i))
		{

		}
	}*/
}

void Marker::Render()
{
	renderer_3D_->DrawMesh(mesh_instance_);
}
