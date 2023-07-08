#ifndef _SHOW_EDITOR
#define _SHOW_EDITOR

class EditorLayer;

class ShowEditorNull
{
public:
	ShowEditorNull() = delete;
	ShowEditorNull(EditorLayer& el);
	ShowEditorNull(const ShowEditorNull&) = delete;
	ShowEditorNull& operator=(const ShowEditorNull&) = delete;
	~ShowEditorNull() = default;

	virtual void Execute();

protected:
	EditorLayer& el;
};

#endif _SHOW_EDITOR
