#include "headers/model.h"
#include <iostream>
#include <glm/glm.hpp>

Model::Model(const char* filePath) {
    // Extract the directory from the file path (needed for textures)
    std::string path = filePath;
    directory = path.substr(0, path.find_last_of('/'));

    // Load the model from the given file
    loadModel(path);
}

void Model::draw(Shader& shader, camera& cam) {
    // Draw all the meshes in this model
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader, cam);
    }
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;

    // Import the model file and apply standard processing
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate |        // Convert all faces to triangles
        aiProcess_GenSmoothNormals |   // Generate normals if missing
        aiProcess_FlipUVs |            // Flip texture coordinates for OpenGL
        aiProcess_JoinIdenticalVertices // Remove duplicate vertices
    );

    // Check if the import succeeded
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ASSIMP Error: " << importer.GetErrorString() << std::endl;
        return;
    }

    // Start processing from the root node
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    // Process each mesh referenced by this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* meshPtr = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(meshPtr, scene));
    }

    // Recursively process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

mesh Model::processMesh(aiMesh* aiMesh, const aiScene* scene) {
    std::vector<vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<texture> textures;

    // Extract vertices
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
        vertex v;

        // Copy position
        v.position.x = aiMesh->mVertices[i].x;
        v.position.y = aiMesh->mVertices[i].y;
        v.position.z = aiMesh->mVertices[i].z;

        // Copy normals if available
        if (aiMesh->HasNormals()) {
            v.normal.x = aiMesh->mNormals[i].x;
            v.normal.y = aiMesh->mNormals[i].y;
            v.normal.z = aiMesh->mNormals[i].z;
        }

        // Copy texture coordinates if available
        if (aiMesh->mTextureCoords[0]) {
            v.texUV.x = aiMesh->mTextureCoords[0][i].x;
            v.texUV.y = aiMesh->mTextureCoords[0][i].y;
        } else {
            v.texUV = glm::vec2(0.0f, 0.0f);
        }

        // Assign default color
        v.color = glm::vec3(1.0f, 1.0f, 1.0f);

        vertices.push_back(v);
    }

    // Extract indices from faces
    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
        aiFace face = aiMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Extract textures from the material
    if (aiMesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];

        // Load diffuse textures
        std::vector<texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Load specular textures
        std::vector<texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    // Return the mesh object
    return mesh(vertices, indices, textures);
}

std::vector<texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<texture> textures;

    // Load each texture of the given type from the material
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        // Build full path to the texture file
        std::string filename = std::string(str.C_Str());
        std::string fullPath = directory + "/" + filename;

        // Determine texture format based on file extension
        GLenum format = fullPath.ends_with(".png") ? GL_RGBA : GL_RGB;

        // Create texture object and add it to the list
        textures.emplace_back(fullPath.c_str(), typeName.c_str(), i, format, GL_UNSIGNED_BYTE);
    }

    return textures;
}
