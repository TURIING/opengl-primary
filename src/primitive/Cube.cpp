/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 20:19
* @version: 1.0
* @description: 
********************************************************************************/

#include "Cube.h"

Cube::Cube(IScene *_parent, std::string _name, std::shared_ptr<ShaderProgram> _shaderProgram) : IPrimitive(_parent, _name) {
    this->setRenderType(RenderType::Primitive);
    this->setPrimitiveType(PrimitiveType::Cube);

    m_vao = std::make_shared<VertexArray>();

    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    auto shadeProgram = _shaderProgram ? _shaderProgram : std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);
    this->setShaderProgram(shadeProgram);

    m_vao->setAttribute<Vertex, decltype(Vertex::position)>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, position));
    m_vao->setAttribute<Vertex, decltype(Vertex::normal)>(this->getShaderProgram()->getAttrLocation("aNormal"), offsetof(Vertex, normal));
    m_vao->setAttribute<Vertex, decltype(Vertex::texCoords)>(this->getShaderProgram()->getAttrLocation("aTexCoord"), offsetof(Vertex, texCoords));
}

void Cube::render() {
    this->preRender();
    this->isEnabledOutline() ? paintWithOutline() : paintNormally();
}

// 常规地绘制立方体
void Cube::paintNormally() {
    m_vao->bind();
    const auto camera = this->getCamera();

    this->getShaderProgram()->setBool("enableOutline", false);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

// 绘制立方体及其轮廓
void Cube::paintWithOutline() {
    //m_vao->bind();
    //this->getShaderProgram()->use();
    //m_texture->activate(GL_TEXTURE0);
    //const auto camera = this->getCamera();
    //
    //glm::mat4 model = glm::mat4(1.0f);
    //
    //glm::mat4 view = glm::mat4(1.0f);
    //view = camera->getViewMatrix();
    //this->getShaderProgram()->setMat4("view", glm::value_ptr(view));
    //
    //glm::mat4 projection = glm::mat4(1.0f);
    //projection = glm::perspective(glm::radians(camera->getFov()), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 100.0f);
    //this->getShaderProgram()->setMat4("projection", glm::value_ptr(projection));
    //
    //// 绘制一个普通的立方体
    //glStencilFunc(GL_ALWAYS, 1, 0xff);
    //glStencilMask(0xff);
    //
    //this->getShaderProgram()->setBool("enableOutline", false);
    //this->getShaderProgram()->setMat4("model", glm::value_ptr(model));
    //
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //
    //// 绘制放大版本的立方体
    //glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    //glStencilMask(0x00);
    //glDisable(GL_DEPTH_TEST);
    //
    //this->getShaderProgram()->setBool("enableOutline", true);
    //const auto [r, g, b, a] = this->getOutlineColor();
    //this->getShaderProgram()->setVec4("outlineColor", glm::vec4(r, g, b, a));
    //
    //float scale = 1 + this->getOutlineWidth() * 0.01;
    //model = glm::scale(model, glm::vec3(scale, scale, scale));
    //this->getShaderProgram()->setMat4("model", glm::value_ptr(model));
    //
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //
    //// 还原状态
    //glStencilMask(0xff);
    //glStencilFunc(GL_ALWAYS, 0, 0xff);
    //glEnable(GL_DEPTH_TEST);
}




