/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/25 22:27
* @version: 1.0
* @description: 地板（平面）类
********************************************************************************/

#include "Floor.h"

Floor::Floor() {
    m_vao = std::make_shared<VertexArray>();
    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    m_shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);

    m_vao->setAttribute<Vertex, VPos>(m_shaderProgram->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, VTexCoord>(m_shaderProgram->getAttrLocation("aTexCoord"), offsetof(Vertex, tex));

    m_texture = std::make_shared<Texture>(TEXTURE_FILE, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    m_shaderProgram->setInt("texture1", 0);
}

void Floor::paint() {
    LOG_ASSERT(this->getCamera());

    m_shaderProgram->use();
    m_vao->bind();
    m_texture->activate(GL_TEXTURE0);

    const auto camera = this->getCamera();

    ImGui::Begin("Floor");
    static float scaleValue[] = {1.0, 1.0, 1.0};
    ImGui::InputFloat3("scale", scaleValue, 2);
    ImGui::End();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::scale(model, glm::vec3(scaleValue[0], scaleValue[1], scaleValue[2]));
    view = camera->getViewMatrix();
    projection = glm::perspective(glm::radians(camera->getFov()), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 100.0f);

    m_shaderProgram->setMat4("model", glm::value_ptr(model));
    m_shaderProgram->setMat4("view", glm::value_ptr(view));
    m_shaderProgram->setMat4("projection", glm::value_ptr(projection));
    m_shaderProgram->setBool("enableOutline", false);

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}
