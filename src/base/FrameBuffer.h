/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/3/10 21:13
* @version: 1.0
* @description: 帧缓冲
********************************************************************************/

#ifndef OPENGL_PRIMARY_FRAMEBUFFER_H
#define OPENGL_PRIMARY_FRAMEBUFFER_H

#include "glad/glad.h"
#include "glog/logging.h"
#include "FrameBufferTexture.h"
#include "RenderBuffer.h"
#include <memory>

// 附件类型
enum class FRAMEBUFFER_ATTACH_TYPE { COLOR, DEPTH, STENCIL, DEPTH_STENCIL };

class FrameBuffer {
public:
    FrameBuffer() {
        glGenFramebuffers(1, &m_id);
        this->bind();
    }

    ~FrameBuffer() {
        LOG_ASSERT(m_id != -1);
        glDeleteFramebuffers(1, &m_id);
    }

    void bind() {
        LOG_ASSERT(m_id != -1);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    }

    void unbind() {
        LOG_ASSERT(m_id != -1);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    static void attachTexture2D(FRAMEBUFFER_ATTACH_TYPE _type, std::shared_ptr<FrameBufferTexture> &_texture) {
        LOG_ASSERT(_texture->getTextureID() != -1);

        GLenum type;
        switch (_type) {
            case FRAMEBUFFER_ATTACH_TYPE::COLOR:            type = GL_COLOR_ATTACHMENT0;            break;
            case FRAMEBUFFER_ATTACH_TYPE::DEPTH:            type = GL_DEPTH_ATTACHMENT;             break;
            case FRAMEBUFFER_ATTACH_TYPE::STENCIL:          type = GL_STENCIL_ATTACHMENT;           break;
            case FRAMEBUFFER_ATTACH_TYPE::DEPTH_STENCIL:    type = GL_DEPTH_STENCIL_ATTACHMENT;     break;
            default:                                        LOG(FATAL) << "Parameter error.";
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, type, GL_TEXTURE_2D, _texture->getTextureID(), 0);
    }

    static void attachTexture(FRAMEBUFFER_ATTACH_TYPE _type, std::shared_ptr<FrameBufferTexture> &_texture) {
        LOG_ASSERT(_texture->getTextureID() != -1);

        GLenum type;
        switch (_type) {
            case FRAMEBUFFER_ATTACH_TYPE::COLOR:            type = GL_COLOR_ATTACHMENT0;            break;
            case FRAMEBUFFER_ATTACH_TYPE::DEPTH:            type = GL_DEPTH_ATTACHMENT;             break;
            case FRAMEBUFFER_ATTACH_TYPE::STENCIL:          type = GL_STENCIL_ATTACHMENT;           break;
            case FRAMEBUFFER_ATTACH_TYPE::DEPTH_STENCIL:    type = GL_DEPTH_STENCIL_ATTACHMENT;     break;
            default:                                        LOG(FATAL) << "Parameter error.";
        }
        glFramebufferTexture(GL_FRAMEBUFFER, type, _texture->getTextureID(), 0);
    }

    void attachRenderBuffer(FRAMEBUFFER_ATTACH_TYPE _type, std::shared_ptr<RenderBuffer> &_rbo) {
        LOG_ASSERT(_rbo->getID() != -1);

        GLenum type;
        switch (_type) {
            case FRAMEBUFFER_ATTACH_TYPE::DEPTH_STENCIL:    type = GL_DEPTH_STENCIL_ATTACHMENT;     break;
            default:                                        LOG(FATAL) << "Parameter error.";
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, type, GL_RENDERBUFFER, _rbo->getID());
    }

    void check() {
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) return;

        LOG(ERROR) << "Framebuffer is not complete!";
    }

private:
    unsigned int m_id = -1;
};

#endif //OPENGL_PRIMARY_FRAMEBUFFER_H
