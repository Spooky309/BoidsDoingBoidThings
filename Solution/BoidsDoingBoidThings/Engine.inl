inline double Engine::GetDeltaTime()
{
	return m_dDeltaTime;
}

inline GLFWwindow* Engine::GetWindow()
{
	return m_pWindow;
}

inline Input& Engine::GetInput()
{
	return m_Input;
}

inline EntityWorld& Engine::GetEntityWorld()
{
	return m_EntityWorld;
}

inline Renderer3D& Engine::GetRenderer3D()
{
	return m_rend3d;
}

inline AssetManager& Engine::GetAssetManager()
{
	return m_assetManager;
}