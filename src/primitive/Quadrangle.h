/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 21:36
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_QUADRANGLE_H
#define OPENGL_PRIMARY_QUADRANGLE_H

#include "../core/IPrimitive.h"
#include "../base/VertexArray.h"
#include "../base/Buffer.h"
#include "../base/ShaderProgram.h"
#include "../base/Texture.h"
#include "../BaseDefine.h"

class Quadrangle: public IPrimitive {
    // 顶点属性结构体定义
    struct Vertex {
        glm::vec2 pos;
        glm::vec2 tex;
    };
public:
    explicit Quadrangle(std::shared_ptr<IScene> &_parent, const std::string &_name);
    void render() override;

private:
    std::unique_ptr<VertexArray> m_vao;
    std::unique_ptr<Buffer<Vertex>> m_vbo;

    const std::string VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/frame_buffer/vertex.glsl";
    const std::string FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/frame_buffer/fragment.glsl";

    std::vector<Vertex> m_vertices = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        Vertex{ glm::vec2 ( -1.0f,  1.0f ), glm::vec2 ( 0.0f, 1.0f ) },
        Vertex{ glm::vec2 ( -1.0f, -1.0f ), glm::vec2 ( 0.0f, 0.0f ) },
        Vertex{ glm::vec2 ( 1.0f,  -1.0f ), glm::vec2 ( 1.0f, 0.0f ) },

        Vertex{ glm::vec2 ( -1.0f,  1.0f ), glm::vec2 ( 0.0f, 1.0f ) },
        Vertex{ glm::vec2 ( 1.0f,  -1.0f ), glm::vec2 ( 1.0f, 0.0f ) },
        Vertex{ glm::vec2 ( 1.0f,   1.0f ), glm::vec2 ( 1.0f, 1.0f ) }
    };
};

#endif //OPENGL_PRIMARY_QUADRANGLE_H
