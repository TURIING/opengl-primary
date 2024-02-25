//
// Created by TURIING on 2023/12/10.
//

#ifndef LEARN_OPENGL_Mesh_H
#define LEARN_OPENGL_Mesh_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Buffer.h"
#include <string>
#include <vector>
#include <memory>
#include "Texture.h"

/* 顶点结构体 */
struct MeshVertex {
    glm::vec3 Position;                         // 顶点的位置向量
    glm::vec3 Normal;                           // 法向量
    glm::vec2 TexCoords;                        // 纹理坐标
};

struct MeshTexture {
    std::shared_ptr<Texture> texture;           // 纹理对象
    std::string type;                           // 纹理类型
    std::string path;                           // 纹理路径
};

class Mesh {
public:
    Mesh(std::shared_ptr<ShaderProgram> _shaderProgram, std::vector<MeshVertex> _vertices, std::vector<unsigned int> _indices, std::vector<MeshTexture> _textures)
        : m_vertices(_vertices), m_indices(_indices), m_textures(_textures)
    {
        m_vao = std::make_shared<VertexArray>();
        m_vbo = std::make_shared<Buffer<MeshVertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);
        m_ebo = std::make_shared<Buffer<unsigned int>>(BUFFER_TYPE::INDEX_BUFFER, m_indices);

        m_vao->setAttribute<MeshVertex, glm::vec3>(0, offsetof(MeshVertex, Position));
        m_vao->setAttribute<MeshVertex, glm::vec3>(1, offsetof(MeshVertex, Normal));
        m_vao->setAttribute<MeshVertex, glm::vec2>(2, offsetof(MeshVertex, TexCoords));
    }

    void paint(std::shared_ptr<ShaderProgram> _shaderProgram) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;

        for(unsigned int i = 0; i < m_textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);

            std::string number;
            std::string name = m_textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);
            else if(name == "texture_normal")
                number = std::to_string(normalNr++);
            else if(name == "texture_height")
                number = std::to_string(heightNr++);

            _shaderProgram->setInt((name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, m_textures[i].texture->getID());
        }
        Texture::activate(GL_TEXTURE0);
        m_vao->bind();

        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    }

private:
    std::vector<MeshVertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<MeshTexture> m_textures;

    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<Buffer<MeshVertex>> m_vbo;
    std::shared_ptr<Buffer<unsigned int>> m_ebo;
};
#endif //LEARN_OPENGL_Mesh_H
