#include "gpch.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameObjectRegCmd.h"
#include "GameObjectDeregCmd.h"

GameObject::GameObject(std::string name, GameObject* const parent, const bool keep_transform)
	: reg_state(REGISTRATION_STATE::CURRENTLY_DEREGISTERED)
{
	scene = SceneManager::GetCurrentScene();
	id = scene->CreateEmpty(name, parent, keep_transform);
	curr_registry = &scene->GetRegistryDisabled();
}
GameObject::GameObject(const GameObject& o)
	: id(o.id), scene(o.scene), reg_state(o.reg_state), curr_registry(o.curr_registry)
{
}
GameObject& GameObject::operator=(const GameObject& o)
{
	id = o.id;
	scene = o.scene;
	reg_state = o.reg_state;
	curr_registry = o.curr_registry;
	return *this;
}
GameObject::GameObject(GameObject&& o)
	: id(o.id), scene(o.scene), reg_state(o.reg_state), curr_registry(o.curr_registry)
{
}
GameObject& GameObject::operator=(GameObject&& o)
{
	id = o.id;
	scene = o.scene;
	reg_state = o.reg_state;
	curr_registry = o.curr_registry;
	return *this;
}
GameObject::~GameObject()
{
	scene->Destroy(id);
}

const entt::entity& GameObject::GetID()
{
	return id;
}

void GameObject::RegisterToScene()
{
	assert(reg_state == REGISTRATION_STATE::CURRENTLY_DEREGISTERED);
	SceneManager::EnqueueCommand(new GameObjectRegCmd(this));
	reg_state = REGISTRATION_STATE::PENDING_REGISTRATION;
}
void GameObject::register_to_scene()
{
	assert(reg_state == REGISTRATION_STATE::PENDING_REGISTRATION);
	SceneManager::GetCurrentScene()->Register(id);
	curr_registry = &SceneManager::GetCurrentScene()->GetRegistry();
	reg_state = REGISTRATION_STATE::CURRENTLY_REGISTERED;
	if (HasComponent<ScriptComponent>())
		GetComponent<ScriptComponent>().script->OnCreate();
}

void GameObject::DeregisterFromScene()
{
	assert(reg_state == REGISTRATION_STATE::CURRENTLY_REGISTERED);
	SceneManager::EnqueueCommand(new GameObjectDeregCmd(this));
	reg_state = REGISTRATION_STATE::PENDING_DEREGISTRATION;
}
void GameObject::deregister_from_scene()
{
	assert(reg_state == REGISTRATION_STATE::PENDING_DEREGISTRATION);
	SceneManager::GetCurrentScene()->Deregister(id);
	curr_registry = &SceneManager::GetCurrentScene()->GetRegistryDisabled();
	reg_state = REGISTRATION_STATE::CURRENTLY_DEREGISTERED;
	if (HasComponent<ScriptComponent>())
		GetComponent<ScriptComponent>().script->OnDestroy();
}