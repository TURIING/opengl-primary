/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/20 21:57
* @version: 1.0
* @description: 面板抽象类
********************************************************************************/

#ifndef OPENGL_PRIMARY_IPANEL_H
#define OPENGL_PRIMARY_IPANEL_H

#include "../BaseDefine.h"

class Application;
class IPanel {
    virtual void render() = 0;
    virtual void dispatch(Event _event, EventParam &_param) = 0;
};

#endif //OPENGL_PRIMARY_IPANEL_H
