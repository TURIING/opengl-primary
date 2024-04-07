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

struct Size;

class Texture {
    // 纹理类型
    enum class TARGET_TYPE { TEXTURE2D, CUBE_MAP};

public:
    enum STORAGE_TYPE { IMAGE2D, STORAGE2D };           // 纹理存储的类型

    Texture(std::string _path, GLenum _unit, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode);
    Texture(const std::vector<std::string> &_pathVec, unsigned int _unit, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode);
    Texture(Size &_scrSize, GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode, STORAGE_TYPE _type);
    ~Texture();
    [[nodiscard]] unsigned int getID() const { return m_id; }
    [[nodiscard]] unsigned int getTextureUnit() const { return m_textureUnit; }
    void activate();

private:
    void bind();
    static void setWrapAndFilter(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _minFilterMode, GLuint _magFilterMode);
    static void setWrapAndFilterForCubeMap(GLuint _wrapModeS, GLuint _wrapModeT, GLuint _wrapModeR, GLuint _minFilterMode, GLuint _magFilterMode);
    void generateTexture(const std::string &_resPath);
    static void generateTextureForCubeMap(const std::vector<std::string> &_pathVec);

private:
    unsigned int m_id = -1;
    TARGET_TYPE m_textureTarget = TARGET_TYPE::TEXTURE2D;
    unsigned int m_textureUnit = -1;
public:
    std::string m_path;
};

#endif //OPENGL_PRIMARY_TEXTURE_H
