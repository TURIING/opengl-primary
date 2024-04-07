/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 10:57
* @version: 1.0
* @description: 所有渲染物体及渲染场景的接口类
********************************************************************************/

#ifndef OPENGL_PRIMARY_IRENDERER_H
#define OPENGL_PRIMARY_IRENDERER_H

#include <tuple>
#include "glad/glad.h"
#include "../BaseDefine.h"
#include "../base/Camera.h"
#include "../base/FrameBuffer.h"
#include <memory>
#include "glog/logging.h"

class IRenderer {
public:
    IRenderer();

    [[nodiscard]] bool getDeepTest() const { return m_enableDeepTest; }
    void setDeepTest(bool _on = true);

    [[nodiscard]] bool getStencilTest() const { return m_enableStencilTest; }
    void setStencilTest(bool _on = true);

    virtual void render() = 0;

    virtual void setWindowSize(std::tuple<int, int> &_windowSize);
    int getWindowWidth() { return std::get<0>(m_windowSize); }
    int getWindowHeight() { return std::get<1>(m_windowSize); }

    RenderType getRenderType();
    std::string getRenderName() { return m_renderName; }
    [[nodiscard]] int getRenderID() const { return m_renderID; }

    void setRenderName(const std::string &_name);

protected:
    virtual void preRender();
    virtual void postRender();
    void setRenderType(RenderType _type);

private:
    std::tuple<int, int> m_windowSize = { 1600, 1200 };                                                              // 窗口尺寸
    RenderType m_renderType = RenderType::None;
    std::string m_renderName;
    int m_renderID = -1;

    bool m_enableDeepTest = false;												                    // 是否启用深度测试
    bool m_enableStencilTest = false;												                // 是否启用模板测试

    static int UUID;
};

#endif //OPENGL_PRIMARY_IRENDERER_H
