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

std::shared_ptr<ShaderProgram> IPrimitive::getShaderProgram() {
    LOG_ASSERT(m_shaderProgram != nullptr);
    return m_shaderProgram;
}

void IPrimitive::setShaderProgram(std::shared_ptr<ShaderProgram> &_shaderProgram) {
    LOG_ASSERT(_shaderProgram != nullptr);
    m_shaderProgram = _shaderProgram;
}

std::shared_ptr<Texture> IPrimitive::getTexture(int _index) {
    LOG_ASSERT(_index >= 0 && _index < m_textures.size());

    return m_textures.at(_index);
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



