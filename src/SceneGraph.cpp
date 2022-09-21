#include "gpch.h"
#include "SceneGraph.h"
#include "GameObjectAtt.h"

SceneGraph::~SceneGraph()
{
	for (auto go : graph)
		delete go;
}

GameObject* const SceneGraph::CreateGameObject(std::string& name, GameObject* const parent, bool keep_world)
{
	keep_world;

	auto& go = graph.emplace_back(new GameObject(name, parent));
	if (parent)
		GameObjectAtt::GetChildren(parent).emplace_back(go);

	return go;
}
void SceneGraph::EraseGameObject(GameObject* const go)
{
	for (auto it = graph.cbegin(); it != graph.cend(); it++)
		if (*it == go)
			graph.erase(it);
}

void SceneGraph::UpdateTransforms()
{
	for (auto& go : graph)
		GameObjectAtt::update_transform(go);
}
