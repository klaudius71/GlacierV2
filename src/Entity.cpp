#include "gpch.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EntityRegCmd.h"
#include "EntityDeregCmd.h"

Entity::Entity(std::string& name)
	: reg_state(REGISTRATION_STATE::CURRENTLY_DEREGISTERED)
{
	scene = SceneManager::GetCurrentScene();
	id = scene->CreateEmpty(name);
	curr_registry = &scene->GetRegistryDisabled();
}
Entity::Entity(std::string& name, GameObject& parent)
	: reg_state(REGISTRATION_STATE::CURRENTLY_DEREGISTERED), parent(parent)
{
	scene = SceneManager::GetCurrentScene();
	id = scene->CreateEmpty(name);
	curr_registry = &scene->GetRegistryDisabled();
}
Entity::~Entity()
{
	scene->Destroy(id);
}

const entt::entity& Entity::GetID() const
{
	return id;
}
const std::list<GameObject>& Entity::GetChildren() const
{
	return children;
}
const size_t Entity::GetChildrenCount() const
{
	return children.size();
}
GameObject Entity::GetChild(int index) const
{
	assert(index >= 0 && index < children.size() && "Child index out of range!");
	auto it = children.begin();
	std::advance(it, index);
	return *it;
}
GameObject Entity::GetParent()
{
	return parent;
}

void Entity::RegisterToScene()
{
	assert(reg_state == REGISTRATION_STATE::CURRENTLY_DEREGISTERED);
	SceneManager::EnqueueCommand(new EntityRegCmd(this));
	reg_state = REGISTRATION_STATE::PENDING_REGISTRATION;
}
void Entity::register_to_scene()
{
	assert(reg_state == REGISTRATION_STATE::PENDING_REGISTRATION);
	SceneManager::GetCurrentScene()->Register(id);
	curr_registry = &SceneManager::GetCurrentScene()->GetRegistry();
	reg_state = REGISTRATION_STATE::CURRENTLY_REGISTERED;
	if (HasComponent<ScriptComponent>())
		GetComponent<ScriptComponent>().script->OnSceneEnter();
}

void Entity::DeregisterFromScene()
{
	assert(reg_state == REGISTRATION_STATE::CURRENTLY_REGISTERED);
	SceneManager::EnqueueCommand(new EntityDeregCmd(this));
	reg_state = REGISTRATION_STATE::PENDING_DEREGISTRATION;
}
void Entity::deregister_from_scene()
{
	assert(reg_state == REGISTRATION_STATE::PENDING_DEREGISTRATION);
	SceneManager::GetCurrentScene()->Deregister(id);
	curr_registry = &SceneManager::GetCurrentScene()->GetRegistryDisabled();
	reg_state = REGISTRATION_STATE::CURRENTLY_DEREGISTERED;
	if (HasComponent<ScriptComponent>())
		GetComponent<ScriptComponent>().script->OnSceneExit();
}

void Entity::update_transform()
{
	TransformComponent& transform = GetComponent<TransformComponent>();
	if (transform.flag_changed)
	{
		transform.flag_changed = false;

		const float pi = glm::pi<float>();
		const float two_pi = pi * 2.0f;
		if (transform.rot.x < -pi)
			transform.rot.x += two_pi;
		else if (transform.rot.x > pi)
			transform.rot.x -= two_pi;
		if (transform.rot.y < -pi)
			transform.rot.y += two_pi;
		else if (transform.rot.y > pi)
			transform.rot.y -= two_pi;
		if (transform.rot.z < -pi)
			transform.rot.z += two_pi;
		else if (transform.rot.z > pi)
			transform.rot.z -= two_pi;

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
void Entity::update_transform_as_child(const glm::mat4& parent_world_matrix)
{
	TransformComponent& transform = GetComponent<TransformComponent>();
	transform.flag_changed = false;

	const float pi = glm::pi<float>();
	const float two_pi = pi * 2.0f;
	if (transform.rot.x < -pi)
		transform.rot.x += two_pi;
	else if (transform.rot.x > pi)
		transform.rot.x -= two_pi;
	if (transform.rot.y < -pi)
		transform.rot.y += two_pi;
	else if (transform.rot.y > pi)
		transform.rot.y -= two_pi;
	if (transform.rot.z < -pi)
		transform.rot.z += two_pi;
	else if (transform.rot.z > pi)
		transform.rot.z -= two_pi;

	const glm::mat4& position = glm::translate(glm::mat4(1.0f), transform.pos);
	const glm::mat4& rot_yxz = glm::rotate(glm::rotate(glm::rotate(glm::mat4(1.0f), transform.rot.z, glm::vec3(0.0f, 0.0f, 1.0f)), transform.rot.x, glm::vec3(1.0f, 0.0f, 0.0f)), transform.rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4& scale = glm::scale(glm::mat4(1.0f), transform.scl);
	transform.world_matrix = parent_world_matrix * position * rot_yxz * scale;

	for (auto& child : children)
		child->update_transform_as_child(transform.world_matrix);
}

std::list<GameObject>::const_iterator& Entity::GetAsChildRef()
{
	return as_child_of_ref;
}
void Entity::SetAsChildRef(std::list<GameObject>::const_iterator ref)
{
	as_child_of_ref = ref;
}

void Entity::SetSceneGraphRef(SceneGraphRef ref)
{
	scene_graph_ref = ref;
}
SceneGraphRef& Entity::GetSceneGraphRef()
{
	return scene_graph_ref;
}

void Entity::EraseChild(std::list<GameObject>::const_iterator ref)
{
	children.erase(ref);
}