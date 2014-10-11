#ifndef IMAGEFORMAT_HPP_INCLUDED
#define IMAGEFORMAT_HPP_INCLUDED

enum class ImageFormat {
    DEPTH_COMPONENT16,
    DEPTH_COMPONENT24,
    DEPTH_COMPONENT32,
    DEPTH_COMPONENT32F,
    // required for textures but optional for renderbuffers
    RGB,
    RGBA
};

#endif
