/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/11 14:20
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_FRAMEBUFFERDEMO_H
#define OPENGL_PRIMARY_FRAMEBUFFERDEMO_H

#include "glad/glad.h"
#include "glog/logging.h"
#include "../../base/IScene.h"
#include "../../primitive/Floor.h"
#include "../../primitive/Cube.h"
#include "imgui/imgui.h"
#include "../../BaseDefine.h"
#include "../../primitive/Quadrangle.h"
#include "../../base/FrameBuffer.h"
#include "../../base/RenderBuffer.h"
#include "../../base/Texture.h"

class FrameBufferDemo: public IScene {
public:
    FrameBufferDemo();

private:
    void paint() override;

private:
    const std::string OBJECT_VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/cube/vertex.glsl";
    const std::string OBJECT_FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/cube/fragment.glsl";

    const std::string SCREEN_VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/frame_buffer/vertex.glsl";
    const std::string SCREEN_FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/frame_buffer/fragment.glsl";

    std::unique_ptr<Floor> m_floor;
    std::unique_ptr<Cube> m_cube1;
    std::unique_ptr<Cube> m_cube2;
    std::unique_ptr<Quadrangle> m_quad;

    std::shared_ptr<ShaderProgram> m_objectShaderProgram;
    std::shared_ptr<ShaderProgram> m_screenShaderProgram;

    std::unique_ptr<FrameBuffer> m_fbo;
    std::unique_ptr<RenderBuffer> m_rbo;

    std::shared_ptr<Texture> m_screenTexture;
};

#endif //OPENGL_PRIMARY_FRAMEBUFFERDEMO_H


