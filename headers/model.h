//
// Created by roufox on 11/17/25.
//

#ifndef OPENGLPROJECT_MODEL_H
#define OPENGLPROJECT_MODEL_H

#pragma once

#include <string>
#include <vector>
#include "mesh.h" // Your existing mesh class
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    Model(const char* filePath);

    void draw(Shader& shader, camera& cam);

private:
    std::vector<mesh> meshes;
    std::string directory;

    void loadModel(const std::string& path);

    void processNode(aiNode* node, const aiScene* scene);

    mesh processMesh(aiMesh* aiMesh, const aiScene* scene);

    std::vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif //OPENGLPROJECT_MODEL_H