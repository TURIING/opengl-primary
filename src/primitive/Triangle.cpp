/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 14:41
* @version: 1.0
* @description:
********************************************************************************/

#include "Triangle.h"

Triangle::Triangle(std::shared_ptr<IScene> &_parent, const std::string &_name): IPrimitive(_parent, _name) {
    m_vao = std::make_shared<VertexArray>();

    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    m_vao->setAttribute<Vertex, VPos>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, VColor>(this->getShaderProgram()->getAttrLocation("aColor"), offsetof(Vertex, color));
}

void Triangle::render() {
    this->getShaderProgram()->use();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
