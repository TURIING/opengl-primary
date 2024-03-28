/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 19:28
* @version: 1.0
* @description:
********************************************************************************/

#include "PhongLight.h"

PhongLight::PhongLight() {
    this->setClearColor(0.2, 0.2, 0.2, 1);
    this->setDeepTest(true);

    m_shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);

    m_cube = std::make_unique<Cube>(this, "cube", m_shaderProgram);

    m_light = std::make_unique<Cube>(this, "light", m_shaderProgram);
    m_light->setPrimitiveType(PrimitiveType::PhongLight);
    m_light->scale(0.2, 0.2, 0.2);
    m_light->translate(1, 1, 1);
}

void PhongLight::render() {
    // 给窗户的位置排序
    //std::map<float, VPos> sortedPos;
    //for(auto &pos: m_windowPos) {
    //    auto distance = glm::length(this->getCamera()->getPosition() - glm::vec3(pos.x, pos.y, pos.z));
    //    sortedPos[distance] = pos;
    //}

    m_cube->render();
    m_light->render();
}
