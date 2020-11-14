#ifndef _RENDER_TARGET_APP_H
#define _RENDER_TARGET_APP_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/matrix33.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include "primitive_builder.h"
#include "Pig.h"
#include "Cow.h"
#include "Camera.h"
#include "Globals.h" // DELETE

#if VITA_MODE
// Vita AR includes
#include <camera.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>
#include <platform/vita/graphics/texture_vita.h>
#endif

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class RenderTarget;
	class TextureVita;
	class InputManager;
}


class ARApp : public gef::Application
{
public:
	ARApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool HandleInput();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();

	void RenderOverlay();
	void SetupLights();
	void LoadMaterials();

	gef::Material* LoadMaterial(char* file_name);

	class gef::Renderer3D* renderer_3d_;
	PrimitiveBuilder* primitive_builder_;
	gef::InputManager* input_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Matrix44 projection_matrix_;
	gef::Matrix44 view_matrix_;
	
	std::vector<Boid*> cows;
	std::vector<gef::Material*> cow_materials_;
	gef::Mesh* table_mesh;
	gef::MeshInstance table_mesh_instance;

	float fps_;

	
	gef::Matrix44 markers_[6];
	gef::Matrix44 previous_marker_;
	gef::Matrix44 selected_marker_;
	int targeted_marker_ID;
	int selected_marker_ID;
	int previous_marker_ID;

	//gef::Mesh* predator_mesh_;
	//gef::MeshInstance predator_mesh_instance_;
	

#if VITA_MODE
	gef::Matrix44 ortho_matrix_;
	gef::Sprite camera_feed_sprite_;
	gef::TextureVita* camera_feed_texture_;
#else
	Camera* camera_;
	gef::Mesh* marker_mesh_;
	gef::MeshInstance marker_mesh_instances_[6];
	gef::Matrix44 marker_transforms_[6];
	gef::Mesh* selected_marker_mesh_;
	gef::MeshInstance selected_marker_mesh_instance;
	gef::Mesh* targeted_marker_mesh_;
	gef::MeshInstance targeted_marker_mesh_instance;
	int anchorX = 0;
	int anchorZ = 0;
#endif

};




#endif // _RENDER_TARGET_APP_H