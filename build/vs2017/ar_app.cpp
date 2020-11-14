#include "ar_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#if VITA_MODE
#include <input/touch_input_manager.h>
#endif
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/render_target.h>
#include <time.h>
#include <input/keyboard.h>

#if VITA_MODE
#include <sony_sample_framework.h>
#include <sony_tracking.h>
#endif

#include <math.h>






ARApp::ARApp(gef::Platform& platform) :
	Application(platform),
	input_manager_(NULL),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL), 
	primitive_builder_(NULL)
{
}

void ARApp::Init()
{
	gef::Matrix44 a;
	a.SetRow(0, gef::Vector4(1, 3, 1, 0));
	a.SetRow(1, gef::Vector4(4, 2, 2, 0));
	a.SetRow(2, gef::Vector4(2, -1, -3, 0));
	a.SetRow(3, gef::Vector4(0, 4, 0, 1));

	gef::Matrix44 b;
	b.AffineInverse(a);
	srand(time(NULL));

	input_manager_ = gef::InputManager::Create(platform_);
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	primitive_builder_ = new PrimitiveBuilder(platform_);

	InitFont();
	SetupLights();
	LoadMaterials();

	for (int i = 0; i < 10; i++)
	{
		cows.push_back(new Cow(*renderer_3d_, *primitive_builder_, cow_materials_));
	}

	table_mesh = primitive_builder_->CreatePlaneMesh(gef::Vector4(ENVIRONMENT_HALF_WIDTH, 0.f, ENVIRONMENT_HALF_DEPTH));
	table_mesh_instance.set_mesh(table_mesh);

	//predator_mesh_ = primitive_builder_->CreateBoxMesh(gef::Vector4(0.59f / 2, 0.59f / 2, 0.59f / 2));
	//predator_mesh_instance_.set_mesh(predator_mesh_);

#if !VITA_MODE
	marker_mesh_ = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f));
	for (int i = 0; i < 6; i++)
	{
		marker_mesh_instances_[i].set_mesh(marker_mesh_);
		marker_transforms_[i].SetIdentity();
	}

	selected_marker_mesh_ = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0, 0, 0), cow_materials_[13]);
	selected_marker_mesh_instance.set_mesh(selected_marker_mesh_);

	targeted_marker_mesh_ = primitive_builder_->CreatePlaneMesh(gef::Vector4(0.059f / 2 * 1000.f, 0, 0.059f / 2 * 1000.f), gef::Vector4(0, 0, 0), cow_materials_[30]);
	targeted_marker_mesh_instance.set_mesh(targeted_marker_mesh_);


	camera_ = new Camera(platform_, input_manager_);

	marker_transforms_[0].RotationY(3.1415f / 4.0f);
	marker_transforms_[0].SetTranslation(gef::Vector4(-500, 0, 0));
	marker_transforms_[1].SetTranslation(gef::Vector4(-250, 0, 0));
	marker_transforms_[2].SetTranslation(gef::Vector4(0, 0, 0));
	marker_transforms_[3].SetTranslation(gef::Vector4(250, 0, 0));
	marker_transforms_[4].SetTranslation(gef::Vector4(500, 0, 0));

	for (int i = 0; i < 6; i++)
	{
		marker_mesh_instances_[i].set_transform(marker_transforms_[i]);
	}

	targeted_marker_ID = 0;
	selected_marker_ID = targeted_marker_ID;
	previous_marker_ID = 0;

	previous_marker_ = marker_transforms_[previous_marker_ID];
	selected_marker_ = marker_transforms_[selected_marker_ID];

	anchorX = marker_transforms_[targeted_marker_ID].GetRow(3).x();
	anchorZ = marker_transforms_[targeted_marker_ID].GetRow(3).z();

	for (Boid* boid : cows)
	{
		Cow* cow = (Cow*)boid;
		cow->local_matrix_.SetIdentity();
		cow->marker_matrix_ = selected_marker_;
	}

#else
	float camera_image_scale_factor = (960.f / 544.f) / (640.f / 480.f);
	gef::Matrix44 scale_matrix;
	scale_matrix.SetIdentity();
	scale_matrix.Scale(gef::Vector4(1.f, camera_image_scale_factor, 1.f, 1.f));

	projection_matrix_ = platform_.PerspectiveProjectionFov(SCE_SMART_IMAGE_FOV, (float)SCE_SMART_IMAGE_WIDTH / (float)SCE_SMART_IMAGE_HEIGHT, .1f, 10.f);
	projection_matrix_ = projection_matrix_ * scale_matrix;

	view_matrix_.SetIdentity();
	ortho_matrix_.SetIdentity();
	ortho_matrix_ = platform_.OrthographicFrustum(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	camera_feed_texture_ = new gef::TextureVita;
	camera_feed_sprite_.set_width(2.f);
	camera_feed_sprite_.set_height(2.f * camera_image_scale_factor);
	camera_feed_sprite_.set_position(gef::Vector4(0.f, 0.f, 1.f, 1.f));
	sprite_renderer_->set_projection_matrix(ortho_matrix_);
	
	// initialise sony framework
	sampleInitialize();
	smartInitialize();
	
	selected_marker_ID = 0;
	previous_marker_ID = 0;
	selected_marker_.SetIdentity();
	previous_marker_.SetIdentity();

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();

	for (int i = 0; i < 6; i++)
	{
		if (sampleIsMarkerFound(i))
		{
			selected_marker_ID = i;
			previous_marker_ID = i;
			sampleGetTransform(i, &selected_marker_);
			sampleGetTransform(i, &previous_marker_);
			break;
		}
	}

	sampleUpdateEnd(dat);

	
#endif
}

void ARApp::CleanUp()
{
	delete primitive_builder_;
	primitive_builder_ = NULL;

	CleanUpFont();
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;

#if VITA_MODE
	smartRelease();
	sampleRelease();

	delete camera_feed_texture_;
	camera_feed_texture_ = NULL;
#endif
}

bool ARApp::HandleInput()
{
#if !VITA_MODE
	if (input_manager_)
	{
		input_manager_->Update();
		gef::Keyboard* keyboard = input_manager_->keyboard();
		if (keyboard)
		{
			if (keyboard->IsKeyDown(gef::Keyboard::KC_ESCAPE))
			{
				return false;
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_1))
			{
				targeted_marker_ID = 0;
				anchorX = marker_transforms_[targeted_marker_ID].GetRow(3).x();
				anchorZ = marker_transforms_[targeted_marker_ID].GetRow(3).z();
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_2))
			{
				targeted_marker_ID = 1;
				anchorX = marker_transforms_[targeted_marker_ID].GetRow(3).x();
				anchorZ = marker_transforms_[targeted_marker_ID].GetRow(3).z();
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_3))
			{
				targeted_marker_ID = 2;
				anchorX = marker_transforms_[targeted_marker_ID].GetRow(3).x();
				anchorZ = marker_transforms_[targeted_marker_ID].GetRow(3).z();
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_4))
			{
				targeted_marker_ID = 3;
				anchorX = marker_transforms_[targeted_marker_ID].GetRow(3).x();
				anchorZ = marker_transforms_[targeted_marker_ID].GetRow(3).z();
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_5))
			{
				targeted_marker_ID = 4;
				anchorX = marker_transforms_[targeted_marker_ID].GetRow(3).x();
				anchorZ = marker_transforms_[targeted_marker_ID].GetRow(3).z();
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_A))
			{
				anchorX -= 10;
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_D))
			{
				anchorX += 10;
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_W))
			{
				anchorZ -= 10;
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_S))
			{
				anchorZ += 10;
			}
			if (keyboard->IsKeyDown(gef::Keyboard::KC_SPACE))
			{
				if (selected_marker_ID != targeted_marker_ID)
				{
					


					previous_marker_ID = selected_marker_ID;
					selected_marker_ID = targeted_marker_ID;

					gef::Matrix44 previous_marker = marker_transforms_[previous_marker_ID];
					gef::Matrix44 selected_marker = marker_transforms_[selected_marker_ID];

					gef::Matrix44 selected_marker_inverse;
					selected_marker_inverse.AffineInverse(selected_marker);

					for (Boid* boid : cows)
					{
						Cow* cow = (Cow*)boid;
						cow->marker_matrix_ = selected_marker;
						cow->local_matrix_ = cow->local_matrix_ * previous_marker * selected_marker_inverse;
					}

				}
			}
		}

		
		//camera_->HandleInput();
	}
#endif
	return true;
}

bool ARApp::Update(float delta_time)
{
	if (!HandleInput()) return false;

	fps_ = 1.0f / delta_time;

	//gef::Matrix44 predator_transform;
	//predator_transform.SetIdentity();
	//Boid* boid = cows.front();
	//predator_transform.SetTranslation(boid->predator_);
	//predator_mesh_instance_.set_transform(selected_marker_ * predator_transform);
	
#if VITA_MODE
	AppData* dat = sampleUpdateBegin();
	// use the tracking library to try and find markers
	smartUpdate(dat->currentImage);

	if (sampleIsMarkerFound(5))
	{
		for (Boid* boid : cows)
		{
			Cow* cow = (Cow*)boid;

			sampleGetTransform(5, &markers_[5]);

			gef::Matrix44 selected_marker_inverse;
			selected_marker_inverse.AffineInverse(selected_marker_);

			cow->SetPredatorLocalTransform((markers_[5] * selected_marker_inverse).GetTranslation());
		}
	}

	// swapping markers if no longer visible
	if (!sampleIsMarkerFound(selected_marker_ID))
	{
		for (int i = 0; i < 2; i++)
		{
			if (i == selected_marker_ID) continue;

			if (sampleIsMarkerFound(i))
			{
				previous_marker_ID = selected_marker_ID;
				selected_marker_ID = i;
				previous_marker_ = selected_marker_;

				sampleGetTransform(selected_marker_ID, &selected_marker_);

				gef::Matrix44 selected_marker_inverse;
				selected_marker_inverse.AffineInverse(selected_marker_);

				for (Boid* boid : cows)
				{
					Cow* cow = (Cow*)boid;
					cow->local_matrix_ = cow->local_matrix_ * previous_marker_ * selected_marker_inverse;
				}

				
				break;
			}
		}
	}
	if (sampleIsMarkerFound(selected_marker_ID))
	{
		sampleGetTransform(selected_marker_ID, &selected_marker_);
		
	}
	for (Boid* boid : cows)
	{
		Cow* cow = (Cow*)boid;
		cow->marker_matrix_ = selected_marker_;
		cow->Flock(cows, delta_time);
		cow->Update(delta_time);
	}




	//for (int marker_id = 0; marker_id < 2; marker_id++)
	//{
	//	

	//	// check to see if a particular marker can be found
	//	if (sampleIsMarkerFound(marker_id)) {
	//
	//		// marker is being tracked, get its transform
	//		sampleGetTransform(marker_id, &selected_marker);

	//		gef::Matrix44 selected_marker_inverse;
	//		selected_marker_inverse.AffineInverse(selected_marker);

	//		// offset needed from previous marker
	//		gef::Matrix44 local_offset = previous_marker * selected_marker_inverse;

	//		// offset needed to keep track of all markers
	//		offset = offset * local_offset;

	//		for (Boid* boid : cows)
	//		{
	//			Cow* cow = (Cow*)boid;
	//			if (targeted_marker_ID == selected_marker_ID)
	//			{
	//				cow->world_matrix_ = selected_marker * offset;
	//			}
	//			cow->Flock(cows, delta_time);
	//			cow->Update(delta_time);
	//		}


	//		

	//		

	//		break;
	//
	//		// set the transform of the 3D mesh instance to draw on top of the marker
	//		//marker_mesh_instances_[i].set_transform(marker_transform);
	//	}
	//}
	sampleUpdateEnd(dat);
#else

	camera_->Update(delta_time);

	previous_marker_ = marker_transforms_[previous_marker_ID];
	selected_marker_ = marker_transforms_[selected_marker_ID];


	for (Boid* boid : cows)
	{
		Cow* cow = (Cow*)boid;
		if (targeted_marker_ID == selected_marker_ID)
		{
			cow->marker_matrix_ = selected_marker_;
			//cow->world_matrix_ = marker_transforms_[selected_marker_ID] * offset;// cow->local_matrix_;
		}
		cow->Flock(cows, delta_time);
		cow->Update(delta_time);
	}


	marker_transforms_[targeted_marker_ID].SetTranslation(gef::Vector4(anchorX, 0, anchorZ));

	

	for (int i = 0; i < 6; i++)
	{
		marker_mesh_instances_[i].set_transform(marker_transforms_[i]);
	}
	targeted_marker_mesh_instance.set_transform(marker_transforms_[targeted_marker_ID]);
	selected_marker_mesh_instance.set_transform(marker_transforms_[selected_marker_ID]);
#endif

	

	return true;
}

void ARApp::Render()
{
#if !VITA_MODE
	// lighting
	float distance = 120.f;
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(cos(0.f) * distance, 40.0f, sin(0.f) * distance)); // not correct
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
	
	// 3D scene
	renderer_3d_->set_projection_matrix(camera_->projection_matrix_);
	renderer_3d_->set_view_matrix(camera_->view_matrix_);
	renderer_3d_->Begin(true);
	
	//renderer_3d_->DrawMesh(table_mesh_instance);

	renderer_3d_->DrawMesh(targeted_marker_mesh_instance);
	renderer_3d_->DrawMesh(selected_marker_mesh_instance);
	for (int i = 0; i < 5; i++)
	{
		if (i == targeted_marker_ID) continue;
		if (i == selected_marker_ID) continue;
		renderer_3d_->DrawMesh(marker_mesh_instances_[i]);
	}

	for (Boid* boid : cows)
	{
		Cow* cow = (Cow*)boid;
		cow->Render();
	}
	renderer_3d_->End();
#else
	AppData* dat = sampleRenderBegin();
	float camera_image_scale_factor = (960.f / 544.f) / (640.f / 480.f);
	// camera feed 
	ortho_matrix_ = platform_.OrthographicFrustum(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);		// why do i need to update this every frame?
	sprite_renderer_->set_projection_matrix(ortho_matrix_);
	if (dat->currentImage)
	{
		camera_feed_texture_->set_texture(dat->currentImage->tex_yuv);
		camera_feed_sprite_.set_texture(camera_feed_texture_);
	}
	sprite_renderer_->Begin(true);
	sprite_renderer_->DrawSprite(camera_feed_sprite_);
	sprite_renderer_->End();

	// 3D scene relative to markers
	renderer_3d_->set_projection_matrix(projection_matrix_);
	renderer_3d_->set_view_matrix(view_matrix_);
	renderer_3d_->Begin(false);
	//renderer_3d_->DrawMesh(marker_mesh_instance_);
	renderer_3d_->End();

	renderer_3d_->Begin(false);
	//renderer_3d_->DrawMesh(predator_mesh_instance_);
	renderer_3d_->DrawMesh(table_mesh_instance);
	for (Boid* boid : cows)
	{
		Cow* cow = (Cow*)boid;
		cow->Render();
	}
	renderer_3d_->End();

	sampleRenderEnd();
	
#endif

	RenderOverlay();
}


void ARApp::RenderOverlay()
{
	//
	// render 2d hud on top
	//
	gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform_.OrthographicFrustum(0.0f, platform_.width(), 0.0f, platform_.height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}


void ARApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void ARApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void ARApp::DrawHUD()
{
	if(font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 410.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "ID: %i", selected_marker_ID);

		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

void ARApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));
	
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

gef::Material* ARApp::LoadMaterial(char* file_name)
{
	gef::ImageData image_data;
	gef::PNGLoader png_loader;
	png_loader.Load(file_name, platform_, image_data);
	gef::Texture* texture = gef::Texture::Create(platform_, image_data);   // calling a static function? different for vita and d3d. 
	gef::Material* material = new gef::Material(); // Will this get cleaned up when the materials vector is deleted?
	material->set_texture(texture);
	return material;
}

void ARApp::LoadMaterials()
{
	cow_materials_.push_back(LoadMaterial("textures/cow/body/front.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/body/back.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/body/left.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/body/right.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/body/top.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/body/bottom.png"));

	cow_materials_.push_back(LoadMaterial("textures/cow/head/front.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/head/back.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/head/left.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/head/right.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/head/top.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/head/bottom.png"));

	cow_materials_.push_back(LoadMaterial("textures/cow/left_leg/front.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/left_leg/back.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/left_leg/left.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/left_leg/right.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/left_leg/top.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/left_leg/bottom.png"));

	cow_materials_.push_back(LoadMaterial("textures/cow/right_leg/front.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/right_leg/back.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/right_leg/left.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/right_leg/right.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/right_leg/top.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/right_leg/bottom.png"));

	cow_materials_.push_back(LoadMaterial("textures/cow/horn/front.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/horn/back.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/horn/left.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/horn/right.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/horn/top.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/horn/bottom.png"));

	cow_materials_.push_back(LoadMaterial("textures/cow/udders/front.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/udders/back.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/udders/left.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/udders/right.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/udders/top.png"));
	cow_materials_.push_back(LoadMaterial("textures/cow/udders/bottom.png"));
}
