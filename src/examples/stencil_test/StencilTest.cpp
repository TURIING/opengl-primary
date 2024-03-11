/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/25 23:15
* @version: 1.0
* @description: 
********************************************************************************/

#include "StencilTest.h"

StencilTest::StencilTest() {
    this->setDeepTest(true);
    glDepthFunc(GL_LESS);

    this->setStencilTest(true);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    m_floor = std::make_shared<Floor>(this);

    m_cube1 = std::make_shared<Cube>(this);
    m_cube1->setOutline(1, {1.0f, 1.0f, 1.0f, 1.0f});

    m_cube2 = std::make_shared<Cube>(this);
    m_cube2->setOutline(1, {1.0f, 1.0f, 1.0f, 1.0f});
}

void StencilTest::paint() {
    this->clear();

    ImGui::Begin("Cube");
    static float translateValue1[] = {-1.0f, 0.0f, -1.0f};
    ImGui::InputFloat3("translate1", translateValue1, 2);

    static float translateValue2[] = {2.0f, 0.0f, 0.0f};
    ImGui::InputFloat3("translate2", translateValue2, 2);
    ImGui::End();

    glStencilMask(0x00);

    m_floor->paint();

    m_cube1->translate(translateValue1[0], translateValue1[1], translateValue1[2]);
    m_cube1->paint();

    m_cube2->translate(translateValue2[0], translateValue2[1], translateValue2[2]);
    m_cube2->paint();
}
