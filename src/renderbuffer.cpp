#include "renderbuffer.hpp"

#include "lib/gl.hpp"

#include <iostream>

Renderbuffer::Renderbuffer()
    : width(0),
      height(0),
      internal_format(ImageFormat::DEPTH_COMPONENT16),
      gl(new GLRenderbuffer())
{
    glGenRenderbuffers(1, &gl->renderbuffer);
}

Renderbuffer::~Renderbuffer()
{
    glDeleteRenderbuffers(1, &gl->renderbuffer);
}

void Renderbuffer::make() const
{
    if (width < 0 || height < 0) {
        std::cerr << "Renderbuffer::make: dimension must be a positive number." << std::endl;
    } else if (width > GL_MAX_RENDERBUFFER_SIZE || height > GL_MAX_RENDERBUFFER_SIZE) {
        std::cerr << "Renderbuffer::make: dimension must be less than " << GL_MAX_RENDERBUFFER_SIZE << "." << std::endl;
    } else {
        GLint current;
        glGetIntegerv(GL_RENDERBUFFER_BINDING, &current);

        glBindRenderbuffer(GL_RENDERBUFFER, gl->renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, translate_image_format(internal_format), width, height);

        glBindRenderbuffer(GL_RENDERBUFFER, current);
    }
}
