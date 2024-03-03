/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 15:04
* @version: 1.0
* @description: 定义基础数据
********************************************************************************/

#ifndef OPENGL_PRIMARY_BASEDEFINE_H
#define OPENGL_PRIMARY_BASEDEFINE_H

#ifdef DEBUG
constexpr char *SHADER_CODE_PATH = "../../../../src/glsl";
constexpr char *TEXTURE_PATH = "../../../../resources/texture";
constexpr char *MODEL_PATH = "../../../../resources/model";
#else
constexpr char *SHADER_CODE_PATH = "./src/glsl";
constexpr char *TEXTURE_PATH = "./resources/texture";
constexpr char *MODEL_PATH = "./resources/model";
#endif

// ----------------------------------------------- 类型定义 ----------------------------------------------------------
using Color = std::tuple<float, float, float, float>;

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
