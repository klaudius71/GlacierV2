#include "gpch.h"
#include "GlacierAPI.h"
#include "MainScene.h"

// Load all assets here
void Glacier::LoadResources()
{
	//ModelLoader::Load("Vampire", "vampire2.gltf");
	ModelLoader::Load("TankBase", "tank_base.gltf");
	ModelLoader::Load("TankWheels", "tank_wheels.gltf");
	ModelLoader::Load("TankTurret", "tank_turret.gltf");
	ModelLoader::Load("TankTurretBarrel", "tank_turret_barrel.gltf");
	ModelLoader::Load("Box", PREMADE_MODELS::UNIT_CUBE_REPEAT_TEXTURE, 10.0f);
	ModelLoader::Load("TempTerrain", "level1_grayscale.png", 3500.0f, 200.0f, 25.0f, 25.0f);
	ModelLoader::Load("TempTerrain1", 3500.0f, 25.0f, 25.0f);

	TextureLoader::Load("Skybox", { "teide/posx.jpg", "teide/negx.jpg", "teide/posy.jpg", "teide/negy.jpg", "teide/posz.jpg", "teide/negz.jpg" });
	TextureLoader::Load("default", "default.tga", { TEXTURE_MIN_FILTER::NEAREST_NEIGHBOR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR });
	TextureLoader::Load("Crate", "crate_diffuse.tga", { TEXTURE_MIN_FILTER::LINEAR_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::LINEAR });
	TextureLoader::Load("Sand", "sand.tga", { TEXTURE_MIN_FILTER::LINEAR_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::LINEAR });
	TextureLoader::Load("Grass", "grass.jpg", { TEXTURE_MIN_FILTER::LINEAR_MIPMAP_LINEAR, TEXTURE_MAG_FILTER::LINEAR });
	TextureLoader::Load("Crosshair", "crosshair.tga", { TEXTURE_MIN_FILTER::NEAREST_NEIGHBOR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR });
	TextureLoader::Load("Desert", glm::vec4(179.0f / 255.0f, 116.0f / 255.0f, 64.0f / 255.0f, 1.0f));

	FontLoader::Load("Times40", "times.ttf", 40);
	FontLoader::Load("CascadiaMono20", "CascadiaMono.ttf", 20);

	SceneManager::SetNextScene(new MainScene);
}