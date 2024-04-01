/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/31 23:31
* @version: 1.0
* @description: 天空盒材质类
********************************************************************************/

#ifndef OPENGL_PRIMARY_SKYBOXMATERIAL_H
#define OPENGL_PRIMARY_SKYBOXMATERIAL_H

#include <vector>
#include <string>
#include <memory>
#include "../base/IMaterial.h"
#include "../base/ShaderProgram.h"
#include "../base/Texture.h"

class SkyboxMaterial: public IMaterial {
public:
    SkyboxMaterial(std::shared_ptr<ShaderProgram> _shaderProgram, const std::vector<std::string> &_pathVec);

    std::shared_ptr<Texture> getTexture() { return m_texture; }
private:
    std::shared_ptr<Texture> m_texture;
};




#endif //OPENGL_PRIMARY_SKYBOXMATERIAL_H
