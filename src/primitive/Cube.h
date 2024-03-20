/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 20:19
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_CUBE_H
#define OPENGL_PRIMARY_CUBE_H

#include "../core/IPrimitive.h"
#include "../base/VertexArray.h"
#include "../base/Buffer.h"
#include "../base/ShaderProgram.h"
#include "../base/Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include <memory>
#include "../BaseDefine.h"
#include "../core/IScene.h"

class Cube: public IPrimitive {
    // 顶点属性结构体定义
    struct Vertex {
        VPos pos;
        VTexCoord tex;
    };
public:
    Cube(IScene *_parent, std::string _name, std::shared_ptr<ShaderProgram> _shaderProgram = nullptr);
    void render() override;

private:
    void paintNormally();
    void paintWithOutline();

private:
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<Buffer<Vertex>> m_vbo;
    std::shared_ptr<Texture> m_texture;

    const std::string VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/cube/vertex.glsl";
    const std::string FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/cube/fragment.glsl";
    const std::string TEXTURE_FILE = std::string(TEXTURE_PATH) + "/container2.png";

    std::vector<Vertex> m_vertices = {
        // Back face
        Vertex{ VPos{ -0.5f, -0.5f, -0.5f }, VTexCoord{ 0.0f, 0.0f }}, // Bottom-left
        Vertex{ VPos{ 0.5f,  0.5f, -0.5f }, VTexCoord{ 1.0f, 1.0f }}, // top-right
        Vertex{ VPos{ 0.5f, -0.5f, -0.5f }, VTexCoord{ 1.0f, 0.0f }}, // bottom-right
        Vertex{ VPos{ 0.5f,  0.5f, -0.5f }, VTexCoord{ 1.0f, 1.0f }}, // top-right
        Vertex{ VPos{ -0.5f, -0.5f, -0.5f }, VTexCoord{ 0.0f, 0.0f }}, // bottom-left
        Vertex{ VPos{ -0.5f,  0.5f, -0.5f }, VTexCoord{ 0.0f, 1.0f }}, // top-left
        // Front face
        Vertex{ VPos{ -0.5f, -0.5f,  0.5f }, VTexCoord{ 0.0f, 0.0f }}, // bottom-left
        Vertex{ VPos{ 0.5f, -0.5f,  0.5f }, VTexCoord{ 1.0f, 0.0f }}, // bottom-right
        Vertex{ VPos{ 0.5f,  0.5f,  0.5f }, VTexCoord{ 1.0f, 1.0f }}, // top-right
        Vertex{ VPos{ 0.5f,  0.5f,  0.5f }, VTexCoord{ 1.0f, 1.0f }}, // top-right
        Vertex{ VPos{ -0.5f,  0.5f,  0.5f }, VTexCoord{ 0.0f, 1.0f }}, // top-left
        Vertex{ VPos{ -0.5f, -0.5f,  0.5f }, VTexCoord{ 0.0f, 0.0f }}, // bottom-left
        // Left face
        Vertex{ VPos{ -0.5f,  0.5f,  0.5f }, VTexCoord{ 1.0f, 0.0f }}, // top-right
        Vertex{ VPos{ -0.5f,  0.5f, -0.5f }, VTexCoord{ 1.0f, 1.0f }}, // top-left
        Vertex{ VPos{ -0.5f, -0.5f, -0.5f }, VTexCoord{ 0.0f, 1.0f }}, // bottom-left
        Vertex{ VPos{ -0.5f, -0.5f, -0.5f }, VTexCoord{ 0.0f, 1.0f }}, // bottom-left
        Vertex{ VPos{ -0.5f, -0.5f,  0.5f }, VTexCoord{ 0.0f, 0.0f }}, // bottom-right
        Vertex{ VPos{ -0.5f,  0.5f,  0.5f }, VTexCoord{ 1.0f, 0.0f }}, // top-right
        // Right face
        Vertex{ VPos{ 0.5f,  0.5f,  0.5f }, VTexCoord{ 1.0f, 0.0f }}, // top-left
        Vertex{ VPos{ 0.5f, -0.5f, -0.5f }, VTexCoord{ 0.0f, 1.0f }}, // bottom-right
        Vertex{ VPos{ 0.5f,  0.5f, -0.5f }, VTexCoord{ 1.0f, 1.0f }}, // top-right
        Vertex{ VPos{ 0.5f, -0.5f, -0.5f }, VTexCoord{ 0.0f, 1.0f }}, // bottom-right
        Vertex{ VPos{ 0.5f,  0.5f,  0.5f }, VTexCoord{ 1.0f, 0.0f }}, // top-left
        Vertex{ VPos{ 0.5f, -0.5f,  0.5f }, VTexCoord{ 0.0f, 0.0f }}, // bottom-left
        // Bottom face
        Vertex{ VPos{ -0.5f, -0.5f, -0.5f }, VTexCoord{ 0.0f, 1.0f }}, // top-right
        Vertex{ VPos{ 0.5f, -0.5f, -0.5f }, VTexCoord{ 1.0f, 1.0f }}, // top-left
        Vertex{ VPos{ 0.5f, -0.5f,  0.5f }, VTexCoord{ 1.0f, 0.0f }}, // bottom-left
        Vertex{ VPos{ 0.5f, -0.5f,  0.5f }, VTexCoord{ 1.0f, 0.0f }}, // bottom-left
        Vertex{ VPos{ -0.5f, -0.5f,  0.5f }, VTexCoord{ 0.0f, 0.0f }}, // bottom-right
        Vertex{ VPos{ -0.5f, -0.5f, -0.5f }, VTexCoord{ 0.0f, 1.0f }}, // top-right
        // Top face
        Vertex{ VPos{ -0.5f,  0.5f, -0.5f }, VTexCoord{ 0.0f, 1.0f }}, // top-left
        Vertex{ VPos{ 0.5f,  0.5f,  0.5f }, VTexCoord{ 1.0f, 0.0f }}, // bottom-right
        Vertex{ VPos{ 0.5f,  0.5f, -0.5f }, VTexCoord{ 1.0f, 1.0f }}, // top-right
        Vertex{ VPos{ 0.5f,  0.5f,  0.5f }, VTexCoord{ 1.0f, 0.0f }}, // bottom-right
        Vertex{ VPos{ -0.5f,  0.5f, -0.5f }, VTexCoord{ 0.0f, 1.0f }}, // top-left
        Vertex{ VPos{ -0.5f,  0.5f,  0.5f }, VTexCoord{ 0.0f, 0.0f }}  // bottom-left
    };
};


#endif //OPENGL_PRIMARY_CUBE_H
