#ifndef _CHARACTER_CONTROLLER_PREFAB
#define _CHARACTER_CONTROLLER_PREFAB

#include "GlacierAPI.h"
#include "FPSCharacterControllerScript.h"

struct FPSCharacterControllerPrefab : public Prefab
{
	virtual GameObject ApplyPrefab(Scene& scn) override
	{
		GameObject character = scn.CreateGameObject("Character");
		character->EmplaceComponent<CameraComponent>();
		character->EmplaceComponent<CharacterControllerComponent>();
		character->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
		character->EmplaceComponent<MaterialComponent>(MaterialComponent(VertexTypes::PhongADS(), TextureLoader::Get("default")));
		character->AddScript<FPSCharacterControllerScript>();
		character->GetComponent<TransformComponent>().scale() = glm::vec3(1.0f);
		
		return character;
	}
};

#endif
