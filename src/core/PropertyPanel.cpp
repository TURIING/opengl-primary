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
#include "../primitive/Model.h"

void PropertyPanel::render() {
    const auto currentScene = Application::instance()->getCurrentScene();
    const auto& primitiveList = currentScene->getAllPrimitive();

    ImGui::Begin("Properties");

    if(ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SeparatorText("All primitives");

        static int primitiveSelected = primitiveList.begin()->second->getRenderID();
        for(const auto& [id, render]: primitiveList) {
            // 展示模型图元
            if(render->getPrimitiveType() == PrimitiveType::Model) {
                const auto model = std::dynamic_pointer_cast<Model>(render);
                if(ImGui::TreeNode(render->getRenderName().c_str())) {
                    for(const auto& mesh: model->getMeshes()) {
                        static auto meshSelected = mesh;
                        const auto& meshName = mesh->getMeshName();
                        if(ImGui::Selectable(meshName.empty() ? "UnName" : meshName.c_str(), meshSelected == mesh)) {
                            primitiveSelected = id;
                            Application::instance()->dispatch(Event::PRIMITIVE_SELECTED, id);

                            meshSelected = mesh;
                            Application::instance()->dispatch(Event::MESH_SELECTED, mesh);
                        }
                    }
                    ImGui::TreePop();
                }
                continue;
            }

            // 展示其他类型的图元
            if(ImGui::Selectable(render->getRenderName().c_str(), primitiveSelected == id)) {
                primitiveSelected = id;
                Application::instance()->dispatch(Event::PRIMITIVE_SELECTED, id);
            }

            // 图元列表右键菜单
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

        ImGui::SeparatorText("Shadow");
        static bool isParallelShadow = false;
        bool tmpIsShadow = false;
        tmpIsShadow = isParallelShadow;
        ImGui::Checkbox("enable parallel shadow", &isParallelShadow);
        if(tmpIsShadow != isParallelShadow) currentScene->setParallelShadow(isParallelShadow);

        static bool isPointShadow = false;
        tmpIsShadow = isPointShadow;
        ImGui::Checkbox("enable point shadow", &isPointShadow);
        if(tmpIsShadow != isPointShadow) currentScene->setPointShadow(isPointShadow);
    }

    ImGui::End();
}

void PropertyPanel::dispatch(Event _event, EventParam _param) {

}