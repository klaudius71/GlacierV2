#ifndef _GAMEOBJECT
#define _GAMEOBJECT

#include "Components.h"
#include "RegistrationState.h"
#include "ScriptAtt.h"
#include "AddScriptCmd.h"

class Scene;

class GameObject final
{
public:
	GameObject() = delete;
	GameObject(std::string& name);
	GameObject(std::string& name, GameObjectRef& parent);
	GameObject(const GameObject& o) = delete;
	GameObject& operator=(const GameObject& o) = delete;
	GameObject(GameObject&& o) = delete;
	GameObject& operator=(GameObject&& o) = delete;
	~GameObject();

	const entt::entity& GetID() const;
	const std::vector<GameObjectRef>& GetChildren() const;

	void RegisterToScene();
	void DeregisterFromScene();

	template<class T>
	bool HasComponent()
	{
		return curr_registry->any_of<T>(id);
	}

	template<class T>
	T& GetComponent()
	{
		assert(HasComponent<T>() && "Entity doesn't have this component!");
		return curr_registry->get<T>(id);
	}
	template<class T>
	bool TryGetComponent(T*& component)
	{
		component = curr_registry->try_get<T>(id);
		return component != nullptr;
	}
	
	template<class T, typename... Args>
	T& EmplaceComponent(Args&&... args)
	{
		assert(!HasComponent<T>() && "Entity already has this component!");
		return curr_registry->emplace<T>(id, std::forward<Args>(args)...);
	}

	template<class T>
	void AddScript()
	{
		static_assert(std::is_base_of<Script, T>(), "AddScript() must take in a type derived from Script!");
		assert(!HasComponent<ScriptComponent>() && "Entity already has a script component!");
		Script* const scrpt = new T;
		ScriptAtt::SetGameObject(scrpt, *scene_graph_ref);
		//SceneManager::EnqueueCommand(new AddScriptCmd(this, scrpt));
		curr_registry->emplace<ScriptComponent>(id, scrpt);
	}
	
	template<class T>
	void RemoveComponent()
	{
		static_assert(std::is_same<T, TransformComponent>, "Can't remove an entity's TransformComponent!");
		assert(HasComponent<T>() && "Entity does not have component to remove!");
		curr_registry->remove<T>(id);
	}

private:
	entt::entity id;
	Scene* scene;
	entt::registry* curr_registry;
	REGISTRATION_STATE reg_state;
	SceneGraphRef scene_graph_ref;
	GameObjectRef parent;
	std::vector<GameObjectRef> children;
	
	void register_to_scene();
	void deregister_from_scene();

	void update_transform();
	void update_transform_as_child(const glm::mat4& parent_world_matrix);

	// Scene Graph
	void SetSceneGraphRef(SceneGraphRef ref);
	SceneGraphRef& GetSceneGraphRef();
	
	std::vector<GameObjectRef>& GetChildren();

	// Attorney
	friend class GameObjectAtt;
};

#endif _GAMEOBJECT