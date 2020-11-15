#include "App.h"

//#include <system/platform.h>
#include <audio/audio_manager.h>
#include <input/input_manager.h>
//#include <input/sony_controller_input_manager.h>
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include <graphics/font.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/texture.h>
#include <graphics/material.h>
#include <time.h>

#include "primitive_builder.h"
#include "StateMachine.h"

App::App(gef::Platform & platform) :
	Application(platform),
	sprite_renderer_(nullptr),
	input_manager_(nullptr),
	font_(nullptr)
{
	
}

void App::Init()
{
	srand(time(NULL));

	input_manager_ = gef::InputManager::Create(platform_);
	audio_manager_ = gef::AudioManager::Create();
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3D_ = gef::Renderer3D::Create(platform_);

	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");

	primitive_builder_ = new PrimitiveBuilder(platform_);

	LoadMaterials();
	CreateMeshes();

	state_machine_ = new StateMachine(&platform_, input_manager_, audio_manager_, sprite_renderer_, renderer_3D_, font_, meshes_);
}

void App::CleanUp()
{
	delete input_manager_;
	input_manager_ = nullptr;

	delete audio_manager_;
	audio_manager_ = nullptr;

	delete sprite_renderer_;
	sprite_renderer_ = nullptr;

	delete font_;
	font_ = nullptr;

	for (gef::Material* material : materials_)
	{
		delete material;
	}

	for (gef::Mesh* mesh : meshes_)
	{
		delete mesh;
	}
}

bool App::Update(float delta_time) 
{
	fps_ = 1.0f / delta_time;

	input_manager_->Update();

	State* state = state_machine_->GetState();
	if (!state->HandleInput()) return false;
	state->Update(delta_time);

	return true;
}

void App::Render()
{
	State* state = state_machine_->GetState();
	state->Render();
}

gef::Material* App::LoadMaterial(char* file_name)
{
	gef::ImageData image_data;
	gef::PNGLoader png_loader;
	png_loader.Load(file_name, platform_, image_data);
	gef::Texture* texture = gef::Texture::Create(platform_, image_data);   // calling a static function? different for vita and d3d. 
	gef::Material* material = new gef::Material();
	material->set_texture(texture);
	return material;
}

void App::LoadMaterials()
{
	// COW
	//////////////////////////////////////////////////////////////////////////////////

	materials_.push_back(LoadMaterial("textures/cow/body/front.png"));			// 0 
	materials_.push_back(LoadMaterial("textures/cow/body/back.png"));			// 1
	materials_.push_back(LoadMaterial("textures/cow/body/left.png"));			// 2
	materials_.push_back(LoadMaterial("textures/cow/body/right.png"));			// 3
	materials_.push_back(LoadMaterial("textures/cow/body/top.png"));			// 4
	materials_.push_back(LoadMaterial("textures/cow/body/bottom.png"));			// 5
	materials_.push_back(LoadMaterial("textures/cow/head/front.png"));			// 6
	materials_.push_back(LoadMaterial("textures/cow/head/back.png"));			// 7
	materials_.push_back(LoadMaterial("textures/cow/head/left.png"));			// 8
	materials_.push_back(LoadMaterial("textures/cow/head/right.png"));			// 9
	materials_.push_back(LoadMaterial("textures/cow/head/top.png"));			// 10
	materials_.push_back(LoadMaterial("textures/cow/head/bottom.png"));			// 11
	materials_.push_back(LoadMaterial("textures/cow/left_leg/front.png"));		// 12
	materials_.push_back(LoadMaterial("textures/cow/left_leg/back.png"));		// 13
	materials_.push_back(LoadMaterial("textures/cow/left_leg/left.png"));		// 14
	materials_.push_back(LoadMaterial("textures/cow/left_leg/right.png"));		// 15
	materials_.push_back(LoadMaterial("textures/cow/left_leg/top.png"));		// 16
	materials_.push_back(LoadMaterial("textures/cow/left_leg/bottom.png"));		// 17
	materials_.push_back(LoadMaterial("textures/cow/right_leg/front.png"));		// 18
	materials_.push_back(LoadMaterial("textures/cow/right_leg/back.png"));		// 19
	materials_.push_back(LoadMaterial("textures/cow/right_leg/left.png"));		// 20
	materials_.push_back(LoadMaterial("textures/cow/right_leg/right.png"));		// 21
	materials_.push_back(LoadMaterial("textures/cow/right_leg/top.png"));		// 22
	materials_.push_back(LoadMaterial("textures/cow/right_leg/bottom.png"));	// 23
	materials_.push_back(LoadMaterial("textures/cow/horn/front.png"));			// 24
	materials_.push_back(LoadMaterial("textures/cow/horn/back.png"));			// 25
	materials_.push_back(LoadMaterial("textures/cow/horn/left.png"));			// 26
	materials_.push_back(LoadMaterial("textures/cow/horn/right.png"));			// 27
	materials_.push_back(LoadMaterial("textures/cow/horn/top.png"));			// 28
	materials_.push_back(LoadMaterial("textures/cow/horn/bottom.png"));			// 29
	materials_.push_back(LoadMaterial("textures/cow/udders/front.png"));		// 30
	materials_.push_back(LoadMaterial("textures/cow/udders/back.png"));			// 31
	materials_.push_back(LoadMaterial("textures/cow/udders/left.png"));			// 32
	materials_.push_back(LoadMaterial("textures/cow/udders/right.png"));		// 33
	materials_.push_back(LoadMaterial("textures/cow/udders/top.png"));			// 34
	materials_.push_back(LoadMaterial("textures/cow/udders/bottom.png"));		// 35

	// MARKERS
	//////////////////////////////////////////////////////////////////////////////////

	materials_.push_back(LoadMaterial("textures/markers/1.png"));				// 36
	materials_.push_back(LoadMaterial("textures/markers/2.png"));				// 37
	materials_.push_back(LoadMaterial("textures/markers/3.png"));				// 38
	materials_.push_back(LoadMaterial("textures/markers/4.png"));				// 39
	materials_.push_back(LoadMaterial("textures/markers/5.png"));				// 40
	materials_.push_back(LoadMaterial("textures/markers/6.png"));				// 41
	materials_.push_back(LoadMaterial("textures/markers/s_1.png"));				// 42
	materials_.push_back(LoadMaterial("textures/markers/s_2.png"));				// 43
	materials_.push_back(LoadMaterial("textures/markers/s_3.png"));				// 44
	materials_.push_back(LoadMaterial("textures/markers/s_4.png"));				// 45
	materials_.push_back(LoadMaterial("textures/markers/s_5.png"));				// 46
	materials_.push_back(LoadMaterial("textures/markers/s_6.png"));				// 47
	materials_.push_back(LoadMaterial("textures/markers/t_1.png"));				// 48
	materials_.push_back(LoadMaterial("textures/markers/t_2.png"));				// 49
	materials_.push_back(LoadMaterial("textures/markers/t_3.png"));				// 50
	materials_.push_back(LoadMaterial("textures/markers/t_4.png"));				// 51
	materials_.push_back(LoadMaterial("textures/markers/t_5.png"));				// 52
	materials_.push_back(LoadMaterial("textures/markers/t_6.png"));				// 53

	// WOLF
	//////////////////////////////////////////////////////////////////////////////////

	materials_.push_back(LoadMaterial("textures/WOLF/body/front.png"));			// 54
	materials_.push_back(LoadMaterial("textures/WOLF/body/back.png"));			// 55
	materials_.push_back(LoadMaterial("textures/WOLF/body/left.png"));			// 56
	materials_.push_back(LoadMaterial("textures/WOLF/body/right.png"));			// 57
	materials_.push_back(LoadMaterial("textures/WOLF/body/top.png"));			// 58
	materials_.push_back(LoadMaterial("textures/WOLF/body/bottom.png"));		// 59

	materials_.push_back(LoadMaterial("textures/WOLF/mane/front.png"));			// 60
	materials_.push_back(LoadMaterial("textures/WOLF/mane/back.png"));			// 61
	materials_.push_back(LoadMaterial("textures/WOLF/mane/left.png"));			// 62
	materials_.push_back(LoadMaterial("textures/WOLF/mane/right.png"));			// 63
	materials_.push_back(LoadMaterial("textures/WOLF/mane/top.png"));			// 64
	materials_.push_back(LoadMaterial("textures/WOLF/mane/bottom.png"));		// 65

	materials_.push_back(LoadMaterial("textures/WOLF/head/front.png"));			// 66
	materials_.push_back(LoadMaterial("textures/WOLF/head/back.png"));			// 67
	materials_.push_back(LoadMaterial("textures/WOLF/head/left.png"));			// 68
	materials_.push_back(LoadMaterial("textures/WOLF/head/right.png"));			// 69
	materials_.push_back(LoadMaterial("textures/WOLF/head/top.png"));			// 70
	materials_.push_back(LoadMaterial("textures/WOLF/head/bottom.png"));		// 71

	materials_.push_back(LoadMaterial("textures/WOLF/nose/front.png"));			// 72
	materials_.push_back(LoadMaterial("textures/WOLF/nose/back.png"));			// 73
	materials_.push_back(LoadMaterial("textures/WOLF/nose/left.png"));			// 74
	materials_.push_back(LoadMaterial("textures/WOLF/nose/right.png"));			// 75
	materials_.push_back(LoadMaterial("textures/WOLF/nose/top.png"));			// 76
	materials_.push_back(LoadMaterial("textures/WOLF/nose/bottom.png"));		// 77

	materials_.push_back(LoadMaterial("textures/WOLF/legs/front.png"));			// 78
	materials_.push_back(LoadMaterial("textures/WOLF/legs/back.png"));			// 79
	materials_.push_back(LoadMaterial("textures/WOLF/legs/left.png"));			// 80
	materials_.push_back(LoadMaterial("textures/WOLF/legs/right.png"));			// 81
	materials_.push_back(LoadMaterial("textures/WOLF/legs/top.png"));			// 82
	materials_.push_back(LoadMaterial("textures/WOLF/legs/bottom.png"));		// 83

	materials_.push_back(LoadMaterial("textures/WOLF/tail/front.png"));			// 84
	materials_.push_back(LoadMaterial("textures/WOLF/tail/back.png"));			// 85
	materials_.push_back(LoadMaterial("textures/WOLF/tail/left.png"));			// 86
	materials_.push_back(LoadMaterial("textures/WOLF/tail/right.png"));			// 87
	materials_.push_back(LoadMaterial("textures/WOLF/tail/top.png"));			// 88
	materials_.push_back(LoadMaterial("textures/WOLF/tail/bottom.png"));		// 89

	materials_.push_back(LoadMaterial("textures/WOLF/ears/left_front.png"));	// 90
	materials_.push_back(LoadMaterial("textures/WOLF/ears/left_back.png"));		// 91
	materials_.push_back(LoadMaterial("textures/WOLF/ears/left.png"));			// 92
	materials_.push_back(LoadMaterial("textures/WOLF/ears/right.png"));			// 93
	materials_.push_back(LoadMaterial("textures/WOLF/ears/top.png"));			// 94
	materials_.push_back(LoadMaterial("textures/WOLF/ears/bottom.png"));		// 95

	materials_.push_back(LoadMaterial("textures/WOLF/ears/right_front.png"));	// 96
	materials_.push_back(LoadMaterial("textures/WOLF/ears/right_back.png"));	// 97
	materials_.push_back(LoadMaterial("textures/WOLF/ears/left.png"));			// 98
	materials_.push_back(LoadMaterial("textures/WOLF/ears/right.png"));			// 99
	materials_.push_back(LoadMaterial("textures/WOLF/ears/top.png"));			// 100
	materials_.push_back(LoadMaterial("textures/WOLF/ears/bottom.png"));		// 101

	// COLOURS
	//////////////////////////////////////////////////////////////////////////////////

	materials_.push_back(LoadMaterial("textures/colours/red.png"));			  
	materials_.push_back(LoadMaterial("textures/colours/green.png"));		  
	materials_.push_back(LoadMaterial("textures/colours/blue.png"));		  
	materials_.push_back(LoadMaterial("textures/colours/yellow.png"));		  
	materials_.push_back(LoadMaterial("textures/colours/cyan.png"));		  
	materials_.push_back(LoadMaterial("textures/colours/magenta.png"));		  
}

void App::CreateMeshes()
{
	// COW
	//////////////////////////////////////////////////////////////////////////////////

	gef::Mesh* cow_body = primitive_builder_->CreateBoxMesh(gef::Vector4(12.f, 10.f, 18.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[0]);
	gef::Mesh* cow_head = primitive_builder_->CreateBoxMesh(gef::Vector4(8.f, 8.f, 6.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[6]);
	gef::Mesh* cow_left_leg = primitive_builder_->CreateBoxMesh(gef::Vector4(4.f, 12.f, 4.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[12]);
	gef::Mesh* cow_right_leg = primitive_builder_->CreateBoxMesh(gef::Vector4(4.f, 12.f, 4.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[18]);
	gef::Mesh* cow_horn = primitive_builder_->CreateBoxMesh(gef::Vector4(1.f, 3.f, 1.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[24]);
	gef::Mesh* cow_udders = primitive_builder_->CreateBoxMesh(gef::Vector4(4.f, 1.f, 6.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[30]);
	
    meshes_.push_back(cow_body);			// 0
	meshes_.push_back(cow_head);			// 1
	meshes_.push_back(cow_left_leg);  		// 2
	meshes_.push_back(cow_right_leg);		// 3
	meshes_.push_back(cow_horn);			// 4
	meshes_.push_back(cow_udders);			// 5

	// MARKERS
	//////////////////////////////////////////////////////////////////////////////////

	gef::Mesh* _1 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[36]);
	gef::Mesh* _2 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[37]);
	gef::Mesh* _3 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[38]);
	gef::Mesh* _4 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[39]);
	gef::Mesh* _5 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[40]);
	gef::Mesh* _6 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[41]);
	gef::Mesh* s_1 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[42]);
	gef::Mesh* s_2 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[43]);
	gef::Mesh* s_3 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[44]);
	gef::Mesh* s_4 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[45]);
	gef::Mesh* s_5 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[46]);
	gef::Mesh* s_6 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[47]);
	gef::Mesh* t_1 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[48]);
	gef::Mesh* t_2 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[49]);
	gef::Mesh* t_3 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[50]);
	gef::Mesh* t_4 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[51]);
	gef::Mesh* t_5 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[52]);
	gef::Mesh* t_6 = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[53]);

	meshes_.push_back(_1);					// 6
	meshes_.push_back(_2);					// 7
	meshes_.push_back(_3);					// 8
	meshes_.push_back(_4);					// 9
	meshes_.push_back(_5);					// 10
	meshes_.push_back(_6);					// 11
	meshes_.push_back(s_1);					// 12
	meshes_.push_back(s_2);					// 13
	meshes_.push_back(s_3);					// 14
	meshes_.push_back(s_4);					// 15
	meshes_.push_back(s_5);					// 16
	meshes_.push_back(s_6);					// 17
	meshes_.push_back(t_1);					// 18
	meshes_.push_back(t_2);					// 19
	meshes_.push_back(t_3);					// 20
	meshes_.push_back(t_4);					// 21
	meshes_.push_back(t_5);					// 22
	meshes_.push_back(t_6);					// 23

	// WOLF
	//////////////////////////////////////////////////////////////////////////////////

	gef::Mesh* wolf_body = primitive_builder_->CreateBoxMesh(gef::Vector4(6.f, 6.f, 9.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[54]);
	gef::Mesh* wolf_mane = primitive_builder_->CreateBoxMesh(gef::Vector4(8.f, 7.f, 6.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[60]);
	gef::Mesh* wolf_head = primitive_builder_->CreateBoxMesh(gef::Vector4(6.f, 6.f, 4.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[66]);
	gef::Mesh* wolf_nose = primitive_builder_->CreateBoxMesh(gef::Vector4(3.f, 3.f, 3.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[72]);
	gef::Mesh* wolf_legs = primitive_builder_->CreateBoxMesh(gef::Vector4(2.f, 8.f, 2.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[78]);
	gef::Mesh* wolf_tail = primitive_builder_->CreateBoxMesh(gef::Vector4(2.f, 8.f, 2.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[84]);
	gef::Mesh* wolf_left_ear = primitive_builder_->CreateBoxMesh(gef::Vector4(2.f, 2.f, 1.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[90]);
	gef::Mesh* wolf_right_ear = primitive_builder_->CreateBoxMesh(gef::Vector4(2.f, 2.f, 1.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[96]);

	meshes_.push_back(wolf_body);			// 24
	meshes_.push_back(wolf_mane);			// 25
	meshes_.push_back(wolf_head);			// 26
	meshes_.push_back(wolf_nose);			// 27
	meshes_.push_back(wolf_legs);			// 28
	meshes_.push_back(wolf_tail);			// 29
	meshes_.push_back(wolf_left_ear);		// 30
	meshes_.push_back(wolf_right_ear);		// 31
}

