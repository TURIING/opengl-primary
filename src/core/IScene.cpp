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

const std::string SHADOW_VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/shadow/vertex.glsl";
const std::string SHADOW_FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/shadow/fragment.glsl";

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
    m_screenTexture = std::make_shared<Texture>(_size, 2, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, Texture::RGB);
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

    this->setDeepTest(true);

    // 传递view、projection矩阵
    m_vpMatricesUbo->setData<glm::mat4>(offsetof(VPMatricesBlock, view), static_cast<const void *>(glm::value_ptr(this->getCamera()->getViewMatrix())));
    m_vpMatricesUbo->setData<glm::mat4>(offsetof(VPMatricesBlock, projection), static_cast<const void *>(glm::value_ptr(this->getCamera()->getProjection())));

    // 传递光源信息
    int pointLightIndex = 0, directionalLightIndex = 0;
    for(const auto& [id, child]: m_primitiveList) {
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

    for(const auto& [id, child]: m_primitiveList) {
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

    // 处理阴影
    glm::mat4 shadowLightSpaceMatrix;
    if(m_isShadow) {
        for(const auto& [id, child]: m_primitiveList) {
            if(child->getLightType() != LightType::DirectionalLight) continue;

            constexpr float nearPlane = 1.0;
            constexpr float farPlane = 7.5;
            const auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
            const auto lightView = glm::lookAt(*child->getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
            shadowLightSpaceMatrix = lightProjection * lightView;

            m_shadowShaderProgram->use();
            m_shadowShaderProgram->setMat4("lightSpaceMatrix", glm::value_ptr(shadowLightSpaceMatrix));
        }

        //glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        m_shadowFbo->bind();
        this->clear();
        m_shadowFboColorTexture->activate();
        for(const auto& [id, child]: m_primitiveList) {
            child->setShaderProgram(m_shadowShaderProgram);
        }
        this->render();
        m_shadowFbo->unbind();

        //glViewport(0, 0, m_windowSize.width, m_windowSize.height);
        for(const auto& [id, child]: m_primitiveList) {
            child->setShaderProgram(m_shaderProgram);
        }
        m_shaderProgram->use();
        m_shaderProgram->setMat4("lightSpaceMatrix", glm::value_ptr(shadowLightSpaceMatrix));
        m_shaderProgram->setInt("shadowMap", m_shadowFboColorTexture->getTextureUnit());
    }

    m_fbo->bind();

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
    m_windowSize = _size;
    // 告诉OpenGL渲染窗口的尺寸大小
    glViewport(0, 0, static_cast<int>(this->getWindowWidth()), static_cast<int>(this->getWindowHeight()));
    this->offScreenRender(_size);
}

void IScene::onKeyPress(KEYBOARD _key) {

}

// 将图元挂载在场景上
void IScene::addPrimitive(std::shared_ptr<IPrimitive> &_render) {
    m_primitiveList.emplace(_render->getRenderID(), _render);

    // 记录天空盒图元
    if(_render->getPrimitiveType() == PrimitiveType::Skybox) {
        m_skyboxPrimitive = _render;
    }
}

void IScene::render() {
    for(const auto& [id, primitive]: this->getAllPrimitive()) {
        primitive->preRender();
        primitive->render();
    }
}

void IScene::deletePrimitive(int _id) {
    m_primitiveList.erase(_id);
}

// 开启/关闭阴影
void IScene::setShadow(bool _on) {
    m_isShadow = _on;

    if(_on) {
        m_shadowShaderProgram = std::make_shared<ShaderProgram>(SHADOW_VERTEX_FILE, SHADOW_FRAGMENT_FILE);
        m_shadowFbo = std::make_unique<FrameBuffer>();
        m_shadowFboColorTexture = std::make_shared<Texture>(m_windowSize, 7, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NEAREST, GL_NEAREST, Texture::DEPTH_COMPONENT);
        m_shadowFbo->attachTexture2D(FRAMEBUFFER_ATTACH_TYPE::DEPTH, m_shadowFboColorTexture);
        glDrawBuffer(GL_NONE);
        glDrawBuffer(GL_NONE);
        m_shadowFbo->unbind();
    }
    else {
        m_shadowShaderProgram.reset();
        m_shadowFbo.reset();
        m_shadowFboColorTexture.reset();
    }

    m_shaderProgram->use();
    m_shaderProgram->setBool("enableShadow", _on);
}
