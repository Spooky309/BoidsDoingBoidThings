inline glm::vec3 Entity::GetPosition() const
{
	return m_vPosition;
}

inline glm::vec3 Entity::GetEuler() const
{
	return m_vEuler;
}

inline glm::vec3 Entity::GetScale() const
{
	return m_vScale;
}

inline glm::mat4 Entity::GetTransformMatrix() const
{
	return m_mTransformMatrix;
}

inline unsigned int Entity::GetEntityID() const
{
	return m_iEntityID;
}

inline const std::string& Entity::GetEntityName() const
{
	return m_sEntityName;
}

inline glm::vec3 Entity::GetForward() const
{
	return m_vFwd;
}

inline glm::vec3 Entity::GetRight() const
{
	return m_vRight;
}

template<typename T>
std::weak_ptr<T> Entity::GetComponent()
{
	std::weak_ptr<T> c;
	for (auto& it : m_vComponents)
	{
		T* cp = dynamic_cast<T*>(it.get());
		if (cp != nullptr)
		{
			c = std::weak_ptr<T>(it);
			break;
		}
	}
	return c; // if we have a component of this type, then this will have its pointer
}

template<typename T>
std::weak_ptr<T> Entity::AddComponent()
{
	// create the new component
	std::shared_ptr<T> newPtr = std::make_shared<T>();
	if (!dynamic_cast<Component*>(newPtr.get()))
	{
		std::cout << "Tried to add a component that's not a component\n";
		return std::weak_ptr<T>();
	}
	dynamic_cast<Component*>(newPtr.get())->m_pParent = this;
	// add it to the list
	m_vComponents.push_back(newPtr);
	// return a ptr to it for if they want to keep it
	return std::weak_ptr<T>(newPtr);
}

template<typename T>
bool Entity::HasComponentOfType()
{
	for (auto& it : m_vComponents)
	{
		if (dynamic_cast<T*>(it.get()))
		{
			return true;
		}
	}
	return false;
}