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

	GameObject* const CreateGameObject(std::string& name, GameObject* const parent = nullptr, bool keep_world = false);
	void EraseGameObject(GameObject* const go);

	void UpdateTransforms();

private:
	std::vector<GameObject*> graph;
};

#endif _SCENE_GRAPH