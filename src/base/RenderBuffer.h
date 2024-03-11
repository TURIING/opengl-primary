/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/11 12:22
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef OPENGL_PRIMARY_RENDERBUFFER_H
#define OPENGL_PRIMARY_RENDERBUFFER_H

#include "glad/glad.h"
#include "glog/logging.h"

// RBO内部格式
enum class RENDER_BUFFER_FORMAT { DEPTH24_STENCIL8 };

class RenderBuffer {
public:
    RenderBuffer() {
        glGenRenderbuffers(1, &m_id);
        this->bind();
    }

    ~RenderBuffer() {
        LOG_ASSERT(m_id != -1);
        glDeleteRenderbuffers(1, &m_id);
    }

    void bind() {
        LOG_ASSERT(m_id != -1);
        glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    }

    void storage(RENDER_BUFFER_FORMAT _format, int _scrWidth, int _scrHeight) {
        glRenderbufferStorage(GL_RENDERBUFFER, transFormatToGLEnum(_format), _scrWidth, _scrHeight);
    }

    unsigned int getID() {
        LOG_ASSERT(m_id != -1);
        return m_id;
    }

private:
    GLenum transFormatToGLEnum(RENDER_BUFFER_FORMAT _format) {
        switch (_format) {
            case RENDER_BUFFER_FORMAT::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
            default:               LOG(FATAL) << "Parameter error.";
        }
    }

private:
    unsigned int m_id = -1;
};

#endif //OPENGL_PRIMARY_RENDERBUFFER_H
