/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/16 15:14
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_ITEXTURE_H
#define OPENGL_PRIMARY_ITEXTURE_H

#include "glad/glad.h"
#include "glog/logging.h"

class ITexture {
public:
    // 纹理类型
    enum TARGET_TYPE { TEXTURE2D, CUBE_MAP };

public:
    explicit ITexture(unsigned int _unit);
    ~ITexture();
    [[nodiscard]] unsigned int getTextureID() const { return m_id; }
    [[nodiscard]] unsigned int getTextureUnit() const { return m_textureUnit; }
    void activate();

protected:
    virtual void bind() {}

private:
    unsigned int m_id = -1;
    unsigned int m_textureUnit = -1;
};

#endif //OPENGL_PRIMARY_ITEXTURE_H
