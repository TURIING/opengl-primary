/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/20 22:08
* @version: 1.0
* @description: 
********************************************************************************/

#include "InspectPanel.h"
#include <tuple>
#include "ImGuiFileDialog/ImGuiFileDialog.h"
#include "imgui/imgui.h"
#include "Application.h"
#include "IPrimitive.h"
#include "IScene.h"
#include "../material/PhongMaterial.h"
#include "../Utility.h"

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
        auto materialTypeSelected = m_currentPrimitive->getMaterial()->getMaterialType();

        if(ImGui::BeginCombo("Material Type", Utility::transformMaterialTypeToStr(materialTypeSelected))) {
            for(auto i = 1; i < static_cast<int>(MaterialType::End); i++) {
                if(const auto type = static_cast<MaterialType>(i); ImGui::Selectable(Utility::transformMaterialTypeToStr(type), materialTypeSelected == type)) {
                    materialTypeSelected = type;
                    m_currentPrimitive->setMaterial(this->makeMaterialByType(type));
                }
            }
            ImGui::EndCombo();
        }

        if(materialTypeSelected != MaterialType::None && materialTypeSelected != MaterialType::End) {
            this->buildMaterialItem(m_currentPrimitive->getMaterial());
        }
    }

    // 光源
    if(ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto lightTypeSelected = m_currentPrimitive->getLightType();

        if(ImGui::BeginCombo("Light Type", Utility::transformLightTypeToStr(lightTypeSelected))) {
            for(auto i = 0; i < static_cast<int>(LightType::End); i++) {
                if(const auto type = static_cast<LightType>(i); ImGui::Selectable(Utility::transformLightTypeToStr(type), lightTypeSelected == type)) {
                    lightTypeSelected = type;
                    m_currentPrimitive->setLightType(type);
                }
            }
            ImGui::EndCombo();
        }

        // 点光源
        if(m_currentPrimitive->getLightType() == LightType::PointLight) {
            ImGui::SeparatorText("Point Light Info");

            auto light = m_currentPrimitive->getPointLight();
            ImGui::DragFloat("constant", (float *)(&light->constant), 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat("linear", (float *)(&light->linear), 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat("quadratic", (float *)(&light->quadratic), 0.001f, 0.0f, 0.1f, "%.3f");
            ImGui::DragFloat3("ambient", (float *)(&light->ambient), 0.01f, 0.0f, 10.0f, "%.2f");
            ImGui::DragFloat3("diffuse", (float *)(&light->diffuse), 0.01f, 0.0f, 10.0f, "%.2f");
            ImGui::DragFloat3("specular", (float *)(&light->specular), 0.01f, 0.0f, 10.0f, "%.2f");
        }

        // 平行光
        if(m_currentPrimitive->getLightType() == LightType::DirectionalLight) {
            ImGui::SeparatorText("Directional Light Info");

            auto light = m_currentPrimitive->getDirectionalLight();
            ImGui::DragFloat3("ambient", (float *)(&light->ambient), 0.01f, 0.0f, 10.0f, "%.2f");
            ImGui::DragFloat3("diffuse", (float *)(&light->diffuse), 0.01f, 0.0f, 10.0f, "%.2f");
            ImGui::DragFloat3("specular", (float *)(&light->specular), 0.01f, 0.0f, 10.0f, "%.2f");
        }
    }

    ImGui::End();
}

void InspectPanel::dispatch(Event _event, EventParam _param) {
    switch (_event) {
        case Event::PRIMITIVE_SELECTED:         this->onPrimitiveSelected(_param);      break;
        case Event::SCENE_CREATED:              m_currentPrimitive = nullptr;              break;
        case Event::PRIMITIVE_DELETED:          m_currentPrimitive = nullptr;              break;
    }
}

void InspectPanel::onPrimitiveSelected(EventParam &_param) {
    const auto primitiveID = std::get<int>(_param);
    m_currentPrimitive = Application::instance()->getCurrentScene()->getPrimitiveByID(primitiveID);
}

void InspectPanel::buildItemForPhongMaterial(std::shared_ptr<IMaterial> &_material) {
    const auto material = std::dynamic_pointer_cast<PhongMaterial>(_material);

    const auto borderCol = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    const auto tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    const auto diffuseTitle = "Diffuse";
    const auto diffuseTexture = material->getDiffuse();
    const auto diffuseKey = diffuseTitle + std::to_string(diffuseTexture->getID());
    ImGui::PushID(diffuseTitle);
    ImGui::Separator();
    ImGui::Text(diffuseTitle);
    ImGui::Image((void*)diffuseTexture->getID(), ImVec2{ 100, 100 }, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintCol, borderCol);
    ImGui::SameLine();
    if(ImGui::Button("Open file")) {
        IGFD::FileDialogConfig config;
        config.path = TEXTURE_PATH;
        ImGuiFileDialog::Instance()->OpenDialog(diffuseKey, "Choose File", ".png,.jpg", config);
    }
    if (ImGuiFileDialog::Instance()->Display(diffuseKey)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            material->resetDiffuse(filePathName);
        }
        ImGuiFileDialog::Instance()->Close();
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset")) {
        material->resetDiffuse(TEXTURE_DEFAULT_FILE);
    }
    ImGui::PopID();

    const auto specularTitle = "Specular";
    const auto specularTexture = material->getSpecular();
    const auto specularKey = specularTitle + std::to_string(specularTexture->getID());
    ImGui::PushID(specularTitle);
    ImGui::Separator();
    ImGui::Text(specularTitle);
    ImGui::Image((void*)specularTexture->getID(), ImVec2{ 100, 100 }, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintCol, borderCol);
    ImGui::SameLine();
    if(ImGui::Button("Open file")) {
        IGFD::FileDialogConfig config;
        config.path = TEXTURE_PATH;
        ImGuiFileDialog::Instance()->OpenDialog(specularKey, "Choose File", ".png,.jpg", config);
    }
    if (ImGuiFileDialog::Instance()->Display(specularKey)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            material->resetSpecular(filePathName);
        }
        ImGuiFileDialog::Instance()->Close();
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset")) {
        material->resetSpecular(TEXTURE_DEFAULT_FILE);
    }
    ImGui::PopID();

    ImGui::DragFloat("shininess", material->getShininess(), 1, 2.0f, 256.0f, "%.2f");
}

// 根据类型生成对应的材质类
std::shared_ptr<IMaterial> InspectPanel::makeMaterialByType(MaterialType _type) {
    LOG_ASSERT(_type != MaterialType::None && _type != MaterialType::End);
    auto&& shaderProgram = m_currentPrimitive->getShaderProgram();
    switch (_type) {
        case MaterialType::Phong:           return std::make_shared<PhongMaterial>(shaderProgram, TEXTURE_DEFAULT_FILE, TEXTURE_DEFAULT_FILE);
        default:                            LOG(FATAL) << " Undefined conditional branch.";
    }
}

void InspectPanel::buildMaterialItem(std::shared_ptr<IMaterial> _material) {
    switch (_material->getMaterialType()) {
        case MaterialType::Phong:                  this->buildItemForPhongMaterial(_material);          break;
        //case MaterialType::ColorMaterial:                  return "ColorMaterial";
        //case MaterialType::TextureMaterial:                return "TextureMaterial";
        //case MaterialType::SkyBox:                 this->buildItemForSkyboxMaterial(_material);          break;
        //default:                                           LOG(FATAL) << " Undefined conditional branch.";
    }
}

void InspectPanel::buildItemForSkyboxMaterial(std::shared_ptr<IMaterial> &_material) {
    //const auto material = std::dynamic_pointer_cast<PhongMaterial>(_material);
    //const auto borderCol = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    //const auto tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    //
    //ImGui::SeparatorText("Skybox Material");
    //
    //ImGui::Text("px");
    //ImGui::SameLine();
    //ImGui::Text("nx");
    //ImGui::Image((void*)diffuseTexture->getID(), ImVec2{ 100, 100 }, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintCol, borderCol);

}
