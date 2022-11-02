#ifndef _SCENE_GRAPH
#define _SCENE_GRAPH

class Entity;

using SceneGraphRef = std::list<std::shared_ptr<Entity>>::iterator;
using GameObject = WeakPtr<Entity>;

class SceneGraph
{
public:
	SceneGraph() = default;
	SceneGraph(const SceneGraph&) = delete;
	SceneGraph& operator=(const SceneGraph&) = delete;
	SceneGraph(SceneGraph&&) = delete;
	SceneGraph& operator=(SceneGraph&&) = delete;
	~SceneGraph();

	const std::list<std::shared_ptr<Entity>>& GetGraph() const;

	GameObject CreateGameObject(std::string& name);
	GameObject CreateGameObject(std::string& name, GameObject& parent, bool keep_world);
	void EraseGameObject(GameObject& go);

	void UpdateTransforms();

private:
	std::list<std::shared_ptr<Entity>> graph;
};

#endif _SCENE_GRAPH