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

    void enableDeepTest() {
        glEnable(GL_DEPTH_TEST);
        m_enableDeepTest = true;
    }

    void clear() {
        std::apply(glClearColor, m_clearColor);                                                // 设置清空屏幕所用的颜色

        if (m_enableDeepTest) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                                // 清空屏幕的颜色缓冲及深度缓冲
        }
        else {
            glClear(GL_COLOR_BUFFER_BIT);                                                      // 清空屏幕的颜色缓冲
        }
    }

    void setClearColor(float _r, float _g, float _b, float _a) {
        m_clearColor = {_r, _g, _b, _a};
    }

    void setCamera(std::shared_ptr<Camera> _camera) { m_camera = _camera; }
    std::shared_ptr<Camera> getCamera() {
        LOG_ASSERT(m_camera) << " Camera pointer not initialized.";
        return m_camera;
    }

    void setWindowSize(std::tuple<int, int> _windowSize) { m_windowSize = _windowSize; }
    int getWindowWidth() { return std::get<0>(m_windowSize); }
    int getWindowHeight() { return std::get<1>(m_windowSize); }
private:
    bool m_enableDeepTest = false;												                    // 是否启用深度测试
    TColor m_clearColor = { 0.26f, 0.30f, 0.31f, 1.0f };					                            // 清屏颜色

    std::tuple<int, int> m_windowSize;                                                              // 窗口尺寸

    std::shared_ptr<Camera> m_camera;
};

#endif //OPENGL_PRIMARY_IRENDERER_H
