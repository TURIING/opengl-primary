/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/25 22:27
* @version: 1.0
* @description: 地板（平面）类
********************************************************************************/

#include "Floor.h"

Floor::Floor(IScene *_parent, std::string _name, std::shared_ptr<ShaderProgram> _shaderProgram): IPrimitive(_parent, _name) {
    this->setPrimitiveType(PrimitiveType::Common);

    m_vao = std::make_shared<VertexArray>();
    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    auto shadeProgram = _shaderProgram ? _shaderProgram : std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);
    this->setShaderProgram(shadeProgram);

    m_vao->setAttribute<Vertex, decltype(Vertex::pos)>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, decltype(Vertex::tex)>(this->getShaderProgram()->getAttrLocation("aTexCoord"), offsetof(Vertex, tex));
}

void Floor::render() {
    this->getShaderProgram()->use();
    m_vao->bind();

    const auto camera = this->getCamera();

    this->getShaderProgram()->setBool("enableOutline", false);

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}
