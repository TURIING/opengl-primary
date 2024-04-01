/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/29 15:14
* @version: 1.0
* @description: 
********************************************************************************/

#include "PhongMaterial.h"

#include <utility>
#include "../base/Texture.h"
#include "../base/ShaderProgram.h"

PhongMaterial::PhongMaterial(std::shared_ptr<ShaderProgram> &_shaderProgram, const std::string &_diffuse, const std::string &_specular, float _shininess): m_shaderProgram(_shaderProgram) {
    this->setMaterialType(MaterialType::Phong);

    m_diffuse = std::make_shared<Texture>(_diffuse, 0, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    m_shaderProgram->setInt("material.diffuse", m_diffuse->getTextureUnit());

    m_specular = std::make_shared<Texture>(_specular, 1, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    m_shaderProgram->setInt("material.specular", m_specular->getTextureUnit());

    m_shininess = _shininess;
}

std::shared_ptr<Texture> &PhongMaterial::getDiffuse() { return m_diffuse; }

std::shared_ptr<Texture> &PhongMaterial::getSpecular() { return m_specular; }

float *PhongMaterial::getShininess() { return &m_shininess; }

void PhongMaterial::setDiffuse(std::shared_ptr<Texture> diffuse) { m_diffuse = std::move(diffuse); }

void PhongMaterial::setSpecular(std::shared_ptr<Texture> specular) { m_specular = std::move(specular); }

void PhongMaterial::setShininess(float shininess) { m_shininess = shininess; }

void PhongMaterial::resetDiffuse(const std::string &_path) {
    m_shaderProgram->setInt("material.diffuse", m_diffuse->getTextureUnit());
    m_diffuse.reset(new Texture(_path, m_diffuse->getTextureUnit(), GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR));
}

void PhongMaterial::resetSpecular(const std::string &_path) {
    m_shaderProgram->setInt("material.specular", m_specular->getTextureUnit());
    m_specular.reset(new Texture(_path, m_specular->getTextureUnit(), GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR));
}