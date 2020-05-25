#include "TextureAsset.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <iostream>
TextureAsset::TextureAsset(const std::string& name) : AssetBase(name)
{
    texID = 0;
    data = stbi_load(name.c_str(), &texWidth, &texHeight, &texChannels, 0);
    if (!data)
    {
        std::cout << "failed to load texture " << name << "\n";
        return;
    }
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    GLint format = texChannels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] data;
}