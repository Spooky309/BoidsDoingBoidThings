#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>
void Entity::SetPosition(const glm::vec3& newPos)
{
	m_vPosition = newPos;
	m_bDirtyTransform = true;
}
void Entity::SetEuler(const glm::vec3& newEuler)
{
	m_vEuler = newEuler;
	m_bDirtyTransform = true;
}
void Entity::SetScale(const glm::vec3& newScale)
{
	m_vScale = newScale;
	m_bDirtyTransform = true;
}
void Entity::Move(const glm::vec3& delta)
{
	m_vPosition += delta;
	m_bDirtyTransform = true;
}
void Entity::Rotate(const glm::vec3& delta)
{
	m_vEuler += delta;
	m_bDirtyTransform = true;
}
void Entity::Scale(const glm::vec3& delta)
{
	m_vScale += delta;
	m_bDirtyTransform = true;
}
void Entity::ScaleBy(const glm::vec3& delta)
{
	m_vScale *= delta;
	m_bDirtyTransform = true;
}

void Entity::Initialize(unsigned int id, const std::string& name)
{
	m_iEntityID = id;
	m_sEntityName = name;
	m_vPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vEuler = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vScale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_bDirtyTransform = false;
	m_mTransformMatrix = glm::mat4(1.0f); // create a new identity matrix
}

void Entity::Update()
{
	for (auto& it : m_vComponents)
	{
		it->Update();
	}
	if (m_bDirtyTransform)
	{
		m_mTransformMatrix = glm::mat4(1.0f);
		m_mTransformMatrix = glm::translate(m_mTransformMatrix, m_vPosition);
		m_mTransformMatrix = glm::rotate(m_mTransformMatrix, m_vEuler.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_mTransformMatrix = glm::rotate(m_mTransformMatrix, m_vEuler.y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_mTransformMatrix = glm::rotate(m_mTransformMatrix, m_vEuler.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_mTransformMatrix = glm::scale(m_mTransformMatrix, m_vScale);

		m_vFwd.x = glm::cos(m_vEuler.y) * glm::cos(m_vEuler.x);
		m_vFwd.y = glm::sin(m_vEuler.x);
		m_vFwd.z = glm::sin(m_vEuler.y) * glm::cos(m_vEuler.x);

		m_vRight = glm::cross(m_vFwd, glm::vec3(0.0f, 1.0f, 0.0f)); // might get problems if it faces exactly up or down, otherwise it's fine
	}
}