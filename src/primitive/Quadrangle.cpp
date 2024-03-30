/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 21:36
* @version: 1.0
* @description: 
********************************************************************************/

#include "Quadrangle.h"

Quadrangle::Quadrangle(std::shared_ptr<IScene> &_parent, const std::string &_name) : IPrimitive(_parent, _name) {
    m_vao = std::make_unique<VertexArray>();

    m_vbo = std::make_unique<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    m_vao->setAttribute<Vertex, glm::vec2>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, glm::vec2>(this->getShaderProgram()->getAttrLocation("aTexCoords"), offsetof(Vertex, tex));
}

void Quadrangle::render() {
    this->getShaderProgram()->use();
    m_vao->bind();
    this->getTexture(0)->activate();

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
