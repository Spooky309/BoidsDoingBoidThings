#pragma once
#ifndef H_ENTITY
#define H_ENTITY
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Component.h"
#include "MeshRenderer.h"

class Entity
{
public:
	inline unsigned int GetEntityID() const;
	inline const std::string& GetEntityName() const;
	inline glm::vec3 GetPosition() const;
	inline glm::vec3 GetEuler() const;
	inline glm::vec3 GetScale() const;
	inline glm::mat4 GetTransformMatrix() const;
	inline glm::vec3 GetForward() const;
	inline glm::vec3 GetRight() const;
	void SetPosition(const glm::vec3& newPos);
	void SetEuler(const glm::vec3& newEuler);
	void SetScale(const glm::vec3& newScale);
	// Move the entity by delta
	void Move(const glm::vec3& delta);
	// Rotate the entity by delta
	void Rotate(const glm::vec3& delta);
	// Increase scale by delta
	void Scale(const glm::vec3& delta);
	// Multiplicative scaling
	void ScaleBy(const glm::vec3& delta);
	// Attach a component
	template<typename T>
	std::weak_ptr<T> AddComponent();
	template<typename T>
	std::weak_ptr<T> GetComponent();
	template<typename T>
	bool HasComponentOfType();
	friend class EntityWorld;
private:
	void Initialize(unsigned int id, const std::string& name);
	void Update();
	std::vector<std::shared_ptr<Component>> m_vComponents;
	std::string m_sEntityName;
	unsigned int m_iEntityID;
	bool m_bDirtyTransform; // set to true when any transform is changed
	glm::vec3 m_vPosition;
	glm::vec3 m_vEuler;
	glm::vec3 m_vScale;
	glm::vec3 m_vFwd;
	glm::vec3 m_vRight;
	glm::mat4 m_mTransformMatrix;
};
#include "Entity.inl"

#endif