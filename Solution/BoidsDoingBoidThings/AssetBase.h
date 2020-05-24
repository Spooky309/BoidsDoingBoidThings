#pragma once
#ifndef H_ASSETBASE
#define H_ASSETBASE
#include <string>

class AssetBase
{
public:
	inline std::string GetName() const { return m_sName; }
protected:
	std::string m_sName;
};

#endif