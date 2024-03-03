/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 11:00
* @version: 1.0
* @description: 图元的接口类
********************************************************************************/

#ifndef OPENGL_PRIMARY_IPRIMITIVE_H
#define OPENGL_PRIMARY_IPRIMITIVE_H

#include "../base/IScene.h"

class IPrimitive: public IRenderer{
public:
    explicit IPrimitive(IScene *_parent);
    IPrimitive() = delete;

    // 缩放
    virtual void scale() {};
    // 平移
    void translate(float _x, float _y, float _z);
    glm::vec3 getTranslatePos() { return m_translatePos; }

    // 轮廓
    void setOutline(int _width, Color _color);
    bool isEnabledOutline() const { return m_enableOutline; }
    Color getOutlineColor() { return m_outlineColor; }
    int getOutlineWidth() const { return m_outlineWidth; }

private:
    glm::vec3 m_translatePos = { 1.0f, 1.0f, 1.0f };                                        // 平移后的位置

    bool m_enableOutline = false;                                                                   // 是否绘制轮廓
    Color m_outlineColor = { 0, 0, 0, 1};                                                           // 轮廓颜色
    int m_outlineWidth = 0;                                                                         // 轮廓宽度
};

#endif //OPENGL_PRIMARY_IPRIMITIVE_H
