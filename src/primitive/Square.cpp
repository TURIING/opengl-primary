/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 21:36
* @version: 1.0
* @description: 
********************************************************************************/

#include "Square.h"
#include "../material/PhongMaterial.h"

Square::Square(std::shared_ptr<IScene> _parent, std::string _name): IPrimitive(_parent, _name) {
    this->setPrimitiveType(PrimitiveType::Square);

    m_vao = std::make_unique<VertexArray>();

    m_vbo = std::make_unique<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

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

    //this->getShaderProgram()->setBool("enableOutline", false);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Square::preRender() {
    IPrimitive::preRender();

    // 传递model矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, *this->getPosition());
    model = glm::scale(model, *this->getScaling());
    model = glm::rotate(model, this->getRotation()->x, glm::vec3{ 1, 0, 0 });
    model = glm::rotate(model, this->getRotation()->y, glm::vec3{ 0, 1, 0 });
    model = glm::rotate(model, this->getRotation()->z, glm::vec3{ 0, 0, 1 });
    this->getShaderProgram()->setMat4("model", glm::value_ptr(model));
}
