/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 19:28
* @version: 1.0
* @description:
********************************************************************************/

#include "Blend.h"

Blend::Blend() {
    this->setClearColor(0, 0, 0, 1);
    this->enableDeepTest();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);

    m_floor = std::make_unique<Floor>(this, m_shaderProgram);

    m_cube1 = std::make_unique<Cube>(this, m_shaderProgram);

    m_cube2 = std::make_unique<Cube>(this, m_shaderProgram);

    m_window1 = std::make_unique<Square>(this, m_shaderProgram);
    m_window2 = std::make_unique<Square>(this, m_shaderProgram);
    m_window3 = std::make_unique<Square>(this, m_shaderProgram);
}

void Blend::paint() {
    this->clear();

    // 给窗户的位置排序
    std::map<float, VPos> sortedPos;
    for(auto &pos: m_windowPos) {
        auto distance = glm::length(this->getCamera()->getPosition() - glm::vec3(pos.x, pos.y, pos.z));
        sortedPos[distance] = pos;
    }

    ImGui::Begin("Cube");
    static float translateValue1[] = {-1.0f, 0.0f, -1.0f};
    ImGui::InputFloat3("translate1", translateValue1, 2);

    static float translateValue2[] = {2.0f, 0.0f, 0.0f};
    ImGui::InputFloat3("translate2", translateValue2, 2);
    ImGui::End();

    m_floor->paint();

    m_cube1->translate(translateValue1[0], translateValue1[1], translateValue1[2]);
    m_cube1->paint();

    m_cube2->translate(translateValue2[0], translateValue2[1], translateValue2[2]);
    m_cube2->paint();

    const auto pos1 = sortedPos.rbegin()->second;
    m_window1->translate(pos1.x, pos1.y, pos1.z);
    m_window1->paint();

    const auto pos2 = sortedPos.begin()->second;
    m_window2->translate(pos2.x, pos2.y, pos2.z);
    m_window2->paint();

}
