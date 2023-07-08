#ifndef _SHOW_EDITOR_DONT
#define _SHOW_EDITOR_DONT

#include "ShowEditorNull.h"

class ShowEditorDont : public ShowEditorNull
{
public:
	ShowEditorDont() = delete;
	ShowEditorDont(EditorLayer& el);
	ShowEditorDont(const ShowEditorDont&) = delete;
	ShowEditorDont& operator=(const ShowEditorDont&) = delete;
	~ShowEditorDont() = default;

	virtual void Execute() override;
};

#endif _SHOW_EDITOR_DONT