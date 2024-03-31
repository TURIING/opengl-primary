/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/30 11:45
* @version: 1.0
* @description: 
********************************************************************************/

#include "Utility.h"
#include "glog/logging.h"

const char *Utility::transformPrimitiveTypeToStr(PrimitiveType _type) {
    switch (_type) {
        case PrimitiveType::None:                       return "None";
        case PrimitiveType::Cube:                       return "Cube";
        case PrimitiveType::Floor:                      return "Floor";
        case PrimitiveType::Square:                     return "Square";
        case PrimitiveType::Skybox:                     return "Skybox";
        default:                                        LOG(FATAL) << " Undefined conditional branch.";
    }
}

const char *Utility::transformLightTypeToStr(LightType _type) {
    switch (_type) {
        case LightType::None:                           return "None";
        case LightType::PhongLight:                     return "PhongLight";
        case LightType::PointLight:                     return "PointLight";
        case LightType::DirectionalLight:               return "DirectionalLight";
        case LightType::BlinnPhong:                     return "BlinnPhong";
        default:                                        LOG(FATAL) << " Undefined conditional branch.";
    }
}
