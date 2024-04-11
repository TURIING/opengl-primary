/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/30 16:13
* @version: 1.0
* @description: 
********************************************************************************/

#include "Common.h"
#include "../Utility.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"

// 绘制添加图元的模态窗口
void Common::paintAddPrimitiveModal() {
    if(ImGui::BeginPopupModal("Add primitive")) {
        // 图元类型
        static PrimitiveType primitiveTypeSelected = PrimitiveType::Cube;
        if(ImGui::BeginCombo("Primitvie Type", Utility::transformPrimitiveTypeToStr(primitiveTypeSelected))) {
            for(auto i = 1; i < static_cast<int>(PrimitiveType::End); i++) {
                if(const auto type = static_cast<PrimitiveType>(i); ImGui::Selectable(Utility::transformPrimitiveTypeToStr(type), primitiveTypeSelected == type)) {
                    primitiveTypeSelected = type;
                }
            }
            ImGui::EndCombo();
        }

        // 图元名称
        static char primitiveName[128] = "";
        ImGui::InputTextWithHint("Primitive Name", "Please enter entity name.", primitiveName, IM_ARRAYSIZE(primitiveName));

        // 模型路径
        static char modelPath[128] = "";
        if(primitiveTypeSelected == PrimitiveType::Model) {
            const std::string key = "model_load";

            ImGui::InputText("model_path", modelPath, IM_ARRAYSIZE(modelPath));

            ImGui::SameLine();

            if(ImGui::Button("Open file")) {
                IGFD::FileDialogConfig config;
                config.path = MODEL_PATH;
                ImGuiFileDialog::Instance()->OpenDialog(key, "Choose File", ".obj,.fbx", config);
            }
            if (ImGuiFileDialog::Instance()->Display(key)) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    strcpy(modelPath, filePathName.c_str());
                }
                ImGuiFileDialog::Instance()->Close();
            }
        }

        // 关闭按钮
        if(ImGui::Button("Close")) {
            ImGui::CloseCurrentPopup();
            primitiveTypeSelected = PrimitiveType::Cube;
            memset(primitiveName, 0, sizeof(primitiveName));
        }

        // 创建按钮
        ImGui::SameLine();
        if(ImGui::Button("Ok")) {
            Application::instance()->dispatch(Event::PRIMITIVE_CREATED, PrimitiveInfo{ primitiveTypeSelected, std::string(primitiveName), std::string(modelPath)});
            primitiveTypeSelected = PrimitiveType::Cube;
            memset(primitiveName, 0, sizeof(primitiveName));
            memset(modelPath, 0, sizeof(modelPath));
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}