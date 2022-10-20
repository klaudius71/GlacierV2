#include "gpch.h"
#include "Scene.h"
#include "Components.h"
#include "GameObject.h"
#include "Glacier.h"
#include "Window.h"

const CameraComponent Scene::default_camera = CameraComponent(glm::perspective(glm::radians(70.0f), 16.0f / 9.0f, 0.1f, 10000.0f));
													
entt::registry& Scene::GetRegistry()
{
	return registry;
}
const entt::registry& Scene::GetRegistry() const
{
	return registry;
}
entt::registry& Scene::GetRegistryDisabled()
{
	return registry_disabled;
}
const entt::registry& Scene::GetRegistryDisabled() const
{
	return registry_disabled;
}

GameObjectRef Scene::CreateGameObject(std::string name)
{
	return scn_graph.CreateGameObject(name);
}
GameObjectRef Scene::CreateGameObject(std::string name, GameObjectRef parent, bool keep_world)
{
	return scn_graph.CreateGameObject(name, parent, keep_world);
}
void Scene::DestroyGameObject(GameObjectRef& go)
{
	scn_graph.EraseGameObject(go);
}

GameObjectRef Scene::FindGameObject(const std::string& name)
{
	size_t name_hash = std::hash<std::string>()(name);
	for (auto& go : scn_graph.GetGraph())
	{
		NameComponent& name_comp = go->GetComponent<NameComponent>();
		if (name_comp.id == name_hash)
			return GameObjectRef(go);
	}
	return GameObjectRef();
}

const entt::entity Scene::CreateEmpty(std::string& name)
{
	entt::entity temp = registry.create(); temp;
	entt::entity ent = registry_disabled.create();
	assert(ent == temp);
	registry_disabled.emplace<NameComponent>(ent, std::move(name));
	registry_disabled.emplace<TransformComponent>(ent);
	return ent;
}

void Scene::Register(const entt::entity& id)
{
	assert(registry_disabled.valid(id) && registry.valid(id) && "Entity not valid!");
	switch_entity_registry(id, registry_disabled, registry);
}
void Scene::Deregister(const entt::entity& id)
{
	assert(registry.valid(id) && registry_disabled.valid(id) && "Entity not valid!");
	switch_entity_registry(id, registry, registry_disabled);
}

void Scene::Destroy(const entt::entity& id)
{
	ScriptComponent* script = registry.try_get<ScriptComponent>(id);
	if (script) 
	{
		script->script->OnDestroy();
		delete script->script;
	}
	else 
	{
		script = registry_disabled.try_get<ScriptComponent>(id);
		if (script) {
			script->script->OnDestroy();
			delete script->script;
		}
	}

	registry.destroy(id);
	registry_disabled.destroy(id);
}

const CameraComponent& Scene::GetActiveCamera() const
{
	auto camera_view = registry.view<CameraComponent>();
	const bool& camera_exists = camera_view.begin() != camera_view.end();
	return camera_exists ? registry.get<CameraComponent>(*camera_view.begin()) : default_camera;
}

void Scene::switch_entity_registry(const entt::entity& id, entt::registry& from, entt::registry& to)
{
	auto [name, transform, camera, script, render2d, render, material, dir_light, skybox] = from.try_get<NameComponent, TransformComponent, CameraComponent, ScriptComponent, SpriteComponent, MeshComponent, MaterialComponent, DirectionalLightComponent, SkyboxComponent>(id);
	assert(name && transform);

	to.emplace<NameComponent>(id, std::move(*name));
	to.emplace<TransformComponent>(id, std::move(*transform));
	from.erase<NameComponent, TransformComponent>(id);
	if (camera)
	{
		to.emplace<CameraComponent>(id, std::move(*camera));
		from.erase<CameraComponent>(id);
	}
	if (script)
	{
		to.emplace<ScriptComponent>(id, std::move(*script));
		from.erase<ScriptComponent>(id);
	}
	if (render2d)
	{
		to.emplace<SpriteComponent>(id, std::move(*render2d));
		from.erase<SpriteComponent>(id);
	}
	if (render)
	{
		to.emplace<MeshComponent>(id, std::move(*render));
		from.erase<MeshComponent>(id);
	}
	if (material)
	{
		to.emplace<MaterialComponent>(id, std::move(*material));
		from.erase<MaterialComponent>(id);
	}
	if (dir_light)
	{
		to.emplace<DirectionalLightComponent>(id, std::move(*dir_light));
		from.erase<DirectionalLightComponent>(id);
	}
	if (skybox)
	{
		to.emplace<SkyboxComponent>(id, std::move(*skybox));
		from.erase<SkyboxComponent>(id);
	}
}