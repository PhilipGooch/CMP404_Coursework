#include "GameStateVITA.h"

#if VITA_MODE
#include <input/touch_input_manager.h>
#include <platform/vita/graphics/texture_vita.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>
#include <sony_sample_framework.h>
#include <sony_tracking.h>
#endif

#include <input/input_manager.h>
#include <graphics/renderer_3d.h>
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <audio/audio_manager.h>
#include <graphics/font.h>
#include "Boid.h"
#include "Cow.h"
#include "Wolf.h"
#include "Marker.h"
#include "Tree.h"
#include "StateMachine.h"

GameStateVITA::GameStateVITA(gef::Platform* platform,
	gef::InputManager* input_manager,
	gef::AudioManager* audio_manager,
	gef::SpriteRenderer* sprite_renderer,
	gef::Renderer3D* renderer_3D,
	gef::Font* font,
	StateMachine* state_machine,
	std::vector<gef::Mesh*> meshes) :
	GameState(platform, input_manager, audio_manager, sprite_renderer, renderer_3D, font, state_machine, meshes)
{
	pm = new PrimitiveBuilder(*platform);
	debugCubeMesh = pm->CreateBoxMesh(gef::Vector4(0.01, 0.01, 0.01));
	debugCube.set_mesh(debugCubeMesh);

	// INITIALIZE SONY FRAMEWORK
	sampleInitialize();
	smartInitialize();

	// PROJECTION, VIEW AND ORTHOGRAPHIC MATRICES
	float camera_image_scale_factor = (960.f / 544.f) / (640.f / 480.f);
	gef::Matrix44 scale_matrix;
	scale_matrix.SetIdentity();
	scale_matrix.Scale(gef::Vector4(1.f, camera_image_scale_factor, 1.f, 1.f));
	projection_matrix_ = platform_->PerspectiveProjectionFov(SCE_SMART_IMAGE_FOV, (float)SCE_SMART_IMAGE_WIDTH / (float)SCE_SMART_IMAGE_HEIGHT, .1f, 10.f);
	projection_matrix_ = projection_matrix_ * scale_matrix;
	view_matrix_.SetIdentity();
	ortho_matrix_.SetIdentity();
	ortho_matrix_ = platform_->OrthographicFrustum(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	// MARKERS
	for (int i = 0; i < number_of_markers_; i++)
	{
		gef::Matrix44 world_matrix;
		world_matrix.SetIdentity();
		markers_.push_back(new Marker(renderer_3D, meshes, world_matrix, i));
	}
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);

	targeted_marker_ = markers_[0];
	selected_marker_ = targeted_marker_;

	cow_marker_ = selected_marker_;
	cow_marker_->child_ = Marker::CHILD::COW;
	cow_marker_->boids_ = &cows_;
	for (Boid* boid : cows_)
	{
		Cow* cow = (Cow*)boid;
		cow->marker_matrix_ = cow_marker_->world_matrix_;
	}

	wolf_marker_ = markers_[2];
	wolf_marker_->child_ = Marker::CHILD::WOLF;
	wolf_marker_->boids_ = &wolves_;
	for (Boid* boid : wolves_)
	{
		Wolf* wolf = (Wolf*)boid;
		wolf->marker_matrix_ = wolf_marker_->world_matrix_;
	}

	tree_marker_ = markers_[3];
	tree_marker_->child_ = Marker::CHILD::TREE;
	tree_marker_->boids_ = &trees_;
	for (Boid* boid : trees_)
	{
		Tree* tree = (Tree*)boid;
		tree->marker_matrix_ = tree_marker_->world_matrix_;
	}

	// CAMERA FEED IMAGE
	camera_feed_texture_ = new gef::TextureVita;
	camera_feed_sprite_.set_width(2.f);
	camera_feed_sprite_.set_height(2.f * camera_image_scale_factor);
	camera_feed_sprite_.set_position(gef::Vector4(0.f, 0.f, 1.f, 1.f));
	sprite_renderer_->set_projection_matrix(ortho_matrix_);
}

GameStateVITA::~GameStateVITA()
{
#if VITA_MODE
	smartRelease();
	sampleRelease();

	delete camera_feed_texture_;
	camera_feed_texture_ = NULL;
#endif
}


bool GameStateVITA::HandleInput()
{
	if (input_manager_)
	{
		input_manager_->Update();
	}
	return true;
}

void GameStateVITA::Update(float delta_time)
{
	fps_ = 1.f / delta_time;

	timer++;

	//for (int i = 4; i <= 11; i++)
	{
		if (timer % 10000)
		{
			if (!audio_manager_->sample_voice_playing(9))
				audio_manager_->PlaySample(9, false);
		}
	}

	// MARKERS
	AppData* dat = sampleUpdateBegin();
	smartUpdate(dat->currentImage);

	if (!sampleIsMarkerFound(selected_marker_->ID_))
	{
		for (int i = 0; i < 5; i++)
		{
			if (i == selected_marker_->ID_) continue;
			if (i == wolf_marker_->ID_) continue;
			if (i == tree_marker_->ID_) continue;
	
			if (sampleIsMarkerFound(i))
			{
				targeted_marker_ = markers_[i];
				gef::Matrix44 previous_marker_matrix = selected_marker_->world_matrix_;
				Marker::CHILD previous_anchor = selected_marker_->child_;
				selected_marker_->child_ = Marker::CHILD::NONE;
				selected_marker_ = targeted_marker_;
				cow_marker_ = selected_marker_;
				selected_marker_->child_ = previous_anchor;
	
				sampleGetTransform(selected_marker_->ID_, &selected_marker_->world_matrix_);
	
				gef::Matrix44 selected_marker_inverse_world_matrix;
				selected_marker_inverse_world_matrix.AffineInverse(selected_marker_->world_matrix_);
	
				for (Boid* boid : cows_)
				{
					Cow* cow = (Cow*)boid;
					cow->marker_matrix_ = cow_marker_->world_matrix_;
					cow->local_marker_matrix_ = cow->local_marker_matrix_ * previous_marker_matrix * selected_marker_inverse_world_matrix;
				}
	
				
	
				break;
			}
		}
	}

	if (sampleIsMarkerFound(selected_marker_->ID_))
	{
		sampleGetTransform(selected_marker_->ID_, &selected_marker_->world_matrix_);
	}

	if (sampleIsMarkerFound(wolf_marker_->ID_))
	{
		sampleGetTransform(wolf_marker_->ID_, &wolf_marker_->world_matrix_);
	}

	if (sampleIsMarkerFound(tree_marker_->ID_))
	{
		sampleGetTransform(tree_marker_->ID_, &tree_marker_->world_matrix_);
		gef::Matrix44 selected_marker_inverse;
		selected_marker_inverse.AffineInverse(selected_marker_->world_matrix_);
		for (Boid* boid : cows_)
		{
			//Cow* cow = (Cow*)boid;
			//cow->SetPredatorLocalTransform((tree_marker_->world_matrix_ * selected_marker_inverse).GetTranslation());
		}
	}


	setDebugCubeTranspose(cows_.front()->tree_world_space);

	/*if (sampleIsMarkerFound(5))
	{
		sampleGetTransform(5, &markers_[5]->world_matrix_);
		for (Boid* boid : cows_)
		{
			Cow* cow = (Cow*)boid;


			gef::Matrix44 selected_marker_inverse;
			selected_marker_inverse.AffineInverse(selected_marker_->world_matrix_);

			cow->SetPredatorLocalTransform((markers_[5]->world_matrix_ * selected_marker_inverse).GetTranslation());
		}
	}*/


	sampleUpdateEnd(dat);


	///////////////////////
	for (Marker* marker : markers_)
	{
		marker->Update();
	}

	for (Boid* boid : cows_)
	{
		Cow* cow = (Cow*)boid;
		cow->marker_matrix_ = cow_marker_->world_matrix_;
		cow->Flock(cows_, delta_time);
		cow->Update(delta_time);
	}

	for (Boid* boid : wolves_)
	{
		Wolf* wolf = (Wolf*)boid;
		wolf->marker_matrix_ = wolf_marker_->world_matrix_;
		wolf->Flock(wolves_, delta_time);
		wolf->Update(delta_time);
	}

	for (Boid* boid : trees_)
	{
		Tree* tree = (Tree*)boid;
		tree->marker_matrix_ = tree_marker_->world_matrix_;
	}
}

void GameStateVITA::Render()
{
	renderer_3D_->set_projection_matrix(projection_matrix_);
	renderer_3D_->set_view_matrix(view_matrix_);

	AppData* dat = sampleRenderBegin();
	// camera feed 
	ortho_matrix_ = platform_->OrthographicFrustum(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);		// why do i need to update this every frame?
	sprite_renderer_->set_projection_matrix(ortho_matrix_);
	if (dat->currentImage)
	{
		camera_feed_texture_->set_texture(dat->currentImage->tex_yuv);
		camera_feed_sprite_.set_texture(camera_feed_texture_);
	}
	sprite_renderer_->Begin(true);
	sprite_renderer_->DrawSprite(camera_feed_sprite_);
	sprite_renderer_->End();



	renderer_3D_->Begin(false);

	for (Marker* marker : markers_)
	{
		if (marker->child_ == Marker::CHILD::COW ||
			marker->child_ == Marker::CHILD::WOLF || 
			marker->child_ == Marker::CHILD::TREE)
		{
			marker->Render();
		}
	}

	for (Boid* boid : cows_)
	{
		Cow* cow = (Cow*)boid;
		cow->Render();
	}
	
	for (Boid* boid : wolves_)
	{
		Wolf* wolf = (Wolf*)boid;
		wolf->Render();
	}
	
	for (Boid* boid : trees_)
	{
		Tree* tree = (Tree*)boid;
		tree->Render(true);
	}

	renderer_3D_->DrawMesh(debugCube);

	renderer_3D_->End();

	sampleRenderEnd();

	DrawHUD();
}

void GameStateVITA::SwapTargetMarker(int target)
{
	targeted_marker_->targeted_ = false;
	targeted_marker_ = markers_[target];
	targeted_marker_->targeted_ = true;
}

void GameStateVITA::SwapMarker()
{
	Marker::CHILD previous_anchor = selected_marker_->child_;
	selected_marker_->child_ = Marker::CHILD::NONE;
	selected_marker_ = targeted_marker_;
	selected_marker_->child_ = previous_anchor;
}

void GameStateVITA::setDebugCubeTranspose(gef::Matrix44 matrix)
{
	debugCube.set_transform(matrix);
}
