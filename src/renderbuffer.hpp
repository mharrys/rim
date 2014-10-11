#ifndef RENDERBUFFER_HPP_INCLUDED
#define RENDERBUFFER_HPP_INCLUDED

#include "imageformat.hpp"

#include <memory>

class Framebuffer;

struct GLRenderbuffer;

class Renderbuffer {
    friend Framebuffer;
public:
    Renderbuffer();
    ~Renderbuffer();

    Renderbuffer(Renderbuffer const &) = delete;
    Renderbuffer & operator=(Renderbuffer const &) = delete;

    void make() const;

    int width;
    int height;
    ImageFormat internal_format;
private:
    std::unique_ptr<GLRenderbuffer> gl;
};

#endif
