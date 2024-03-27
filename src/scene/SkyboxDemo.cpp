/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 19:28
* @version: 1.0
* @description:
********************************************************************************/

#include "SkyboxDemo.h"

const std::string VERTEX_FILE = std::string(SHADER_CODE_PATH) + "/cube/vertex.glsl";
const std::string FRAGMENT_FILE = std::string(SHADER_CODE_PATH) + "/cube/fragment.glsl";

SkyboxDemo::SkyboxDemo() {
    this->setClearColor(0.2, 0.2, 0.2, 1);
    this->setDeepTest(true);

    //m_shaderProgram = std::make_shared<ShaderProgram>(VERTEX_FILE, FRAGMENT_FILE);

    //m_cube1 = std::make_unique<Cube>(this, "cube_1", m_shaderProgram);
    //m_cube2 = std::make_unique<Cube>(this, "cube_2", m_shaderProgram);
    m_skybox = std::make_unique<Skybox>(this, "skybox");
}

void SkyboxDemo::render() {
    //m_cube1->render();
    //m_cube2->render();
    m_skybox->render();
}
