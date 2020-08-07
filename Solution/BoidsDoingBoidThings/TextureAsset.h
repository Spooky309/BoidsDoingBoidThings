#pragma once
#ifndef H_TEXASSET
#define H_TEXASSET
#include "AssetBase.h"
#include <glad/glad.h>

class TextureAsset : public AssetBase
{
public:
    TextureAsset(const std::string& name);
    inline GLuint GetGLTexture() const { return texID; }
    inline GLuint GetTextureWidth() const { return texWidth; }
    inline GLuint GetTextureHeight() const { return texHeight; }
    inline GLuint GetTextureChannels() const { return texChannels; }
private:
    GLuint texID;
    int texWidth;
    int texHeight;
    int texChannels;
    unsigned char* data;
};
#endif