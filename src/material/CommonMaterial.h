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

class ImageTexture;
class ShaderProgram;

class CommonMaterial: public IMaterial {
public:
    explicit CommonMaterial(std::shared_ptr<ShaderProgram> _shaderProgram);

    void setDiffuseMap(std::shared_ptr<ImageTexture> _texture);
    void setSpecularMap(std::shared_ptr<ImageTexture> _texture);
    void setNormalMap(std::shared_ptr<ImageTexture> _texture);
    void setHeightMap(std::shared_ptr<ImageTexture> _texture);

    std::shared_ptr<ImageTexture> getDiffuseMap() { return m_diffuseMap; }
    std::shared_ptr<ImageTexture> getSpecularMap() { return m_specularMap; }
    std::shared_ptr<ImageTexture> getNormalMap() { return m_normalMap; }
    std::shared_ptr<ImageTexture> getHeightMap() { return m_heightMap; }

    void resetDiffuseMap(const std::string &_file);
    void resetSpecularMap(const std::string &_file);
    void resetNormalMap(const std::string &_file);
    void resetHeightMap(const std::string &_file);

private:
    std::shared_ptr<ImageTexture> m_diffuseMap;
    std::shared_ptr<ImageTexture> m_specularMap;
    std::shared_ptr<ImageTexture> m_normalMap;
    std::shared_ptr<ImageTexture> m_heightMap;
    std::weak_ptr<ShaderProgram> m_shaderProgram;
};


#endif //OPENGL_PRIMARY_COMMONMATERIAL_H
