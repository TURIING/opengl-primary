/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 11:00
* @version: 1.0
* @description: 图元的接口类
********************************************************************************/

#ifndef OPENGL_PRIMARY_IPRIMITIVE_H
#define OPENGL_PRIMARY_IPRIMITIVE_H

#include "../base/ImageTexture.h"
#include "../base/ShaderProgram.h"
#include "IRenderer.h"
class IScene;
class IMaterial;

class IPrimitive: public IRenderer{
public:
    explicit IPrimitive(std::shared_ptr<IScene> &_parent, const std::string &_name, std::shared_ptr<ShaderProgram> _shaderProgram = nullptr);

    // 摄像机
    void setCamera(std::shared_ptr<Camera> &_camera);
    std::shared_ptr<Camera> getCamera();

    // 缩放
    void scale(float _scaling) { m_scaling = _scaling; }
    float *getScaling() { return &m_scaling; }

    // 平移
    void translate(float _x, float _y, float _z) { m_position = { _x, _y, _z }; }
    glm::vec3 *getPosition() { return &m_position; }

    // 旋转
    void rotate(float _x, float _y, float _z) { m_rotation = { _x, _y, _z }; }
    glm::vec3 *getRotation() { return &m_rotation; }

    // 轮廓
    void setOutline(int _width, Color _color);
    bool isEnabledOutline() const { return m_enableOutline; }
    Color getOutlineColor() { return m_outlineColor; }
    int getOutlineWidth() const { return m_outlineWidth; }

    // 纹理
    void addTexture(std::shared_ptr<ImageTexture> _texture);
    void resetTexture(unsigned int _index, ImageTexture* _texture);
    std::shared_ptr<ImageTexture> getTexture(int _index);
    [[nodiscard]] std::vector<std::shared_ptr<ImageTexture>> getTextureList() { return m_textures; }

    // 材质
    void setMaterial(std::shared_ptr<IMaterial> _material);
    std::shared_ptr<IMaterial> getMaterial() { return m_material; }

    // 光源
    PointLight *getPointLight();
    DirectionalLight *getDirectionalLight();
    void setLightType(LightType _type) { m_lightType = _type; }
    LightType getLightType() { return m_lightType; }
    std::shared_ptr<ShaderProgram> getShaderProgram();

    void setPrimitiveType(PrimitiveType _type) { m_primitiveType = _type;}

    void setShaderProgram(std::shared_ptr<ShaderProgram> &_shaderProgram);
    PrimitiveType getPrimitiveType();
    void preRender() override;

private:
    void transmitMaterialToShader();

private:
    glm::vec3 m_position = { -1.0f, -1.0f, -1.0f };                                         // 平移后的位置
    float m_scaling = 1.0f;
    glm::vec3 m_rotation = { 0.0f , 0.0f, 0.0f };

    bool m_enableOutline = false;                                                                   // 是否绘制轮廓
    Color m_outlineColor = { 0, 0, 0, 1};                                                           // 轮廓颜色
    int m_outlineWidth = 0;                                                                         // 轮廓宽度

    std::vector<std::shared_ptr<ImageTexture>> m_textures;
    std::shared_ptr<ShaderProgram> m_shaderProgram;

    std::shared_ptr<Camera> m_camera;

    PrimitiveType m_primitiveType = PrimitiveType::None;

    std::shared_ptr<IMaterial> m_material;

    LightType m_lightType = LightType::None;                                                        // 光源类型
    PointLight m_pointLight = {
        .position =  glm::vec3(0.0f, 0.0f, 0.0f),
        .constant = 1.0f,
        .ambient = glm::vec3(0.2f, 0.2f, 0.2f),
        .linear = 0.09f,
        .diffuse = glm::vec3(0.5f, 0.5f, 0.5f),
        .quadratic = 0.032f,
        .specular = glm::vec3(1.0f, 1.0f, 1.0f)
    };

    DirectionalLight m_directionalLight {
        .direction = glm::vec3(0.0f, 0.0f, 0.0f),
        .ambient = glm::vec3(0.05f, 0.05f, 0.05f),
        .diffuse = glm::vec3(0.4f, 0.4f, 0.4f),
        .specular = glm::vec3(0.5f, 0.5f, 0.5f)
    };
};

#endif //OPENGL_PRIMARY_IPRIMITIVE_H
