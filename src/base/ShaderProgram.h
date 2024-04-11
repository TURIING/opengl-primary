#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glog/logging.h>

class ShaderProgram
{
public:
	ShaderProgram(const std::string& _vertexPath, const std::string& _fragmentPath);
	~ShaderProgram();

	void use() const;

    // -----------------------------------------------------------------------------------------------
    [[nodiscard]] unsigned int getAttrLocation(const std::string &_attr);

    [[nodiscard]] int getUniformLocation(const std::string &_name) const;

    [[nodiscard]] bool isExistUniformVariable(const std::string &_name) const;
    // -----------------------------------------------------------------------------------------------
    void setBool(const std::string &_name, bool _value) const;

    // -----------------------------------------------------------------------------------------------
    void setInt(const std::string &_name, int _value) const;

    void setFloat(const std::string &_name, float _value) const;

    void setFloat(const std::string &_name, float _v1, float _v2, float _v3) const;

    // -----------------------------------------------------------------------------------------------
    void setVec3(const std::string &_name, float _v1, float _v2, float _v3) const;

    void setVec3(const std::string &_name, const glm::vec3 &_value) const;

    // ------------------------------------------------------------------------------------------------
    void setVec4(const std::string &_name, float _v1, float _v2, float _v3, float _v4) const;

    void setVec4(const std::string &_name, const glm::vec4 &_value) const;

    // ------------------------------------------------------------------------------------------------
    void setMat4(const std::string &_name, const float *_value) const;

private:
	static void checkLinkError(unsigned int _id);

    inline void checkCurrentProgramBeUsed() const {
        LOG_ASSERT(m_currentUseProgramId == m_id);
    }

private:
	unsigned int m_id = -1;
    static unsigned int m_currentUseProgramId;
};

