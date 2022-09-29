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
	GameObjectAtt::GetChildren(**parent).emplace_back(*it);
	return *it;
}
void SceneGraph::EraseGameObject(GameObjectRef& go)
{
	graph.erase(GameObjectAtt::GetSceneGraphRef(**go));
}

void SceneGraph::UpdateTransforms()
{
	for (auto& go : graph)
		GameObjectAtt::update_transform(*go);
}
