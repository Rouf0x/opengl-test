//
// Created by roufox on 11/17/25.
//

#include "headers/model.h"
#include <iostream>
#include <glm/glm.hpp>

Model::Model(const char* filePath) {
    std::string path = filePath;
    // Find the directory path (e.g., if path is "assets/box.obj", directory is "assets")
    directory = path.substr(0, path.find_last_of('/'));
    loadModel(path);
}

void Model::draw(Shader& shader, camera& cam) {
    // Simply tell every mesh in the model to draw itself
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader, cam);
    }
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;

    // 1. Tell Assimp to read the file and do some cleanup/optimization
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate        | // Convert all faces to triangles (required for OpenGL)
        aiProcess_GenSmoothNormals   | // Generate normals if the file is missing them
        aiProcess_FlipUVs            | // Flip texture coordinates (often needed for OpenGL)
        aiProcess_JoinIdenticalVertices); // Combine duplicate vertices

    // 2. Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ASSIMP Error: " << importer.GetErrorString() << std::endl;
        return;
    }

    // 3. Start the recursive processing from the root node
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process all the meshes in this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // Process all the children of this node (recursion)
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

mesh Model::processMesh(aiMesh* aiMesh, const aiScene* scene) {
    std::vector<vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<texture> textures;

    // --- 1. Extract Vertices ---
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
        vertex v;

        // Position (always present)
        v.position.x = aiMesh->mVertices[i].x;
        v.position.y = aiMesh->mVertices[i].y;
        v.position.z = aiMesh->mVertices[i].z;

        // Normals (check if present)
        if (aiMesh->HasNormals()) {
            v.normal.x = aiMesh->mNormals[i].x;
            v.normal.y = aiMesh->mNormals[i].y;
            v.normal.z = aiMesh->mNormals[i].z;
        }

        // Texture Coordinates (check if present, Assimp supports multiple sets, we use the first one [0])
        if (aiMesh->mTextureCoords[0]) {
            v.texUV.x = aiMesh->mTextureCoords[0][i].x;
            v.texUV.y = aiMesh->mTextureCoords[0][i].y;
        } else {
            v.texUV = glm::vec2(0.0f, 0.0f);
        }

        // Color (you might use a placeholder or check for vertex colors)
        v.color = glm::vec3(1.0f, 1.0f, 1.0f); // Default white color

        vertices.push_back(v);
    }

    // --- 2. Extract Indices (Faces) ---
    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
        aiFace face = aiMesh->mFaces[i];
        // Since we use aiProcess_Triangulate, all faces have 3 indices
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // --- 3. Extract Materials/Textures ---
    if (aiMesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];

        // Load Diffuse (main) textures
        std::vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Load Specular (shine) textures
        std::vector<texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    // Return a new mesh object using the data we just extracted
    return mesh(vertices, indices, textures);
}

std::vector<texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Combine the model's directory with the texture file name
        std::string filename = std::string(str.C_Str());
        std::string fullPath = directory + "/" + filename;

        // Use your existing texture constructor:
        // texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType)
        GLenum format = fullPath.ends_with(".png") ? GL_RGBA : GL_RGB;
        textures.emplace_back(fullPath.c_str(), typeName.c_str(), i, format, GL_UNSIGNED_BYTE);

    }
    return textures;
}