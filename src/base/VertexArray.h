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
	 * \param _size 数据个数
	 * \param _stride 步长
	 * \param _offset 该结构体字段的偏移个数
	 */
	void setAttribute(unsigned int _attrPos, int _size, int _stride, int _offset) const;

private:
	unsigned int m_id = -1;
};
#endif