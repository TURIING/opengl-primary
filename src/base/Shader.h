#pragma once

enum class SHADER_TYPE { VERTEX, FRAGMENT, GEOMETRY };

class Shader
{
public:
	Shader(const std::string& _path, SHADER_TYPE _type);
	~Shader();

	[[nodiscard]] unsigned int getShaderId() const { return m_id; }

private:
	static GLenum transformShaderTypeToGlEnum(SHADER_TYPE _type);
	void checkCompileErrors(unsigned int shader, SHADER_TYPE type);

private:
	unsigned int m_id = -1;
    std::string m_path;
};

