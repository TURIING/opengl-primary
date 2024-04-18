/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/29 15:14
* @version: 1.0
* @description: 冯氏光照材质类
********************************************************************************/

#ifndef OPENGL_PRIMARY_PHONGMATERIAL_H
#define OPENGL_PRIMARY_PHONGMATERIAL_H

#include <memory>
#include "../base/IMaterial.h"

class ImageTexture;
class ShaderProgram;

class PhongMaterial: public IMaterial{
public:
    PhongMaterial(std::shared_ptr<ShaderProgram> &_shaderProgram, const std::string &_diffuse, const std::string &_specular, float _shininess = 32);

    std::shared_ptr<ImageTexture> &getDiffuse();
    std::shared_ptr<ImageTexture> &getSpecular();
    float *getShininess();

    void setDiffuse(std::shared_ptr<ImageTexture> diffuse);
    void setSpecular(std::shared_ptr<ImageTexture> specular);
    void setShininess(float shininess);

    void resetDiffuse(const std::string &_path);
    void resetSpecular(const std::string &_path);

private:
    // 移除了环境光材质颜色向量，是因为环境光颜色在几乎所有情况下都等于漫反射颜色
    std::shared_ptr<ImageTexture> m_diffuse;
    std::shared_ptr<ImageTexture> m_specular;
    float m_shininess = 32.0f;
    std::shared_ptr<ShaderProgram> m_shaderProgram;
};
#endif //OPENGL_PRIMARY_PHONGMATERIAL_H
