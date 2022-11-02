#ifndef _EDITOR_LAYER
#define _EDITOR_LAYER

#include "Entity.h"
#include "ShowEditorNull.h"

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
	~EditorLayer();

	ShowEditorNull* show_editor_cmd;

	GameObject selected_go;
	glm::ivec2 viewport_size;

	void newFrame();
	void render();

	void drawGraph(GameObject go);

public:
	void showEditor();
	void showEditorDont();

public:
	static void Initialize() { Instance(); }

	static void NewFrame() { instance->newFrame(); }
	static void ShowEditor() { assert(instance && "EditorLayer not initialized!"); instance->show_editor_cmd->Execute(); };
	static void Render() { instance->render(); }

	static void Terminate() { delete instance; instance = nullptr; }
};

#endif _EDITOR_LAYER