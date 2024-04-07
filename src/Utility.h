/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/30 11:45
* @version: 1.0
* @description: 存放工具函数
********************************************************************************/

#ifndef OPENGL_PRIMARY_UTILITY_H
#define OPENGL_PRIMARY_UTILITY_H

#include "BaseDefine.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace Utility {

    const char* transformPrimitiveTypeToStr(PrimitiveType _type);
    const char* transformLightTypeToStr(LightType _type);
    const char* transformMaterialTypeToStr(MaterialType _type);
    aiTextureType transformTextureTypeToAssimp(TextureType _type);
}
#endif //OPENGL_PRIMARY_UTILITY_H
