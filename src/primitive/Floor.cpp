/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/25 22:27
* @version: 1.0
* @description: 地板（平面）类
********************************************************************************/

#include "Floor.h"
#include "../material/PhongMaterial.h"

Floor::Floor(std::shared_ptr<IScene> _parent, std::string _name): IPrimitive(_parent, _name) {
    this->setPrimitiveType(PrimitiveType::Floor);

    m_vao = std::make_shared<VertexArray>();
    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    m_vao->setAttribute<Vertex, decltype(Vertex::position)>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, position));
    m_vao->setAttribute<Vertex, decltype(Vertex::normal)>(this->getShaderProgram()->getAttrLocation("aNormal"), offsetof(Vertex, normal));
    m_vao->setAttribute<Vertex, decltype(Vertex::texCoord)>(this->getShaderProgram()->getAttrLocation("aTexCoord"), offsetof(Vertex, texCoord));
}

void Floor::render() {
    m_vao->bind();

    //const auto camera = this->getCamera();

    //this->getShaderProgram()->setBool("enableOutline", false);

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

void Floor::preRender() {
    IPrimitive::preRender();

    // 传递model矩阵
    const auto scaling = *this->getScaling();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, *this->getPosition());
    model = glm::scale(model, glm::vec3(scaling, scaling, scaling));
    model = glm::rotate(model, this->getRotation()->x, glm::vec3{ 1, 0, 0 });
    model = glm::rotate(model, this->getRotation()->y, glm::vec3{ 0, 1, 0 });
    model = glm::rotate(model, this->getRotation()->z, glm::vec3{ 0, 0, 1 });
    this->getShaderProgram()->setMat4("model", glm::value_ptr(model));
}
