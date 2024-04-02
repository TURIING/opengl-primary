/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 18:30
* @version: 1.0
* @description: 
********************************************************************************/
#include "IPrimitive.h"
#include "IScene.h"
#include "../material/PhongMaterial.h"
#include "../material/SkyboxMaterial.h"
#include "../material/ReflectMaterial.h"

IPrimitive::IPrimitive(std::shared_ptr<IScene> &_parent, const std::string &_name, std::shared_ptr<ShaderProgram> _shaderProgram) {
    this->setRenderType(RenderType::Primitive);
    m_shaderProgram = _shaderProgram ? _shaderProgram : _parent->getShaderProgram();
    this->setCamera(_parent->getCamera());
    this->setRenderName(_name);

    this->setMaterial(std::make_shared<IMaterial>());
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

// 重置纹理
void IPrimitive::resetTexture(unsigned int _index, Texture* _texture) {
    LOG_ASSERT(_texture != nullptr);

    m_textures[_index].reset(_texture);
    const auto name = std::string("texture") + std::to_string(_index + 1);
    this->getShaderProgram()->setInt(name, _index);
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

void IPrimitive::setMaterial(std::shared_ptr<IMaterial> _material) {
    m_material = _material;
}

void IPrimitive::preRender() {
    m_shaderProgram->use();

    // 传递材质
    this->transmitMaterialToShader();
}

PointLight *IPrimitive::getPointLight() {
    LOG_ASSERT(m_lightType == LightType::PointLight);
    return &m_pointLight;
}

DirectionalLight *IPrimitive::getDirectionalLight() {
    LOG_ASSERT(m_lightType == LightType::DirectionalLight);
    return &m_directionalLight;
}

// 传输材质信息到shader
void IPrimitive::transmitMaterialToShader() {
    switch (m_material->getMaterialType()) {
        case MaterialType::Phong: {
            const auto material = std::dynamic_pointer_cast<PhongMaterial>(this->getMaterial());
            this->getShaderProgram()->setInt("material.type", static_cast<int>(material->getMaterialType()));
            this->getShaderProgram()->setInt("material.diffuse", material->getDiffuse()->getTextureUnit());
            this->getShaderProgram()->setInt("material.specular", material->getSpecular()->getTextureUnit());
            this->getShaderProgram()->setFloat("material.shininess", *(material->getShininess()));
            material->getDiffuse()->activate();
            material->getSpecular()->activate();
            break;
        }
        case MaterialType::Reflect: {
            const auto material = std::dynamic_pointer_cast<ReflectMaterial>(this->getMaterial());
            this->getShaderProgram()->setInt("material.type", static_cast<int>(material->getMaterialType()));
            const auto& texture = material->getSkyboxTexture().lock();
            this->getShaderProgram()->setInt("material.skybox", texture ? texture->getTextureUnit() : -1);
            break;
        }
    }
}
