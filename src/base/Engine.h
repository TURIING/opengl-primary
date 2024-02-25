#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <string>
#include <memory>
#include <tuple>

#include "Camera.h"
#include "IRenderer.h"

class Engine
{
public:
	Engine(const std::string &_title, bool _enableCamera = true, int _width = 1600, int _height = 1200);
    ~Engine();

    void setRenderer(std::shared_ptr<IRenderer> _renderer);
	void execute();

private:
	void init();
    void initImgui();
	static void frameBufferSizeCallback(GLFWwindow* _window, int _width, int _height);
	void dealKeyBoardEvent(GLFWwindow* _window);
	void dealMouseMoveEvent(GLFWwindow* _window, double _xPosIn, double _yPosIn);
	void dealMouseWheelEvent(GLFWwindow* _window, double _xOffset, double _yOffset) const;
    void dealCameraParam();
    void registerEvent();
	void paint();

    static void mouseMoveCallback(GLFWwindow* _window, double _xPosIn, double _yPosIn);

private:
    std::tuple<int, int> m_windowSize;                                          // 窗口尺寸
    std::string m_title;                                                        // 窗口标题

	GLFWwindow *m_window = nullptr;

    float m_deltaTime = 0.0f;													// 上一帧跟当前帧的时间间隔

	float m_lastFrame = 0.0f;													// 记录当前帧的时间
    bool m_bFirstCaptureMouse = true;											// 鼠标是否刚被捕获
    bool m_isCaptureCursor = true;                                              // glfw是否处于捕获鼠标的状态
    float m_fLastX = 0.0f;

	float m_fLastY = 0.0f;
    std::shared_ptr<Camera> m_camera;

    std::shared_ptr<IRenderer> m_renderer;
    bool m_enableCamera;                                                        // 是否开启摄像机
};

