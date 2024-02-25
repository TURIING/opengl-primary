/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/25 14:51
* @version: 1.0
* @description:
********************************************************************************/

#include "ModelLoad.h"

/**
 *
 * @param _path 模型文件路径
 */
ModelLoad::ModelLoad(std::string _path) {
    this->enableDeepTest();
    m_shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);
    m_model = std::make_shared<Model>(_path, std::vector<MaterialType>{ MaterialType::Diffuse }, m_shaderProgram);
}

void ModelLoad::paint() {
    this->clear();
    m_shaderProgram->use();

    const auto camera = this->getCamera();

    ImGui::Begin("ModelLoad");

    static float scaleValue = 0.1;
    ImGui::InputFloat("scale", &scaleValue);

    static float translateValue[] = { 1.0, 1.0, 1.0 };
    ImGui::InputFloat3("translate", translateValue);

    static float rotateValue[] = { 0.0, 0.0, 0.0 };
    ImGui::InputFloat3("rotate", rotateValue);

    ImGui::End();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::scale(model, glm::vec3(scaleValue, scaleValue, scaleValue));
    model = glm::translate(model, glm::vec3(translateValue[0], translateValue[1], translateValue[2]));
    model = glm::rotate(model, rotateValue[0], glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, rotateValue[1], glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, rotateValue[2], glm::vec3(0.0, 0.0, 1.0));
    view = camera->getViewMatrix();
    projection = glm::perspective(glm::radians(camera->getFov()), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 100.0f);

    m_shaderProgram->setMat4("model", glm::value_ptr(model));
    m_shaderProgram->setMat4("view", glm::value_ptr(view));
    m_shaderProgram->setMat4("projection", glm::value_ptr(projection));

    m_model->paint();
}
