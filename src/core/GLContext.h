/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/12 14:52
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_GLCONTEXT_H
#define OPENGL_PRIMARY_GLCONTEXT_H

#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../BaseDefine.h"
#include "glog/logging.h"
#include "IContext.h"

class GLContext: public IContext {
public:
    GLContext(Size _winSize, const std::string &_title);
    ~GLContext();

    GLFWwindow *getWindowHandle();
    void preRender() override;
    void postRender() override;
    void registerKeyBoard(KEYBOARD _key);
    void detectEvent();
private:
    void registerEvent();
    static void frameBufferSizeCallback(GLFWwindow* _window, int _width, int _height);
    static void windowCloseCallback(GLFWwindow* _window);
    static void mouseMoveCallback(GLFWwindow *_window, double _xPosIn, double _yPosIn);
    void dealMouseWheelEvent(GLFWwindow* _window, double _xOffset, double _yOffset) const;

    static MOUSE_BUTTON detectMouseButton(GLFWwindow *_window);
private:
    Size m_winSize;                                      // 窗口尺寸
    GLFWwindow *m_window = nullptr;
    std::vector<KEYBOARD> m_regKeys;                     // 保存已注册的键盘按键
};


#endif //OPENGL_PRIMARY_GLCONTEXT_H
