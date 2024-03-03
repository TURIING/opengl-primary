/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 20:38
* @version: 1.0
* @description: 纹理类
********************************************************************************/

#ifndef OPENGL_PRIMARY_TEXTURE_H
#define OPENGL_PRIMARY_TEXTURE_H

#include "glad/glad.h"
#include "glog/logging.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture {
public:
    Texture(std::string _path, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode) {
        LOG_ASSERT(!_path.empty());

        glGenTextures(1, &m_id);

        this->bind();

        this->setWrapAndFilter(_wrapModeS, _wrapModeT, _minFilterMode, _magFilterMode);

        this->generateTexture(_path);
    }

    ~Texture() {
        LOG_ASSERT(m_id != -1);
        glDeleteTextures(1, &m_id);
    }

    unsigned int getID() { return m_id; }

    void activate(unsigned int _unit) {
        glActiveTexture(_unit);
        this->bind();
    }
private:
    void bind() {
        LOG_ASSERT(m_id != -1);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    // 设置环绕方式、过滤方式
    void setWrapAndFilter(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapModeT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilterMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilterMode);
    }

    /**
     * 生成纹理
     * @param _resPath 图片资源路径
     * @param _target 纹理目标
     */
    static void generateTexture(const std::string &_resPath) {
        LOG_ASSERT(!_resPath.empty());

        // 加载纹理图片
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        GLenum format;

        unsigned char *data = stbi_load(_resPath.c_str(), &width, &height, &nrChannels, 0);
        LOG_IF(FATAL, !data) << "Image(" << _resPath << ") loading failed.";

        switch (nrChannels) {
            case 1:     format = GL_RED;    break;
            case 2:     format = GL_RG;     break;
            case 3:     format = GL_RGB;    break;
            case 4:     format = GL_RGBA;   break;
            default:    LOG(FATAL) << "Wrong number of image channels.";
        }

        // 生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // 生成多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);

        // 释放图像
        stbi_image_free(data);
    }
private:
    unsigned int m_id = -1;

};

#endif //OPENGL_PRIMARY_TEXTURE_H
