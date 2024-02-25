//
// Created by TURIING on 2023/12/10.
//

#ifndef LEARN_OPENGL_Mesh_H
#define LEARN_OPENGL_Mesh_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
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
    Mesh(std::vector<MeshVertex> _vertices, std::vector<unsigned int> _indices, std::vector<MeshTexture> _textures): m_vertices(_vertices), m_indices(_indices), m_textures(_textures) {
        this->setupMesh();
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
        glActiveTexture(GL_TEXTURE0);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    void setupMesh() {
        // VAO
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        // VBO
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(MeshVertex), &m_vertices[0], GL_STATIC_DRAW);

        // EBO
        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *) offsetof(MeshVertex, Normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *) offsetof(MeshVertex, TexCoords));

        // 解绑VAO
        glBindVertexArray(0);
    }

private:
    std::vector<MeshVertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<MeshTexture> m_textures;
    unsigned int m_VAO, m_VBO, m_EBO;
};
#endif //LEARN_OPENGL_Mesh_H
