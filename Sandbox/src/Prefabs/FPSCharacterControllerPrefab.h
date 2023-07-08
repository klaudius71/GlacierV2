#ifndef _CHARACTER_CONTROLLER_PREFAB
#define _CHARACTER_CONTROLLER_PREFAB

#include "GlacierAPI.h"
#include "FPSCharacterControllerScript.h"

struct FPSCharacterControllerPrefab : public Prefab
{
	virtual GameObject ApplyPrefab(Scene& scn) override
	{
		GameObject character = scn.CreateGameObject("Character");
		const glm::ivec2& screen_size = Renderer::GetMainFramebuffer().GetSize();
		character->EmplaceComponent<CameraComponent>(glm::perspective(glm::radians(90.0f), (float)screen_size.x / (float)screen_size.y, 0.1f, 10000.0f));
		character->EmplaceComponent<CharacterControllerComponent>();
		//character->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
		//character->EmplaceComponent<MaterialComponent>(MaterialComponent(VertexTypes::PhongADS(), TextureLoader::Get("default")));
		character->AddScript<FPSCharacterControllerScript>();
		character->GetComponent<TransformComponent>().scale() = glm::vec3(1.0f);
		
		return character;
	}
};

#endif
