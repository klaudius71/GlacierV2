#include "gpch.h"
#include "GlacierAPI.h"
#include "ModelLoaderAtt.h"
#include "MainScene.h"

// Load all assets here
void Glacier::LoadResources()
{
	//ModelLoader::Load("Vampire", "vampire2.gltf");
	ModelLoader::Load("Box", PREMADE_MODELS::UNIT_CUBE_REPEAT_TEXTURE, 10.0f);
	ModelLoader::Load("TempTerrain", "level1_grayscale.png", 3500.0f, 200.0f, 50.0f, 50.0f);
	ModelLoader::Load("TempTerrain1", 3500.0f, 50.0f, 50.0f);

	TextureLoader::Load("default", "default.tga");
	TextureLoader::Load("Crate", "crate_diffuse.tga");
	TextureLoader::Load("Sand", "sand.tga");
	TextureLoader::Load("Grass", "grass.jpg");
	TextureLoader::Load("Skybox", { "teide/posx.jpg", "teide/negx.jpg", "teide/posy.jpg", "teide/negy.jpg", "teide/posz.jpg", "teide/negz.jpg" });

	SceneManager::SetNextScene(new MainScene);
}