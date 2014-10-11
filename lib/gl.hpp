#ifndef GL_HPP_INCLUDED
#define GL_HPP_INCLUDED

#include "src/imageformat.hpp"

#include "GL/glew.h"

#include <iostream>
#include <string>

enum VertexAttribIndex {
    VERTEX_POSITION_INDEX = 0,
    VERTEX_NORMAL_INDEX,
    VERTEX_COLOR_INDEX,
    VERTEX_TEX_COORD_INDEX
};

struct GLVertexArray {
    GLuint name;
    GLuint indices_size;
    GLuint positions_size;
    GLuint positions_buffer;
    GLuint normals_buffer;
    GLuint colors_buffer;
    GLuint tex_coords_buffer;
    GLuint indices_buffer;
};

struct GLShader {
    GLenum type;
    GLuint shader;
};

struct GLProgram {
    GLuint program;
};

struct GLTexture {
    GLenum target;
    GLuint texture;
};

struct GLFramebuffer {
    GLuint framebuffer;
};

struct GLRenderbuffer {
    GLuint renderbuffer;
};

static inline void gl_print_error()
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::string error_str;
        switch (error) {
            case GL_INVALID_ENUM:
                error_str =
                    "GL_INVALID_ENUM. An unacceptable value is specified for an "
                    "enumerated argument. The offending command is ignored and "
                    "has no other side effect than to set the error flag.";
                break;
            case GL_INVALID_VALUE:
                error_str =
                    "GL_INVALID_VALUE. A numeric argument is out of range. The "
                    "offending command is ignored and has no other side effect "
                    "than to set the error flag.";
                break;
            case GL_INVALID_OPERATION:
                error_str =
                    "GL_INVALID_OPERATION. The specified operation is not "
                    "allowed in the current state. The offending command "
                    "is ignored and has no other side effect than to set "
                    "the error flag.";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error_str =
                    "GL_INVALID_FRAMEBUFFER_OPERATION. The framebuffer object "
                    "is not complete. The offending command is ignored and "
                    "has no other side effect than to set the error flag.";
                break;
            case GL_OUT_OF_MEMORY:
                error_str =
                    "GL_OUT_OF_MEMORY. There is not enough memory left to "
                    "execute the command. The state of the GL is undefined, "
                    "except for the state of the error flags, after this error "
                    "is recorded.";
                break;
            default:
                error_str = "Uknown error code.";
                break;
        }
        std::cerr << "OpenGL error (" << error << "): " << error_str << std::endl;
    }
}

static inline void gl_print_fbo_status(GLenum status)
{
    std::string error_str;
    switch (status) {
    case GL_FRAMEBUFFER_UNDEFINED:
        error_str =
            "GL_FRAMEBUFFER_UNDEFINED. Target is the default framebuffer, "
            "but the default framebuffer does not exist.";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        error_str =
            "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT. One or more of the "
            "framebuffer attachment points are framebuffer incomplete.";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        error_str =
            "GL_FRAMEBUFFER_INCOMPELTE_MISSING_ATTACHMENT. Framebuffer "
            "must have at least one image attached to it.";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        error_str =
            "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER. Value of "
            "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any "
            "color attachment point(s) named by GL_DRAWBUFFERi.";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        error_str =
            "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER. GL_READ_BUFFER is not "
            "GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE "
            "is GL_NONE for the color attachment point named by "
            "GL_READ_BUFFER.";
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        error_str =
            "GL_FRAMEBUFFER_UNSUPPORTED. Combination of internal formats of "
            "the attached images violates an implementation-dependent set of "
            "restrictions.";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        error_str =
            "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE.\n\n"

            "The value of GL_RENDERBUFFER_SAMPLES is not the same for all "
            "attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is "
            "the not same for all attached textures; or, if the attached "
            "images are a mix of renderbuffers and textures, the value of "
            "GL_RENDERBUFFER_SAMPLES does not match the value of "
            "GL_TEXTURE_SAMPLES.\n\n"

            "or\n\n"

            "The value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same "
            "for all attached textures; or, if the attached images are a mix "
            "of renderbuffers and textures, the value of "
            "GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all "
            "attached textures.";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        error_str =
            "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS. One or more framebuffer "
            "attachment(s) is layered, and any populated attachment is not "
            "layered, or if all populated color attachments are not from "
            "textures of the same target.";
        break;
    default:
        error_str = "Unknown error code.";
        break;
    }

    std::cerr << "OpenGL framebuffer incomplete (" << status << "): " << error_str << std::endl;
}

static inline GLint translate_image_format(ImageFormat format)
{
    switch (format) {
    case ImageFormat::RGB:
        return GL_RGB;
    case ImageFormat::RGBA:
        return GL_RGBA;
    case ImageFormat::DEPTH_COMPONENT16:
        return GL_DEPTH_COMPONENT16;
    case ImageFormat::DEPTH_COMPONENT24:
        return GL_DEPTH_COMPONENT24;
    case ImageFormat::DEPTH_COMPONENT32:
        return GL_DEPTH_COMPONENT32;
    case ImageFormat::DEPTH_COMPONENT32F:
        return GL_DEPTH_COMPONENT32F;
    default:
        return 0;
    }
}

#endif
