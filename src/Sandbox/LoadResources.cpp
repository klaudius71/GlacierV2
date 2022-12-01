#include "gpch.h"
#include "GlacierAPI.h"
#include "MainScene.h"

// Load all assets here
void Glacier::LoadResources()
{
	//ModelLoader::Load("Vampire", "vampire2.gltf");
	ModelLoader::Load("Dude", "dude.gltf");
	ModelLoader::Load("TankBase", "tank_base.gltf");
	ModelLoader::Load("TankWheels", "tank_wheels.gltf");
	ModelLoader::Load("TankTurret", "tank_turret.gltf");
	ModelLoader::Load("TankTurretBarrel", "tank_turret_barrel.gltf");
	ModelLoader::Load("Box", PREMADE_MODELS::UNIT_CUBE_REPEAT_TEXTURE, 10.0f);
	ModelLoader::Load("Sphere", 20, 20);
	//ModelLoader::Load("Sphere2", 4, 4);
	ModelLoader::Load("TempTerrain", "level1_grayscale.png", 3500.0f, 200.0f, 25.0f, 25.0f);
	ModelLoader::Load("TempTerrain1", 3500.0f, 25.0f, 25.0f);

	TextureLoader::Load("Skybox", { "teide/posx.jpg", "teide/negx.jpg", "teide/posy.jpg", "teide/negy.jpg", "teide/posz.jpg", "teide/negz.jpg" });
	TextureLoader::Load("default", "default.tga", { TEXTURE_MIN_FILTER::NEAREST_NEIGHBOR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR });
	TextureLoader::Load("default_normal", glm::vec4{ 0.5f, 0.5f, 1.0f, 1.0f } );
	TextureLoader::Load("Crate", "crate_diffuse.tga");
	TextureLoader::Load("CrateNormal", "crate_normal.tga", { TEXTURE_MIN_FILTER::NEAREST_NEIGHBOR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR });
	TextureLoader::Load("Sand", "sand.tga");
	TextureLoader::Load("Grass", "grass.jpg");
	TextureLoader::Load("Crosshair", "crosshair.tga", { TEXTURE_MIN_FILTER::NEAREST_NEIGHBOR, TEXTURE_MAG_FILTER::NEAREST_NEIGHBOR });
	TextureLoader::Load("Desert", glm::vec4(179.0f / 255.0f, 116.0f / 255.0f, 64.0f / 255.0f, 1.0f));
	TextureLoader::Load("Mahogany", glm::vec4(103 / 255.0f, 10 / 255.0f, 10 / 255.0f, 1.0f));
	TextureLoader::Load("Rock", "strata-rock-2_albedo.tga");
	TextureLoader::Load("RockNormal", "strata-rock-2_normal-ogl.tga");
	TextureLoader::Load("VampireDiffuse", "Vampire_diffuse.png");
	TextureLoader::Load("VampireNormal", "Vampire_normal.png");
	TextureLoader::Load("Dude", "dude_packed0_diffuse.png");
	TextureLoader::Load("DudeNormal", "dude_packed0_normal.png");

	FontLoader::Load("Times40", "times.ttf", 40);
	FontLoader::Load("CascadiaMono20", "CascadiaMono.ttf", 20);

	//SkeletalAnimationLoader::Load("VampireDancing", "vampire_anim_2.gltf");
	SkeletalAnimationLoader::Load("VampireIdle", "vampire_standing idle.gltf");
	SkeletalAnimationLoader::Load("DudeIdle", "dude_standing idle.gltf");

	SceneManager::SetNextScene(new MainScene);
}