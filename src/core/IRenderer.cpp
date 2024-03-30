/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/26 23:23
* @version: 1.0
* @description: 
********************************************************************************/
#include "IRenderer.h"

int IRenderer::UUID = 1000;

IRenderer::IRenderer() {
    UUID += 1;
    m_renderID = UUID;
}

void IRenderer::setWindowSize(std::tuple<int, int> &_windowSize) {
    m_windowSize = _windowSize;
}

// 渲染前处理
void IRenderer::preRender() {

}

// 渲染后处理
void IRenderer::postRender() {

}

RenderType IRenderer::getRenderType() {
    LOG_ASSERT(m_renderType != RenderType::None);
    return m_renderType;
}

// 设置渲染类型
void IRenderer::setRenderType(RenderType _type) {
    m_renderType = _type;
}

// 设置渲染物体的名称
void IRenderer::setRenderName(const std::string &_name) {
    LOG_ASSERT(!_name.empty());
    m_renderName = _name;
}

// 启用深度测试
void IRenderer::setDeepTest(bool _on) {
    if(_on) {
        glEnable(GL_DEPTH_TEST);
        m_enableDeepTest = true;
    }
    else {
        glDisable(GL_DEPTH_TEST);
        m_enableDeepTest = false;
    }
}

// 启用模板测试
void IRenderer::setStencilTest(bool _on) {
    if(_on) {
        glEnable(GL_STENCIL_TEST);
        m_enableStencilTest = true;
    }
    else {
        glDisable(GL_STENCIL_TEST);
        m_enableStencilTest = false;
    }
}


