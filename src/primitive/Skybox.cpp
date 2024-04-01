/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/24 19:35
* @version: 1.0
* @description: 
********************************************************************************/

#include "Skybox.h"
#include "../material/SkyboxMaterial.h"

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

Skybox::Skybox(std::shared_ptr<IScene> &_parent, const std::string &_name): IPrimitive(_parent, _name, std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE)){
    this->setRenderType(RenderType::Primitive);
    this->setPrimitiveType(PrimitiveType::Skybox);

    m_vao = std::make_shared<VertexArray>();

    m_vbo = std::make_shared<Buffer<Vertex>>(BUFFER_TYPE::VERTEX_BUFFER, m_vertices);

    m_vao->setAttribute<Vertex, decltype(Vertex::position)>(this->getShaderProgram()->getAttrLocation("aPos"), offsetof(Vertex, position));

    this->setMaterial(std::make_shared<SkyboxMaterial>(this->getShaderProgram(), pathVec));
}

void Skybox::preRender() {
    IPrimitive::preRender();
}

void Skybox::render() {
    glDepthFunc(GL_LEQUAL);
    const auto material = std::dynamic_pointer_cast<SkyboxMaterial>(this->getMaterial());

    this->getShaderProgram()->use();
    m_vao->bind();
    material->getTexture()->activate();
    const auto camera = this->getCamera();

    glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMatrix()));
    glm::mat4 projection = camera->getProjection();

    this->getShaderProgram()->setMat4("view", glm::value_ptr(view));
    this->getShaderProgram()->setMat4("projection", glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS);
}
