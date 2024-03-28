/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/20 22:08
* @version: 1.0
* @description: 
********************************************************************************/

#include "InspectPanel.h"
#include <tuple>
#include "imgui/imgui.h"
#include "Application.h"
#include "IPrimitive.h"

void InspectPanel::render() {
    ImGui::Begin("Inspect");
    if(!m_currentPrimitive) {
        ImGui::End();
        return;
    }

    // 基本变换
    if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Position", (float *)m_currentPrimitive->getPosition(), 0.01f, -100.0f, 100.0f, "%.2f");
        ImGui::DragFloat3("Scale", (float *)m_currentPrimitive->getScaling(), 0.01f, 0.0f, 10.0f, "%.2f");
        ImGui::DragFloat3("Rotate", (float *)m_currentPrimitive->getRotation(), 0.01f, -180.0f, 180.0f, "%.2f");
    }

    // 材质
    if(ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto material = m_currentPrimitive->getMaterial();
        ImGui::DragFloat3("ambient", (float *)(&material->ambient), 0.01f, 0.0f, 10.0f, "%.2f");
        ImGui::DragFloat3("diffuse", (float *)(&material->diffuse), 0.01f, 0.0f, 10.0f, "%.2f");
        ImGui::DragFloat3("specular", (float *)(&material->specular), 0.01f, 0.0f, 10.0f, "%.2f");
        ImGui::DragFloat("shininess", &material->shininess, 1, 0.0f, 100.0f, "%.2f");
    }

    // 点光源
    if(m_currentPrimitive->getPrimitiveType() == PrimitiveType::PhongLight) {
        if(ImGui::CollapsingHeader("PointLight", ImGuiTreeNodeFlags_DefaultOpen)) {
            auto light = m_currentPrimitive->getPointLight();
            ImGui::DragFloat3("ambient", (float *)(&light->ambient), 0.01f, 0.0f, 10.0f, "%.2f");
            ImGui::DragFloat3("diffuse", (float *)(&light->diffuse), 0.01f, 0.0f, 10.0f, "%.2f");
            ImGui::DragFloat3("specular", (float *)(&light->specular), 0.01f, 0.0f, 10.0f, "%.2f");
        }
    }

    ImGui::End();
}

void InspectPanel::dispatch(Event _event, EventParam &_param) {
    switch (_event) {
        case Event::PRIMITIVE_SELECTED: this->onPrimitiveSelected(_param);
    }
}

void InspectPanel::onPrimitiveSelected(EventParam &_param) {
    const auto primitiveID = std::get<int>(_param);
    m_currentPrimitive = Application::instance()->getCurrentScene()->getPrimitiveByID(primitiveID);
}
