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
#include "../base/ImageTexture.h"

class ReflectMaterial: public IMaterial {
public:
    explicit ReflectMaterial(std::shared_ptr<ImageTexture> _skyboxTexture);
    std::weak_ptr<ImageTexture> getSkyboxTexture() { return m_skyboxTexture; }

private:
    std::weak_ptr<ImageTexture> m_skyboxTexture;
};

#endif //OPENGL_PRIMARY_REFLECTMATERIAL_H
