#include "EntityWorld.h"
#include "Entity.h"
void EntityWorld::Update()
{
	for (auto& it : m_vEntityList)
	{
		it->Update();
	}
}

std::weak_ptr<Entity> EntityWorld::GetEntityByID(unsigned int id)
{
	std::weak_ptr<Entity> e;
	for (auto& it : m_vEntityList)
	{
		if (it->GetEntityID() == id)
		{
			e = std::weak_ptr<Entity>(it);
			break;
		}
	}
	return e;
}

std::weak_ptr<Entity> EntityWorld::GetEntityByName(const std::string& name)
{
	std::weak_ptr<Entity> e;
	for (auto& it : m_vEntityList)
	{
		if (it->GetEntityName() == name)
		{
			e = std::weak_ptr<Entity>(it);
			break;
		}
	}
	return e;
}

std::weak_ptr<Entity> EntityWorld::CreateEntity(const std::string& name)
{
	std::shared_ptr<Entity> newE = std::make_shared<Entity>();
	newE->Initialize(++m_iLastID, name);
	m_vEntityList.push_back(newE);
	return std::weak_ptr<Entity>(newE);
}

void EntityWorld::DestroyEntity(std::weak_ptr<Entity>& e)
{
	for (std::size_t i = 0; i < m_vEntityList.size(); i++)
	{
		if (e.lock()->GetEntityID() == m_vEntityList[i]->GetEntityID())
		{
			m_vEntityList.erase(m_vEntityList.begin() + i);
			break;
		}
	}
}

std::vector<std::weak_ptr<Entity>> EntityWorld::GetEntitiesWithName(const std::string& name)
{
	std::vector<std::weak_ptr<Entity>> vec;
	for (auto& it : m_vEntityList)
	{
		if (it->GetEntityName() == name)
		{
			vec.push_back(std::weak_ptr<Entity>(it));
		}
	}
	return vec;
}