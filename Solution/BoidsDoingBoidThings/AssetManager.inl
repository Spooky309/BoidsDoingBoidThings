template<typename T>
T* AssetManager::LoadAsset(const std::string& name)
{
    // if we already have this asset we can just return a ptr to it already in memory
    for (AssetBase* a : m_loadedAssets)
    {
        if (a->GetName() == name)
        {
            return static_cast<T*>(a);
        }
    }
    T* t = new T(name);
    m_loadedAssets.emplace_back(t);
    return t;
}