/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 21:36
* @version: 1.0
* @description: 
********************************************************************************/

#include "Quadrangle.h"

Quadrangle::Quadrangle(IScene *_parent, std::string _name, std::shared_ptr<ShaderProgram> _shaderProgram) : IPrimitive(_parent, _name) {
    m_vao = std::make_unique<VertexArray>();

    m_vbo = std::make_unique<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    auto shadeProgram = _shaderProgram ? _shaderProgram : std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);
    this->setShaderProgram(shadeProgram);

    m_vao->setAttribute<Vertex, glm::vec2>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, glm::vec2>(this->getShaderProgram()->getAttrLocation("aTexCoords"), offsetof(Vertex, tex));
}

void Quadrangle::render() {
    this->getShaderProgram()->use();
    m_vao->bind();
    this->getTexture(0)->activate(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
