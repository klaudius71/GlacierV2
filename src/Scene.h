#ifndef _SCENE
#define _SCENE

#include "CommandBroker.h"
#include "SceneGraph.h"

struct CameraComponent;

class Scene
{
public:
	Scene() = default;
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(Scene&&) = delete;
	virtual ~Scene() = default;

	entt::registry& GetRegistry();
	const entt::registry& GetRegistry() const;
	entt::registry& GetRegistryDisabled();
	const entt::registry& GetRegistryDisabled() const;

	GameObjectRef CreateGameObject(std::string name);
	GameObjectRef CreateGameObject(std::string name, GameObjectRef parent, bool keep_world = false);
	void DestroyGameObject(GameObjectRef& go);

	GameObjectRef FindGameObject(const std::string& name);

	void Register(const entt::entity& id);
	void Deregister(const entt::entity& id);

	const CameraComponent& GetActiveCamera() const;

protected:
	virtual void InitializeScene() {}
	virtual void EndScene() {}

private:
	entt::registry registry;
	entt::registry registry_disabled;
	CommandBroker cmd_broker;
	SceneGraph scn_graph;

	void Destroy(const entt::entity& id);

	static const CameraComponent default_camera;

	static void switch_entity_registry(const entt::entity& id, entt::registry& from, entt::registry& to);

	const entt::entity CreateEmpty(std::string& name);

	friend class SceneManager;
	friend class GameObject;
};

#endif _SCENE