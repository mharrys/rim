#include "texture.hpp"

#include "image.hpp"

#include "lib/gl.hpp"

#include <iostream>

static GLenum translate_texture_target(TextureTarget target)
{
    switch (target) {
    case TextureTarget::TEXTURE_2D:
        return GL_TEXTURE_2D;
    default:
        return 0;
    }
}

Texture::Texture(TextureTarget target)
    : internal_format(ImageFormat::RGB),
      source_format(PixelFormat::RGB),
      min_filter(FilterMode::LINEAR),
      mag_filter(FilterMode::LINEAR),
      wrap_s(WrapMode::REPEAT),
      wrap_t(WrapMode::REPEAT),
      wrap_r(WrapMode::REPEAT),
      gl(new GLTexture())
{
    gl->target = translate_texture_target(target);
    glGenBuffers(1, &gl->texture);
}

Texture::~Texture()
{
    glDeleteBuffers(1, &gl->texture);
}

void Texture::bind(int unit) const
{
    GLenum texture_unit = GL_TEXTURE0 + unit;

    if (texture_unit >= (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1)) {
        std::cerr << "Texture::bind: texture unit is out of range." << std::endl;
    } else {
        glActiveTexture(texture_unit);
        glBindTexture(gl->target, gl->texture);
    }
}

static GLint translate_pixel_format(PixelFormat format)
{
    switch (format) {
    case PixelFormat::RGB:
        return GL_RGB;
    case PixelFormat::RGBA:
        return GL_RGBA;
    default:
        return 0;
    }
}

void Texture::make(int width, int height, std::vector<unsigned char> data) const
{
    set_parameters();

    glTexImage2D(
        gl->target,
        0,
        translate_image_format(internal_format),
        width,
        height,
        0,
        translate_pixel_format(source_format),
        GL_UNSIGNED_BYTE,
        &data[0]
    );
}

void Texture::make(Image const & image) const
{
    make(image.width, image.height, image.data);
}

static GLint translate_filter_mode(FilterMode mode)
{
    switch (mode) {
    case FilterMode::NEAREST:
        return GL_NEAREST;
    case FilterMode::LINEAR:
        return GL_LINEAR;
    default:
        return 0;
    }
}

static GLint translate_wrap_mode(WrapMode mode)
{
    switch(mode) {
    case WrapMode::CLAMP_TO_EDGE:
         return GL_CLAMP_TO_EDGE;
    case WrapMode::MIRRORED_REPEAT:
         return GL_MIRRORED_REPEAT;
    case WrapMode::REPEAT:
         return GL_REPEAT;
    default:
         return 0;
    }
}

void Texture::set_parameters() const
{
    glTexParameteri(gl->target, GL_TEXTURE_MIN_FILTER, translate_filter_mode(min_filter));
    glTexParameteri(gl->target, GL_TEXTURE_MAG_FILTER, translate_filter_mode(mag_filter));
    glTexParameteri(gl->target, GL_TEXTURE_WRAP_S, translate_wrap_mode(wrap_s));
    glTexParameteri(gl->target, GL_TEXTURE_WRAP_T, translate_wrap_mode(wrap_t));
    glTexParameteri(gl->target, GL_TEXTURE_WRAP_R, translate_wrap_mode(wrap_r));
}
