#pragma once
#ifndef H_ASSETMANAGER
#define H_ASSETMANAGER
#include "AssetBase.h"
#include <vector>
#include "Engine.h"

class AssetManager
{
public:
    template<typename T>
    T* LoadAsset(const std::string& name);
private:
    std::vector<AssetBase*> m_loadedAssets;
};
#include "AssetManager.inl"
#endif