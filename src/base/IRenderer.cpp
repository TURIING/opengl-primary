/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/26 23:23
* @version: 1.0
* @description: 
********************************************************************************/
#include "IRenderer.h"


std::shared_ptr<Camera> IRenderer::getCamera() {
    LOG_ASSERT(m_camera) << " Camera pointer not initialized.";
    return m_camera;
}

void IRenderer::setCamera(std::shared_ptr<Camera> &_camera) {
    m_camera = _camera;
}

void IRenderer::setWindowSize(std::tuple<int, int> &_windowSize) {
    m_windowSize = _windowSize;
}
