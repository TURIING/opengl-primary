/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/12 14:19
* @version: 1.0
* @description: 
********************************************************************************/

#include "Application.h"
#include "InspectPanel.h"
#include "../primitive/Cube.h"
#include "../primitive/Floor.h"
#include "../primitive/Square.h"
#include "../primitive/Skybox.h"
#include "../primitive/Model.h"

void Application::init() {
    const auto winSize = Size{ SCREEN_WIDTH, SCREEN_HEIGHT };

    m_glContext = std::make_unique<GLContext>(winSize, WINDOW_TITLE);
    m_glContext->registerKeyBoard(KEYBOARD::KEY_ESCAPE);

    m_uiContext = std::make_unique<UIContext>();
    m_propertyPanel = std::make_unique<PropertyPanel>();
    m_inspectPanel = std::make_unique<InspectPanel>();

    m_currentScene = std::make_shared<IScene>();

    // 初始化窗口时，先向scene分发一次resize事件，初始化视口
    m_currentScene->dispatch(Event::WINDOW_RESIZE, winSize);
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
        case Event::WINDOW_RESIZE:                     this->onWindowResize(_param);                               break;
        case Event::WINDOW_CLOSE:                      this->onWindowClose(_param);                                break;
        case Event::MOUSE_MOVE:                        this->onMouseMove(_param);                                  break;
        case Event::MOUSE_WHEEL:                       this->onMouseWheelScroll(_param);                           break;
        case Event::KEY_PRESS:                         this->onKeyPress(_param);                                   break;
        case Event::PRIMITIVE_SELECTED:                m_inspectPanel->dispatch(_event, _param);          break;
        case Event::MESH_SELECTED:                     m_inspectPanel->dispatch(_event, _param);          break;
        case Event::SCENE_CREATED:                     this->onSceneCreated();                                        break;
        case Event::PRIMITIVE_CREATED:                 this->onPrimitiveAdded(_param);                             break;
        case Event::PRIMITIVE_DELETED:                 this->onPrimitiveDeleted(_param);                           break;
        default:                                       LOG(FATAL) << "Parameter error.";
    }
}

void Application::render() {
    m_glContext->preRender();
    m_uiContext->preRender();

    m_currentScene->preRender();
    m_currentScene->render();
    m_currentScene->postRender();
    m_propertyPanel->render();
    m_inspectPanel->render();

    m_uiContext->postRender();
    m_glContext->postRender();

    m_glContext->detectEvent();
}

void Application::onWindowResize(EventParam &_param) {
    m_currentScene->dispatch(Event::WINDOW_RESIZE, _param);
    this->render();
}

void Application::onWindowClose(EventParam &_param) {
    m_shouldClose = true;
}

void Application::onMouseMove(EventParam &_param) {
    m_currentScene->dispatch(Event::MOUSE_MOVE, _param);
}

void Application::onMouseWheelScroll(EventParam &_param) {
    m_currentScene->dispatch(Event::MOUSE_WHEEL, _param);
}

void Application::onKeyPress(EventParam &_param) {
    // 拦截Esc关闭窗口事件
    if(std::get<KEYBOARD>(_param) == KEYBOARD::KEY_ESCAPE) {
        m_shouldClose = true;
        return;
    }

    m_currentScene->dispatch(Event::KEY_PRESS, _param);
}

void Application::onSceneCreated() {
    m_currentScene.reset(new IScene());

    const auto winSize = Size{ SCREEN_WIDTH, SCREEN_HEIGHT };
    m_currentScene->dispatch(Event::WINDOW_RESIZE, winSize);
    m_inspectPanel->dispatch(Event::SCENE_CREATED, -1);
}

void Application::onPrimitiveAdded(EventParam &_param) {
    const auto& [type, name, modelPath] = std::get<PrimitiveInfo>(_param);
    std::shared_ptr<IPrimitive> primitive = this->makePrimitiveByType(type, name, modelPath);
    m_currentScene->addPrimitive(primitive);
    this->dispatch(Event::PRIMITIVE_SELECTED, primitive->getRenderID());
}

// 通过给定的类型生成对应的图元派生类
std::shared_ptr<IPrimitive> Application::makePrimitiveByType(PrimitiveType _type, const std::string &_name, const std::string &_modelPath) {
    LOG_ASSERT(m_currentScene);

    switch (_type) {
        case PrimitiveType::Cube:           return std::make_shared<Cube>(m_currentScene, _name);
        case PrimitiveType::Floor:          return std::make_shared<Floor>(m_currentScene, _name);
        case PrimitiveType::Square:         return std::make_shared<Square>(m_currentScene, _name);
        case PrimitiveType::Skybox:         return std::make_shared<Skybox>(m_currentScene, _name);
        case PrimitiveType::Model:          return std::make_shared<Model>(m_currentScene, _name, _modelPath);
        default:                            LOG(FATAL) << " Undefined conditional branch.";
    }
}

void Application::onPrimitiveDeleted(EventParam &_param) {
    m_currentScene->dispatch(Event::PRIMITIVE_DELETED, _param);
    m_inspectPanel->dispatch(Event::PRIMITIVE_DELETED, _param);
}

