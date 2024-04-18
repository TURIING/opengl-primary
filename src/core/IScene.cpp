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

const std::string PARALLEL_SHADOW_VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/parallel_shadow/vertex.glsl";
const std::string PARALLEL_SHADOW_FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/parallel_shadow/fragment.glsl";

const std::string POINT_SHADOW_VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/point_shadow/vertex.glsl";
const std::string POINT_SHADOW_FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/point_shadow/fragment.glsl";
const std::string POINT_SHADOW_GEOMETRY_FILE = std::string(SHADER_CODE_PATH) + "/point_shadow/geometry.glsl";

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
    m_screenTexture = std::make_shared<FrameBufferTexture>(_size, 2, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, FrameBufferTexture::RGB);
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
    if(m_isParallelShadow) {
        glm::mat4 shadowLightSpaceMatrix;

        for(const auto& [id, child]: m_primitiveList) {
            if(child->getLightType() != LightType::DirectionalLight) continue;

            constexpr float nearPlane = 1.0;
            constexpr float farPlane = 7.5;
            const auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
            const auto lightView = glm::lookAt(*child->getPosition(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
            shadowLightSpaceMatrix = lightProjection * lightView;

            m_parallelShadowShaderProgram->use();
            m_parallelShadowShaderProgram->setMat4("lightSpaceMatrix", glm::value_ptr(shadowLightSpaceMatrix));
        }

        // 先渲染深度贴图
        //glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        m_parallelShadowFbo->bind();
        this->clear();
        m_parallelShadowFboColorTexture->activate();
        for(const auto& [id, child]: m_primitiveList) {
            child->setShaderProgram(m_parallelShadowShaderProgram);
        }
        this->render();
        m_parallelShadowFbo->unbind();

        // 再正常渲染
        //glViewport(0, 0, m_windowSize.width, m_windowSize.height);
        for(const auto& [id, child]: m_primitiveList) {
            child->setShaderProgram(m_shaderProgram);
        }
        m_shaderProgram->use();
        m_shaderProgram->setMat4("lightSpaceMatrix", glm::value_ptr(shadowLightSpaceMatrix));
        m_shaderProgram->setInt("parallelShadowMap", m_parallelShadowFboColorTexture->getTextureUnit());
    }

    if(m_isPointShadow) {
        constexpr float nearPlane = 1.0;
        constexpr float farPlane = 25.0;

        for(const auto& [id, child]: m_primitiveList) {
            if(child->getLightType() != LightType::PointLight) continue;

            const auto lightProjection = glm::perspective(90.0f, SHADOW_WIDTH / SHADOW_HEIGHT, nearPlane, farPlane);
            const auto& lightPosition = *child->getPosition();

            std::vector<glm::mat4> shadowMatrices;
            shadowMatrices.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
            shadowMatrices.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
            shadowMatrices.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
            shadowMatrices.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
            shadowMatrices.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
            shadowMatrices.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

            m_pointShadowShaderProgram->use();
            for(auto i = 0; i < 6; i++) {
                m_pointShadowShaderProgram->setMat4("shadowMatrices[" + std::to_string(i) + ']', glm::value_ptr(shadowMatrices[i]));
            }
            m_pointShadowShaderProgram->setFloat("far_plane", farPlane);
            m_pointShadowShaderProgram->setVec3("lightPos", lightPosition);
        }

        // 先渲染深度贴图
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        m_pointShadowFbo->bind();
        this->clear();
        m_pointShadowFboColorTexture->activate();
        for(const auto& [id, child]: m_primitiveList) {
            child->setShaderProgram(m_pointShadowShaderProgram);
        }
        this->render();
        m_pointShadowFbo->unbind();

        // 再正常渲染
        glViewport(0, 0, m_windowSize.width, m_windowSize.height);
        for(const auto& [id, child]: m_primitiveList) {
            child->setShaderProgram(m_shaderProgram);
        }
        m_shaderProgram->use();
        m_shaderProgram->setFloat("far_plane", farPlane);
        m_shaderProgram->setInt("pointShadowMap", m_pointShadowFboColorTexture->getTextureUnit());
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
    ImGui::Image(reinterpret_cast<void*>(m_screenTexture->getTextureID()), viewPortSize, ImVec2 { 0, 1 }, ImVec2 { 1, 0 });
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

// 开启/关闭平行光阴影
void IScene::setParallelShadow(bool _on) {
    m_isParallelShadow = _on;

    if(_on) {
        m_parallelShadowShaderProgram = std::make_shared<ShaderProgram>(PARALLEL_SHADOW_VERTEX_FILE, PARALLEL_SHADOW_FRAGMENT_FILE);
        m_parallelShadowFbo = std::make_unique<FrameBuffer>();
        m_parallelShadowFboColorTexture = std::make_shared<FrameBufferTexture>(m_windowSize, 7, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NEAREST, GL_NEAREST, FrameBufferTexture::DEPTH_COMPONENT);
        m_parallelShadowFbo->attachTexture2D(FRAMEBUFFER_ATTACH_TYPE::DEPTH, m_parallelShadowFboColorTexture);
        glDrawBuffer(GL_NONE);
        glDrawBuffer(GL_NONE);
        m_parallelShadowFbo->unbind();
    }
    else {
        m_parallelShadowShaderProgram.reset();
        m_parallelShadowFbo.reset();
        m_parallelShadowFboColorTexture.reset();
    }

    m_shaderProgram->use();
    m_shaderProgram->setBool("enableParallelShadow", _on);
}

// 开启/关闭点光阴影
void IScene::setPointShadow(bool _on) {
    m_isPointShadow = _on;

    if(_on) {
        m_pointShadowShaderProgram = std::make_shared<ShaderProgram>(POINT_SHADOW_VERTEX_FILE, POINT_SHADOW_FRAGMENT_FILE, POINT_SHADOW_GEOMETRY_FILE);
        m_pointShadowFbo = std::make_unique<FrameBuffer>();
        m_pointShadowFboColorTexture = std::make_shared<FrameBufferTexture>(Size {SHADOW_WIDTH, SHADOW_HEIGHT}, 7, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
        m_pointShadowFbo->attachTexture(FRAMEBUFFER_ATTACH_TYPE::DEPTH, m_pointShadowFboColorTexture);
        glDrawBuffer(GL_NONE);
        glDrawBuffer(GL_NONE);
        m_pointShadowFbo->unbind();
    }
    else {
        m_pointShadowShaderProgram.reset();
        m_pointShadowFbo.reset();
        m_pointShadowFboColorTexture.reset();
    }

    m_shaderProgram->use();
    m_shaderProgram->setBool("enablePointShadow", _on);
}
