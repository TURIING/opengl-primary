/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 21:36
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_SQUARE_H
#define OPENGL_PRIMARY_SQUARE_H

#include "../core/IPrimitive.h"
#include "../core/IScene.h"
#include "../base/VertexArray.h"
#include "../base/Buffer.h"
#include "../base/ShaderProgram.h"
#include "../base/Texture.h"
#include "../BaseDefine.h"

class Square: public IPrimitive{
    // 顶点属性结构体定义
    struct Vertex {
        VPos pos;
        VTexCoord tex;
    };
public:
    void render() override;

    Square(std::shared_ptr<IScene> _parent, std::string _name);

    void preRender() override;

private:
    std::unique_ptr<VertexArray> m_vao;
    std::unique_ptr<Buffer<Vertex>> m_vbo;
    std::unique_ptr<Texture> m_texture;

    const std::string TEXTURE_FILE = std::string(TEXTURE_PATH) + "/window.png";

    std::vector<Vertex> m_vertices = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        Vertex{ VPos{ 0.0f,  0.5f,  0.0f }, VTexCoord{ 0.0f,  0.0f } },
        Vertex{ VPos{ 0.0f, -0.5f,  0.0f }, VTexCoord{ 0.0f,  1.0f } },
        Vertex{ VPos{ 1.0f, -0.5f,  0.0f }, VTexCoord{ 1.0f,  1.0f } },

        Vertex{ VPos{ 0.0f,  0.5f,  0.0f }, VTexCoord{ 0.0f,  0.0f } },
        Vertex{ VPos{ 1.0f, -0.5f,  0.0f }, VTexCoord{ 1.0f,  1.0f } },
        Vertex{ VPos{ 1.0f,  0.5f,  0.0f }, VTexCoord{ 1.0f,  0.0f } }
    };
};


#endif //OPENGL_PRIMARY_SQUARE_H
