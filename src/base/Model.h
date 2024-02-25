//
// Created by TURIING on 2024/1/2.
//

#ifndef LEARN_OPENGL_MODEL_H
#define LEARN_OPENGL_MODEL_H
#include <string>
#include <vector>
#include "Mesh.h"
#include "glog/logging.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <optional>

enum class MaterialType { Diffuse, Specular, Normal, Height };

class Model {
public:
    /**
     *
     * @param _path
     * @param _loadType nullopt代表全部类型都不加载，需要加载啥类型插入啥
     */
    Model(std::string _path, std::optional<std::vector<MaterialType>> _loadType, std::shared_ptr<ShaderProgram> _shaderProgram): m_loadType(_loadType), m_shaderProgram(_shaderProgram) {
        this->loadModel(_path);
    }

    void paint() {
        for(auto i = 0; i < m_meshes.size(); i++) {
            m_meshes[i].paint(m_shaderProgram);
        }
    }

private:
    void loadModel(std::string const &_path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        // 检查场景和其根节点不为null、检查标记(Flag)来查看返回的数据是不是不完整的
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            LOG(ERROR) << importer.GetErrorString();
            return;
        }

        m_directory = _path.substr(0, _path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode *_node, const aiScene *_scene) {
        for(auto i = 0; i < _node->mNumMeshes; i++) {
            aiMesh *mesh = _scene->mMeshes[_node->mMeshes[i]];
            m_meshes.push_back(processMesh(mesh, _scene));
        }

        for(auto i = 0; i < _node->mNumChildren; i++) {
            processNode(_node->mChildren[i], _scene);
        }
    }

    Mesh processMesh(aiMesh *_mesh, const aiScene *_scene) {
        std::vector<MeshVertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<MeshTexture> textures;

        for(auto i = 0; i < _mesh->mNumVertices; i++) {
            MeshVertex vertex;

            // 顶点坐标数据
            glm::vec3 vector;
            vector.x = _mesh->mVertices[i].x;
            vector.y = _mesh->mVertices[i].y;
            vector.z = _mesh->mVertices[i].z;
            vertex.Position = vector;

            // 法线数据
            if(_mesh->HasNormals()) {
                vector.x = _mesh->mNormals[i].x;
                vector.y = _mesh->mNormals[i].y;
                vector.z = _mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            // 纹理坐标数据
            // 只关心第一组纹理坐标
            if(_mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = _mesh->mTextureCoords[0][i].x;
                vec.y = _mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        // 索引（indices）数据
        for(auto i = 0; i < _mesh->mNumFaces; i++) {
            aiFace face = _mesh->mFaces[i];
            for(auto j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        auto meterial = _scene->mMaterials[_mesh->mMaterialIndex];
        std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(meterial, aiTextureType_DIFFUSE, "texture_diffuse");
        if(m_loadType.has_value() && isContainMaterialType(m_loadType.value(), MaterialType::Diffuse) && diffuseMaps.size() != 0)
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<MeshTexture> specularMaps = loadMaterialTextures(meterial, aiTextureType_SPECULAR, "texture_specular");
        if(m_loadType.has_value() && isContainMaterialType(m_loadType.value(), MaterialType::Specular) && specularMaps.size() != 0)
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<MeshTexture> normalMaps = loadMaterialTextures(meterial, aiTextureType_HEIGHT, "texture_normal");
        if(m_loadType.has_value() && isContainMaterialType(m_loadType.value(), MaterialType::Normal) && normalMaps.size() != 0)
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<MeshTexture> heightMaps = loadMaterialTextures(meterial, aiTextureType_AMBIENT, "texture_height");
        if(m_loadType.has_value() && isContainMaterialType(m_loadType.value(), MaterialType::Height) && heightMaps.size() != 0)
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        return Mesh(m_shaderProgram, vertices, indices, textures);
    }

    std::vector<MeshTexture> loadMaterialTextures(aiMaterial *_mat, aiTextureType _type, std::string _typeName) {
        std::vector<MeshTexture> textures;
        for(auto i = 0; i < _mat->GetTextureCount(_type); i++) {
            aiString str;
            _mat->GetTexture(_type, i, &str);
            auto skip = false;
            for(auto j = 0; j < m_textures_loaded.size(); j++) {
                if(std::strcmp(m_textures_loaded[j].path.data(), str.C_Str()) == 0) {
                    textures.push_back(m_textures_loaded[j]);
                    skip = true;
                    break;
                }
            }

            // 如果纹理还没有被加载，则加载它
            if(!skip) {
                MeshTexture texture;

                const auto path = m_directory + '/' + std::string(str.C_Str());
                texture.texture = std::make_shared<Texture>(path, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

                texture.type = _typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);

                m_textures_loaded.push_back(texture);
            }
        }
        return textures;
    }

    static bool isContainMaterialType(std::vector<MaterialType> &_vec, MaterialType _type) {
        for(auto type: _vec) {
            if(type == _type) return true;
        }
        return false;
    }

private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<MeshTexture> m_textures_loaded;                  // 存储所有已经加载的纹理
    std::optional<std::vector<MaterialType>> m_loadType;         // 定义加载类型
    std::shared_ptr<ShaderProgram> m_shaderProgram;
};

#endif //LEARN_OPENGL_MODEL_H
