#include "gpch.h"
#include "GameObject.h"
#include "SceneManager.h"
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
	std::weak_ptr<GameObject> woah;
	scene->Destroy(this->id);
	parent = nullptr;
	children.clear();
}

const entt::entity& GameObject::GetID() const
{
	return id;
}
const std::vector<GameObjectRef>& GameObject::GetChildren() const
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
		const glm::mat4& rot_yxz = glm::rotate(glm::mat4(1.0f), transform.rot.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), transform.rot.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), transform.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
		const glm::mat4& scale = glm::scale(glm::mat4(1.0f), transform.scl);
		transform.world_matrix = position * rot_yxz * scale;

		if (!parent.isExpired())
			transform.world_matrix = parent->GetComponent<TransformComponent>().world_matrix * transform.world_matrix;

		for (auto& child : children)
			child->update_transform_as_child(transform.world_matrix);
	}
}
void GameObject::update_transform_as_child(const glm::mat4& parent_world_matrix)
{
	TransformComponent& transform = GetComponent<TransformComponent>();

	transform.flag_changed = false;

	const glm::mat4& position = glm::translate(glm::mat4(1.0f), transform.pos);
	const glm::mat4& rot_yxz = glm::rotate(glm::mat4(1.0f), transform.rot.y, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), transform.rot.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), transform.rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
	const glm::mat4& scale = glm::scale(glm::mat4(1.0f), transform.scl);
	transform.world_matrix = parent_world_matrix * position * rot_yxz * scale;

	for (auto& child : children)
		child->update_transform_as_child(transform.world_matrix);
}

std::vector<GameObjectRef>& GameObject::GetChildren()
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