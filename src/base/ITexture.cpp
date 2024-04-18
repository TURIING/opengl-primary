/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/16 15:40
* @version: 1.0
* @description: 
********************************************************************************/

#include "ITexture.h"

ITexture::ITexture(unsigned int _unit): m_textureUnit(_unit) {
    glGenTextures(1, &m_id);
}

ITexture::~ITexture() {
    LOG_ASSERT(m_id != -1);
    glDeleteTextures(1, &m_id);
}

void ITexture::activate() {
    LOG_ASSERT(m_textureUnit != -1);

    glActiveTexture(GL_TEXTURE0 + m_textureUnit);
    this->bind();
}