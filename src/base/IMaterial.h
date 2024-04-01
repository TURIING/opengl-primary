/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/31 21:55
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_IMATERIAL_H
#define OPENGL_PRIMARY_IMATERIAL_H

#include "../BaseDefine.h"

class IMaterial {
public:
    MaterialType getMaterialType() { return m_materialType; }

protected:
    virtual void setMaterialType(MaterialType _type) { m_materialType = _type; }        // note: 此virtual待移除

private:
    MaterialType m_materialType = MaterialType::None;
};


#endif //OPENGL_PRIMARY_IMATERIAL_H
