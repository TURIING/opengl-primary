/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/12 14:46
* @version: 1.0
* @description: 上下文基类
********************************************************************************/

#ifndef OPENGL_PRIMARY_ICONTEXT_H
#define OPENGL_PRIMARY_ICONTEXT_H

class IContext {
public:
    // 渲染前处理
    virtual void preRender() = 0;
    // 渲染后处理
    virtual void postRender() = 0;
};

#endif //OPENGL_PRIMARY_ICONTEXT_H
