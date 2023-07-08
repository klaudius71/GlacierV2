#include "gpch.h"
#include "ShowEditorNull.h"
#include "EditorLayer.h"

ShowEditorNull::ShowEditorNull(EditorLayer& el)
	: el(el)
{}

void ShowEditorNull::Execute()
{
	el.showEditor();
}
