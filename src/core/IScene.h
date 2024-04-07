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

#define MAX_LIGHT_NUM 10

class IScene: public IRenderer{
    struct VPMatricesBlock {
        glm::mat4 view;
        glm::mat4 projection;
    };

    struct LightBlock {
        PointLight pointLight[MAX_LIGHT_NUM];
        DirectionalLight directionalLight[MAX_LIGHT_NUM];
        int actualPointLightNum = 0;
        int actualDirectionalLightNum = 0;
    };

    struct CameraInfoBlock {
        glm::vec3 cameraPos;
        int :8;
    };

public:
    IScene();

    // 清屏颜色
    void clear();
    void setClearColor(float _r, float _g, float _b, float _a) { m_clearColor = {_r, _g, _b, _a}; }

    void addPrimitive(std::shared_ptr<IPrimitive> &_render);
    void deletePrimitive(int _id);
    std::shared_ptr<IPrimitive> getPrimitiveByID(unsigned int _id) { return m_primitiveList.find(_id)->second; }
    std::map<int, std::shared_ptr<IPrimitive>> getAllPrimitive() { return m_primitiveList; }

    void render() override;

    void setWindowSize(std::tuple<int, int> &_windowSize) final;

    void dispatch(Event _event, EventParam _param);

    void preRender() override;
    void postRender() override;

    std::shared_ptr<Camera>& getCamera();
    std::shared_ptr<ShaderProgram> getShaderProgram() { return m_shaderProgram; }

    std::shared_ptr<IPrimitive> getSkyboxPrimitive() { return m_skyboxPrimitive; }

    void setShadow(bool _on) { m_isShadow = _on; }

protected:
    virtual void onWindowResize(Size &_size);
    virtual void onKeyPress(KEYBOARD _key);

private:
    void offScreenRender(Size &_size);

private:
    std::map<int, std::shared_ptr<IPrimitive>> m_primitiveList;                                     // 场景中的图元集合
    Color m_clearColor = { 0.2, 0.2, 0.2, 1 };					                                    // 清屏颜色
    std::shared_ptr<Camera> m_camera;
    std::unique_ptr<FrameBuffer> m_fbo;
    std::shared_ptr<RenderBuffer> m_rbo;
    std::shared_ptr<Texture> m_fboColorTexture;
    std::shared_ptr<Texture> m_fboDepthTexture;
    std::shared_ptr<Texture> m_screenTexture;
    std::unique_ptr<Buffer<VPMatricesBlock>> m_vpMatricesUbo;
    std::unique_ptr<Buffer<LightBlock>> m_lightUbo;
    std::unique_ptr<Buffer<CameraInfoBlock>> m_cameraUbo;
    std::shared_ptr<ShaderProgram> m_shaderProgram;
    std::shared_ptr<IPrimitive> m_skyboxPrimitive;                                                  // 需要记录下来天空盒的图元
    bool m_isShadow = false;
};

#endif //OPENGL_PRIMARY_ISCENE_H
