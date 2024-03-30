/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/12 14:52
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_UICONTEXT_H
#define OPENGL_PRIMARY_UICONTEXT_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "IContext.h"

class UIContext: public IContext {
public:
    UIContext();
    ~UIContext();

    void preRender() override;

    void postRender() override;

private:
    void paintMenuBar();
};


#endif //OPENGL_PRIMARY_UICONTEXT_H
