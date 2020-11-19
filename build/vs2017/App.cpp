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

	LoadAudio();

	state_machine_ = new StateMachine(&platform_, input_manager_, audio_manager_, sprite_renderer_, renderer_3D_, font_, meshes_);

	/*bool toggle = true;
	if (!toggle)
		audio_manager_->StopPlayingSampleVoice(0);
	else
	{
		audio_manager_->PlaySample(4, true);
	}*/
	audio_manager_->PlayMusic();
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
	if (!state) return false;
	if (!state->HandleInput()) return false;

	state->Update(delta_time);

	return true;
}

void App::Render()
{
	State* state = state_machine_->GetState();
	if (!state) return;
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

	// WOLF
	//////////////////////////////////////////////////////////////////////////////////

	materials_.push_back(LoadMaterial("textures/WOLF/body/front.png"));			// 36
	materials_.push_back(LoadMaterial("textures/WOLF/body/back.png"));			// 37
	materials_.push_back(LoadMaterial("textures/WOLF/body/left.png"));			// 38
	materials_.push_back(LoadMaterial("textures/WOLF/body/right.png"));			// 39
	materials_.push_back(LoadMaterial("textures/WOLF/body/top.png"));			// 40
	materials_.push_back(LoadMaterial("textures/WOLF/body/bottom.png"));		// 41
	materials_.push_back(LoadMaterial("textures/WOLF/mane/front.png"));			// 42
	materials_.push_back(LoadMaterial("textures/WOLF/mane/back.png"));			// 43
	materials_.push_back(LoadMaterial("textures/WOLF/mane/left.png"));			// 44
	materials_.push_back(LoadMaterial("textures/WOLF/mane/right.png"));			// 45
	materials_.push_back(LoadMaterial("textures/WOLF/mane/top.png"));			// 46
	materials_.push_back(LoadMaterial("textures/WOLF/mane/bottom.png"));		// 47
	materials_.push_back(LoadMaterial("textures/WOLF/head/front.png"));			// 48
	materials_.push_back(LoadMaterial("textures/WOLF/head/back.png"));			// 49
	materials_.push_back(LoadMaterial("textures/WOLF/head/left.png"));			// 50
	materials_.push_back(LoadMaterial("textures/WOLF/head/right.png"));			// 51
	materials_.push_back(LoadMaterial("textures/WOLF/head/top.png"));			// 52
	materials_.push_back(LoadMaterial("textures/WOLF/head/bottom.png"));		// 53
	materials_.push_back(LoadMaterial("textures/WOLF/nose/front.png"));			// 54
	materials_.push_back(LoadMaterial("textures/WOLF/nose/back.png"));			// 55
	materials_.push_back(LoadMaterial("textures/WOLF/nose/left.png"));			// 56
	materials_.push_back(LoadMaterial("textures/WOLF/nose/right.png"));			// 57
	materials_.push_back(LoadMaterial("textures/WOLF/nose/top.png"));			// 58
	materials_.push_back(LoadMaterial("textures/WOLF/nose/bottom.png"));		// 59
	materials_.push_back(LoadMaterial("textures/WOLF/legs/front.png"));			// 60
	materials_.push_back(LoadMaterial("textures/WOLF/legs/back.png"));			// 61
	materials_.push_back(LoadMaterial("textures/WOLF/legs/left.png"));			// 62
	materials_.push_back(LoadMaterial("textures/WOLF/legs/right.png"));			// 63
	materials_.push_back(LoadMaterial("textures/WOLF/legs/top.png"));			// 64
	materials_.push_back(LoadMaterial("textures/WOLF/legs/bottom.png"));		// 65
	materials_.push_back(LoadMaterial("textures/WOLF/tail/front.png"));			// 66
	materials_.push_back(LoadMaterial("textures/WOLF/tail/back.png"));			// 67
	materials_.push_back(LoadMaterial("textures/WOLF/tail/left.png"));			// 68
	materials_.push_back(LoadMaterial("textures/WOLF/tail/right.png"));			// 69
	materials_.push_back(LoadMaterial("textures/WOLF/tail/top.png"));			// 70
	materials_.push_back(LoadMaterial("textures/WOLF/tail/bottom.png"));		// 71
	materials_.push_back(LoadMaterial("textures/WOLF/ears/left_front.png"));	// 72
	materials_.push_back(LoadMaterial("textures/WOLF/ears/left_back.png"));		// 73
	materials_.push_back(LoadMaterial("textures/WOLF/ears/left.png"));			// 74
	materials_.push_back(LoadMaterial("textures/WOLF/ears/right.png"));			// 75
	materials_.push_back(LoadMaterial("textures/WOLF/ears/top.png"));			// 76
	materials_.push_back(LoadMaterial("textures/WOLF/ears/bottom.png"));		// 77
	materials_.push_back(LoadMaterial("textures/WOLF/ears/right_front.png"));	// 78
	materials_.push_back(LoadMaterial("textures/WOLF/ears/right_back.png"));	// 79
	materials_.push_back(LoadMaterial("textures/WOLF/ears/left.png"));			// 80
	materials_.push_back(LoadMaterial("textures/WOLF/ears/right.png"));			// 81
	materials_.push_back(LoadMaterial("textures/WOLF/ears/top.png"));			// 82
	materials_.push_back(LoadMaterial("textures/WOLF/ears/bottom.png"));		// 83

	// TREE
	//////////////////////////////////////////////////////////////////////////////////

	materials_.push_back(LoadMaterial("textures/tree/wood/xz.png"));			// 84
	materials_.push_back(LoadMaterial("textures/tree/wood/xz.png"));			// 85
	materials_.push_back(LoadMaterial("textures/tree/wood/xz.png"));			// 86
	materials_.push_back(LoadMaterial("textures/tree/wood/xz.png"));			// 87
	materials_.push_back(LoadMaterial("textures/tree/wood/y.png"));				// 88
	materials_.push_back(LoadMaterial("textures/tree/wood/y.png"));				// 89
	materials_.push_back(LoadMaterial("textures/tree/leaves/xz.png"));			// 90
	materials_.push_back(LoadMaterial("textures/tree/leaves/xz.png"));			// 91
	materials_.push_back(LoadMaterial("textures/tree/leaves/xz.png"));			// 92
	materials_.push_back(LoadMaterial("textures/tree/leaves/xz.png"));			// 93
	materials_.push_back(LoadMaterial("textures/tree/leaves/y.png"));			// 94
	materials_.push_back(LoadMaterial("textures/tree/leaves/y.png"));			// 95                         
																				
	// MARKERS																	
	//////////////////////////////////////////////////////////////////////////////////

	materials_.push_back(LoadMaterial("textures/markers/w_1.png"));				// 96
	materials_.push_back(LoadMaterial("textures/markers/w_2.png"));				// 97
	materials_.push_back(LoadMaterial("textures/markers/w_3.png"));				// 98
	materials_.push_back(LoadMaterial("textures/markers/w_4.png"));				// 99
	materials_.push_back(LoadMaterial("textures/markers/w_5.png"));				// 100
	materials_.push_back(LoadMaterial("textures/markers/w_6.png"));				// 101
	materials_.push_back(LoadMaterial("textures/markers/r_1.png"));				// 102
	materials_.push_back(LoadMaterial("textures/markers/r_2.png"));				// 103
	materials_.push_back(LoadMaterial("textures/markers/r_3.png"));				// 104
	materials_.push_back(LoadMaterial("textures/markers/r_4.png"));				// 105
	materials_.push_back(LoadMaterial("textures/markers/r_5.png"));				// 106
	materials_.push_back(LoadMaterial("textures/markers/r_6.png"));				// 107
	materials_.push_back(LoadMaterial("textures/markers/g_1.png"));				// 108
	materials_.push_back(LoadMaterial("textures/markers/g_2.png"));				// 109
	materials_.push_back(LoadMaterial("textures/markers/g_3.png"));				// 110
	materials_.push_back(LoadMaterial("textures/markers/g_4.png"));				// 111
	materials_.push_back(LoadMaterial("textures/markers/g_5.png"));				// 112
	materials_.push_back(LoadMaterial("textures/markers/g_6.png"));				// 113
	materials_.push_back(LoadMaterial("textures/markers/b_1.png"));				// 114
	materials_.push_back(LoadMaterial("textures/markers/b_2.png"));				// 115
	materials_.push_back(LoadMaterial("textures/markers/b_3.png"));				// 116
	materials_.push_back(LoadMaterial("textures/markers/b_4.png"));				// 117
	materials_.push_back(LoadMaterial("textures/markers/b_5.png"));				// 118
	materials_.push_back(LoadMaterial("textures/markers/b_6.png"));				// 119
	materials_.push_back(LoadMaterial("textures/markers/y_1.png"));				// 120
	materials_.push_back(LoadMaterial("textures/markers/y_2.png"));				// 121
	materials_.push_back(LoadMaterial("textures/markers/y_3.png"));				// 122
	materials_.push_back(LoadMaterial("textures/markers/y_4.png"));				// 123
	materials_.push_back(LoadMaterial("textures/markers/y_5.png"));				// 124
	materials_.push_back(LoadMaterial("textures/markers/y_6.png"));				// 125

	// COLOURS
	//////////////////////////////////////////////////////////////////////////////////

	materials_.push_back(LoadMaterial("textures/colours/green.png"));			// 126
	materials_.push_back(LoadMaterial("textures/colours/red.png"));			  
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

	// WOLF
	//////////////////////////////////////////////////////////////////////////////////

	gef::Mesh* wolf_body = primitive_builder_->CreateBoxMesh(gef::Vector4(6.f, 6.f, 9.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[36]);
	gef::Mesh* wolf_mane = primitive_builder_->CreateBoxMesh(gef::Vector4(8.f, 7.f, 6.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[42]);
	gef::Mesh* wolf_head = primitive_builder_->CreateBoxMesh(gef::Vector4(6.f, 6.f, 4.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[48]);
	gef::Mesh* wolf_nose = primitive_builder_->CreateBoxMesh(gef::Vector4(3.f, 3.f, 3.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[54]);
	gef::Mesh* wolf_legs = primitive_builder_->CreateBoxMesh(gef::Vector4(2.f, 8.f, 2.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[60]);
	gef::Mesh* wolf_tail = primitive_builder_->CreateBoxMesh(gef::Vector4(2.f, 8.f, 2.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[66]);
	gef::Mesh* wolf_left_ear = primitive_builder_->CreateBoxMesh(gef::Vector4(2.f, 2.f, 1.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[72]);
	gef::Mesh* wolf_right_ear = primitive_builder_->CreateBoxMesh(gef::Vector4(2.f, 2.f, 1.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[78]);

	meshes_.push_back(wolf_body);			// 6
	meshes_.push_back(wolf_mane);			// 7
	meshes_.push_back(wolf_head);			// 8
	meshes_.push_back(wolf_nose);			// 9
	meshes_.push_back(wolf_legs);			// 10
	meshes_.push_back(wolf_tail);			// 11
	meshes_.push_back(wolf_left_ear);		// 12
	meshes_.push_back(wolf_right_ear);		// 13

	// TREE
	//////////////////////////////////////////////////////////////////////////////////
	gef::Mesh* wood = primitive_builder_->CreateBoxMesh(gef::Vector4(16.f, 16.f * 3, 16.f), gef::Vector4(0.f, 0.f, 0.f), &materials_[84]);
	gef::Mesh* leaves = primitive_builder_->CreateBoxMesh(gef::Vector4(16.f * 3, 16.f * 5, 16.f * 3), gef::Vector4(0.f, 0.f, 0.f), &materials_[90]);

	meshes_.push_back(leaves);				// 14
	meshes_.push_back(wood);				// 15

	// MARKERS
	//////////////////////////////////////////////////////////////////////////////////
	
	gef::Vector4 marker_half_size = gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f);
	gef::Mesh* w_1 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[96]);
	gef::Mesh* w_2 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[97]);
	gef::Mesh* w_3 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[98]);
	gef::Mesh* w_4 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[99]);
	gef::Mesh* w_5 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[100]);
	gef::Mesh* w_6 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[101]);
	gef::Mesh* r_1 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[102]);
	gef::Mesh* r_2 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[103]);
	gef::Mesh* r_3 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[104]);
	gef::Mesh* r_4 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[105]);
	gef::Mesh* r_5 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[106]);
	gef::Mesh* r_6 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[107]);
	gef::Mesh* g_1 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[108]);
	gef::Mesh* g_2 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[109]);
	gef::Mesh* g_3 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[110]);
	gef::Mesh* g_4 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[111]);
	gef::Mesh* g_5 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[112]);
	gef::Mesh* g_6 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[113]);
	gef::Mesh* b_1 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[114]);
	gef::Mesh* b_2 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[115]);
	gef::Mesh* b_3 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[116]);
	gef::Mesh* b_4 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[117]);
	gef::Mesh* b_5 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[118]);
	gef::Mesh* b_6 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[119]);
	gef::Mesh* y_1 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[120]);
	gef::Mesh* y_2 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[121]);
	gef::Mesh* y_3 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[122]);
	gef::Mesh* y_4 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[123]);
	gef::Mesh* y_5 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[124]);
	gef::Mesh* y_6 = primitive_builder_->CreatePlaneMesh(marker_half_size, gef::Vector4(0.f, 0.f, 0.f), materials_[125]);
	
	meshes_.push_back(w_1);					// 16																									
	meshes_.push_back(w_2);					// 17																										
	meshes_.push_back(w_3);					// 18																										
	meshes_.push_back(w_4);					// 19																										
	meshes_.push_back(w_5);					// 20																										
	meshes_.push_back(w_6);					// 21
	meshes_.push_back(r_1);					// 22
	meshes_.push_back(r_2);					// 23
	meshes_.push_back(r_3);					// 24
	meshes_.push_back(r_4);					// 25
	meshes_.push_back(r_5);					// 26
	meshes_.push_back(r_6);					// 27
	meshes_.push_back(g_1);					// 28
	meshes_.push_back(g_2);					// 29
	meshes_.push_back(g_3);					// 30
	meshes_.push_back(g_4);					// 31
	meshes_.push_back(g_5);					// 32
	meshes_.push_back(g_6);					// 33
	meshes_.push_back(b_1);					// 34
	meshes_.push_back(b_2);					// 35
	meshes_.push_back(b_3);					// 36
	meshes_.push_back(b_4);					// 37
	meshes_.push_back(b_5);					// 38
	meshes_.push_back(b_6);					// 39
	meshes_.push_back(y_1);					// 40
	meshes_.push_back(y_2);					// 41
	meshes_.push_back(y_3);					// 42
	meshes_.push_back(y_4);					// 43
	meshes_.push_back(y_5);					// 44
	meshes_.push_back(y_6);					// 45

	// MENU
	//////////////////////////////////////////////////////////////////////////////////

	gef::Mesh* plane_mesh = primitive_builder_->CreatePlaneMesh(gef::Vector4(10000.f, 0.f, 1000.f), gef::Vector4(0.f, 0.f, 0.f), materials_[126]);

	meshes_.push_back(plane_mesh);			// 46

	
}

void App::LoadAudio()
{
	audio_manager_->LoadMusic("audio/music/music_full.wav", platform_);						// 0
																			
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/buttons/button.wav", platform_));	// 0

	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/cow/breath_1.wav", platform_));	// 1
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/cow/moo_1.wav", platform_));		// 2
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/cow/moo_2.wav", platform_));		// 3
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/cow/moo_3.wav", platform_));		// 4
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/cow/moo_4.wav", platform_));		// 5
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/cow/moo_5.wav", platform_));		// 6
																
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/wolf/bark_1.wav", platform_));		// 7
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/wolf/bark_2.wav", platform_));		// 8
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/wolf/bark_3.wav", platform_));		// 9
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/wolf/growl_1.wav", platform_));	// 
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/wolf/growl_2.wav", platform_));	// 
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/wolf/hurt_1.wav", platform_));		// 
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/wolf/hurt_2.wav", platform_));		// 
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/wolf/hurt_3.wav", platform_));		// 
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/wolf/pant.wav", platform_));		// 
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/wolf/whine.wav", platform_));		// 
																	
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/cow/footsteps.wav", platform_));	// 
	sfx_IDs_.push_back(audio_manager_->LoadSample("audio/cow/hit.wav", platform_));			// 
}																			

