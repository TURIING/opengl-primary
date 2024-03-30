/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/12 16:05
* @version: 1.0
* @description: 
********************************************************************************/

#include "PropertyPanel.h"
#include "Application.h"
#include "IScene.h"
#include "Common.h"
#include "../Utility.h"

PropertyPanel::PropertyPanel() {

}

void PropertyPanel::render() {
    const auto currentScene = Application::instance()->getCurrentScene();
    const auto& primitiveList = currentScene->getAllPrimitive();

    ImGui::Begin("Properties");

    if(ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SeparatorText("All primitives");

        static int primitiveSelected = primitiveList.begin()->second->getRenderID();
        for(const auto [id, render]: primitiveList) {
            if(ImGui::Selectable(render->getRenderName().c_str(), primitiveSelected == id)) {
                primitiveSelected = id;
                Application::instance()->dispatch(Event::PRIMITIVE_SELECTED, id);
            }

            if(ImGui::BeginPopupContextItem()) {
                primitiveSelected = id;
                Application::instance()->dispatch(Event::PRIMITIVE_SELECTED, id);

                if(ImGui::Button("Add")) {
                    ImGui::OpenPopup("Add primitive");
                }

                if(ImGui::Button("Delete")) {
                    Application::instance()->dispatch(Event::PRIMITIVE_DELETED, id);
                }

                Common::paintAddPrimitiveModal();
                ImGui::EndPopup();
            }
        }
    }

    ImGui::End();
}

void PropertyPanel::dispatch(Event _event, EventParam _param) {

}