#pragma once

#include "AbilityComponents.h"
namespace AbilityEditorNameSpace
{
	class OnCreate
	{
	public:
		OnCreate();
		~OnCreate();
		void AddEntity(std::string p_name);
		void AddEntity(AbilityEntity::Entity* p_entity);
		void AddComponent(unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type);
		void AddComponent(unsigned int p_id, QString p_name);
		void RemoveComponent(unsigned int p_id, AbilityComponents::ComponentType::ComponentType p_type);
		void RemoveComponent(unsigned int p_id, QString p_name);
	
		std::vector<AbilityEntity::Entity*>* GetEntities() { return &m_entities; }
	private:
		std::vector<AbilityEntity::Entity*> m_entities;
	};
}