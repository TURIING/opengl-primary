/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 18:13
* @version: 1.0
* @description: 场景接口类
********************************************************************************/

#ifndef OPENGL_PRIMARY_ISCENE_H
#define OPENGL_PRIMARY_ISCENE_H

#include "IRenderer.h"

class IScene: public IRenderer{
public:
    void enableDeepTest();
    void enableStencilTest();

    // 清屏颜色
    void clear();
    void setClearColor(float _r, float _g, float _b, float _a) { m_clearColor = {_r, _g, _b, _a}; }

    void addRender(IRenderer *_render) { m_renderList.push_back(_render); }

    void setWindowSize(std::tuple<int, int> &_windowSize) override;
    void setCamera(std::shared_ptr<Camera> &_camera) override;

private:
    std::vector<IRenderer *> m_renderList;                                                         // 场景中的物体集合
    Color m_clearColor = { 0.26f, 0.30f, 0.31f, 1.0f };					                            // 清屏颜色

    bool m_enableDeepTest = false;												                    // 是否启用深度测试
    bool m_enableStencilTest = false;												                // 是否启用模板测试
};

#endif //OPENGL_PRIMARY_ISCENE_H
