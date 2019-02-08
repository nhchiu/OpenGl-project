//
//  model.hpp
//  Lesson 7 - Model Importing
//
//  Created by Nathan on 2019/1/26.
//  Copyright © 2019 Nathan. All rights reserved.
//

#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "texture.hpp"

class Model {
public:
    Model() {}

    void LoadModel(const std::string &file_name);
    void RenderModel();
    void ClearModel();

    ~Model() {}
private:
    void LoadNode(aiNode *node, const aiScene *scene);
    void LoadMesh(aiMesh *mesh, const aiScene *scene);
    void LoadMaterials(const aiScene *scene);

    std::vector<Mesh*> mesh_list;
    std::vector<Texture*> texture_list;
    std::vector<unsigned int> mesh_to_tex;
};

#endif /* _MODEL_HPP_ */
