/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 14:41
* @version: 1.0
* @description: 三角形图元类
********************************************************************************/

#ifndef OPENGL_PRIMARY_TRIANGLE_H
#define OPENGL_PRIMARY_TRIANGLE_H

#include "IPrimitive.h"
#include "../base/VertexArray.h"
#include "../base/Buffer.h"
#include "../base/ShaderProgram.h"
#include <memory>
#include "../BaseDefine.h"
#include "imgui/imgui.h"


class Triangle: public IPrimitive {
public:
    Triangle();
    void paint() override;

private:
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<Buffer<float>> m_vbo;
    std::shared_ptr<ShaderProgram> m_shaderProgram;

    std::vector<float> m_vertices = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    const std::string VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/triangle/vertex.glsl";
    const std::string FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/triangle/fragment.glsl";
};


#endif //OPENGL_PRIMARY_TRIANGLE_H
