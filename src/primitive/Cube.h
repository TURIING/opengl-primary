/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 20:19
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_CUBE_H
#define OPENGL_PRIMARY_CUBE_H

#include "../primitive/IPrimitive.h"
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

class Cube: public IPrimitive {
    struct Vertex {
        Pos pos;
        TexCoord tex;
    };
public:
    Cube();
    void paint() override;

private:
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<Buffer<Vertex>> m_vbo;
    std::shared_ptr<ShaderProgram> m_shaderProgram;
    std::shared_ptr<Texture> m_texture;

    const std::string VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/cube/vertex.glsl";
    const std::string FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/cube/fragment.glsl";
    const std::string TEXTURE_FILE = std::string(TEXTURE_PATH) + "/container2.png";

    std::vector<Vertex> m_vertices = {
        Vertex { Pos{-0.5f, -0.5f, -0.5f}, TexCoord{0.0f, 0.0f} },
        Vertex { Pos{0.5f, -0.5f, -0.5f}, TexCoord{1.0f, 0.0f} },
        Vertex { Pos{0.5f,  0.5f, -0.5f}, TexCoord{1.0f, 1.0f} },
        Vertex { Pos{0.5f,  0.5f, -0.5f}, TexCoord{1.0f, 1.0f} },
        Vertex { Pos{-0.5f,  0.5f, -0.5f}, TexCoord{0.0f, 1.0f} },
        Vertex { Pos{-0.5f, -0.5f, -0.5f}, TexCoord{0.0f, 0.0f} },

        Vertex { Pos{-0.5f, -0.5f,  0.5f}, TexCoord{0.0f, 0.0f} },
        Vertex { Pos{0.5f, -0.5f,  0.5f}, TexCoord{1.0f, 0.0f} },
        Vertex { Pos{0.5f,  0.5f,  0.5f}, TexCoord{1.0f, 1.0f} },
        Vertex { Pos{0.5f,  0.5f,  0.5f}, TexCoord{1.0f, 1.0f} },
        Vertex { Pos{-0.5f,  0.5f,  0.5f}, TexCoord{0.0f, 1.0f} },
        Vertex { Pos{-0.5f, -0.5f,  0.5f}, TexCoord{0.0f, 0.0f} },

        Vertex { Pos{-0.5f,  0.5f,  0.5f}, TexCoord{1.0f, 0.0f} },
        Vertex { Pos{-0.5f,  0.5f, -0.5f}, TexCoord{1.0f, 1.0f} },
        Vertex { Pos{-0.5f, -0.5f, -0.5f}, TexCoord{0.0f, 1.0f} },
        Vertex { Pos{-0.5f, -0.5f, -0.5f}, TexCoord{0.0f, 1.0f} },
        Vertex { Pos{-0.5f, -0.5f,  0.5f}, TexCoord{0.0f, 0.0f} },
        Vertex { Pos{-0.5f,  0.5f,  0.5f}, TexCoord{1.0f, 0.0f} },

        Vertex { Pos{0.5f,  0.5f,  0.5f}, TexCoord{1.0f, 0.0f} },
        Vertex { Pos{0.5f,  0.5f, -0.5f}, TexCoord{1.0f, 1.0f} },
        Vertex { Pos{0.5f, -0.5f, -0.5f}, TexCoord{0.0f, 1.0f} },
        Vertex { Pos{0.5f, -0.5f, -0.5f}, TexCoord{0.0f, 1.0f} },
        Vertex { Pos{0.5f, -0.5f,  0.5f}, TexCoord{0.0f, 0.0f} },
        Vertex { Pos{0.5f,  0.5f,  0.5f}, TexCoord{1.0f, 0.0f} },

        Vertex { Pos{-0.5f, -0.5f, -0.5f}, TexCoord{ 0.0f, 1.0f} },
        Vertex { Pos{0.5f, -0.5f,  0.5f}, TexCoord{1.0f, 0.0f} },
        Vertex { Pos{0.5f, -0.5f, -0.5f}, TexCoord{1.0f, 1.0f} },
        Vertex { Pos{0.5f, -0.5f,  0.5f}, TexCoord{1.0f, 0.0f} },
        Vertex { Pos{-0.5f, -0.5f,  0.5f}, TexCoord{0.0f, 0.0f} },
        Vertex { Pos{-0.5f, -0.5f, -0.5f}, TexCoord{0.0f, 1.0f} },


        Vertex { Pos{-0.5f,  0.5f, -0.5f}, TexCoord{0.0f, 1.0f} },
        Vertex { Pos{0.5f,   0.5f, -0.5f}, TexCoord{1.0f, 1.0f} },
        Vertex { Pos{0.5f,   0.5f,  0.5f}, TexCoord{1.0f, 0.0f} },
        Vertex { Pos{0.5f,   0.5f,  0.5f}, TexCoord{1.0f, 0.0f} },
        Vertex { Pos{-0.5f,  0.5f,  0.5f}, TexCoord{0.0f, 0.0f} },
        Vertex { Pos{-0.5f,  0.5f, -0.5f}, TexCoord{0.0f, 1.0f} },
    };
};


#endif //OPENGL_PRIMARY_CUBE_H
