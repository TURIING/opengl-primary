#include <string>
#include <glad/glad.h>
#include "Shader.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glog/logging.h"
#include <GL/gl.h>

/**
 * \brief
 * \param _path glsl文件路径
 * \param _type Shader类型
 */
Shader::Shader(const std::string& _path, SHADER_TYPE _type): m_path(_path)
{
    const auto type = transformShaderTypeToGlEnum(_type);

	std::string source;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(_path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        source = shaderStream.str();
    }
    catch (std::ifstream::failure& _e) {
        LOG(ERROR) << "path(" << _path << ")" << _e.what();
        exit(-1);
    }

    const auto code = source.c_str();

    m_id = glCreateShader(type);
    glShaderSource(m_id, 1, &code, nullptr);
    glCompileShader(m_id);
    checkCompileErrors(m_id, SHADER_TYPE::VERTEX);
}

Shader::~Shader()
{
	assert(m_id != -1);

	glDeleteShader(m_id);
}

GLenum Shader::transformShaderTypeToGlEnum(SHADER_TYPE _type)
{
	switch (_type)
	{
    case SHADER_TYPE::VERTEX:   return GL_VERTEX_SHADER;
    case SHADER_TYPE::FRAGMENT: return GL_FRAGMENT_SHADER;
    case SHADER_TYPE::GEOMETRY: return GL_GEOMETRY_SHADER;
	}
}

/**
 * \brief 检查编译错误
 * \param shader id
 * \param type 代码类型
 */
void Shader::checkCompileErrors(unsigned int shader, SHADER_TYPE type)
{
	int success;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        LOG(ERROR) << "An error occurred while compiling shader code (" << m_path << ") " << infoLog;
        exit(-1);
    }
}
