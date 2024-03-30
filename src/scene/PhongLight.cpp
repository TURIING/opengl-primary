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

    //m_cube = std::make_unique<Cube>(this, "cube");
    //
    //m_light = std::make_unique<Cube>(this, "light");
    //m_light->setPrimitiveType(PrimitiveType::PhongLight);
    //m_light->scale(0.2, 0.2, 0.2);
    //m_light->translate(1, 1, 1);
    //
    //m_light1 = std::make_unique<Cube>(this, "light1", m_shaderProgram);
    //m_light1->setPrimitiveType(PrimitiveType::PhongLight);
    //m_light1->scale(0.2, 0.2, 0.2);
    //m_light1->translate(2, 2, 2);
    //
    //m_light2 = std::make_unique<Cube>(this, "light2", m_shaderProgram);
    //m_light2->setPrimitiveType(PrimitiveType::PhongLight);
    //m_light2->scale(0.2, 0.2, 0.2);
    //m_light2->translate(-1, -1, -1);
}

void PhongLight::render() {
    IScene::render();
}
