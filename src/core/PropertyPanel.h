/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/12 16:05
* @version: 1.0
* @description: imgui控制面板
********************************************************************************/

#ifndef OPENGL_PRIMARY_PROPERTYPANEL_H
#define OPENGL_PRIMARY_PROPERTYPANEL_H

#include <vector>
#include <map>
#include <string>
#include "imgui/imgui.h"
#include "IPanel.h"

class IRenderer;

class PropertyPanel: public IPanel {
public:
    PropertyPanel();
    void render() override;

private:
    void dispatch(Event _event, EventParam _param) override;
};
#endif //OPENGL_PRIMARY_PROPERTYPANEL_H
