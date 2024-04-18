/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 21:36
* @version: 1.0
* @description: 
********************************************************************************/

#include "Quadrangle.h"

Quadrangle::Quadrangle(std::shared_ptr<IScene> _parent, const std::string &_name, std::shared_ptr<ShaderProgram> &_shaderProgram) : IPrimitive(_parent, _name, _shaderProgram) {
    m_vao = std::make_unique<VertexArray>();

    m_vbo = std::make_unique<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    m_vao->setAttribute<Vertex, decltype(Vertex::pos)>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, decltype(Vertex::tex)>(this->getShaderProgram()->getAttrLocation("aTexCoords"), offsetof(Vertex, tex));
}

void Quadrangle::render() {
    this->getShaderProgram()->use();
    m_vao->bind();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
