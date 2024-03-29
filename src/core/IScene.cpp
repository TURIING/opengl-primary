/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 18:13
* @version: 1.0
* @description: 
********************************************************************************/

#include "IScene.h"

IScene::IScene() {
    m_camera = std::make_shared<Camera>();
    m_vpMatricesUbo = std::make_unique<Buffer<VPMatricesBlock>>(0);
    m_pointLightUbo = std::make_unique<Buffer<PointLight>>(1);
}

void IScene::clear() {
    std::apply(glClearColor, m_clearColor);                                                // 设置清空屏幕所用的颜色

    if (this->getDeepTest()) {
        glClear(GL_DEPTH_BUFFER_BIT);                                                      // 清空屏幕的颜色缓冲及深度缓冲
    }

    if(this->getStencilTest()) {
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    glClear(GL_COLOR_BUFFER_BIT);                                                          // 清空屏幕的颜色缓冲
}

void IScene::setWindowSize(std::tuple<int, int> &_windowSize) {
    for(auto [id, render]: m_primitiveList) {
        render->setWindowSize(_windowSize);
    }

    IRenderer::setWindowSize(_windowSize);
}

std::shared_ptr<Camera>& IScene::getCamera() {
    LOG_ASSERT(m_camera) << " Camera pointer not initialized.";
    return m_camera;
}

// 离屏渲染，重新调整framebuffer
void IScene::offScreenRender(Size &_size) {
    if(!m_fbo) {
        m_fbo.reset();
        m_screenTexture.reset();
        m_rbo.reset();
    }

    m_fbo = std::make_unique<FrameBuffer>();
    m_screenTexture = std::make_shared<Texture>(_size, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, Texture::IMAGE2D);
    m_fbo->attachTexture2D(FRAMEBUFFER_ATTACH_TYPE::COLOR, m_screenTexture);

    m_rbo = std::make_shared<RenderBuffer>();
    m_rbo->storage(RENDER_BUFFER_FORMAT::DEPTH24_STENCIL8, _size);
    m_fbo->attachRenderBuffer(FRAMEBUFFER_ATTACH_TYPE::DEPTH_STENCIL, m_rbo);

    m_fbo->check();
    m_fbo->unbind();
}

void IScene::preRender() {
    auto viewPortSize = ImGui::GetContentRegionAvail();
    this->getCamera()->setWindowSize({ viewPortSize.x, viewPortSize.y });

    m_fbo->bind();

    this->setDeepTest(true);

    // 传递view、projection矩阵
    m_vpMatricesUbo->setData<glm::mat4>(offsetof(VPMatricesBlock, view), static_cast<const void *>(glm::value_ptr(this->getCamera()->getViewMatrix())));
    m_vpMatricesUbo->setData<glm::mat4>(offsetof(VPMatricesBlock, projection), static_cast<const void *>(glm::value_ptr(this->getCamera()->getProjection())));

    // 传递光源信息
    for(const auto [id, child]: m_primitiveList) {
        if(child->getPrimitiveType() == PrimitiveType::PhongLight) {
            const auto light = child->getPointLight();
            m_pointLightUbo->setData<glm::vec3>(offsetof(PointLight, position), static_cast<const void *>(glm::value_ptr(*child->getPosition())));
            m_pointLightUbo->setData<glm::vec3>(offsetof(PointLight, ambient), static_cast<const void *>(glm::value_ptr(light->ambient)));
            m_pointLightUbo->setData<glm::vec3>(offsetof(PointLight, diffuse), static_cast<const void *>(glm::value_ptr(light->diffuse)));
            m_pointLightUbo->setData<glm::vec3>(offsetof(PointLight, specular), static_cast<const void *>(glm::value_ptr(light->specular)));
        }
    }

    this->clear();
}

void IScene::postRender() {
    m_fbo->unbind();
    this->clear();

    ImGui::Begin("Scene");
    const auto viewPortSize = ImGui::GetContentRegionAvail();
    ImGui::Image(reinterpret_cast<void*>(m_screenTexture->getID()), viewPortSize, ImVec2 { 0, 1 }, ImVec2 { 1, 0 });
    ImGui::End();
}

// 分发事件
void IScene::dispatch(Event _event, EventParam _param) {
    switch (_event) {
        case Event::WINDOW_RESIZE:  this->onWindowResize(std::get<Size>(_param));           break;
        case Event::MOUSE_MOVE:
        case Event::MOUSE_WHEEL:    m_camera->dispatch(_event, _param);               break;
        case Event::KEY_PRESS:      this->onKeyPress(std::get<KEYBOARD>(_param));          break;
        default:                    LOG(FATAL) << "Parameter error.";
    }
}

// 窗口尺寸改变时触发的事件
void IScene::onWindowResize(Size &_size) {
    const auto [width, height] = _size;
    // 告诉OpenGL渲染窗口的尺寸大小
    glViewport(0, 0, width, height);
    this->offScreenRender(_size);
}

void IScene::onKeyPress(KEYBOARD _key) {

}

// 将图元挂载在场景上
void IScene::addPrimitive(IPrimitive *_render) {
    m_primitiveList.emplace(_render->getRenderID(), _render);
}






