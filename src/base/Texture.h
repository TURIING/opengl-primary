/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 20:38
* @version: 1.0
* @description: 纹理类
********************************************************************************/

#ifndef OPENGL_PRIMARY_TEXTURE_H
#define OPENGL_PRIMARY_TEXTURE_H

#include <tuple>

#include "glad/glad.h"
#include "glog/logging.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../BaseDefine.h"

class Texture {
    // 纹理类型
    enum class TARGET_TYPE { TEXTURE2D, CUBE_MAP};

public:
    enum STORAGE_TYPE { IMAGE2D, STORAGE2D };           // 纹理存储的类型

    // 用于构造生成来自图像文件的纹理
    Texture(std::string _path, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode) {
        LOG_ASSERT(!_path.empty());

        glGenTextures(1, &m_id);

        this->bind();

        this->setWrapAndFilter(_wrapModeS, _wrapModeT, _minFilterMode, _magFilterMode);

        this->generateTexture(_path);
    }

    // 用于生成立方体贴图的纹理
    Texture(const std::vector<std::string> &_pathVec, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode) {
        LOG_ASSERT(!_pathVec.empty());

        glGenTextures(1, &m_id);
        m_textureTarget = TARGET_TYPE::CUBE_MAP;

        this->bind();
        this->generateTextureForCubeMap(_pathVec);
        this->setWrapAndFilterForCubeMap(_wrapModeS, _wrapModeT, _wrapModeR, _minFilterMode, _magFilterMode);
    }

    /**
     * 用于构造生成frame buffer的纹理
     * @param _scrWidth 屏幕宽度
     * @param _scrHeight 屏幕高度
     * @param _type 纹理存储的类型
     */
    Texture(Size &_scrSize, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode, STORAGE_TYPE _type) {
        const auto [width, height] = _scrSize;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        this->bind();

        this->setWrapAndFilter(_wrapModeS, _wrapModeT, _minFilterMode, _magFilterMode);
        switch (_type) {
            case STORAGE_TYPE::IMAGE2D:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                break;
            case STORAGE_TYPE::STORAGE2D:
                glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
                break;
        }
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

        switch (m_textureTarget) {
            case TARGET_TYPE::TEXTURE2D:    glBindTexture(GL_TEXTURE_2D, m_id);             break;
            case TARGET_TYPE::CUBE_MAP:     glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);       break;
            default:                        LOG(FATAL) << " Undefined conditional branch.";
        }
    }

    // 设置环绕方式、过滤方式
    void setWrapAndFilter(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapModeT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilterMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilterMode);
    }

    // 设置立方体贴图的环绕方式、过滤方式
    void setWrapAndFilterForCubeMap(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, _wrapModeS);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, _wrapModeT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, _wrapModeR);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, _minFilterMode);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, _magFilterMode);
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

    /**
     * 生成立方体贴图纹理
     * @param _resPath 图片资源路径
     * @param _target 纹理目标
     */
    static void generateTextureForCubeMap(const std::vector<std::string> &_pathVec) {
        LOG_ASSERT(!_pathVec.empty());

        // 加载纹理图片
        int width, height, nrChannels;
        //stbi_set_flip_vertically_on_load(true);
        GLenum format;

        for(auto i = 0; i < _pathVec.size(); i++) {
            unsigned char *data = stbi_load(_pathVec[i].c_str(), &width, &height, &nrChannels, 0);
            LOG_IF(FATAL, !data) << "Image(" << _pathVec[i] << ") loading failed.";

            switch (nrChannels) {
                case 1:     format = GL_RED;    break;
                case 2:     format = GL_RG;     break;
                case 3:     format = GL_RGB;    break;
                case 4:     format = GL_RGBA;   break;
                default:    LOG(FATAL) << "Wrong number of image channels.";
            }

            // 生成纹理
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            // 释放图像
            stbi_image_free(data);
        }
    }

private:
    unsigned int m_id = -1;
    TARGET_TYPE m_textureTarget = TARGET_TYPE::TEXTURE2D;
};

#endif //OPENGL_PRIMARY_TEXTURE_H
