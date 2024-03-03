/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/26 23:23
* @version: 1.0
* @description: 
********************************************************************************/
#include "IRenderer.h"

// 启用模板测试
void IRenderer::enableStencilTest() {
    glEnable(GL_STENCIL_TEST);
    m_enableStencilTest = true;
}

void IRenderer::clear() {
    std::apply(glClearColor, m_clearColor);                                                // 设置清空屏幕所用的颜色

    if (m_enableDeepTest) {
        glClear(GL_DEPTH_BUFFER_BIT);                                                      // 清空屏幕的颜色缓冲及深度缓冲
    }

    if(m_enableStencilTest) {
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    glClear(GL_COLOR_BUFFER_BIT);                                                           // 清空屏幕的颜色缓冲
}

/**
 * 开启轮廓绘制并设定轮廓参数
 * @param _width
 * @param _color
 */
void IRenderer::setOutline(int _width, Color _color) {
    m_enableOutline = true;
    m_outlineColor = _color;
    m_outlineWidth = _width;
}

// 平移
void IRenderer::translate(float _x, float _y, float _z) {
    m_translatePos = { _x, _y, _z };
}

void IRenderer::setWindowSize(std::tuple<int, int> &_windowSize) {
    m_windowSize = _windowSize;

    for(auto render: m_renderList) {
        render->setWindowSize(_windowSize);
    }
}

void IRenderer::setCamera(std::shared_ptr<Camera> &_camera) {
    m_camera = _camera;

    for(auto render: m_renderList) {
        render->setCamera(_camera);
    }
}
