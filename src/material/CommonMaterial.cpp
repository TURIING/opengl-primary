/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/3 15:55
* @version: 1.0
* @description: 
********************************************************************************/

#include "CommonMaterial.h"
#include "../base/ImageTexture.h"
#include "../base/ShaderProgram.h"

CommonMaterial::CommonMaterial(std::shared_ptr<ShaderProgram> _shaderProgram) {
    LOG_ASSERT(_shaderProgram);
    this->setMaterialType(MaterialType::Common);
    m_shaderProgram = _shaderProgram;
}

void CommonMaterial::setDiffuseMap(std::shared_ptr<ImageTexture> _texture) {
    m_diffuseMap = std::move(_texture);
}

void CommonMaterial::setHeightMap(std::shared_ptr<ImageTexture> _texture) {
    m_heightMap = std::move(_texture);
}

void CommonMaterial::setNormalMap(std::shared_ptr<ImageTexture> _texture) {
    m_normalMap = std::move(_texture);
}

void CommonMaterial::setSpecularMap(std::shared_ptr<ImageTexture> _texture) {
    m_specularMap = std::move(_texture);
}

void CommonMaterial::resetDiffuseMap(const std::string &_file) {
    m_diffuseMap = std::make_shared<ImageTexture>(_file, m_diffuseMap->getTextureUnit(), GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
}

void CommonMaterial::resetSpecularMap(const std::string &_file) {
    m_specularMap = std::make_shared<ImageTexture>(_file, m_specularMap->getTextureUnit(), GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
}

void CommonMaterial::resetNormalMap(const std::string &_file) {
    m_normalMap = std::make_shared<ImageTexture>(_file, m_normalMap->getTextureUnit(), GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
}

void CommonMaterial::resetHeightMap(const std::string &_file) {
    m_heightMap = std::make_shared<ImageTexture>(_file, m_heightMap->getTextureUnit(), GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
}
