#pragma once
#ifndef H_MODASSET
#define H_MODASSET
#include "AssetBase.h"
#include <glad/glad.h>
#include <map>

class ModelAsset : public AssetBase
{
public:
    ModelAsset(const std::string& name);
    bool GetAnimation(const std::string& animName, float** vertPtr, int* animFrameLen = nullptr);
    inline size_t GetModelDataSize() { return modelDataLen; }
    inline size_t GetTriangleCount() { return triCount; }
private:
    void LoadOBJ(const std::string& path);
    size_t modelDataLen;
    size_t triCount;
    float* frames;
    std::map<std::string, std::pair<GLuint, GLuint>> animationIndices;
};

#endif