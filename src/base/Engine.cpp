//#include "Engine.h"
//#include "glog/logging.h"
//
//Engine::Engine(const std::string& _title, bool _enableCamera, int _width, int _height):m_title(_title), m_enableCamera(_enableCamera)
//{
//    m_windowSize = std::make_tuple(_width, _height);
//    this->init();
//}
//
//Engine::~Engine() {
//    // 释放ImGui资源
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwTerminate();
//}
//
///**
// * \brief 初始化GLFW、GLAD
// */
//void Engine::init() {
//    /* 初始化GLFW */
//    glfwInit();                                                                                            // 初始化GLFW
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                                              // 设置主版本号和次版本号
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);                              // 设置使用核心模式
//
//    /* 创建窗口 */
//    auto [width, height] = m_windowSize;
//    m_window = glfwCreateWindow(width, height, m_title.c_str(), nullptr, nullptr);
//    LOG_IF(FATAL, !m_window) << "Failed to create GLFW window";
//
//    glfwMakeContextCurrent(m_window);                                                                 // 将窗口的上下文设置为当前线程的主上下文
//    glfwSetWindowUserPointer(m_window, this);                                                  // 将this传递给glfw，用于处理事件
//
//    this->registerEvent();
//
//    /* 初始化GLAD */
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        LOG(FATAL) << "Failed to initialize Glad";
//    }
//
//    // 摄像机对象
//    if(m_enableCamera) {
//        m_camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 4.0f));
//    }
//
//    initImgui();
//}
//
//void Engine::execute() {
//    /* 循环渲染 */
//    while (!glfwWindowShouldClose(m_window)) {
//        this->dealCameraParam();
//
//        dealKeyBoardEvent(m_window);
//
//        paint();
//
//        glfwSwapBuffers(m_window);                                                              // 交换颜色缓冲
//        glfwPollEvents();                                                                              // 检查是否触发事件
//    }
//}
//
//void Engine::paint()
//{
//    LOG_IF(FATAL, !m_renderer) << "Renderer object has not been assigned.";
//
//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();
//
//    m_renderer->paint();
//
//    ImGui::Render();
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//}
//
//void Engine::dealKeyBoardEvent(GLFWwindow* _window) {
//    if(m_enableCamera) {
//        if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
//            m_camera->processKeyboard(CAMERA_DIRECTION::FORWARD, m_deltaTime);
//        if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
//            m_camera->processKeyboard(CAMERA_DIRECTION::BACKWARD, m_deltaTime);
//        if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
//            m_camera->processKeyboard(CAMERA_DIRECTION::LEFT, m_deltaTime);
//        if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
//            m_camera->processKeyboard(CAMERA_DIRECTION::RIGHT, m_deltaTime);
//    }
//
//    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//        glfwSetWindowShouldClose(_window, true);
//    }
//    // 是否停止捕获鼠标
//    else if(glfwGetKey(_window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
//        if(m_isCaptureCursor) {
//            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//            m_isCaptureCursor = false;
//            glfwSetCursorPosCallback(m_window, nullptr);
//        }
//        else {
//            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//            m_isCaptureCursor = true;
//            glfwSetCursorPosCallback(m_window, mouseMoveCallback);
//        }
//    }
//}
//
//void Engine::dealMouseMoveEvent(GLFWwindow* _window, double _xPosIn, double _yPosIn) {
//    auto xpos = static_cast<float>(_xPosIn);
//    auto ypos = static_cast<float>(_yPosIn);
//
//    if (m_bFirstCaptureMouse) {
//        m_fLastX = xpos;
//        m_fLastY = ypos;
//        m_bFirstCaptureMouse = false;
//    }
//
//    const float xOffset = xpos - m_fLastX;
//    const float yOffset = m_fLastY - ypos;
//
//    m_fLastX = xpos;
//    m_fLastY = ypos;
//
//    m_camera->processMouseMovement(xOffset, yOffset);
//}
//
//void Engine::dealMouseWheelEvent(GLFWwindow* _window, double _xOffset, double _yOffset) const {
//    m_camera->processMouseScroll(static_cast<float>(_yOffset));
//}
//
///**
// * \brief 窗口尺寸改变，视口回调函数
// * \param _window
// * \param _width
// * \param _height
// */
//void Engine::frameBufferSizeCallback(GLFWwindow* _window, int _width, int _height) {
//    // 告诉OpenGL渲染窗口的尺寸大小
//    glViewport(0, 0, _width, _height);
//}
//
//void Engine::dealCameraParam() {
//    if(!m_enableCamera) return;
//
//    const auto currentFrame = static_cast<float>(glfwGetTime());
//    m_deltaTime = currentFrame - m_lastFrame;
//    m_lastFrame = currentFrame;
//}
//
//void Engine::registerEvent() {
//    LOG_ASSERT(m_window);
//
//    // 绑定窗口尺寸大小事件
//    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);
//
//    if(!m_enableCamera) return;
//
//    // 绑定鼠标移动事件
//    glfwSetCursorPosCallback(m_window, mouseMoveCallback);
//
//    // 绑定鼠标滚轮事件
//    const auto pMouseWheelCallBack = [](GLFWwindow* _window, double _xOffset, double _yOffset)
//    {
//        static_cast<Engine*>(glfwGetWindowUserPointer(_window))->dealMouseWheelEvent(_window, _xOffset, _yOffset);
//    };
//    glfwSetScrollCallback(m_window, pMouseWheelCallBack);
//
//    // 捕获鼠标
//    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//}
//
//void Engine::setRenderer(std::shared_ptr<IRenderer> &&_renderer) {
//    m_renderer = _renderer;
//
//    m_renderer->setWindowSize(m_windowSize);
//    m_renderer->setCamera(m_camera);
//}
//
//void Engine::initImgui() {
//    LOG_ASSERT(m_window);
//
//    //创建并绑定ImGui
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGui::StyleColorsDark();
//    ImGuiIO &io = ImGui::GetIO();
//    (void)io;
//    io.FontGlobalScale = 1.3f;
//    ImGui_ImplGlfw_InitForOpenGL(m_window, true);//GLFWwindow
//    ImGui_ImplOpenGL3_Init("#version 330");//自己的版本
//}
//
//void Engine::mouseMoveCallback(GLFWwindow *_window, double _xPosIn, double _yPosIn) {
//    static_cast<Engine*>(glfwGetWindowUserPointer(_window))->dealMouseMoveEvent(_window, _xPosIn, _yPosIn);
//}
