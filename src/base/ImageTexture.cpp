/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/29 15:05
* @version: 1.0
* @description: 
********************************************************************************/

#include "ImageTexture.h"

// 用于构造生成来自图像文件的纹理
ImageTexture::ImageTexture(std::string _path, unsigned int _unit, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode): ITexture(_unit), m_path(_path) {
    LOG_ASSERT(!_path.empty());

    glBindTexture(GL_TEXTURE_2D, this->getTextureID());

    ImageTexture::setWrapAndFilter(_wrapModeS, _wrapModeT, _minFilterMode, _magFilterMode);

    this->generateTexture(_path);
}


// 用于生成立方体贴图的纹理
ImageTexture::ImageTexture(const std::vector<std::string> &_pathVec, unsigned int _unit, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode): ITexture(_unit) {
    LOG_ASSERT(!_pathVec.empty());

    glBindTexture(GL_TEXTURE_CUBE_MAP, this->getTextureID());

    m_textureTarget = TARGET_TYPE::CUBE_MAP;

    ImageTexture::generateTextureForCubeMap(_pathVec);
    ImageTexture::setWrapAndFilterForCubeMap(_wrapModeS, _wrapModeT, _wrapModeR, _minFilterMode, _magFilterMode);
}

// 设置环绕方式、过滤方式
void ImageTexture::setWrapAndFilter(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode) {
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
void ImageTexture::setWrapAndFilterForCubeMap(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode) {
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
void ImageTexture::generateTexture(const std::string &_resPath) {
    LOG_ASSERT(!_resPath.empty());

    // 加载纹理图片
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true);
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
void ImageTexture::generateTextureForCubeMap(const std::vector<std::string> &_pathVec) {
    LOG_ASSERT(!_pathVec.empty());

    // 加载纹理图片
    int width, height, nrChannels;
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

void ImageTexture::bind() {
    LOG_ASSERT(this->getTextureID() != -1);

    switch (m_textureTarget) {
        case TARGET_TYPE::TEXTURE2D: glBindTexture(GL_TEXTURE_2D, this->getTextureID());        break;
        case TARGET_TYPE::CUBE_MAP:  glBindTexture(GL_TEXTURE_CUBE_MAP, this->getTextureID());  break;
        default:                     LOG(FATAL) << " Undefined conditional branch.";
    }
}
