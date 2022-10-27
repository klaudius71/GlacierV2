#ifndef _EDITOR_LAYER
#define _EDITOR_LAYER

#include "GameObject.h"

class EditorLayer
{
private:
	static EditorLayer* instance;
	static EditorLayer& Instance()
	{
		if (!instance)
			instance = new EditorLayer;

		return *instance;
	}

	EditorLayer();
	EditorLayer(const EditorLayer&) = delete;
	EditorLayer& operator=(const EditorLayer&) = delete;
	~EditorLayer() = default;

	GameObjectRef selected_go;

	void drawGraph(GameObjectRef go);
	void showEditor();

public:
	static void Initialize() { Instance(); }

	static void ShowEditor() { assert(instance && "EditorLayer not initialized!"); instance->showEditor(); };

	static void Terminate() { delete instance; instance = nullptr; }
};

#endif _EDITOR_LAYER