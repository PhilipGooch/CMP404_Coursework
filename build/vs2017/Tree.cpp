#include "Tree.h"
#include <maths/vector4.h>

Tree::Tree(gef::Renderer3D* renderer_3d, std::vector<gef::Mesh*> meshes) :
	Model(renderer_3d, meshes)
{
	leaves_.set_mesh(meshes[14]);
	wood_.set_mesh(meshes[15]);

	leaves_dimensions_ = gef::Vector4(16.f * 3, 16.f * 5, 16.f * 3);
	wood_dimensions_ = gef::Vector4(16.f, 16.f * 3, 16.f * 0);

	leaves_offset_ = gef::Vector4(0.f, wood_dimensions_.y() + leaves_dimensions_.y(), 0.f);
	wood_offset_ = gef::Vector4(0.f, wood_dimensions_.y(), 0.f);
}

void Tree::Update(float delta_time)
{
	
}

void Tree::Render(bool game_state)
{
	PushMatrix();	
#if VITA_MODE
		Scale(gef::Vector4(0.001f, 0.001f, 0.001f, 1.f)); // scale down to sony unit sizes
		RotateX(3.141592 / 2); // rotate to sony coordinate system
#endif
		if (game_state)
		{
			Translate(position_);
		}
		Translate(wood_offset_);
		Draw(wood_);
		PushMatrix();
			Translate(leaves_offset_);
			Draw(leaves_);
			RotateX(3.1415f / 2);
			Draw(leaves_);
			RotateZ(3.1415f / 2);
			Draw(leaves_);
		PopMatrix();
	PopMatrix();
}

void Tree::SetBlockTransform(gef::MeshInstance& mesh_instance, gef::Vector4 position)
{
	gef::Matrix44 matrix;
	matrix.SetIdentity();
	matrix.SetTranslation(position);
	mesh_instance.set_transform(matrix);
}
