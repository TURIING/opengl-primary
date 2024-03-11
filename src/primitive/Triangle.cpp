/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 14:41
* @version: 1.0
* @description:
********************************************************************************/

#include "Triangle.h"

Triangle::Triangle(IScene *_parent): IPrimitive(_parent) {
    m_vao = std::make_shared<VertexArray>();

    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    auto shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);
    this->setShaderProgram(shaderProgram);

    m_vao->setAttribute<Vertex, VPos>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, VColor>(this->getShaderProgram()->getAttrLocation("aColor"), offsetof(Vertex, color));
}

void Triangle::paint() {
    this->getShaderProgram()->use();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
