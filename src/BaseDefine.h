/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 15:04
* @version: 1.0
* @description: 定义基础数据
********************************************************************************/

#ifndef OPENGL_PRIMARY_BASEDEFINE_H
#define OPENGL_PRIMARY_BASEDEFINE_H

#include <variant>
#include <string>

constexpr int SCREEN_WIDTH = 2250;                                  // 窗口宽度
constexpr int SCREEN_HEIGHT = 1340;                                 // 窗口高度
constexpr const char *WINDOW_TITLE = "opengl_primary";              // 窗口标题

#ifdef DEBUG
constexpr char *SHADER_CODE_PATH = "../src/glsl";
constexpr char *TEXTURE_PATH = "../resources/texture";
constexpr char *MODEL_PATH = "../resources/model";
#else
constexpr char *SHADER_CODE_PATH = "./src/glsl";
constexpr char *TEXTURE_PATH = "./resources/texture";
constexpr char *MODEL_PATH = "./resources/model";
#endif

// ----------------------------------------------- 类型定义 ----------------------------------------------------------
using Color = std::tuple<float, float, float, float>;

using Size = std::tuple<float, float>;

// 图元类型
enum class PrimitiveType {
    None,
    Cube,
    PhongLight,
};

// 渲染物体的类型
enum class RenderType { None, Scene, Primitive };

enum class KEYBOARD {
    KEY_A = 65,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_ESCAPE = 256
};

enum class Event {
    WINDOW_CLOSE,
    WINDOW_RESIZE,
    MOUSE_WHEEL,
    MOUSE_MOVE,
    MOUSE_PRESS,
    KEY_PRESS,

    PRIMITIVE_SELECTED,             // 当前选中的图元被改变
    SCENE_SELECTED,                 // 当前选中的场景被改变
};

enum class MOUSE_BUTTON { None, Left, Right, Middle };

using MouseState = std::tuple<MOUSE_BUTTON, double, double>;                        // 鼠标按键的状态以及鼠标的位置

using EventParam = std::variant<KEYBOARD, MouseState, Size, double, int, std::string>;

struct VPos {
    float x;
    float y;
    float z;
};

struct VColor {
    float r;
    float g;
    float b;
};

struct VTexCoord {
    float x;
    float y;
};

struct Vertex {
    VPos pos;
    VColor color;
    VTexCoord tex;
};

#endif //OPENGL_PRIMARY_BASEDEFINE_H
