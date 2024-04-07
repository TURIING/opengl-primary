/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/4/3 15:55
* @version: 1.0
* @description: 初步用于加载模型中的材质
********************************************************************************/

#ifndef OPENGL_PRIMARY_COMMONMATERIAL_H
#define OPENGL_PRIMARY_COMMONMATERIAL_H

#include <memory>
#include "../base/IMaterial.h"

class Texture;
class ShaderProgram;

class CommonMaterial: public IMaterial {
public:
    explicit CommonMaterial(std::shared_ptr<ShaderProgram> _shaderProgram);

    void setDiffuseMap(std::shared_ptr<Texture> _texture);
    void setSpecularMap(std::shared_ptr<Texture> _texture);
    void setNormalMap(std::shared_ptr<Texture> _texture);
    void setHeightMap(std::shared_ptr<Texture> _texture);

    std::shared_ptr<Texture> getDiffuseMap() { return m_diffuseMap; }
    std::shared_ptr<Texture> getSpecularMap() { return m_specularMap; }
    std::shared_ptr<Texture> getNormalMap() { return m_normalMap; }
    std::shared_ptr<Texture> getHeightMap() { return m_heightMap; }

    void resetDiffuseMap(const std::string &_file);
    void resetSpecularMap(const std::string &_file);
    void resetNormalMap(const std::string &_file);
    void resetHeightMap(const std::string &_file);

private:
    std::shared_ptr<Texture> m_diffuseMap;
    std::shared_ptr<Texture> m_specularMap;
    std::shared_ptr<Texture> m_normalMap;
    std::shared_ptr<Texture> m_heightMap;
    std::weak_ptr<ShaderProgram> m_shaderProgram;
};


#endif //OPENGL_PRIMARY_COMMONMATERIAL_H
