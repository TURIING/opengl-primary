/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/25 14:51
* @version: 1.0
* @description: 测试模型加载
********************************************************************************/

#ifndef OPENGL_PRIMARY_MODELLOAD_H
#define OPENGL_PRIMARY_MODELLOAD_H

#include "../../base/IRenderer.h"
#include "../../base/Model.h"
#include "imgui/imgui.h"

class ModelLoad: public IRenderer {
public:
    ModelLoad(std::string _path);

private:
    void paint() override;

private:
    std::shared_ptr<Model> m_model;
    std::shared_ptr<ShaderProgram> m_shaderProgram;

    const std::string VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/model_load/vertex.glsl";
    const std::string FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/model_load/fragment.glsl";
};


#endif //OPENGL_PRIMARY_MODELLOAD_H
