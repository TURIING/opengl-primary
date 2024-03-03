/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 21:36
* @version: 1.0
* @description: 
********************************************************************************/

#include "Square.h"

Square::Square(IScene *_parent, std::shared_ptr<ShaderProgram> _shaderProgram) : IPrimitive(_parent) {
    m_vao = std::make_unique<VertexArray>();

    m_vbo = std::make_unique<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    m_shaderProgram = _shaderProgram ? _shaderProgram : std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);

    m_texture = std::make_unique<Texture>(TEXTURE_FILE, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    m_shaderProgram->setInt("texture1", 0);

    m_vao->setAttribute<Vertex, VPos>(m_shaderProgram->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, VTexCoord>(m_shaderProgram->getAttrLocation("aTexCoord"), offsetof(Vertex, tex));
}

void Square::paint() {
    m_shaderProgram->use();
    m_vao->bind();
    m_texture->activate(GL_TEXTURE0);
    const auto camera = this->getCamera();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::translate(model, this->getTranslatePos());
    view = camera->getViewMatrix();
    projection = glm::perspective(glm::radians(camera->getFov()), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 100.0f);

    m_shaderProgram->setMat4("model", glm::value_ptr(model));
    m_shaderProgram->setMat4("view", glm::value_ptr(view));
    m_shaderProgram->setMat4("projection", glm::value_ptr(projection));
    m_shaderProgram->setBool("enableOutline", false);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
