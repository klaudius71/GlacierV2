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
	const std::list<GameObjectRef>& GetChildren() const;
	GameObjectRef GetParent();

	void RegisterToScene();
	void DeregisterFromScene();

	template<class T>
	bool HasComponent() const
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
	const T& GetComponent() const
	{
		assert(HasComponent<T>() && "Entity doesn't have this component!");
		return curr_registry->get<T>(id);
	}
	template<typename... Component>
	auto TryGetComponent()
	{
		return curr_registry->try_get<Component...>(id);
	}
	
	template<class T, typename... Args>
	T& EmplaceComponent(Args&&... args)
	{
		assert(!HasComponent<T>() && "Entity already has this component!");
		return curr_registry->emplace<T>(id, std::forward<Args>(args)...);
	}

	//template<class T>
	//void AddScript()
	//{
	//	static_assert(std::is_base_of<Script, T>(), "AddScript() must take in a type derived from Script!");
	//	assert(!HasComponent<ScriptComponent>() && "Entity already has a script component!");
	//	Script* const scrpt = new T;
	//	ScriptAtt::SetGameObject(scrpt, *scene_graph_ref);
	//	//SceneManager::EnqueueCommand(new AddScriptCmd(this, scrpt));
	//	curr_registry->emplace<ScriptComponent>(id, scrpt);
	//}

	template<class T, typename... Args>
	void AddScript(Args&&... args)
	{
		static_assert(std::is_base_of<Script, T>(), "AddScript() must take in a type derived from Script!");
		assert(!HasComponent<ScriptComponent>() && "Entity already has a script component!");
		Script* const scrpt = new T(std::forward<Args>(args)...);
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
	std::list<GameObjectRef>::const_iterator as_child_ref;
	std::list<GameObjectRef> children;
	
	void register_to_scene();
	void deregister_from_scene();

	void update_transform();
	void update_transform_as_child(const glm::mat4& parent_world_matrix);

	// Scene Graph
	void SetSceneGraphRef(SceneGraphRef ref);
	SceneGraphRef& GetSceneGraphRef();

	void EraseChild(std::list<GameObjectRef>::const_iterator ref);
	
	std::list<GameObjectRef>::const_iterator& GetAsChildRef();
	void SetAsChildRef(std::list<GameObjectRef>::const_iterator ref);
	std::list<GameObjectRef>& GetChildren();

	// Attorney
	friend class GameObjectAtt;
};

#endif _GAMEOBJECT