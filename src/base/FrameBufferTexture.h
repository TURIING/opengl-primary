/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/16 15:21
* @version: 1.0
* @description: 生成用于帧缓冲的纹理
********************************************************************************/

#ifndef OPENGL_PRIMARY_FRAMEBUFFERTEXTURE_H
#define OPENGL_PRIMARY_FRAMEBUFFERTEXTURE_H

#include "ITexture.h"
struct Size;

class FrameBufferTexture: public ITexture {
public:
    enum INTERNAL_FORMAT { RGB, DEPTH_COMPONENT };              // 内部格式

public:
    FrameBufferTexture(Size _scrSize, unsigned int _unit, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode, INTERNAL_FORMAT _format);
    FrameBufferTexture(Size _scrSize, unsigned int _unit, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode);

protected:
    void bind() override;

private:
    static void setWrapAndFilter(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode);
    static void setWrapAndFilterForCubeMap(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode);

private:
    TARGET_TYPE m_textureTarget = TARGET_TYPE::TEXTURE2D;
};

#endif //OPENGL_PRIMARY_FRAMEBUFFERTEXTURE_H
