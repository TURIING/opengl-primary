/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/3 16:37
* @version: 1.0
* @description: 模型类
********************************************************************************/

#ifndef OPENGL_PRIMARY_MODEL_H
#define OPENGL_PRIMARY_MODEL_H

#include <map>
#include <memory>
#include "../core/IPrimitive.h"
#include "../base/Mesh.h"

class aiNode;
class aiScene;
class aiMesh;
class aiMaterial;

class Model: public IPrimitive {
public:
    Model(std::shared_ptr<IScene> &_parent, const std::string &_name, const std::string &_path);
    void render() override;
    void preRender() override;
    std::vector<std::shared_ptr<Mesh>> getMeshes() { return m_meshes; }

private:
    void load(const std::string &_path);
    void processNode(aiNode *_node, const aiScene *_scene);
    std::shared_ptr<Mesh> processMesh(aiMesh *_mesh, const aiScene *_scene);
    void processMaterial(const aiScene *_scene);
    void loadTextureForMaterial(std::shared_ptr<IMaterial> &_material, const aiMaterial *_asMaterial, TextureType _type);

private:
    std::string m_directory;
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::map<int, std::shared_ptr<IMaterial>> m_materials;
    std::map<std::string, std::weak_ptr<Texture>> m_textureLoaded;                         // 用于去除重复纹理
};


#endif //OPENGL_PRIMARY_MODEL_H
