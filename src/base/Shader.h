#pragma once
#include <string>
#include <GL/gl.h>

enum class SHADER_TYPE { VERTEX, FRAGMENT };

class Shader
{
public:
	/**
	 * \brief 
	 * \param _path glsl文件路径
	 * \param _type Shader类型
	 */
	Shader(const std::string& _path, SHADER_TYPE _type);

	~Shader();

	unsigned int getShaderId() const { return m_id; }

private:
	static GLenum transformShaderTypeToGlEnum(SHADER_TYPE _type);

	/**
	 * \brief 检查编译错误
	 * \param shader id
	 * \param type 代码类型
	 */
	static void checkCompileErrors(unsigned int shader, SHADER_TYPE type);

private:
	unsigned int m_id = -1;
};

