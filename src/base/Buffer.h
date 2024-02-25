#ifndef OPENGL_PRIMARY_BUFFER_H
#define OPENGL_PRIMARY_BUFFER_H

#include <vector>
#include <glad/glad.h>
#include <assert.h>

enum class BUFFER_TYPE
{
	VERTEX_BUFFER,
	INDEX_BUFFER
};

template<class T>
class Buffer
{
public:
	Buffer(BUFFER_TYPE _type, const std::vector<T> &_data);
	~Buffer();

	void bind() const { glBindBuffer(m_target, m_id); }

private:
	void setData(const std::vector<T>& _data);
	static GLenum transformBufferTypeToTarget(BUFFER_TYPE _type);
private:
	unsigned int m_id = -1;
	GLenum m_target;
};

template <class T>
Buffer<T>::Buffer(BUFFER_TYPE _type, const std::vector<T> &_data)
{
    glGenBuffers(1, &m_id);

    m_target = transformBufferTypeToTarget(_type);

    this->bind();

    this->setData(_data);
}

template <class T>
Buffer<T>::~Buffer()
{
    assert(m_id != -1);

    glDeleteBuffers(1, &m_id);
}

template <class T>
void Buffer<T>::setData(const std::vector<T>& _data)
{
    glBufferData(m_target, _data.size() * sizeof(T), _data.data(), GL_STATIC_DRAW);
}

template <class T>
GLenum Buffer<T>::transformBufferTypeToTarget(BUFFER_TYPE _type)
{
    switch (_type)
    {
        case BUFFER_TYPE::VERTEX_BUFFER:		return GL_ARRAY_BUFFER;
        case BUFFER_TYPE::INDEX_BUFFER:			return GL_ELEMENT_ARRAY_BUFFER;
        default:								return GL_ARRAY_BUFFER;
    }
}
#endif
