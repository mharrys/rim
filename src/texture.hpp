#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include "imageformat.hpp"

#include <memory>
#include <vector>

class Framebuffer;
struct Image;

struct GLTexture;

enum class TextureTarget {
    TEXTURE_2D
};

enum class PixelFormat {
    RGB,
    RGBA
};

enum class FilterMode {
    NEAREST,
    LINEAR
};

enum class WrapMode {
    CLAMP_TO_EDGE,
    MIRRORED_REPEAT,
    REPEAT
};

class Texture {
    friend Framebuffer;
public:
    Texture(TextureTarget target);
    ~Texture();

    Texture(Texture const &) = delete;
    Texture & operator=(Texture const &) = delete;

    void bind(int unit = 0) const;

    void make(int width, int height, std::vector<unsigned char> data = {}) const;
    void make(Image const & image) const;

    ImageFormat internal_format;
    PixelFormat source_format;

    FilterMode min_filter;
    FilterMode mag_filter;

    WrapMode wrap_s;
    WrapMode wrap_t;
    WrapMode wrap_r;
private:
    void set_parameters() const;

    std::unique_ptr<GLTexture> gl;
};

#endif
