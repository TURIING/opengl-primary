/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/25 22:27
* @version: 1.0
* @description: 地板（平面）类
********************************************************************************/

#include "Floor.h"

Floor::Floor(IScene *_parent, std::shared_ptr<ShaderProgram> _shaderProgram): IPrimitive(_parent) {
    m_vao = std::make_shared<VertexArray>();
    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    auto shadeProgram = _shaderProgram ? _shaderProgram : std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);
    this->setShaderProgram(shadeProgram);

    m_vao->setAttribute<Vertex, VPos>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, VTexCoord>(this->getShaderProgram()->getAttrLocation("aTexCoord"), offsetof(Vertex, tex));

    m_texture = std::make_shared<Texture>(TEXTURE_FILE, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    this->getShaderProgram()->setInt("texture1", 0);
}

void Floor::paint() {
    this->getShaderProgram()->use();
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

    this->getShaderProgram()->setMat4("model", glm::value_ptr(model));
    this->getShaderProgram()->setMat4("view", glm::value_ptr(view));
    this->getShaderProgram()->setMat4("projection", glm::value_ptr(projection));
    this->getShaderProgram()->setBool("enableOutline", false);

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}
