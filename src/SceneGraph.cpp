#include "gpch.h"
#include "SceneGraph.h"
#include "GameObjectAtt.h"
#include "GameObject.h"

SceneGraph::~SceneGraph()
{
	while (!graph.empty())
		graph.pop_front();
}

const std::list<std::shared_ptr<GameObject>>& SceneGraph::GetGraph() const
{
	return graph;
}

GameObjectRef SceneGraph::CreateGameObject(std::string& name)
{
	auto it = graph.emplace(graph.end(), std::make_shared<GameObject>(name));
	GameObjectAtt::SetSceneGraphRef(**it, it);
	return *it;
}
GameObjectRef SceneGraph::CreateGameObject(std::string& name, GameObjectRef& parent, bool keep_world)
{
	UNREFERENCED_PARAMETER(keep_world);

	auto it = graph.emplace(graph.end(), std::make_shared<GameObject>(name, parent));
	GameObjectAtt::SetSceneGraphRef(**it, it);

	auto& parent_children = GameObjectAtt::GetChildren(**parent);
	auto parent_children_it = parent_children.emplace(parent_children.cend(), *it);
	GameObjectAtt::SetAsChildRef(**it, parent_children_it);
	return *it;
}
void SceneGraph::EraseGameObject(GameObjectRef& go)
{
	GameObjectRef& parent = GameObjectAtt::GetParent(**go);
	if (!parent.isExpired())
		GameObjectAtt::EraseChild(**parent, GameObjectAtt::GetAsChildRef(**go));
	
	graph.erase(GameObjectAtt::GetSceneGraphRef(**go));
}

void SceneGraph::UpdateTransforms()
{
	for (auto& go : graph)
		GameObjectAtt::update_transform(*go);
}
