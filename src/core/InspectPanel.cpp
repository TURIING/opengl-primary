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
#include "../material/ReflectMaterial.h"
#include "../material/SkyboxMaterial.h"
#include "../material/CommonMaterial.h"
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
        ImGui::DragFloat("Scale", m_currentPrimitive->getScaling(), 0.01f, 0.0f, 10.0f, "%.2f");
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

        if(materialTypeSelected == MaterialType::Phong && m_currentPrimitive->getPrimitiveType() == PrimitiveType::Model && m_currentMesh) {
            this->buildMaterialItem(m_currentMesh->getMaterial());
        }
        else if(materialTypeSelected != MaterialType::None && materialTypeSelected != MaterialType::End) {
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
        case Event::MESH_SELECTED:              this->onMeshSelected(_param);           break;
        case Event::SCENE_CREATED:              m_currentPrimitive = nullptr;              break;
        case Event::PRIMITIVE_DELETED:          m_currentPrimitive = nullptr;              break;
    }
}

void InspectPanel::onPrimitiveSelected(EventParam &_param) {
    const auto primitiveID = std::get<int>(_param);
    m_currentPrimitive = Application::instance()->getCurrentScene()->getPrimitiveByID(primitiveID);
    m_currentMesh = nullptr;
}

void InspectPanel::buildItemForPhongMaterial(std::shared_ptr<IMaterial> &_material) {
    const auto material = std::dynamic_pointer_cast<PhongMaterial>(_material);

    const auto borderCol = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    const auto tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    const auto diffuseTitle = "Diffuse";
    const auto diffuseTexture = material->getDiffuse();
    const auto diffuseKey = diffuseTitle + std::to_string(diffuseTexture->getTextureID());
    ImGui::PushID(diffuseTitle);
    ImGui::Separator();
    ImGui::Text(diffuseTitle);
    ImGui::Image((void*)diffuseTexture->getTextureID(), ImVec2{ 100, 100 }, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintCol, borderCol);
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
    const auto specularKey = specularTitle + std::to_string(specularTexture->getTextureID());
    ImGui::PushID(specularTitle);
    ImGui::Separator();
    ImGui::Text(specularTitle);
    ImGui::Image((void*)specularTexture->getTextureID(), ImVec2{ 100, 100 }, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tintCol, borderCol);
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
        case MaterialType::Reflect: {
            const auto material = std::dynamic_pointer_cast<SkyboxMaterial>(Application::instance()->getCurrentScene()->getSkyboxPrimitive()->getMaterial());
            LOG_IF(ERROR, !material);
            return std::make_shared<ReflectMaterial>(material->getTexture());
        }
        default:                            LOG(FATAL) << " Undefined conditional branch.";
    }
}

void InspectPanel::buildMaterialItem(std::shared_ptr<IMaterial> _material) {
    switch (_material->getMaterialType()) {
        case MaterialType::Phong:                  this->buildItemForPhongMaterial(_material);          break;
        case MaterialType::Common:                 this->buildItemForCommonMaterial(_material);         break;
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

void InspectPanel::onMeshSelected(EventParam &_param) {
    m_currentMesh = std::move(std::get<std::shared_ptr<Mesh>>(_param));
}

void InspectPanel::buildItemForCommonMaterial(std::shared_ptr<IMaterial> &_material) {
    const auto material = std::dynamic_pointer_cast<CommonMaterial>(_material);

    const auto borderCol = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    const auto tintCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Diffuse
    {
        const auto diffuseTitle = "Diffuse";
        const auto diffuseTexture = material->getDiffuseMap();
        const auto diffuseKey = diffuseTitle + std::to_string(diffuseTexture->getTextureID());
        ImGui::PushID(diffuseTitle);
        ImGui::Separator();
        ImGui::Text(diffuseTitle);
        ImGui::Image((void *) diffuseTexture->getTextureID(), ImVec2{100, 100}, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
                     tintCol, borderCol);
        ImGui::SameLine();
        if (ImGui::Button("Open file")) {
            IGFD::FileDialogConfig config;
            config.path = TEXTURE_PATH;
            ImGuiFileDialog::Instance()->OpenDialog(diffuseKey, "Choose File", ".png,.jpg", config);
        }
        if (ImGuiFileDialog::Instance()->Display(diffuseKey)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                material->resetDiffuseMap(filePathName);
            }
            ImGuiFileDialog::Instance()->Close();
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            material->resetDiffuseMap(TEXTURE_DEFAULT_FILE);
        }
        ImGui::PopID();
    }

    // Specular
    {
        const auto specularTitle = "Specular";
        const auto specularTexture = material->getSpecularMap();
        if(specularTexture) {
            const auto specularKey = specularTitle + std::to_string(specularTexture->getTextureID());
            ImGui::PushID(specularTitle);
            ImGui::Separator();
            ImGui::Text(specularTitle);
            ImGui::Image((void *) specularTexture->getTextureID(), ImVec2{100, 100}, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
                         tintCol, borderCol);
            ImGui::SameLine();
            if (ImGui::Button("Open file")) {
                IGFD::FileDialogConfig config;
                config.path = TEXTURE_PATH;
                ImGuiFileDialog::Instance()->OpenDialog(specularKey, "Choose File", ".png,.jpg", config);
            }
            if (ImGuiFileDialog::Instance()->Display(specularKey)) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    material->resetSpecularMap(filePathName);
                }
                ImGuiFileDialog::Instance()->Close();
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset")) {
                material->resetSpecularMap(TEXTURE_DEFAULT_FILE);
            }
            ImGui::PopID();
        }
    }
    // Normal
    {
        const auto normalTitle = "Normal";
        const auto normalTexture = material->getNormalMap();
        if(normalTexture) {
            const auto normalKey = normalTitle + std::to_string(normalTexture->getTextureID());
            ImGui::PushID(normalTitle);
            ImGui::Separator();
            ImGui::Text(normalTitle);
            ImGui::Image((void *) normalTexture->getTextureID(), ImVec2{100, 100}, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
                         tintCol, borderCol);
            ImGui::SameLine();
            if (ImGui::Button("Open file")) {
                IGFD::FileDialogConfig config;
                config.path = TEXTURE_PATH;
                ImGuiFileDialog::Instance()->OpenDialog(normalKey, "Choose File", ".png,.jpg", config);
            }
            if (ImGuiFileDialog::Instance()->Display(normalKey)) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    material->resetNormalMap(filePathName);
                }
                ImGuiFileDialog::Instance()->Close();
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset")) {
                material->resetNormalMap(TEXTURE_DEFAULT_FILE);
            }
            ImGui::PopID();
        }
    }

    // Height
    {
        const auto heightTitle = "Height";
        const auto heightTexture = material->getHeightMap();
        if(heightTexture) {
            const auto heightKey = heightTitle + std::to_string(heightTexture->getTextureID());
            ImGui::PushID(heightTitle);
            ImGui::Separator();
            ImGui::Text(heightTitle);
            ImGui::Image((void *) heightTexture->getTextureID(), ImVec2{100, 100}, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
                         tintCol, borderCol);
            ImGui::SameLine();
            if (ImGui::Button("Open file")) {
                IGFD::FileDialogConfig config;
                config.path = TEXTURE_PATH;
                ImGuiFileDialog::Instance()->OpenDialog(heightKey, "Choose File", ".png,.jpg", config);
            }
            if (ImGuiFileDialog::Instance()->Display(heightKey)) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    material->resetHeightMap(filePathName);
                }
                ImGuiFileDialog::Instance()->Close();
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset")) {
                material->resetHeightMap(TEXTURE_DEFAULT_FILE);
            }
            ImGui::PopID();
        }
    }
}
