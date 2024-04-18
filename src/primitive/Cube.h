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
#include "../base/ImageTexture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include <memory>
#include "../BaseDefine.h"
#include "../core/IScene.h"

class Cube: public IPrimitive {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

public:
    void preRender() override;

public:
    Cube(std::shared_ptr<IScene> &_parent, const std::string &_name);
    void render() override;

private:
    void paintNormally();
    void paintWithOutline();

private:
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<Buffer<Vertex>> m_vbo;

    const std::string VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/cube/vertex.glsl";
    const std::string FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/cube/fragment.glsl";

    std::vector<Vertex> m_vertices = {
        // positions          // normals           // texture coords
        Vertex { glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 0.0f,  0.0f } },
        Vertex { glm::vec3{ 0.5f,  -0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 1.0f,  0.0f } },
        Vertex { glm::vec3{ 0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 1.0f,  1.0f } },
        Vertex { glm::vec3{ 0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 1.0f,  1.0f } },
        Vertex { glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 0.0f,  1.0f } },
        Vertex { glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 0.0f,  0.0f } },

        Vertex { glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 0.0f,  0.0f } },
        Vertex { glm::vec3{ 0.5f,  -0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 1.0f,  0.0f } },
        Vertex { glm::vec3{ 0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 1.0f,  1.0f  } },
        Vertex { glm::vec3{ 0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 1.0f,  1.0f  } },
        Vertex { glm::vec3{ -0.5f, 0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 0.0f,  1.0f } },
        Vertex { glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 0.0f,  0.0f } },

        Vertex { glm::vec3{ -0.5f,  0.5f,  0.5f }, glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f,  0.0f } },
        Vertex { glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f,  1.0f } },
        Vertex { glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f,  1.0f } },
        Vertex { glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f,  1.0f } },
        Vertex { glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f,  0.0f } },
        Vertex { glm::vec3{ -0.5f,  0.5f,  0.5f }, glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f,  0.0f } },

        Vertex { glm::vec3{ 0.5f,  0.5f,  0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f,  0.0f } },
        Vertex { glm::vec3{ 0.5f,  0.5f, -0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f,  1.0f } },
        Vertex { glm::vec3{ 0.5f, -0.5f, -0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f,  1.0f } },
        Vertex { glm::vec3{ 0.5f, -0.5f, -0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f,  1.0f } },
        Vertex { glm::vec3{ 0.5f, -0.5f,  0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f,  0.0f } },
        Vertex { glm::vec3{ 0.5f,  0.5f,  0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f,  0.0f } },

        Vertex { glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 0.0f,  1.0f } },
        Vertex { glm::vec3{ 0.5f,  -0.5f, -0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 1.0f,  1.0f } },
        Vertex { glm::vec3{ 0.5f,  -0.5f,  0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 1.0f,  0.0f } },
        Vertex { glm::vec3{ 0.5f,  -0.5f,  0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 1.0f,  0.0f } },
        Vertex { glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 0.0f,  0.0f } },
        Vertex { glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 0.0f,  1.0f } },

        Vertex { glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 0.0f,  1.0f } },
        Vertex { glm::vec3{ 0.5f,   0.5f, -0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 1.0f,  1.0f } },
        Vertex { glm::vec3{ 0.5f,   0.5f,  0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 1.0f,  0.0f } },
        Vertex { glm::vec3{ 0.5f,   0.5f,  0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 1.0f,  0.0f } },
        Vertex { glm::vec3{ -0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 0.0f,  0.0f } },
        Vertex { glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 0.0f,  1.0f } }
    };
};


#endif //OPENGL_PRIMARY_CUBE_H
