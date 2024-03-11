/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 10:57
* @version: 1.0
* @description: 所有渲染物体及渲染场景的接口类
********************************************************************************/

#ifndef OPENGL_PRIMARY_IRENDERER_H
#define OPENGL_PRIMARY_IRENDERER_H

#include <tuple>
#include "glad/glad.h"
#include "../BaseDefine.h"
#include "Camera.h"
#include <memory>
#include "glog/logging.h"

class IRenderer {
public:
    virtual void paint() = 0;

    // 摄像机
    virtual void setCamera(std::shared_ptr<Camera> &_camera);
    std::shared_ptr<Camera> getCamera();

    // 窗口尺寸
    virtual void setWindowSize(std::tuple<int, int> &_windowSize);
    int getWindowWidth() { return std::get<0>(m_windowSize); }
    int getWindowHeight() { return std::get<1>(m_windowSize); }

private:
    std::tuple<int, int> m_windowSize = { 1600, 1200 };                                                              // 窗口尺寸

    std::shared_ptr<Camera> m_camera;
};

#endif //OPENGL_PRIMARY_IRENDERER_H
