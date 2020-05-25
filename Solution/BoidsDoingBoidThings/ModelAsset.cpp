#include "ModelAsset.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <vector>

ModelAsset::ModelAsset(const std::string& name) : AssetBase(name)
{
    std::string ext = name.substr(name.find_last_of("."));
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {return std::tolower(c); });
    if (ext == ".obj")
    {
        LoadOBJ(name);
    }
    else
    {
        std::cout << name << " unknown model format\n";
        return;
    }
}

void ModelAsset::LoadOBJ(const std::string& path)
{
    animationIndices.insert(std::pair<std::string, std::pair<GLuint, GLuint>>("", std::pair<GLuint, GLuint>(0, 1)));
    std::vector<GLuint> vertIndices;
    std::vector<GLuint> normIndices;
    std::vector<GLuint> uvIndices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvs;
    std::vector<GLfloat> verts;
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "couldn't open file for whatever reason " << path << "\n";
        return;
    }
    GLuint ind;
    GLfloat ver;
    std::string in;
    triCount = 0;
    char dummy;
    while (!file.eof())
    {
        in = "";
        file >> in;
        if (in == "v")
        {
            for (unsigned char i = 0; i < 3; i++)
            {
                file >> ver;
                verts.push_back(ver);
            }
        }
        if (in == "vn")
        {
            for (unsigned char i = 0; i < 3; i++)
            {
                file >> ver;
                normals.push_back(ver);
            }
        }
        if (in == "vt")
        {
            for (unsigned char i = 0; i < 2; i++)
            {
                file >> ver;
                uvs.push_back(ver);
            }
        }
        if (in == "f")
        {
            for (unsigned char i = 0; i < 3; i++)
            {
                file >> ind;
                ind -= 1;
                vertIndices.push_back(ind);
                file.read(&dummy, 1); // seekg wasn't doing its job so i've just had to have it read into a dummy char instead, this is to skip the forward slashes in the obj file
                file >> ind;
                ind -= 1;
                uvIndices.push_back(ind);
                file.read(&dummy, 1);
                file >> ind;
                ind -= 1;
                normIndices.push_back(ind);
            }
            triCount++;
        }
    }
    modelDataLen = (normIndices.size() * 3) + (uvIndices.size() * 2) + (vertIndices.size() * 3);
    frames = new GLfloat[modelDataLen];

    size_t vi = 0;
    for (size_t i = 0; i < vertIndices.size(); i++)
    {
        frames[vi++] = verts[(vertIndices[i] * 3)];
        frames[vi++] = verts[(vertIndices[i] * 3) + 1];
        frames[vi++] = verts[(vertIndices[i] * 3) + 2];
        frames[vi++] = normals[(normIndices[i] * 3)];
        frames[vi++] = normals[(normIndices[i] * 3) + 1];
        frames[vi++] = normals[(normIndices[i] * 3) + 2];
        frames[vi++] = uvs[(uvIndices[i] * 2)];
        frames[vi++] = uvs[(uvIndices[i] * 2) + 1];
    }
}

bool ModelAsset::GetAnimation(const std::string& animName, float** vertPtr, int* animFrameLen)
{
    if (!frames) return false;
    auto an = animationIndices.find(animName);
    if (an == animationIndices.end()) return false;
    *vertPtr = &frames[modelDataLen * an->second.first];
    if (animFrameLen) *animFrameLen = an->second.second;
    return true;
}