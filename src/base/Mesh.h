/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/3 15:37
* @version: 1.0
* @description: 模型网格类
********************************************************************************/

#ifndef OPENGL_PRIMARY_MESH_H
#define OPENGL_PRIMARY_MESH_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../material/CommonMaterial.h"
#include "../base/VertexArray.h"
#include "../base/Buffer.h"
#include "../base/IMaterial.h"

class Mesh {
public:
    struct Vertex {
        glm::vec3 position;                         // 顶点的位置向量
        glm::vec3 normal;                           // 法向量
        glm::vec2 texCoord;                         // 纹理坐标
    };

public:
    Mesh(const std::string &_name, std::shared_ptr<ShaderProgram> _shaderProgram, std::vector<Vertex> &_vertices, std::vector<unsigned int> &_indices, std::shared_ptr<IMaterial> &_material);
    void render();
    std::string getMeshName() { return m_name; }
    std::shared_ptr<IMaterial> getMaterial() { return m_material.lock(); }

private:
    std::string m_name;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::weak_ptr<IMaterial> m_material;
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<Buffer<Vertex>> m_vbo;
    std::shared_ptr<Buffer<unsigned int>> m_ebo;
    std::shared_ptr<ShaderProgram> m_shaderProgram;
};


#endif //OPENGL_PRIMARY_MESH_H
