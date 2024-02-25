/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 14:41
* @version: 1.0
* @description:
********************************************************************************/

#include "Triangle.h"

Triangle::Triangle() {
    m_vao = std::make_shared<VertexArray>();

    m_vbo = std::make_shared<Buffer<float>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    m_shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);

    m_vao->setAttribute(m_shaderProgram->getAttrLocation("aPos"), 3, 6, 0);
    m_vao->setAttribute(m_shaderProgram->getAttrLocation("aColor"), 3, 6, 3);
}

void Triangle::paint() {
    this->clear();

    m_shaderProgram->use();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
