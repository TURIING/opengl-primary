#ifndef OPENGL_PRIMARY_BUFFER_H
#define OPENGL_PRIMARY_BUFFER_H

#include <vector>
#include <glad/glad.h>
#include <assert.h>

enum class BUFFER_TYPE
{
	VERTEX_BUFFER,
	INDEX_BUFFER,
    UNIFORM_BUFFER,             // Uniform缓冲对象
};

template<class T>
class Buffer
{
public:
	Buffer(BUFFER_TYPE _type, const std::vector<T> &_data): m_bufferType(_type)
    {
        glGenBuffers(1, &m_id);

        this->bind();

        this->setData(_data);
    }

    /**
     * 生成uniform buffer
     * @tparam T
     * @param _bufferID 绑定点id
     */
    Buffer(unsigned int _bindPoint): m_bufferType(BUFFER_TYPE::UNIFORM_BUFFER), m_bindPoint(_bindPoint) {
        const auto target = this->transformBufferTypeToTarget(m_bufferType);

        glGenBuffers(1, &m_id);
        this->bind();
        glBufferData(target, sizeof(T), nullptr, GL_STATIC_DRAW);

        glBindBuffer(target, _bindPoint);
        glBindBufferRange(target, _bindPoint, m_id, 0, sizeof(T));
    }

	~Buffer() {
        assert(m_id != -1);
        glDeleteBuffers(1, &m_id);
    }

	void bind() const {
        const auto target = this->transformBufferTypeToTarget(m_bufferType);
        glBindBuffer(target, m_id);
    }

    void unbind() const {
        const auto target = this->transformBufferTypeToTarget(m_bufferType);
        glBindBuffer(target, 0);
    }

    // 用于给ubo设置数据
    template<class U>
    void setData(unsigned int _offset, const void *_dataPtr) {
        this->bind();

        const auto target = this->transformBufferTypeToTarget(m_bufferType);
        glBufferSubData(target, _offset, sizeof(U), _dataPtr);
        glBindBuffer(target, m_bindPoint);
    }

private:
	void setData(const std::vector<T>& _data) {
        const auto target = this->transformBufferTypeToTarget(m_bufferType);
        glBufferData(target, _data.size() * sizeof(T), _data.data(), GL_STATIC_DRAW);
    }

	void setData(const T &_data) {
        const auto target = this->transformBufferTypeToTarget(m_bufferType);
        glBufferData(target, sizeof(T), nullptr, GL_STATIC_DRAW);
    }

	static GLenum transformBufferTypeToTarget(BUFFER_TYPE _type) {
        switch (_type)
        {
            case BUFFER_TYPE::VERTEX_BUFFER:		return GL_ARRAY_BUFFER;
            case BUFFER_TYPE::INDEX_BUFFER:			return GL_ELEMENT_ARRAY_BUFFER;
            case BUFFER_TYPE::UNIFORM_BUFFER:       return GL_UNIFORM_BUFFER;
            default:								return GL_ARRAY_BUFFER;
        }
    }

private:
	unsigned int m_id = -1;
    BUFFER_TYPE m_bufferType;
    unsigned int m_bindPoint = -1;                  // ubo的绑定点
};

#endif
