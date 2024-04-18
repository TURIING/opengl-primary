/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 20:38
* @version: 1.0
* @description: 生成使用图片的纹理
********************************************************************************/

#ifndef OPENGL_PRIMARY_IMAGETEXTURE_H
#define OPENGL_PRIMARY_IMAGETEXTURE_H

#include <tuple>

#include "glad/glad.h"
#include "glog/logging.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ITexture.h"

struct Size;

class ImageTexture: public ITexture {
public:
    ImageTexture(std::string _path, GLenum _unit, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode);
    ImageTexture(const std::vector<std::string> &_pathVec, unsigned int _unit, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode);

protected:
    void bind() override;

private:
    static void setWrapAndFilter(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode);
    static void setWrapAndFilterForCubeMap(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode);
    static void generateTexture(const std::string &_resPath);
    static void generateTextureForCubeMap(const std::vector<std::string> &_pathVec);

private:
    TARGET_TYPE m_textureTarget = TARGET_TYPE::TEXTURE2D;

public:
    std::string m_path;
};

#endif //OPENGL_PRIMARY_IMAGETEXTURE_H
