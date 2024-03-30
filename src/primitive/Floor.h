/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/25 22:27
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_FLOOR_H
#define OPENGL_PRIMARY_FLOOR_H

#include "../core/IPrimitive.h"
#include "../base/VertexArray.h"
#include "../base/Buffer.h"
#include "../base/ShaderProgram.h"
#include "../base/Texture.h"
#include "../core/IScene.h"

#include "glog/logging.h"
#include "imgui/imgui.h"
#include <memory>

class Floor: public IPrimitive{
    // 顶点属性结构体定义
    struct Vertex {
        VPos pos;
        VTexCoord tex;
    };
public:
    Floor(std::shared_ptr<IScene> _parent, std::string _name);
    void render() override;

private:
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<Buffer<Vertex>> m_vbo;

    std::vector<Vertex> m_vertices = {
        Vertex{ VPos{5.0f,  -0.5f,  5.0f,}, VTexCoord{2.0f, 0.0f,}},
        Vertex{ VPos{-5.0f, -0.5f,  5.0f,}, VTexCoord{0.0f, 0.0f,}},
        Vertex{ VPos{-5.0f, -0.5f, -5.0f,}, VTexCoord{0.0f, 2.0f,}},

        Vertex{ VPos{5.0f,  -0.5f,  5.0f,}, VTexCoord{2.0f, 0.0f,}},
        Vertex{ VPos{-5.0f, -0.5f, -5.0f,}, VTexCoord{0.0f, 2.0f,}},
        Vertex{ VPos{5.0f,  -0.5f, -5.0f,}, VTexCoord{2.0f, 2.0f}},
    };

};


#endif //OPENGL_PRIMARY_FLOOR_H
