#include "framebuffer.hpp"

#include "renderbuffer.hpp"
#include "texture2d.hpp"

#include "lib/gl.hpp"

Framebuffer::Framebuffer()
    : gl(new GLFramebuffer())
{
    glGenFramebuffers(1, &gl->framebuffer);
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &gl->framebuffer);
}

static GLenum translate_bind_target(BindTarget target)
{
    switch (target) {
    case BindTarget::DRAW:
        return GL_DRAW_FRAMEBUFFER;
    case BindTarget::READ:
        return GL_READ_FRAMEBUFFER;
    case BindTarget::FRAMEBUFFER:
        return GL_FRAMEBUFFER;
    default:
        return 0;
    }
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::status(BindTarget target)
{
    GLenum status = glCheckFramebufferStatus(translate_bind_target(target));
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        gl_print_fbo_status(status);
    }
}

void Framebuffer::bind(BindTarget target) const
{
    glBindFramebuffer(translate_bind_target(target), gl->framebuffer);
}

void Framebuffer::attach_color(BindTarget target, Renderbuffer const & renderbuffer, int n) const
{
    glFramebufferRenderbuffer(
        translate_bind_target(target),
        GL_COLOR_ATTACHMENT0 + n,
        GL_RENDERBUFFER,
        renderbuffer.gl->renderbuffer
    );
}

void Framebuffer::attach_color(BindTarget target, Texture2D const & texture, int n) const
{
    glFramebufferTexture2D(
        translate_bind_target(target),
        GL_COLOR_ATTACHMENT0 + n,
        GL_TEXTURE_2D,
        texture.gl->texture,
        0
    );
}

void Framebuffer::attach_depth(BindTarget target, Renderbuffer const & renderbuffer) const
{
    glFramebufferRenderbuffer(
        translate_bind_target(target),
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER,
        renderbuffer.gl->renderbuffer
    );
}

void Framebuffer::attach_depth(BindTarget target, Texture2D const & texture) const
{
    glFramebufferTexture2D(
        translate_bind_target(target),
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        texture.gl->texture,
        0
    );
}
