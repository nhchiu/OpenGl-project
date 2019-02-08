//
//  model.cpp
//  Lesson 7 - Model Importing
//
//  Created by Nathan on 2019/1/26.
//  Copyright © 2019 Nathan. All rights reserved.
//

#include "model.hpp"


void Model::LoadModel(const std::string &file_name) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(file_name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene) {
        printf("Model (%s) failed to load: %s\n", file_name.c_str(), importer.GetErrorString());
        return;
    }

    LoadNode(scene->mRootNode, scene);

    LoadMaterials(scene);
}

void Model::RenderModel() {
    for (size_t i = 0; i < mesh_list.size(); i++) {
        unsigned int material_index = mesh_to_tex[i];

        if (material_index < texture_list.size() && texture_list[material_index]) {
            texture_list[material_index]->UseTexture();
        }

        mesh_list[i]->RenderMesh();
    }
}

void Model::ClearModel() {
    for (size_t i = 0; i < mesh_list.size(); i++) {
        if (mesh_list[i]) {
            delete mesh_list[i];
            mesh_list[i] = nullptr;
        }
    }

    for (size_t i = 0; i < texture_list.size(); i++) {
        if (texture_list[i]) {
            delete texture_list[i];
            texture_list[i] = nullptr;
        }
    }
}

void Model::LoadNode(aiNode *node, const aiScene *scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (size_t i = 0; i < node->mNumChildren; i++) {
        LoadNode(node->mChildren[i], scene);
    }
}

void Model::LoadMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});
        if (mesh->mTextureCoords[0]) {
            vertices.insert(vertices.end(), {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
        }
        else {
            vertices.insert(vertices.end(), {0.0f, 0.0f});
        }
        vertices.insert(vertices.end(), {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z});
    }
    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* new_mesh = new Mesh();
    new_mesh->CreateMesh(&(vertices[0]), &(indices[0]), (unsigned int)(vertices.size()), (unsigned int)(indices.size()));
    mesh_list.push_back(new_mesh);
    mesh_to_tex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene *scene) {
    texture_list.resize(scene->mNumMaterials);
    for (size_t i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];

        texture_list[i] = nullptr;

        if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
                unsigned long idx = std::string(path.data).rfind("\\");
                std::string filename = std::string(path.data).substr(idx + 1);
                idx = filename.rfind("/");
                filename = filename.substr(idx + 1);

                std::string tex_path = std::string("Textures/") + filename;

                texture_list[i] = new Texture(tex_path.c_str());

                if (!texture_list[i]->LoadTexture()) {
                    printf("Failed to load texture at: %s\n", tex_path.c_str());
                    delete texture_list[i];
                    texture_list[i] = nullptr;
                }
            }
        }
        if (!texture_list[i]) {
            texture_list[i] = new Texture("Textures/plain.png");
            texture_list[i]->LoadAlphaTexture();
        }
    }
}
