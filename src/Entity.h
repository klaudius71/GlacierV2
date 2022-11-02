#ifndef _GAMEOBJECT
#define _GAMEOBJECT

#include "Components.h"
#include "RegistrationState.h"
#include "ScriptAtt.h"
#include "AddScriptCmd.h"

class Scene;

class Entity final
{
public:
	Entity(std::string& name);
	Entity(std::string& name, GameObject& parent);

	Entity() = delete;
	Entity(const Entity& o) = delete;
	Entity& operator=(const Entity& o) = delete;
	Entity(Entity&& o) = delete;
	Entity& operator=(Entity&& o) = delete;
	~Entity();

	const entt::entity& GetID() const;
	const std::list<GameObject>& GetChildren() const;
	GameObject GetParent();

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
		static_assert(std::is_same<T, NameComponent>, "Can't remove an entity's NameComponent!");
		assert(HasComponent<T>() && "Entity does not have component to remove!");
		curr_registry->remove<T>(id);
	}

private:
	// ECS
	entt::entity id;
	entt::registry* curr_registry;
	// Scene reference
	Scene* scene;
	// Scene registration state
	REGISTRATION_STATE reg_state;
	// Scene Graph
	SceneGraphRef scene_graph_ref;
	GameObject parent;
	std::list<GameObject>::const_iterator as_child_of_ref; // This is the reference that the parent has of this GameObject as a child
	std::list<GameObject> children;
	
	// Scene registration
	void register_to_scene();
	void deregister_from_scene();

	void update_transform();
	void update_transform_as_child(const glm::mat4& parent_world_matrix);

	// Scene Graph
	void SetSceneGraphRef(SceneGraphRef ref);
	SceneGraphRef& GetSceneGraphRef();
	void EraseChild(std::list<GameObject>::const_iterator ref);
	std::list<GameObject>::const_iterator& GetAsChildRef();
	void SetAsChildRef(std::list<GameObject>::const_iterator ref);
	std::list<GameObject>& GetChildren();

	// Attorney
	friend class EntityAtt;
};

#endif _GAMEOBJECT