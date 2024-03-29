/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 19:28
* @version: 1.0
* @description:
********************************************************************************/

#include "PhongLight.h"
#include "../base/Material.h"

PhongLight::PhongLight() {
    this->setClearColor(0.2, 0.2, 0.2, 1);
    this->setDeepTest(true);

    m_shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);

    m_cube = std::make_unique<Cube>(this, "cube", m_shaderProgram);

    m_light = std::make_unique<Cube>(this, "light", m_shaderProgram);
    m_light->setPrimitiveType(PrimitiveType::PhongLight);
    m_light->scale(0.2, 0.2, 0.2);
    m_light->translate(1, 1, 1);

    m_light1 = std::make_unique<Cube>(this, "light1", m_shaderProgram);
    m_light1->setPrimitiveType(PrimitiveType::PhongLight);
    m_light1->scale(0.2, 0.2, 0.2);
    m_light1->translate(1, 1, 1);
}

void PhongLight::render() {
    m_cube->render();
    m_light->render();
    m_light1->render();
}
