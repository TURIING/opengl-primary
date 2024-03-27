/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 11:00
* @version: 1.0
* @description: 图元的接口类
********************************************************************************/

#ifndef OPENGL_PRIMARY_IPRIMITIVE_H
#define OPENGL_PRIMARY_IPRIMITIVE_H

#include "../base/Texture.h"
#include "../base/ShaderProgram.h"
#include "IRenderer.h"
class IScene;

class IPrimitive: public IRenderer{
public:
    explicit IPrimitive(IScene *_parent, std::string &_name);
    IPrimitive(IScene *_parent, std::string &_name, std::shared_ptr<ShaderProgram> &_shaderProgram);

    // 摄像机
    virtual void setCamera(std::shared_ptr<Camera> &_camera);
    std::shared_ptr<Camera> getCamera();

    // 缩放
    void scale(float _x, float _y, float _z) { m_scaling = { _x, _y, _z }; }
    glm::vec3 *getScaling() { return &m_scaling; }
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

    void addTexture(std::shared_ptr<Texture> _texture);
    std::shared_ptr<Texture> getTexture(int _index);

    std::shared_ptr<ShaderProgram> getShaderProgram();

public:
    void setShaderProgram(std::shared_ptr<ShaderProgram> &_shaderProgram);

    PrimitiveType getPrimitiveType();

protected:
    void setPrimitiveType(PrimitiveType _type) { m_primitiveType = _type;}

private:
    glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };                                            // 平移后的位置
    glm::vec3 m_scaling = { 1.0f , 1.0f, 1.0f };
    glm::vec3 m_rotation = { 0.0f , 0.0f, 0.0f };

    bool m_enableOutline = false;                                                                   // 是否绘制轮廓
    Color m_outlineColor = { 0, 0, 0, 1};                                                           // 轮廓颜色
    int m_outlineWidth = 0;                                                                         // 轮廓宽度

    std::vector<std::shared_ptr<Texture>> m_textures;                                               // 纹理集合
    std::shared_ptr<ShaderProgram> m_shaderProgram;

    std::shared_ptr<Camera> m_camera;

    PrimitiveType m_primitiveType = PrimitiveType::None;
};

#endif //OPENGL_PRIMARY_IPRIMITIVE_H
