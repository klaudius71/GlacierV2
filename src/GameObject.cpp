#include "gpch.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObjectRegCmd.h"
#include "GameObjectDeregCmd.h"

GameObject::GameObject(std::string& name)
	: reg_state(REGISTRATION_STATE::CURRENTLY_DEREGISTERED)
{
	scene = SceneManager::GetCurrentScene();
	id = scene->CreateEmpty(name);
	curr_registry = &scene->GetRegistryDisabled();
}
GameObject::GameObject(std::string& name, GameObjectRef& parent)
	: reg_state(REGISTRATION_STATE::CURRENTLY_DEREGISTERED), parent(parent)
{
	scene = SceneManager::GetCurrentScene();
	id = scene->CreateEmpty(name);
	curr_registry = &scene->GetRegistryDisabled();
}
GameObject::~GameObject()
{
	scene->Destroy(id);
}

const entt::entity& GameObject::GetID() const
{
	return id;
}
const std::list<GameObjectRef>& GameObject::GetChildren() const
{
	return children;
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

void GameObject::update_transform()
{
	TransformComponent& transform = GetComponent<TransformComponent>();
	if (transform.flag_changed)
	{
		transform.flag_changed = false;

		const glm::mat4& position = glm::translate(glm::mat4(1.0f), transform.pos);
		const glm::mat4& rot_yxz = glm::rotate(glm::rotate(glm::rotate(glm::mat4(1.0f), transform.rot.z, glm::vec3(0.0f, 0.0f, 1.0f)), transform.rot.x, glm::vec3(1.0f, 0.0f, 0.0f)), transform.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4& scale = glm::scale(glm::mat4(1.0f), transform.scl);
		const glm::mat4& world = position * rot_yxz * scale;

		if (!parent.isExpired())
			transform.world_matrix = parent->GetComponent<TransformComponent>().world_matrix * world;
		else
			transform.world_matrix = world;

		for(auto& child : children)
			child->update_transform_as_child(transform.world_matrix);
	}
}
void GameObject::update_transform_as_child(const glm::mat4& parent_world_matrix)
{
	TransformComponent& transform = GetComponent<TransformComponent>();
	transform.flag_changed = false;

	const glm::mat4& position = glm::translate(glm::mat4(1.0f), transform.pos);
	const glm::mat4& rot_yxz = glm::rotate(glm::rotate(glm::rotate(glm::mat4(1.0f), transform.rot.z, glm::vec3(0.0f, 0.0f, 1.0f)), transform.rot.x, glm::vec3(1.0f, 0.0f, 0.0f)), transform.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4& scale = glm::scale(glm::mat4(1.0f), transform.scl);
	transform.world_matrix = parent_world_matrix * position * rot_yxz * scale;

	for (auto& child : children)
		child->update_transform_as_child(transform.world_matrix);
}

GameObjectRef& GameObject::GetParent()
{
	return parent;
}
std::list<GameObjectRef>::const_iterator& GameObject::GetAsChildRef()
{
	return as_child_ref;
}
void GameObject::SetAsChildRef(std::list<GameObjectRef>::const_iterator ref)
{
	as_child_ref = ref;
}
std::list<GameObjectRef>& GameObject::GetChildren()
{
	return children;
}

void GameObject::SetSceneGraphRef(SceneGraphRef ref)
{
	scene_graph_ref = ref;
}
SceneGraphRef& GameObject::GetSceneGraphRef()
{
	return scene_graph_ref;
}

void GameObject::EraseChild(std::list<GameObjectRef>::const_iterator ref)
{
	children.erase(ref);
}