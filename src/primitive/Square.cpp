/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 21:36
* @version: 1.0
* @description: 
********************************************************************************/

#include "Square.h"

Square::Square(IScene *_parent, std::string _name, std::shared_ptr<ShaderProgram> _shaderProgram) : IPrimitive(_parent, _name) {
    this->setPrimitiveType(PrimitiveType::Common);

    m_vao = std::make_unique<VertexArray>();

    m_vbo = std::make_unique<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    auto shadeProgram = _shaderProgram ? _shaderProgram : std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);
    this->setShaderProgram(shadeProgram);

    m_texture = std::make_unique<Texture>(TEXTURE_FILE, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    this->getShaderProgram()->setInt("texture1", m_texture->getTextureUnit());

    m_vao->setAttribute<Vertex, VPos>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, VTexCoord>(this->getShaderProgram()->getAttrLocation("aTexCoord"), offsetof(Vertex, tex));
}

void Square::render() {
    this->getShaderProgram()->use();
    m_vao->bind();
    m_texture->activate();
    const auto camera = this->getCamera();

    this->getShaderProgram()->setBool("enableOutline", false);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
