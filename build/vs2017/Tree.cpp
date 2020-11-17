#include "Tree.h"
#include <maths/vector4.h>

Tree::Tree(gef::Renderer3D* renderer_3d, std::vector<gef::Mesh*> meshes) :
	Model(renderer_3d, meshes)
{
	leaves.set_mesh(meshes[39]);
	wood.set_mesh(meshes[40]);
}

void Tree::Update(float delta_time)
{
	
}

void Tree::Render()
{
	PushMatrix();		// very expensive rendering... and a lot of manual data entry... only doing creating one model like this though so its easier to justify. 
		Draw(wood);
		Translate(gef::Vector4(0, 32, 0));
		Draw(wood);
		Translate(gef::Vector4(0, 32, 0));
		Draw(wood);
		Translate(gef::Vector4(-32, 32, -32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(-64, 32, 0));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(-32, 32, 32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(-32, 32, 32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(0, 32, 32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, -32));
		Draw(leaves);
		Translate(gef::Vector4(-32, 0, 0));
		Draw(leaves);
		Translate(gef::Vector4(0, 0, 32));
		Draw(leaves);
	PopMatrix();
}

void Tree::SetBlockTransform(gef::MeshInstance& mesh_instance, gef::Vector4 position)
{
	gef::Matrix44 matrix;
	matrix.SetIdentity();
	matrix.SetTranslation(position);
	mesh_instance.set_transform(matrix);
}
