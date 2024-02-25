/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/2/24 18:20
* @version: 1.0
* @description: 
********************************************************************************/
#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
    this->bind();
}

VertexArray::~VertexArray()
{
    LOG_ASSERT(m_id != -1);

    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::unBind() const
{
    glBindVertexArray(0);
}