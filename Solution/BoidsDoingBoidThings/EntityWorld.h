#pragma once
#ifndef H_ENTITY_WORLD
#define H_ENTITY_WORLD
#include <vector>
#include <memory>
#include <string>
class Entity; // forward definition to avoid cyclic dependency

// this class holds pointers to all the entities
// it only passes out weak_ptrs which will be invalidated when the only owning object (this)
// releases its shared_ptr
// this is so objects can be only created and destroyed by this class
// to prevent weird issues with dangling pointers and entities that shouldn't exist still being accessed
class EntityWorld
{
public:
	friend class Engine;
	std::weak_ptr<Entity> GetEntityByID(unsigned int id);
	std::weak_ptr<Entity> GetEntityByName(const std::string& name);
	std::weak_ptr<Entity> CreateEntity(const std::string& name);
	void DestroyEntity(std::weak_ptr<Entity>& e);
private:
	std::vector<std::shared_ptr<Entity>> m_vEntityList;
	unsigned int m_iLastID = 0;
	EntityWorld() = default;
	~EntityWorld() = default;
	EntityWorld(const EntityWorld&) = delete;
};

#endif