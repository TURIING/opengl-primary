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

PropertyPanel::PropertyPanel() {

}

void PropertyPanel::render() {
    const auto currentScene = Application::instance()->getCurrentScene();
    const auto& primitiveList = currentScene->getAllPrimitive();

    ImGui::Begin("Properties");

    if(ImGui::CollapsingHeader("Scene")) {

        //const auto renderNameList = this->getRenderNameList(primitiveList);
        static std::string currentItemStr = primitiveList.begin()->second->getRenderName();
        if(ImGui::BeginCombo("Primitive", currentItemStr.c_str())) {
            for(const auto [id, render]: primitiveList) {
                if(const auto value = render->getRenderName(); ImGui::Selectable(render->getRenderName().c_str(), currentItemStr == value)) {
                    currentItemStr = value;
                    Application::instance()->dispatch(Event::PRIMITIVE_SELECTED, id);
                }
            }
            ImGui::EndCombo();
        }
    }

    ImGui::End();
}

void PropertyPanel::dispatch(Event _event, EventParam &_param) {

}
