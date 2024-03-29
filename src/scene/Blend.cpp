/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 19:28
* @version: 1.0
* @description:
********************************************************************************/

#include "Blend.h"

Blend::Blend() {
    this->setClearColor(0.2, 0.2, 0.2, 1);
    this->setDeepTest(true);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);

    m_floor = std::make_unique<Floor>(this, "floor", m_shaderProgram);
    //m_floor->addTexture(std::make_shared<Texture>())

    m_cube1 = std::make_unique<Cube>(this, "cube_1", m_shaderProgram);

    m_cube2 = std::make_unique<Cube>(this, "cube_2", m_shaderProgram);

    m_window1 = std::make_unique<Square>(this, "window_1", m_shaderProgram);
    m_window2 = std::make_unique<Square>(this, "window_2", m_shaderProgram);
}

void Blend::render() {
    // 给窗户的位置排序
    //std::map<float, VPos> sortedPos;
    //for(auto &pos: m_windowPos) {
    //    auto distance = glm::length(this->getCamera()->getPosition() - glm::vec3(pos.x, pos.y, pos.z));
    //    sortedPos[distance] = pos;
    //}

    //ImGui::Begin("Cube");
    //static float translateValue1[] = {-1.0f, 0.0f, -1.0f};
    //ImGui::InputFloat3("translate1", translateValue1, 2);
    //
    //static float translateValue2[] = {2.0f, 0.0f, -1.2f};
    //ImGui::InputFloat3("translate2", translateValue2, 2);
    //ImGui::End();

    m_floor->render();

    //m_cube1->translate(-1.0f, 0.0f, -1.0f);
    m_cube1->render();

    //m_cube2->translate(2.0f, 0.0f, -1.2f);
    m_cube2->render();

    //const auto pos1 = sortedPos.rbegin()->second;
    //m_window1->translate(1.5f, 0.0f, -0.48f);
    m_window1->render();

    //const auto pos2 = sortedPos.begin()->second;
    //m_window2->translate(0.5f, 0.0f, -0.6f);
    m_window2->render();

}
