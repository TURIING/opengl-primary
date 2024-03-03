/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/3 18:30
* @version: 1.0
* @description: 
********************************************************************************/
#include "IPrimitive.h"

IPrimitive::IPrimitive(IScene *_parent) {
    _parent->addRender(this);
}

/**
 * 开启轮廓绘制并设定轮廓参数
 * @param _width
 * @param _color
 */
void IPrimitive::setOutline(int _width, Color _color) {
    m_enableOutline = true;
    m_outlineColor = _color;
    m_outlineWidth = _width;
}

// 平移
void IPrimitive::translate(float _x, float _y, float _z) {
    m_translatePos = { _x, _y, _z };
}


