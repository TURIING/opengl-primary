#ifndef OPENGL_PRIMARY_VERTEXARRAY_H
#define OPENGL_PRIMARY_VERTEXARRAY_H

#include <glad/glad.h>
#include <cstddef>
#include "glog/logging.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void bind() const;
	void unBind() const;

	/**
	 * \brief 解释如何解析顶点位置数据并启用位置顶点属性
	 * \param _attrPos position 索引
	 * \param _offset 该结构体字段的偏移个数
	 */
    template<class T1, class T2>
    void setAttribute(unsigned int _attrPos, size_t _offset) const
    {
        glVertexAttribPointer(_attrPos, sizeof(T2) / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(T1), (void *)(_offset));
        glEnableVertexAttribArray(_attrPos);
    }

private:
	unsigned int m_id = -1;
};
#endif