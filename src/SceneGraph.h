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

	GameObject CreateGameObject(const std::string& name);
	GameObject CreateGameObject(const std::string& name, GameObject& parent);
	void EraseGameObject(GameObject& go);

	GameObject const FindGameObject(const std::string& name) const;

	void UpdateTransforms();

private:
	std::unordered_multimap<std::string, uint64_t> uuids;
	std::list<std::shared_ptr<Entity>> graph;
};

#endif _SCENE_GRAPH