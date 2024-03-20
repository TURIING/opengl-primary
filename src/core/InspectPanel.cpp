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

    if(ImGui::CollapsingHeader("Transform")) {
        if(m_currentPrimitive) {
            const auto name = m_currentPrimitive->getRenderName();

            ImGui::Text(name.c_str());
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
