/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/16 15:21
* @version: 1.0
* @description: 
********************************************************************************/

#include "FrameBufferTexture.h"
#include "../BaseDefine.h"

// 用于构造生成frame buffer的图片纹理
FrameBufferTexture::FrameBufferTexture(Size _scrSize, unsigned int _unit, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode, INTERNAL_FORMAT _format): ITexture(_unit) {
    const auto [width, height] = _scrSize;

    glBindTexture(GL_TEXTURE_2D, this->getTextureID());

    FrameBufferTexture::setWrapAndFilter(_wrapModeS, _wrapModeT, _minFilterMode, _magFilterMode);
    switch (_format) {
        case INTERNAL_FORMAT::RGB: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            break;
        }
        case INTERNAL_FORMAT::DEPTH_COMPONENT: {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            break;
        }
    }
}

// 用于构造生成frame buffer的立方体贴图纹理
FrameBufferTexture::FrameBufferTexture(Size _scrSize, unsigned int _unit, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode): ITexture(_unit)  {
    const auto [width, height] = _scrSize;
    m_textureTarget = TARGET_TYPE::CUBE_MAP;

    glBindTexture(GL_TEXTURE_CUBE_MAP, this->getTextureID());

    for(auto i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    FrameBufferTexture::setWrapAndFilterForCubeMap(_wrapModeS, _wrapModeT, _wrapModeR, _minFilterMode, _magFilterMode);
}

void FrameBufferTexture::bind() {
    LOG_ASSERT(this->getTextureID() != -1);

    switch (m_textureTarget) {
        case TARGET_TYPE::TEXTURE2D: glBindTexture(GL_TEXTURE_2D, this->getTextureID());        break;
        case TARGET_TYPE::CUBE_MAP:  glBindTexture(GL_TEXTURE_CUBE_MAP, this->getTextureID());  break;
        default:                     LOG(FATAL) << " Undefined conditional branch.";
    }
}

// 设置环绕方式、过滤方式
void FrameBufferTexture::setWrapAndFilter(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapModeT);

    if(_wrapModeS == GL_CLAMP_TO_BORDER || _wrapModeT == GL_CLAMP_TO_BORDER) {
        GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilterMode);
}

// 设置立方体贴图的环绕方式、过滤方式
void FrameBufferTexture::setWrapAndFilterForCubeMap(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode) {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, _wrapModeS);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, _wrapModeT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, _wrapModeR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, _minFilterMode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, _magFilterMode);
}
