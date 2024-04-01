/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 18:13
* @version: 1.0
* @description: 
********************************************************************************/

#include "IScene.h"

const std::string VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/cube/vertex.glsl";
const std::string FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/cube/fragment.glsl";

IScene::IScene() {
    this->setDeepTest(true);
    m_camera = std::make_shared<Camera>();
    m_vpMatricesUbo = std::make_unique<Buffer<VPMatricesBlock>>(0);
    m_lightUbo = std::make_unique<Buffer<LightBlock>>(1);
    m_cameraUbo = std::make_unique<Buffer<CameraInfoBlock>>(2);
    m_shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);
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
    int pointLightIndex = 0, directionalLightIndex = 0;
    for(const auto [id, child]: m_primitiveList) {
        if(child->getLightType() == LightType::PointLight) {
            const auto extraOffset = pointLightIndex * sizeof(PointLight);
            const auto light = child->getPointLight();
            m_lightUbo->setData<glm::vec3>(extraOffset + offsetof(PointLight, position), static_cast<const void *>(glm::value_ptr(*child->getPosition())));
            m_lightUbo->setData<float>(extraOffset + offsetof(PointLight, constant), static_cast<const void *>(&light->constant));
            m_lightUbo->setData<glm::vec3>(extraOffset + offsetof(PointLight, ambient), static_cast<const void *>(glm::value_ptr(light->ambient)));
            m_lightUbo->setData<float>(extraOffset + offsetof(PointLight, linear), static_cast<const void *>(&light->linear));
            m_lightUbo->setData<glm::vec3>(extraOffset + offsetof(PointLight, diffuse), static_cast<const void *>(glm::value_ptr(light->diffuse)));
            m_lightUbo->setData<float>(extraOffset + offsetof(PointLight, quadratic), static_cast<const void *>(&light->quadratic));
            m_lightUbo->setData<glm::vec3>(extraOffset + offsetof(PointLight, specular), static_cast<const void *>(glm::value_ptr(light->specular)));
            pointLightIndex++;
        }
    }

    for(const auto [id, child]: m_primitiveList) {
        if(child->getLightType() == LightType::DirectionalLight) {
            const auto extraOffset = directionalLightIndex * sizeof(DirectionalLight) + MAX_LIGHT_NUM * sizeof(PointLight);
            const auto light = child->getDirectionalLight();
            const auto direction = -(*child->getPosition());
            m_lightUbo->setData<glm::vec3>(extraOffset + offsetof(DirectionalLight, direction), static_cast<const void *>(glm::value_ptr(direction)));
            m_lightUbo->setData<glm::vec3>(extraOffset + offsetof(DirectionalLight, ambient), static_cast<const void *>(glm::value_ptr(light->ambient)));
            m_lightUbo->setData<glm::vec3>(extraOffset + offsetof(DirectionalLight, diffuse), static_cast<const void *>(glm::value_ptr(light->diffuse)));
            m_lightUbo->setData<glm::vec3>(extraOffset + offsetof(DirectionalLight, specular), static_cast<const void *>(glm::value_ptr(light->specular)));
            directionalLightIndex++;
        }
    }

    m_lightUbo->setData<int>(MAX_LIGHT_NUM * (sizeof(PointLight) + sizeof(DirectionalLight)), static_cast<const void *>(&pointLightIndex));
    m_lightUbo->setData<int>(MAX_LIGHT_NUM * (sizeof(PointLight) + sizeof(DirectionalLight)) + sizeof(int), static_cast<const void *>(&directionalLightIndex));

    // 传递摄像机信息
    m_cameraUbo->setData<glm::vec3>(offsetof(CameraInfoBlock, cameraPos), static_cast<const void *>(glm::value_ptr(this->getCamera()->getPosition())));

    // 清屏
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
        case Event::WINDOW_RESIZE:              this->onWindowResize(std::get<Size>(_param));           break;
        case Event::MOUSE_MOVE:
        case Event::MOUSE_WHEEL:                m_camera->dispatch(_event, _param);               break;
        case Event::KEY_PRESS:                  this->onKeyPress(std::get<KEYBOARD>(_param));          break;
        case Event::PRIMITIVE_DELETED:          this->deletePrimitive(std::get<int>(_param));           break;
        default:                                LOG(FATAL) << " Undefined conditional branch.";
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
void IScene::addPrimitive(std::shared_ptr<IPrimitive> &_render) {
    m_primitiveList.emplace(_render->getRenderID(), _render);
}

void IScene::render() {
    for(auto [id, primitive]: this->getAllPrimitive()) {
        primitive->preRender();
        primitive->render();
    }
}

void IScene::deletePrimitive(int _id) {
    m_primitiveList.erase(_id);
}
