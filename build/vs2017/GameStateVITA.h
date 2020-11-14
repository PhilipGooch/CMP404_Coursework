#pragma once

#include "GameState.h"

class GameStateVITA : public GameState
{
public:
		GameStateVITA(gef::Platform* platform,
		gef::InputManager* input_manager,
		gef::AudioManager* audio_manager,
		gef::SpriteRenderer* sprite_renderer,
		gef::Renderer3D* renderer_3D,
		gef::Font* font,
		std::vector<gef::Mesh*> meshes);

	bool HandleInput() override;
	void Update(float delta_time) override;
	void Render() override;

private:
	void UpdateMarkers() override;

	gef::Matrix44 projection_matrix_;
	gef::Matrix44 view_matrix_;

	int number_of_cows_;
	std::vector<Boid*> cows_;

	int targeted_marker_ID;
	int selected_marker_ID;
	int previous_marker_ID;

	gef::Matrix44 markers_[6];
	gef::Matrix44 previous_marker_;
	gef::Matrix44 selected_marker_;

	bool visible_markers_[6];

	gef::MeshInstance marker_mesh_instances_[6];
	gef::MeshInstance selected_marker_mesh_instance;
	gef::MeshInstance targeted_marker_mesh_instance;

	int anchorX = 0;
	int anchorZ = 0;

	// VITA_MODE
	//	gef::Matrix44 ortho_matrix_;
	//	gef::Sprite camera_feed_sprite_;
	//	gef::TextureVita* camera_feed_texture_;

};

