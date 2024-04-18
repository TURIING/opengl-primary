#include "glad/glad.h"

#include <string>
#include <iostream>
#include <glog/logging.h>
#include "ShaderProgram.h"
#include "Shader.h"

unsigned int ShaderProgram::m_currentUseProgramId = -1;

ShaderProgram::ShaderProgram(const std::string& _vertexPath, const std::string& _fragmentPath, const std::string &_geometryPath)
{
    LOG_ASSERT(!_vertexPath.empty());
    LOG_ASSERT(!_fragmentPath.empty());

    m_id = glCreateProgram();
    const Shader vertexShader(_vertexPath, SHADER_TYPE::VERTEX);
    glAttachShader(m_id, vertexShader.getShaderId());

    const Shader fragmentShader(_fragmentPath, SHADER_TYPE::FRAGMENT);
    glAttachShader(m_id, fragmentShader.getShaderId());

    if(!_geometryPath.empty()) {
        const Shader geometryShader(_geometryPath, SHADER_TYPE::GEOMETRY);
        glAttachShader(m_id, geometryShader.getShaderId());
    }

	glLinkProgram(m_id);

    this->use();
}

ShaderProgram::~ShaderProgram()
{
    LOG_ASSERT(m_id != -1);

	glDeleteProgram(m_id);
}

void ShaderProgram::use() const
{
    LOG_ASSERT(m_id != -1);
    ShaderProgram::m_currentUseProgramId = m_id;
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
unsigned int ShaderProgram::getAttrLocation(const std::string &_attr) {
    this->checkCurrentProgramBeUsed();
    LOG_ASSERT(m_id != -1);

    const auto value = glGetAttribLocation(m_id, _attr.c_str());
    LOG_IF(ERROR, value == -1) << "Failed to get location value" << "(" << _attr << ")" << " corresponding to attribute.";

    return value;
}

int ShaderProgram::getUniformLocation(const std::string &_name) const {
    this->checkCurrentProgramBeUsed();
    LOG_ASSERT(m_id != -1);

    const auto value =  glGetUniformLocation(m_id, _name.c_str());
    LOG_IF(INFO, value == -1) << "Failed to get location value" << "(" << _name << ")" << " corresponding to uniform.";
    return value;
}

void ShaderProgram::setBool(const std::string &_name, bool _value) const {
    this->checkCurrentProgramBeUsed();
    if(const auto location = this->getUniformLocation(_name); location != -1) {
        glUniform1i(location, (int)_value);
    }
}

void ShaderProgram::setInt(const std::string &_name, int _value) const {
    this->checkCurrentProgramBeUsed();
    if(const auto location = this->getUniformLocation(_name); location != -1) {
        glUniform1i(location, _value);
    }
}

void ShaderProgram::setFloat(const std::string &_name, float _value) const {
    this->checkCurrentProgramBeUsed();
    if(const auto location = this->getUniformLocation(_name); location != -1) {
        glUniform1f(location, _value);
    }
}

void ShaderProgram::setFloat(const std::string &_name, float _v1, float _v2, float _v3) const {
    this->checkCurrentProgramBeUsed();
    if(const auto location = this->getUniformLocation(_name); location != -1) {
        glUniform3f(location, _v1, _v2, _v3);
    }
}

void ShaderProgram::setVec3(const std::string &_name, float _v1, float _v2, float _v3) const {
    this->checkCurrentProgramBeUsed();
    if(const auto location = this->getUniformLocation(_name); location != -1) {
        glUniform3f(location, _v1, _v2, _v3);
    }
}

void ShaderProgram::setVec3(const std::string &_name, const glm::vec3 &_value) const {
    this->checkCurrentProgramBeUsed();
    if(const auto location = this->getUniformLocation(_name); location != -1) {
        glUniform3fv(location, 1, &_value[0]);
    }
}

void ShaderProgram::setVec4(const std::string &_name, float _v1, float _v2, float _v3, float _v4) const {
    this->checkCurrentProgramBeUsed();
    if(const auto location = this->getUniformLocation(_name); location != -1) {
        glUniform4f(location, _v1, _v2, _v3, _v4);
    }
}

void ShaderProgram::setVec4(const std::string &_name, const glm::vec4 &_value) const {
    this->checkCurrentProgramBeUsed();
    if(const auto location = this->getUniformLocation(_name); location != -1) {
        glUniform4fv(location, 1, &_value[0]);
    }
}

void ShaderProgram::setMat4(const std::string &_name, const GLfloat *_value) const {
    this->checkCurrentProgramBeUsed();
    if(const auto location = this->getUniformLocation(_name); location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, _value);
    }
}

bool ShaderProgram::isExistUniformVariable(const std::string &_name) const {
    return glGetUniformLocation(m_id, _name.c_str()) != -1;
}

void ShaderProgram::checkCurrentProgramBeUsed() const {
    LOG_ASSERT(m_currentUseProgramId == m_id);
}

