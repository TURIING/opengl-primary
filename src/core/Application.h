/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/12 14:19
* @version: 1.0
* @description: 应用程序类
********************************************************************************/

#ifndef OPENGL_PRIMARY_APPLICATION_H
#define OPENGL_PRIMARY_APPLICATION_H

#include <string>
#include <memory>
#include "GLContext.h"
#include "UIContext.h"
#include "../BaseDefine.h"
#include "GLFW/glfw3.h"
#include "PropertyPanel.h"

class InspectPanel;
class IScene;
class IPrimitive;

class Application {
public:
    static Application* instance();
    void init();
    void loop();
    void setName(std::string &_name) { m_appName = _name; }
    void dispatch(Event _event, EventParam _param);
    GLFWwindow* getWindowHandle() { return m_glContext->getWindowHandle(); }
    std::shared_ptr<IScene> getCurrentScene() { return m_currentScene; }

private:
    Application() = default;
    void render();
    inline void onWindowResize(EventParam &_param);
    inline void onWindowClose(EventParam &_param);
    inline void onMouseMove(EventParam &_param);
    inline void onMouseWheelScroll(EventParam &_param);
    inline void onKeyPress(EventParam &_param);
    inline void onSceneCreated();
    inline void onPrimitiveAdded(EventParam &_param);
    inline void onPrimitiveDeleted(EventParam &_param);
    std::shared_ptr<IPrimitive> makePrimitiveByType(PrimitiveType _type, const std::string &_name, const std::string &_modelPath);

private:
    std::string m_appName;
    std::unique_ptr<GLContext> m_glContext;
    std::unique_ptr<UIContext> m_uiContext;
    std::unique_ptr<PropertyPanel> m_propertyPanel;
    std::unique_ptr<InspectPanel> m_inspectPanel;
    bool m_shouldClose = false;                                     // 程序是否应该关闭
    std::shared_ptr<IScene> m_currentScene;
};


#endif //OPENGL_PRIMARY_APPLICATION_H
