/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 18:13
* @version: 1.0
* @description: 场景接口类
********************************************************************************/

#ifndef OPENGL_PRIMARY_ISCENE_H
#define OPENGL_PRIMARY_ISCENE_H

#include <map>

#include "imgui/imgui.h"
#include "../BaseDefine.h"
#include "../base/RenderBuffer.h"
#include "IRenderer.h"
#include "../primitive/Quadrangle.h"

class IScene: public IRenderer{
    struct VPMatrices {
        glm::mat4 view;
        glm::mat4 projection;
    };

public:
    IScene();

    // 清屏颜色
    void clear();
    void setClearColor(float _r, float _g, float _b, float _a) { m_clearColor = {_r, _g, _b, _a}; }

    void addPrimitive(IPrimitive *_render);
    IPrimitive *getPrimitiveByID(unsigned int _id) { return m_primitiveList.find(_id)->second; }
    std::map<int, IPrimitive *> getAllPrimitive() { return m_primitiveList; }

    void setWindowSize(std::tuple<int, int> &_windowSize) final;

    void dispatch(Event _event, EventParam _param);

    void preRender() override;
    void postRender() override;

    std::shared_ptr<Camera>& getCamera();

protected:
    virtual void onWindowResize(Size &_size);
    virtual void onKeyPress(KEYBOARD _key);

private:
    void offScreenRender(Size &_size);

private:
    std::map<int, IPrimitive *> m_primitiveList;                                                    // 场景中的图元集合
    Color m_clearColor = { 0.26f, 0.30f, 0.31f, 1.0f };					                            // 清屏颜色

    std::shared_ptr<Camera> m_camera;
private:
    std::unique_ptr<FrameBuffer> m_fbo;
    std::shared_ptr<RenderBuffer> m_rbo;
    std::shared_ptr<Texture> m_fboColorTexture;
    std::shared_ptr<Texture> m_fboDepthTexture;
    std::shared_ptr<Texture> m_screenTexture;
    std::unique_ptr<Buffer<VPMatrices>> m_ubo;
};

#endif //OPENGL_PRIMARY_ISCENE_H
