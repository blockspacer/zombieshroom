#pragma once

#include <set>
#include <Utility/ECS/Include/Entity.h>
#include <qstring.h>

namespace ToolMode
{
	enum ToolMode
	{
		GLOBAL,
		LOCAL
	};
}

class EditorInterface
{
public:
	virtual void Select(ECS::Entity* p_entity) = 0;
	virtual void Select(const std::set<ECS::Entity*>& p_entities) = 0;
	virtual void AddToSelection(ECS::Entity* p_entity) = 0;
	virtual void ClearSelection() = 0;
	virtual const std::set<ECS::Entity*>& GetSelection() const = 0;
	virtual void RenameEntity(ECS::Entity* p_entity, const QString& p_name) = 0;
	virtual ToolMode::ToolMode GetToolMode() = 0;
};