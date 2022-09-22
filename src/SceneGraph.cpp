#include "gpch.h"
#include "SceneGraph.h"
#include "GameObjectAtt.h"
#include "GameObject.h"

SceneGraph::~SceneGraph()
{
	for (auto& go : graph)
		delete go;
}

GameObject& SceneGraph::CreateGameObject(std::string& name)
{
	auto it = graph.emplace(graph.end(), new GameObject(name));
	GameObjectAtt::SetSceneGraphRef(**it, it);
	return **it;
}
GameObject& SceneGraph::CreateGameObject(std::string& name, GameObject& parent, bool keep_world)
{
	UNREFERENCED_PARAMETER(keep_world);

	auto it = graph.emplace(graph.end(), new GameObject(name, &parent));
	GameObjectAtt::SetSceneGraphRef(**it, it);
	GameObjectAtt::GetChildren(parent).emplace_back(&(**it));
	return **it;
}
void SceneGraph::EraseGameObject(GameObject* const go)
{
	graph.erase(GameObjectAtt::GetSceneGraphRef(*go));
	delete go;
}

void SceneGraph::UpdateTransforms()
{
	for (auto& go : graph)
		GameObjectAtt::update_transform(*go);
}
