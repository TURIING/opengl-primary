#include "glad/glad.h"

#include "ShaderProgram.h"
#include "glog/logging.h"
#include <iostream>
#include "Shader.h"

ShaderProgram::ShaderProgram(const std::string& _vertexPath, const std::string& _fragmentPath)
{
    LOG_ASSERT(!_vertexPath.empty());
    LOG_ASSERT(!_fragmentPath.empty());

	const Shader vertexShader(_vertexPath, SHADER_TYPE::VERTEX);
	const Shader fragmentShader(_fragmentPath, SHADER_TYPE::FRAGMENT);

	m_id = glCreateProgram();
	glAttachShader(m_id, vertexShader.getShaderId());
	glAttachShader(m_id, fragmentShader.getShaderId());
	glLinkProgram(m_id);

}

ShaderProgram::~ShaderProgram()
{
    LOG_ASSERT(m_id != -1);

	glDeleteProgram(m_id);
}

void ShaderProgram::use() const
{
    LOG_ASSERT(m_id != -1);

	glUseProgram(m_id);
}

void ShaderProgram::checkLinkError(unsigned _id)
{
    LOG_ASSERT(_id != -1);

	int success;

	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];

		glGetProgramInfoLog(_id, 1024, nullptr, infoLog);
        LOG(ERROR) << "An error occurred while linking shader code (" << infoLog << ")";
		exit(-1);
	}
}

/**
 * 获取属性变量的位置值
 * @param _attr 变量名
 * @return location
 */
unsigned int ShaderProgram::getAttrLocation(std::string _attr) {
    LOG_ASSERT(m_id != -1);

    const auto value = glGetAttribLocation(m_id, _attr.c_str());
    LOG_IF(ERROR, value == -1) << "Failed to get location value" << "(" << _attr << ")" << " corresponding to attribute.";

    return value;
}

int ShaderProgram::getUniformLocation(const std::string &_name) const {
    LOG_ASSERT(m_id != -1);

    const auto value =  glGetUniformLocation(m_id, _name.c_str());
    LOG_IF(ERROR, value == -1) << "Failed to get location value" << "(" << _name << ")" << " corresponding to uniform.";
}

void ShaderProgram::setBool(const std::string &_name, bool _value) const {
    glUniform1i(getUniformLocation(_name), (int)_value);
}

void ShaderProgram::setInt(const std::string &_name, int _value) const {
    glUniform1i(getUniformLocation(_name), _value);
}

void ShaderProgram::setFloat(const std::string &_name, float _value) const {
    glUniform1f(getUniformLocation(_name), _value);
}

void ShaderProgram::setFloat(const std::string &_name, float _v1, float _v2, float _v3) const {
    glUniform3f(getUniformLocation(_name), _v1, _v2, _v3);
}

void ShaderProgram::setVec3(const std::string &_name, float _v1, float _v2, float _v3) const {
    glUniform3f(getUniformLocation(_name), _v1, _v2, _v3);
}

void ShaderProgram::setVec3(const std::string &_name, const glm::vec3 &_value) const {
    glUniform3fv(getUniformLocation(_name), 1, &_value[0]);
}

void ShaderProgram::setVec4(const std::string &_name, float _v1, float _v2, float _v3, float _v4) const {
    glUniform4f(getUniformLocation(_name), _v1, _v2, _v3, _v4);
}

void ShaderProgram::setVec4(const std::string &_name, const glm::vec4 &_value) const {
    glUniform4fv(getUniformLocation(_name), 1, &_value[0]);
}

void ShaderProgram::setMat4(const std::string &_name, const GLfloat *_value) const {
    glUniformMatrix4fv(getUniformLocation(_name), 1, GL_FALSE, _value);
}

