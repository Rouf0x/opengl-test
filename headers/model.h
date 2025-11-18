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
    // 1. The constructor loads the file
    Model(const char* filePath);

    // 2. The Draw method calls Draw on all its meshes
    void draw(Shader& shader, camera& cam);

private:
    std::vector<mesh> meshes;
    std::string directory; // To find textures relative to the OBJ file location

    // --- Assimp Internal Methods ---
    void loadModel(const std::string& path);

    // Handles the hierarchy (bones/parts) of the model file
    void processNode(aiNode* node, const aiScene* scene);

    // Converts an Assimp mesh into *your* mesh object
    mesh processMesh(aiMesh* aiMesh, const aiScene* scene);

    // Loads textures from the material data
    std::vector<texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    // (Optional but helpful) A simple function to load textures you define
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};

#endif //OPENGLPROJECT_MODEL_H