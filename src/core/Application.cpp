/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/12 14:19
* @version: 1.0
* @description: 
********************************************************************************/

#include "Application.h"
#include "../BaseDefine.h"
#include "InspectPanel.h"

Application::Application() { }

void Application::init() {
    const auto winSize = Size{ SCREEN_WIDTH, SCREEN_HEIGHT };

    m_glContext = std::make_unique<GLContext>(winSize, WINDOW_TITLE);
    m_glContext->registerKeyBoard(KEYBOARD::KEY_ESCAPE);

    m_uiContext = std::make_unique<UIContext>();
    m_propertyPanel = std::make_unique<PropertyPanel>();
    m_inspectPanel = std::make_unique<InspectPanel>();
    m_scene = std::make_shared<Blend>();

    m_currentScene = m_scene;

    // 初始化窗口时，先向scene分发一次resize事件，初始化视口
    m_scene->dispatch(Event::WINDOW_RESIZE, winSize);
}

void Application::loop() {
    while (!m_shouldClose) {
        this->render();
    }
}

Application* Application::instance() {
    static Application app;
    return &app;
}

void Application::dispatch(Event _event, EventParam _param) {
    switch (_event) {
        case Event::WINDOW_RESIZE:                          this->onWindowResize(_param);                               break;
        case Event::WINDOW_CLOSE:                           this->onWindowClose(_param);                                break;
        case Event::MOUSE_MOVE:                             this->onMouseMove(_param);                                  break;
        case Event::MOUSE_WHEEL:                            this->onMouseWheelScroll(_param);                           break;
        case Event::KEY_PRESS:                              this->onKeyPress(_param);                                   break;
        case Event::PRIMITIVE_SELECTED:                     m_inspectPanel->dispatch(_event, _param);             break;
        default:                    LOG(FATAL) << "Parameter error.";
    }
}

void Application::render() {
    m_glContext->preRender();
    m_uiContext->preRender();

    m_scene->preRender();
    m_scene->render();
    m_scene->postRender();
    m_propertyPanel->render();
    m_inspectPanel->render();

    m_uiContext->postRender();
    m_glContext->postRender();

    m_glContext->detectEvent();
}

void Application::onWindowResize(EventParam &_param) {
    m_scene->dispatch(Event::WINDOW_RESIZE, _param);
    this->render();
}

void Application::onWindowClose(EventParam &_param) {
    m_shouldClose = true;
}

void Application::onMouseMove(EventParam &_param) {
    m_scene->dispatch(Event::MOUSE_MOVE, _param);
}

void Application::onMouseWheelScroll(EventParam &_param) {
    m_scene->dispatch(Event::MOUSE_WHEEL, _param);
}

void Application::onKeyPress(EventParam &_param) {
    // 拦截Esc关闭窗口事件
    if(std::get<KEYBOARD>(_param) == KEYBOARD::KEY_ESCAPE) {
        m_shouldClose = true;
        return;
    }

    m_scene->dispatch(Event::KEY_PRESS, _param);
}

