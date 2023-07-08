#include "gpch.h"
#include "ShowEditorDont.h"
#include "EditorLayer.h"

ShowEditorDont::ShowEditorDont(EditorLayer& el)
	: ShowEditorNull(el)
{}

void ShowEditorDont::Execute()
{
	el.showEditorDont();
}
