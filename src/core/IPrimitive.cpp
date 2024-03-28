/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 18:30
* @version: 1.0
* @description: 
********************************************************************************/
#include "IPrimitive.h"
#include "IScene.h"

IPrimitive::IPrimitive(IScene *_parent, std::string &_name) {
    _parent->addPrimitive(this);
    this->setCamera(_parent->getCamera());
    this->setRenderName(_name);
}

IPrimitive::IPrimitive(IScene *_parent, std::string &_name, std::shared_ptr<ShaderProgram> &_shaderProgram): IPrimitive(_parent, _name) {
    m_shaderProgram = _shaderProgram;
}

/**
 * 开启轮廓绘制并设定轮廓参数
 * @param _width
 * @param _color
 */
void IPrimitive::setOutline(int _width, Color _color) {
    m_enableOutline = true;
    m_outlineColor = _color;
    m_outlineWidth = _width;
}

// 添加纹理
void IPrimitive::addTexture(std::shared_ptr<Texture> _texture) {
    m_textures.push_back(_texture);

    const auto name = std::string("texture") + std::to_string(m_textures.size());
    this->getShaderProgram()->setInt(name, m_textures.size() - 1);
}

std::shared_ptr<Texture> IPrimitive::getTexture(int _index) {
    LOG_ASSERT(_index >= 0 && _index < m_textures.size());
    return m_textures.at(_index);
}

std::shared_ptr<ShaderProgram> IPrimitive::getShaderProgram() {
    LOG_ASSERT(m_shaderProgram != nullptr);
    return m_shaderProgram;
}

void IPrimitive::setShaderProgram(std::shared_ptr<ShaderProgram> &_shaderProgram) {
    LOG_ASSERT(_shaderProgram != nullptr);
    m_shaderProgram = _shaderProgram;
}

std::shared_ptr<Camera> IPrimitive::getCamera() {
    LOG_ASSERT(m_camera) << " Camera pointer not initialized.";
    return m_camera;
}

void IPrimitive::setCamera(std::shared_ptr<Camera> &_camera) {
    m_camera = _camera;
}

PrimitiveType IPrimitive::getPrimitiveType() {
    LOG_ASSERT(m_primitiveType != PrimitiveType::None);
    return m_primitiveType;
}

void IPrimitive::setMaterial(const Material &_material) {
    m_material = _material;
}

void IPrimitive::preRender() {
    m_shaderProgram->use();
    // 传递材质
    m_shaderProgram->setVec3("material.ambient", m_material.ambient);
    m_shaderProgram->setVec3("material.diffuse", m_material.diffuse);
    m_shaderProgram->setVec3("material.specular", m_material.specular);
    m_shaderProgram->setFloat("material.shininess", m_material.shininess);

    // 传递model矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, *this->getPosition());
    model = glm::scale(model, *this->getScaling());
    model = glm::rotate(model, this->getRotation()->x, glm::vec3{ 1, 0, 0 });
    model = glm::rotate(model, this->getRotation()->y, glm::vec3{ 0, 1, 0 });
    model = glm::rotate(model, this->getRotation()->z, glm::vec3{ 0, 0, 1 });
    this->getShaderProgram()->setMat4("model", glm::value_ptr(model));

    // 激活纹理
    if(m_textures.size() == 0) {
        this->getShaderProgram()->setBool("enableTexture", false);
    }
    else {
        this->getShaderProgram()->setBool("enableTexture", true);
        for(auto i = 0; i < m_textures.size(); i++) {
            m_textures[i]->activate(GL_TEXTURE0 + i);
        }
    }

    // 摄像机信息
     m_shaderProgram->setVec3("cameraPos", this->getCamera()->getPosition());
}

PointLight *IPrimitive::getPointLight() {
    LOG_ASSERT(m_primitiveType == PrimitiveType::PhongLight);
    return &m_pointLight;
}


