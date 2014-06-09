#pragma once


#include <RootTools/Treenity/Include/Tool.h>
#include <map>

namespace ToolBox
{
	enum ToolBox
	{
		ROTATION_TOOL,
		TRANSLATION_TOOL,
		TERRAIN_TOOL,
		TERRAIN_TEXTURE_TOOL,
	};
}

class ToolManager
{
public:
	ToolManager();
	void SetEditorInterface(EditorInterface* p_editorInterface);
	void Initialize(ECS::World* p_world);
	Tool* GetSelectedTool(); 
	void SetTool(ToolBox::ToolBox p_tool);

private:
	std::map<ToolBox::ToolBox, Tool*> m_tools; 
	Tool* m_selectedTool;
	EditorInterface* m_editorInterface;
};