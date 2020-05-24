#pragma once
#ifndef H_COMPONENT
#define H_COMPONENT
#include <memory>
class Entity;
class Component
{
public:
	inline Entity* GetParentEntity() { return m_pParent; }
	virtual void Update() = 0;
	friend class Entity;
private:
	Entity* m_pParent;
};

#endif