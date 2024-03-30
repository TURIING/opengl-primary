/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 19:28
* @version: 1.0
* @description: Alpha测试
********************************************************************************/

#ifndef OPENGL_PRIMARY_PHONGLIGHT_H
#define OPENGL_PRIMARY_PHONGLIGHT_H

#include "../core/IScene.h"
#include "../primitive/Floor.h"
#include "../primitive/Cube.h"
#include "../primitive/Square.h"
#include "imgui/imgui.h"
#include "../BaseDefine.h"
#include <map>

class PhongLight: public IScene {
public:
    PhongLight();

    void render() override;
private:
    std::unique_ptr<Cube> m_cube;
    //std::unique_ptr<Cube> m_light;
    //std::unique_ptr<Cube> m_light1;
    //std::unique_ptr<Cube> m_light2;

};


#endif //OPENGL_PRIMARY_PHONGLIGHT_H
