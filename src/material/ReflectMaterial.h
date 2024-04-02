/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/1 21:31
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_REFLECTMATERIAL_H
#define OPENGL_PRIMARY_REFLECTMATERIAL_H

#include <memory>
#include "../base/IMaterial.h"
#include "../base/Texture.h"

class ReflectMaterial: public IMaterial {
public:
    explicit ReflectMaterial(std::shared_ptr<Texture> _skyboxTexture);
    std::weak_ptr<Texture> getSkyboxTexture() { return m_skyboxTexture; }

private:
    std::weak_ptr<Texture> m_skyboxTexture;
};

#endif //OPENGL_PRIMARY_REFLECTMATERIAL_H
