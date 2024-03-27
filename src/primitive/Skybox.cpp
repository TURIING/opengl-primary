/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/24 19:35
* @version: 1.0
* @description: 
********************************************************************************/

#include "Skybox.h"

const std::vector<std::string> pathVec = {
    std::string(TEXTURE_PATH) + "/skybox/right.jpg",
    std::string(TEXTURE_PATH) + "/skybox/left.jpg",
    std::string(TEXTURE_PATH) + "/skybox/top.jpg",
    std::string(TEXTURE_PATH) + "/skybox/bottom.jpg",
    std::string(TEXTURE_PATH) + "/skybox/front.jpg",
    std::string(TEXTURE_PATH) + "/skybox/back.jpg",
};

const std::string VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/skybox/vertex.glsl";
const std::string FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/skybox/fragment.glsl";

Skybox::Skybox(IScene *_parent, std::string _name, std::shared_ptr<ShaderProgram> _shaderProgram): IPrimitive(_parent, _name){
    this->setRenderType(RenderType::Primitive);
    this->setPrimitiveType(PrimitiveType::Skybox);

    m_vao = std::make_shared<VertexArray>();

    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    auto shadeProgram = _shaderProgram ? _shaderProgram : std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);
    this->setShaderProgram(shadeProgram);

    m_vao->setAttribute<Vertex, glm::vec3>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, position));

    m_texture = std::make_shared<Texture>(pathVec, GL_REPEAT, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    this->getShaderProgram()->setInt("skybox", 0);

}

void Skybox::render() {
    this->setDeepTest(true);
    glDepthFunc(GL_LEQUAL);

    this->getShaderProgram()->use();
    m_vao->bind();
    m_texture->activate(GL_TEXTURE0);
    const auto camera = this->getCamera();

    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjection();

    this->getShaderProgram()->setMat4("view", glm::value_ptr(view));
    this->getShaderProgram()->setMat4("projection", glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS);
}
