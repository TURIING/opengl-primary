/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/24 19:35
* @version: 1.0
* @description: 天空盒
********************************************************************************/

#ifndef OPENGL_PRIMARY_SKYBOX_H
#define OPENGL_PRIMARY_SKYBOX_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/IPrimitive.h"
#include "../base/VertexArray.h"
#include "../base/Buffer.h"
#include "../base/ShaderProgram.h"
#include "../base/Texture.h"
#include "../BaseDefine.h"

class Skybox: public IPrimitive {
    struct Vertex{
        glm::vec2 position;
    };

public:
    Skybox(std::shared_ptr<IScene> &_parent, const std::string &_name);

public:
    void render() override;

private:
    std::shared_ptr<VertexArray> m_vao;
    std::shared_ptr<Buffer<Vertex>> m_vbo;
    std::shared_ptr<Texture> m_texture;

    std::vector<Vertex> m_vertices = {
        // positions
        Vertex{ glm::vec3 { -1.0f,  1.0f, -1.0f } },
        Vertex{ glm::vec3 { -1.0f, -1.0f, -1.0f } },
        Vertex{ glm::vec3 { 1.0f, -1.0f, -1.0f } },
        Vertex{ glm::vec3 { 1.0f, -1.0f, -1.0f } },
        Vertex{ glm::vec3 { 1.0f,  1.0f, -1.0f } },
        Vertex{ glm::vec3 { -1.0f,  1.0f, -1.0f } },

        Vertex{ glm::vec3 { -1.0f, -1.0f,  1.0f } },
        Vertex{ glm::vec3 { -1.0f, -1.0f, -1.0f } },
        Vertex{ glm::vec3 { -1.0f,  1.0f, -1.0f } },
        Vertex{ glm::vec3 { -1.0f,  1.0f, -1.0f } },
        Vertex{ glm::vec3 { -1.0f,  1.0f,  1.0f } },
        Vertex{ glm::vec3 { -1.0f, -1.0f,  1.0f } },

        Vertex{ glm::vec3 { 1.0f, -1.0f, -1.0f } },
        Vertex{ glm::vec3 { 1.0f, -1.0f,  1.0f } },
        Vertex{ glm::vec3 { 1.0f,  1.0f,  1.0f } },
        Vertex{ glm::vec3 { 1.0f,  1.0f,  1.0f } },
        Vertex{ glm::vec3 { 1.0f,  1.0f, -1.0f } },
        Vertex{ glm::vec3 { 1.0f, -1.0f, -1.0f } },

        Vertex{ glm::vec3 { -1.0f, -1.0f,  1.0f } },
        Vertex{ glm::vec3 { -1.0f,  1.0f,  1.0f } },
        Vertex{ glm::vec3 { 1.0f,  1.0f,  1.0f } },
        Vertex{ glm::vec3 { 1.0f,  1.0f,  1.0f } },
        Vertex{ glm::vec3 { 1.0f, -1.0f,  1.0f } },
        Vertex{ glm::vec3 { -1.0f, -1.0f,  1.0f } },

        Vertex{ glm::vec3 { -1.0f,  1.0f, -1.0f } },
        Vertex{ glm::vec3 { 1.0f,  1.0f, -1.0f } },
        Vertex{ glm::vec3 { 1.0f,  1.0f,  1.0f } },
        Vertex{ glm::vec3 { 1.0f,  1.0f,  1.0f } },
        Vertex{ glm::vec3 { -1.0f,  1.0f,  1.0f } },
        Vertex{ glm::vec3 { -1.0f,  1.0f, -1.0f } },

        Vertex{ glm::vec3 { -1.0f, -1.0f, -1.0f } },
        Vertex{ glm::vec3 { -1.0f, -1.0f,  1.0f } },
        Vertex{ glm::vec3 { 1.0f, -1.0f, -1.0f } },
        Vertex{ glm::vec3 { 1.0f, -1.0f, -1.0f } },
        Vertex{ glm::vec3 { -1.0f, -1.0f,  1.0f } },
        Vertex{ glm::vec3 { 1.0f, -1.0f,  1.0f } },
    };
};


#endif //OPENGL_PRIMARY_SKYBOX_H
