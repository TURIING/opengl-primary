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

    m_shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);

    m_vao->setAttribute<Vertex, VPos>(m_shaderProgram->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, VColor>(m_shaderProgram->getAttrLocation("aColor"), offsetof(Vertex, color));
}

void Triangle::paint() {
    m_shaderProgram->use();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
