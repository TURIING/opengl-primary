/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 18:13
* @version: 1.0
* @description: 
********************************************************************************/

#include "IScene.h"

void IScene::enableDeepTest() {
    glEnable(GL_DEPTH_TEST);
    m_enableDeepTest = true;
}

// 启用模板测试
void IScene::enableStencilTest() {
    glEnable(GL_STENCIL_TEST);
    m_enableStencilTest = true;
}

void IScene::clear() {
    std::apply(glClearColor, m_clearColor);                                                // 设置清空屏幕所用的颜色

    if (m_enableDeepTest) {
        glClear(GL_DEPTH_BUFFER_BIT);                                                      // 清空屏幕的颜色缓冲及深度缓冲
    }

    if(m_enableStencilTest) {
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    glClear(GL_COLOR_BUFFER_BIT);                                                           // 清空屏幕的颜色缓冲
}

void IScene::setWindowSize(std::tuple<int, int> &_windowSize) {
    for(auto render: m_renderList) {
        render->setWindowSize(_windowSize);
    }

    IRenderer::setWindowSize(_windowSize);
}

void IScene::setCamera(std::shared_ptr<Camera> &_camera) {
    for(auto render: m_renderList) {
        render->setCamera(_camera);
    }

    IRenderer::setCamera(_camera);
}
