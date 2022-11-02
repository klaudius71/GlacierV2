#include "GlacierAPI.h"

struct PlayerTankPrefab : public Prefab
{
	virtual GameObject ApplyPrefab(Scene& scn) override
	{
		GameObject base = scn.CreateGameObject("Tank");
		base->EmplaceComponent<MeshComponent>(ModelLoader::Get("Box"));
		base->EmplaceComponent<MaterialComponent>(VertexTypes::PhongADS(), 0, Colors::White);
		return base;
	}
};