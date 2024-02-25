/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 11:00
* @version: 1.0
* @description: 图元的接口类
********************************************************************************/

#ifndef OPENGL_PRIMARY_IPRIMITIVE_H
#define OPENGL_PRIMARY_IPRIMITIVE_H

#include "../base/IRenderer.h"

class IPrimitive: public IRenderer{
public:
    virtual void scale() {};
};

#endif //OPENGL_PRIMARY_IPRIMITIVE_H
