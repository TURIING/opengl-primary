/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/11 14:20
* @version: 1.0
* @description: 
********************************************************************************/

#include "FrameBufferDemo.h"

FrameBufferDemo::FrameBufferDemo() {
    this->setClearColor(0, 0, 0, 1);
    this->setDeepTest(true);

    m_objectShaderProgram = std::make_shared<ShaderProgram>(OBJECT_VERTEX_FILE, OBJECT_FRAGMENT_FILE);
    m_screenShaderProgram = std::make_shared<ShaderProgram>(SCREEN_VERTEX_FILE, SCREEN_FRAGMENT_FILE);

    m_floor = std::make_unique<Floor>(this, m_objectShaderProgram);

    m_cube1 = std::make_unique<Cube>(this, m_objectShaderProgram);

    m_cube2 = std::make_unique<Cube>(this, m_objectShaderProgram);

    m_quad = std::make_unique<Quadrangle>(this, m_screenShaderProgram);

    m_fbo = std::make_unique<FrameBuffer>();
    m_screenTexture = std::make_shared<Texture>(getWindowWidth(), getWindowHeight(), GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    m_quad->addTexture(m_screenTexture);
    m_fbo->attachTexture2D(FRAMEBUFFER_ATTACH_TYPE::COLOR, m_screenTexture->getID());

    m_rbo = std::make_unique<RenderBuffer>();
    m_rbo->storage(RENDER_BUFFER_FORMAT::DEPTH24_STENCIL8, getWindowWidth(), getWindowHeight());
    m_fbo->attachRenderBuffer(FRAMEBUFFER_ATTACH_TYPE::DEPTH_STENCIL, m_rbo->getID());

    m_fbo->check();
    m_fbo->unbind();
}

void FrameBufferDemo::paint() {
    m_fbo->bind();
    this->setDeepTest(true);
    this->clear();

    m_objectShaderProgram->use();
    ImGui::Begin("Cube");
    static float translateValue1[] = {-1.0f, 0.0f, -1.0f};
    ImGui::InputFloat3("translate1", translateValue1, 2);

    static float translateValue2[] = {2.0f, 0.0f, -1.2f};
    ImGui::InputFloat3("translate2", translateValue2, 2);
    ImGui::End();

    m_floor->paint();

    m_cube1->translate(translateValue1[0], translateValue1[1], translateValue1[2]);
    m_cube1->paint();

    m_cube2->translate(translateValue2[0], translateValue2[1], translateValue2[2]);
    m_cube2->paint();
    m_floor->paint();

    m_fbo->unbind();

    this->setDeepTest(false);
    this->clear();

    m_quad->paint();
}