/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 19:28
* @version: 1.0
* @description: 天空盒演示
********************************************************************************/

#ifndef OPENGL_PRIMARY_SkyboxDemo_H
#define OPENGL_PRIMARY_SkyboxDemo_H

#include <map>
#include "../core/IScene.h"
#include "../primitive/Floor.h"
#include "../primitive/Cube.h"
#include "../primitive/Square.h"
#include "imgui/imgui.h"
#include "../BaseDefine.h"
#include "../primitive/Skybox.h"

class SkyboxDemo: public IScene{
public:
    SkyboxDemo();

    void render() override;
private:
    //std::unique_ptr<Cube> m_cube1;
    //std::unique_ptr<Cube> m_cube2;
    std::unique_ptr<Skybox> m_skybox;
    std::shared_ptr<ShaderProgram> m_shaderProgram;
};


#endif //OPENGL_PRIMARY_SkyboxDemo_H
