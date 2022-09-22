#ifndef _SCENE_GRAPH
#define _SCENE_GRAPH

class GameObject;

class SceneGraph
{
public:
	SceneGraph() = default;
	SceneGraph(const SceneGraph&) = delete;
	SceneGraph& operator=(const SceneGraph&) = delete;
	SceneGraph(SceneGraph&&) = delete;
	SceneGraph& operator=(SceneGraph&&) = delete;
	~SceneGraph();

	GameObject& CreateGameObject(std::string& name);
	GameObject& CreateGameObject(std::string& name, GameObject& parent, bool keep_world);
	void EraseGameObject(GameObject* const go);

	void UpdateTransforms();

	using SceneGraphRef = std::list<GameObject*>::const_iterator;

private:
	std::list<GameObject*> graph;
};

#endif _SCENE_GRAPH