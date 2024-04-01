/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/31 23:31
* @version: 1.0
* @description: 
********************************************************************************/

#include "SkyboxMaterial.h"

SkyboxMaterial::SkyboxMaterial(std::shared_ptr<ShaderProgram> _shaderProgram, const std::vector<std::string> &_pathVec) {
    this->setMaterialType(MaterialType::SkyBox);

    m_texture = std::make_shared<Texture>(_pathVec, 0, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR,
                                          GL_LINEAR);
    _shaderProgram->setInt("material.skybox", m_texture->getTextureUnit());
}