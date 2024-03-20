/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/12 14:52
* @version: 1.0
* @description: 
********************************************************************************/

#include <tuple>
#include "GLContext.h"
#include "Application.h"

/**
 *
 * @param _winSize 窗口尺寸
 * @param _title 窗口标题
 */
GLContext::GLContext(Size _winSize, const std::string &_title): m_winSize(_winSize) {
    /* 初始化GLFW */
    glfwInit();                                                                                            // 初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                                              // 设置主版本号和次版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);                              // 设置使用核心模式

    /* 创建窗口 */
    auto [width, height] = _winSize;
    m_window = glfwCreateWindow(width, height, _title.c_str(), nullptr, nullptr);
    LOG_IF(FATAL, !m_window) << "Failed to create GLFW window";

    glfwMakeContextCurrent(m_window);                                                                 // 将窗口的上下文设置为当前线程的主上下文
    glfwSetWindowUserPointer(m_window, this);                                                  // 将this传递给glfw，用于处理事件

    this->registerEvent();

    /* 初始化GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG(FATAL) << "Failed to initialize Glad";
    }

    glEnable(GL_DEPTH_TEST);
}

GLContext::~GLContext() {
    glfwDestroyWindow(Application::instance()->getWindowHandle());
    glfwTerminate();
}

void GLContext::registerEvent() {
    LOG_ASSERT(m_window);

    // 绑定窗口尺寸大小事件
    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);

    // 绑定鼠标移动事件
    //glfwSetCursorPosCallback(m_window, mouseMoveCallback);

    // 绑定鼠标滚轮事件
    const auto pMouseWheelCallBack = [](GLFWwindow* _window, double _xOffset, double _yOffset)
    {
        static_cast<GLContext*>(glfwGetWindowUserPointer(_window))->dealMouseWheelEvent(_window, _xOffset, _yOffset);
    };
    glfwSetScrollCallback(m_window, pMouseWheelCallBack);

    // 窗口关闭事件
    glfwSetWindowCloseCallback(m_window, windowCloseCallback);
}

/**
 * \brief 窗口尺寸改变，视口回调函数
 * \param _window
 * \param _width
 * \param _height
 */
void GLContext::frameBufferSizeCallback(GLFWwindow* _window, int _width, int _height) {
    Application::instance()->dispatch(Event::WINDOW_RESIZE, Size { _width, _height });
}

//void GLContext::mouseMoveCallback(GLFWwindow *_window, double _xPosIn, double _yPosIn) {
//    MOUSE_BUTTON button;
//    if(glfwGetMouseButton(_window, 0) == GLFW_PRESS) button = MOUSE_BUTTON::Left;
//    else if(glfwGetMouseButton(_window, 1) == GLFW_PRESS) button = MOUSE_BUTTON::Right;
//    else if(glfwGetMouseButton(_window, 2) == GLFW_PRESS) button = MOUSE_BUTTON::Middle;
//
//    Application::instance()->dispatch(Event::MOUSE_MOVE, MouseState { button, _xPosIn, _yPosIn });
//}

void GLContext::dealMouseWheelEvent(GLFWwindow* _window, double _xOffset, double _yOffset) const {
    Application::instance()->dispatch(Event::MOUSE_WHEEL, _yOffset);
}

GLFWwindow *GLContext::getWindowHandle() {
    LOG_ASSERT(m_window);

    return m_window;
}

void GLContext::preRender() {

}

void GLContext::postRender() {
    glfwSwapBuffers(Application::instance()->getWindowHandle());                                                              // 交换颜色缓冲
    glfwPollEvents();
}

void GLContext::registerKeyBoard(KEYBOARD _key) {
    m_regKeys.push_back(_key);
}

// 检测事件是否触发
void GLContext::detectEvent() {
    auto window = Application::instance()->getWindowHandle();

    // 检测键盘
    for(auto key : m_regKeys) {
        if(glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS) {
            Application::instance()->dispatch(Event::KEY_PRESS, key);
        }
    }

    // 检测鼠标
    const auto button = this->detectMouseButton(window);
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    Application::instance()->dispatch(Event::MOUSE_MOVE, MouseState { button, x, y });
}

// 检测鼠标按键是否按下
MOUSE_BUTTON GLContext::detectMouseButton(GLFWwindow *_window) {
    if(glfwGetMouseButton(_window, 0) == GLFW_PRESS)
        return MOUSE_BUTTON::Left;
    else if(glfwGetMouseButton(_window, 1) == GLFW_PRESS)
        return MOUSE_BUTTON::Right;
    else if(glfwGetMouseButton(_window, 2) == GLFW_PRESS)
        return MOUSE_BUTTON::Middle;

    return MOUSE_BUTTON::None;
}

void GLContext::windowCloseCallback(GLFWwindow *_window) {
    Application::instance()->dispatch(Event::WINDOW_CLOSE, -1);
}


