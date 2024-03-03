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

    // 启用深度测试
    void enableDeepTest() {
        glEnable(GL_DEPTH_TEST);
        m_enableDeepTest = true;
    }

    void enableStencilTest();

    // 变换
    void translate(float _x, float _y, float _z);
    glm::vec3 getTranslatePos() { return m_translatePos; }

    // 清屏颜色
    void clear();
    void setClearColor(float _r, float _g, float _b, float _a) {
        m_clearColor = {_r, _g, _b, _a};
    }

    // 轮廓
    void setOutline(int _width, Color _color);
    bool isEnabledOutline() { return m_enableOutline; }
    Color getOutlineColor() { return m_outlineColor; }
    int getOutlineWidth() { return m_outlineWidth; }

    // 摄像机
    void setCamera(std::shared_ptr<Camera> &_camera);
    std::shared_ptr<Camera> getCamera() {
        LOG_ASSERT(m_camera) << " Camera pointer not initialized.";
        return m_camera;
    }

    // 窗口尺寸
    void setWindowSize(std::tuple<int, int> &_windowSize);
    int getWindowWidth() { return std::get<0>(m_windowSize); }
    int getWindowHeight() { return std::get<1>(m_windowSize); }

    void addRender(IRenderer *_render) { m_renderList.push_back(_render); }

private:
    bool m_enableDeepTest = false;												                    // 是否启用深度测试
    bool m_enableStencilTest = false;												                // 是否启用模板测试

    bool m_enableOutline = false;                                                                   // 是否绘制轮廓
    Color m_outlineColor = { 0, 0, 0, 1};                                                           // 轮廓颜色
    int m_outlineWidth = 0;                                                                         // 轮廓宽度

    Color m_clearColor = { 0.26f, 0.30f, 0.31f, 1.0f };					                            // 清屏颜色

    glm::vec3 m_translatePos = { 1.0f, 1.0f, 1.0f };                                        // 平移后的位置

    std::tuple<int, int> m_windowSize;                                                              // 窗口尺寸

    std::shared_ptr<Camera> m_camera;

    std::vector<IRenderer *> m_renderList;                                                          // 场景中的物体集合
};

#endif //OPENGL_PRIMARY_IRENDERER_H
