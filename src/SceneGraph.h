#ifndef _SCENE_GRAPH
#define _SCENE_GRAPH

class GameObject;

using SceneGraphRef = std::list<std::shared_ptr<GameObject>>::iterator;
using GameObjectRef = WeakPtr<GameObject>;

class SceneGraph
{
public:
	SceneGraph() = default;
	SceneGraph(const SceneGraph&) = delete;
	SceneGraph& operator=(const SceneGraph&) = delete;
	SceneGraph(SceneGraph&&) = delete;
	SceneGraph& operator=(SceneGraph&&) = delete;
	~SceneGraph();

	const std::list<std::shared_ptr<GameObject>>& GetGraph() const;

	GameObjectRef CreateGameObject(std::string& name);
	GameObjectRef CreateGameObject(std::string& name, GameObjectRef& parent, bool keep_world);
	void EraseGameObject(GameObjectRef& go);

	void UpdateTransforms();

private:
	std::list<std::shared_ptr<GameObject>> graph;
};

#endif _SCENE_GRAPH