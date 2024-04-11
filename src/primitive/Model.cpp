/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/3 16:37
* @version: 1.0
* @description: 
********************************************************************************/

#include "Model.h"
#include <format>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "../Utility.h"

Model::Model(std::shared_ptr<IScene> &_parent, const std::string &_name, const std::string &_path): IPrimitive(_parent, _name) {
    this->setPrimitiveType(PrimitiveType::Model);
    this->load(_path);
}

void Model::preRender() {
    IPrimitive::preRender();

    // 传递model矩阵
    const auto scaling = *this->getScaling();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, *this->getPosition());
    model = glm::scale(model, glm::vec3(scaling, scaling, scaling));
    model = glm::rotate(model, this->getRotation()->x, glm::vec3{ 1, 0, 0 });
    model = glm::rotate(model, this->getRotation()->y, glm::vec3{ 0, 1, 0 });
    model = glm::rotate(model, this->getRotation()->z, glm::vec3{ 0, 0, 1 });
    this->getShaderProgram()->setMat4("model", glm::value_ptr(model));
}

void Model::render() {
    for(const auto& mesh: m_meshes) {
        mesh->render();
    }
}

// 加载模型
void Model::load(const std::string &_path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    // 检查场景和其根节点不为null、检查标记(Flag)来查看返回的数据是不是不完整的
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG(ERROR) << importer.GetErrorString();
        return;
    }

    m_directory = _path.substr(0, _path.find_last_of('\\'));

    this->processMaterial(scene);
    this->processNode(scene->mRootNode, scene);
}

// 遍历节点
void Model::processNode(aiNode *_node, const aiScene *_scene) {
    for(auto i = 0; i < _node->mNumMeshes; i++) {
        aiMesh *mesh = _scene->mMeshes[_node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, _scene));
    }

    for(auto i = 0; i < _node->mNumChildren; i++) {
        processNode(_node->mChildren[i], _scene);
    }
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh *_mesh, const aiScene *_scene) {
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;

    // 顶点数据
    for(auto i = 0; i < _mesh->mNumVertices; i++) {
        Mesh::Vertex vertex{};

        // 顶点坐标数据
        vertex.position = glm::vec3(_mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z);

        // 法线数据
        if(_mesh->HasNormals()) {
            vertex.normal = glm::vec3(_mesh->mNormals[i].x, _mesh->mNormals[i].y, _mesh->mNormals[i].z);
        }

        // 纹理坐标数据
        // 只关心第一组纹理坐标
        if(_mesh->mTextureCoords[0]) {
            vertex.texCoord = glm::vec2(_mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y);
        }
        else {
            vertex.texCoord = glm::vec2(0.0f, 0.0f);
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

    // Mesh名称
    const auto meshName = _mesh->mName.C_Str();
    LOG(INFO) << "_mesh->mMaterialIndex: " << _mesh->mMaterialIndex;
    return std::make_shared<Mesh>(meshName, this->getShaderProgram(), vertices, indices, m_materials[_mesh->mMaterialIndex]);
}

void Model::processMaterial(const aiScene *_scene) {
    LOG(INFO) << "_scene->mNumMaterials: " << _scene->mNumMaterials;
    for(auto i = 0; i < _scene->mNumMaterials; i++) {
        const auto asMaterial = _scene->mMaterials[i];
        std::shared_ptr<IMaterial> material = std::make_shared<CommonMaterial>(this->getShaderProgram());
        this->loadTextureForMaterial(material, asMaterial, TextureType::Diffuse);
        this->loadTextureForMaterial(material, asMaterial, TextureType::Specular);
        this->loadTextureForMaterial(material, asMaterial, TextureType::Height);
        this->loadTextureForMaterial(material, asMaterial, TextureType::Normal);
        m_materials.insert({ i, material } );
        LOG(INFO) << "i = " << i;
    }
}

// 加载特定类型的纹理
void Model::loadTextureForMaterial(std::shared_ptr<IMaterial> &_material, const aiMaterial *_asMaterial, TextureType _type) {
    LOG_IF(INFO, _asMaterial->GetTextureCount(Utility::transformTextureTypeToAssimp(_type)) > 1);

    if(_asMaterial->GetTextureCount(Utility::transformTextureTypeToAssimp(_type)) == 0) return;

    auto material = std::dynamic_pointer_cast<CommonMaterial>(_material);
    aiString texFileName;
    _asMaterial->GetTexture(Utility::transformTextureTypeToAssimp(_type), 0, &texFileName);
    const auto tmpFilename = std::string(texFileName.C_Str());

    // 纹理已被加载过
    if(m_textureLoaded.find(tmpFilename) != m_textureLoaded.end()) {
        switch (_type) {
            case TextureType::Diffuse:          material->setDiffuseMap(m_textureLoaded[tmpFilename].lock());            break;
            case TextureType::Specular:         material->setSpecularMap(m_textureLoaded[tmpFilename].lock());           break;
            case TextureType::Height:           material->setHeightMap(m_textureLoaded[tmpFilename].lock());             break;
            case TextureType::Normal:           material->setNormalMap(m_textureLoaded[tmpFilename].lock());             break;
            default:                            LOG(FATAL) << " Undefined conditional branch.";
        }
    }
    // 纹理尚未被加载
    else {
        const auto &texPath = std::format("{}\\{}", m_directory, tmpFilename);
        std::shared_ptr<Texture> texture;
        switch (_type) {
            case TextureType::Diffuse: {
                texture = std::make_shared<Texture>(texPath, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
                material->setDiffuseMap(texture);
                break;
            }
            case TextureType::Specular: {
                texture = std::make_shared<Texture>(texPath, 1, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
                material->setSpecularMap(texture);
                break;
            }
            case TextureType::Height: {
                texture = std::make_shared<Texture>(texPath, 2, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
                material->setHeightMap(texture);
                break;
            }
            case TextureType::Normal: {
                texture = std::make_shared<Texture>(texPath, 3, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
                material->setNormalMap(texture);
                break;
            }
            default: LOG(FATAL) << " Undefined conditional branch.";
        }
        m_textureLoaded.insert({ tmpFilename, texture });
    }
}
