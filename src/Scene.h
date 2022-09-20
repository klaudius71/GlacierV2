#ifndef _SCENE
#define _SCENE

#include "CommandBroker.h"

class GameObject;
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

	const entt::entity CreateEmpty(std::string& name, GameObject* const parent, const bool keep_transform);

	void Register(const entt::entity& id);
	void Deregister(const entt::entity& id);
	void Destroy(const entt::entity& id);

	const CameraComponent& GetActiveCamera() const;

private:
	entt::registry registry;
	entt::registry registry_disabled;
	CommandBroker cmd_broker;
	
	static const CameraComponent default_camera;

	static void switch_entity_registry(const entt::entity& id, entt::registry& from, entt::registry& to);

	virtual void InitializeScene() {}
	virtual void EndScene() {}

	friend class SceneManager;
};

#endif _SCENE