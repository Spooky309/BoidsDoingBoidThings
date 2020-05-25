#pragma once
#ifndef H_ASSETBASE
#define H_ASSETBASE
#include <string>

class AssetBase
{
public:
    AssetBase(const std::string& name);
    inline std::string GetName() const { return name; }
private:
    std::string name;
};

#endif