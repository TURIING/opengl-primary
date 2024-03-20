/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 19:28
* @version: 1.0
* @description: Alpha测试
********************************************************************************/

#ifndef OPENGL_PRIMARY_BLEND_H
#define OPENGL_PRIMARY_BLEND_H

#include "../core/IScene.h"
#include "../primitive/Floor.h"
#include "../primitive/Cube.h"
#include "../primitive/Square.h"
#include "imgui/imgui.h"
#include "../BaseDefine.h"
#include <map>

class Blend: public IScene{
public:
    Blend();

    void render() override;
private:
    const std::string VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/cube/vertex.glsl";
    const std::string FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/cube/fragment.glsl";

    std::unique_ptr<Floor> m_floor;
    std::unique_ptr<Cube> m_cube1;
    std::unique_ptr<Cube> m_cube2;
    std::unique_ptr<Square> m_window1;
    std::unique_ptr<Square> m_window2;
    //std::unique_ptr<Square> m_window3;

    std::shared_ptr<ShaderProgram> m_shaderProgram;

    std::vector<VPos> m_windowPos = {
        VPos { 1.5f, 0.0f, -0.48f },
        //VPos { 1.5f, 0.0f, 0.51f },
        //VPos { 0.0f, 0.0f, 0.7f },
        //VPos { 0.3f, 0.0f, -2.3f },
        VPos { 0.5f, 0.0f, -0.6f }
    };
};


#endif //OPENGL_PRIMARY_BLEND_H
