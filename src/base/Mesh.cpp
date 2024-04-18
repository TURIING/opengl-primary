/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/3 15:37
* @version: 1.0
* @description: 
********************************************************************************/

#include "Mesh.h"
#include "ShaderProgram.h"
#include "ImageTexture.h"

Mesh::Mesh(const std::string &_name, std::shared_ptr<ShaderProgram> _shaderProgram, std::vector<Vertex> &_vertices, std::vector<unsigned int> &_indices, std::shared_ptr<IMaterial> &_material)
    :m_material(_material), m_name(_name), m_vertices(_vertices), m_indices(_indices), m_shaderProgram(std::move(_shaderProgram))
{
    LOG_ASSERT(_material);
    LOG_ASSERT(m_shaderProgram);

    m_vao = std::make_shared<VertexArray>();
    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);
    m_ebo = std::make_shared<Buffer<unsigned int>>(BUFFER_TYPE::INDEX_BUFFER, m_indices);

    m_vao->setAttribute<Vertex, decltype(Vertex::position)>(m_shaderProgram->getAttrLocation("aPos"), offsetof(Vertex, position));
    m_vao->setAttribute<Vertex, decltype(Vertex::normal)>(m_shaderProgram->getAttrLocation("aNormal"), offsetof(Vertex, normal));
    m_vao->setAttribute<Vertex, decltype(Vertex::texCoord)>(m_shaderProgram->getAttrLocation("aTexCoord"), offsetof(Vertex, texCoord));
}

void Mesh::render() {
    const auto material = std::dynamic_pointer_cast<CommonMaterial>(m_material.lock());

    m_vao->bind();
    if(const auto texture = material->getDiffuseMap(); texture && m_shaderProgram->isExistUniformVariable("material.diffuse")) {
        m_shaderProgram->setInt("material.diffuse", static_cast<int>(texture->getTextureUnit()));
        texture->activate();
    }
    if(const auto texture = material->getSpecularMap(); texture && m_shaderProgram->isExistUniformVariable("material.specular")) {
        m_shaderProgram->setInt("material.specular", static_cast<int>(texture->getTextureUnit()));
        texture->activate();
    }
    if(const auto texture = material->getNormalMap(); texture && m_shaderProgram->isExistUniformVariable("material.normal")) {
        m_shaderProgram->setInt("material.normal", static_cast<int>(texture->getTextureUnit()));
        texture->activate();
    }
    if(const auto texture = material->getHeightMap(); texture && m_shaderProgram->isExistUniformVariable("material.height")) {
        m_shaderProgram->setInt("material.height", static_cast<int>(texture->getTextureUnit()));
        texture->activate();
    }

    glDrawElements(GL_TRIANGLES, static_cast<int>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
}
