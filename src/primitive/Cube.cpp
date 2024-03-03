/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 20:19
* @version: 1.0
* @description: 
********************************************************************************/

#include "Cube.h"

Cube::Cube(IScene *_parent, std::shared_ptr<ShaderProgram> _shaderProgram) : IPrimitive(_parent) {
    m_vao = std::make_shared<VertexArray>();

    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    m_shaderProgram = _shaderProgram ? _shaderProgram : std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);

    m_texture = std::make_shared<Texture>(TEXTURE_FILE, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    m_shaderProgram->setInt("texture1", 0);

    m_vao->setAttribute<Vertex, VPos>(m_shaderProgram->getAttrLocation("aPos"), offsetof(Vertex, pos));
    m_vao->setAttribute<Vertex, VTexCoord>(m_shaderProgram->getAttrLocation("aTexCoord"), offsetof(Vertex, tex));
}

void Cube::paint() {
    this->isEnabledOutline() ? paintWithOutline() : paintNormally();
}

// 常规地绘制立方体
void Cube::paintNormally() {
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

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// 绘制立方体及其轮廓
void Cube::paintWithOutline() {
    m_vao->bind();
    m_shaderProgram->use();
    m_texture->activate(GL_TEXTURE0);
    const auto camera = this->getCamera();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->getTranslatePos());

    glm::mat4 view = glm::mat4(1.0f);
    view = camera->getViewMatrix();
    m_shaderProgram->setMat4("view", glm::value_ptr(view));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera->getFov()), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 100.0f);
    m_shaderProgram->setMat4("projection", glm::value_ptr(projection));

    // 绘制一个普通的立方体
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xff);

    m_shaderProgram->setBool("enableOutline", false);
    m_shaderProgram->setMat4("model", glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 绘制放大版本的立方体
    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    m_shaderProgram->setBool("enableOutline", true);
    const auto [r, g, b, a] = this->getOutlineColor();
    m_shaderProgram->setVec4("outlineColor", glm::vec4(r, g, b, a));

    float scale = 1 + this->getOutlineWidth() * 0.01;
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    m_shaderProgram->setMat4("model", glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 还原状态
    glStencilMask(0xff);
    glStencilFunc(GL_ALWAYS, 0, 0xff);
    glEnable(GL_DEPTH_TEST);
}




