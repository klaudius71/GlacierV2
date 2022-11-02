#include "gpch.h"
#include "SceneGraph.h"
#include "EntityAtt.h"
#include "Entity.h"

SceneGraph::~SceneGraph()
{
	while (!graph.empty())
		graph.pop_front();
}

const std::list<std::shared_ptr<Entity>>& SceneGraph::GetGraph() const
{
	return graph;
}

GameObject SceneGraph::CreateGameObject(std::string& name)
{
	auto it = graph.emplace(graph.end(), std::make_shared<Entity>(name));
	EntityAtt::SetSceneGraphRef(**it, it);
	return *it;
}
GameObject SceneGraph::CreateGameObject(std::string& name, GameObject& parent, bool keep_world)
{
	UNREFERENCED_PARAMETER(keep_world);

	auto it = graph.emplace(graph.end(), std::make_shared<Entity>(name, parent));
	EntityAtt::SetSceneGraphRef(**it, it);

	auto& parent_children = EntityAtt::GetChildren(**parent);
	auto parent_children_it = parent_children.emplace(parent_children.cend(), *it);
	EntityAtt::SetAsChildRef(**it, parent_children_it);
	return *it;
}
void SceneGraph::EraseGameObject(GameObject& go)
{
	GameObject parent = go->GetParent();
	if (!parent.isExpired())
		EntityAtt::EraseChild(**parent, EntityAtt::GetAsChildRef(**go));
	
	graph.erase(EntityAtt::GetSceneGraphRef(**go));
}

void SceneGraph::UpdateTransforms()
{
	for (auto& go : graph)
		EntityAtt::update_transform(*go);
}