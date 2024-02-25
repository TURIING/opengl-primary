/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 15:04
* @version: 1.0
* @description: 定义基础数据
********************************************************************************/

#ifndef OPENGL_PRIMARY_BASEDEFINE_H
#define OPENGL_PRIMARY_BASEDEFINE_H

constexpr char *SHADER_CODE_PATH = "../../../../src/glsl";
constexpr char *TEXTURE_PATH = "../../../../resources/texture";
constexpr char *MODEL_PATH = "../../../../resources/model";

// ----------------------------------------------- 类型定义 ----------------------------------------------------------
using TColor = std::tuple<float, float, float, float>;

struct Pos {
    float x;
    float y;
    float z;
};

struct Color {
    float r;
    float g;
    float b;
};

struct TexCoord {
    float x;
    float y;
};

struct Vertex {
    Pos pos;
    Color color;
    TexCoord tex;
};

#endif //OPENGL_PRIMARY_BASEDEFINE_H