#ifndef _GAMEOBJECT
#define _GAMEOBJECT

#include "Scene.h"
#include "Components.h"
#include "SceneManager.h"
#include "RegistrationState.h"
#include "ScriptAtt.h"
#include "AddScriptCmd.h"

class GameObject final
{
public:
	GameObject() = delete;
	GameObject(std::string name = "Default Entity", GameObject* const parent = nullptr, const bool keep_transform = false);
	GameObject(const GameObject& o);
	GameObject& operator=(const GameObject& o);
	GameObject(GameObject&& o);
	GameObject& operator=(GameObject&& o);
	~GameObject();

	const entt::entity& GetID();

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
		ScriptAtt::SetGameObject(scrpt, this);
		//SceneManager::EnqueueCommand(new AddScriptCmd(this, scrpt));
		curr_registry->emplace<ScriptComponent>(id, scrpt);
	}
	
	template<class T>
	void RemoveComponent()
	{
		assert(HasComponent<T>() && "Entity does not have component to remove!");
		curr_registry->remove<T>(id);
	}

private:
	void register_to_scene();
	void deregister_from_scene();

	friend class GameObjectAtt;

	entt::entity id;
	Scene* scene;
	entt::registry* curr_registry;
	REGISTRATION_STATE reg_state;
};

#endif _GAMEOBJECT