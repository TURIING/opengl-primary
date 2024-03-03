/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/25 23:15
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_STENCILTEST_H
#define OPENGL_PRIMARY_STENCILTEST_H

#include "../../base/IRenderer.h"
#include "../../primitive/Floor.h"
#include "../../primitive/Cube.h"
#include "imgui/imgui.h"

#include <memory>

class StencilTest: public IRenderer{
public:
    StencilTest();

protected:

public:
    void paint() override;

private:
    std::shared_ptr<Floor> m_floor;
    std::shared_ptr<Cube> m_cube1;
    std::shared_ptr<Cube> m_cube2;
};


#endif //OPENGL_PRIMARY_STENCILTEST_H
