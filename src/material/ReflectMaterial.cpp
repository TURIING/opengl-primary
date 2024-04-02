/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/1 21:31
* @version: 1.0
* @description: 
********************************************************************************/

#include "ReflectMaterial.h"
#include "glog/logging.h"

ReflectMaterial::ReflectMaterial(std::shared_ptr<Texture> _skyboxTexture) {
    LOG_ASSERT(_skyboxTexture);
    this->setMaterialType(MaterialType::Reflect);
    m_skyboxTexture = _skyboxTexture;
}
