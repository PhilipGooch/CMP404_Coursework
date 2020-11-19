#pragma once

#include <system/application.h>
#include <vector>
#include <map>
#include <string>

namespace gef
{
	class Platform;
	class InputManager;
	class AudioManager;
	class SpriteRenderer;
	class Renderer3D;
	class Font;

	class Material;
	class Mesh;
}
class StateMachine;
class PrimitiveBuilder;

class App : public gef::Application
{
public:
	App(gef::Platform& platform);

	void Init();
	void CleanUp();
	bool Update(float delta_time) override;
	void Render() override;

private:
	gef::Material* LoadMaterial(char* file_name);
	void LoadMaterials();
	void CreateMeshes();
	void LoadAudio();

	gef::InputManager* input_manager_;
	gef::AudioManager* audio_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::Renderer3D* renderer_3D_;
	gef::Font* font_;
	PrimitiveBuilder* primitive_builder_;

	float fps_;

	StateMachine* state_machine_;

	std::map<std::string, gef::Material*> materials_map_;
	std::map<gef::Mesh*, std::string> meshes_map_;

	std::vector<gef::Material*> materials_;
	std::vector<gef::Mesh*> meshes_;

	std::vector<int> sfx_IDs_;
};

