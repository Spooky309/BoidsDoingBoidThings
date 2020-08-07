inline double Input::GetMousePosX() const
{
	return m_dXPos;
}
inline double Input::GetMousePosY() const
{
	return m_dYPos;
}
inline bool Input::GetMouseButton(int button) const
{
	return m_bBtns[button];
}
inline bool Input::GetMouseButtonDown(int button) const
{
	return (m_bBtns[button] && !m_bLastBtns[button]);
}
inline bool Input::GetMouseButtonUp(int button) const
{
	return (!m_bBtns[button] && m_bLastBtns[button]);
}
inline bool Input::GetKey(int key) const
{
	return m_bKeys[key];
}
inline bool Input::GetKeyDown(int key) const
{
	return (m_bKeys[key] && !m_bLastKeys[key]);
}
inline bool Input::GetKeyUp(int key) const
{
	return (!m_bKeys[key] && m_bLastKeys[key]);
}