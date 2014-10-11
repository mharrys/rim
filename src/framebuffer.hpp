#ifndef FRAMEBUFFER_HPP_INCLUDED
#define FRAMEBUFFER_HPP_INCLUDED

#include <memory>

struct Renderbuffer;
struct Texture2D;

struct GLFramebuffer;

enum class BindTarget {
    DRAW,
    READ,
    FRAMEBUFFER
};

class Framebuffer {
public:
    Framebuffer();
    ~Framebuffer();

    Framebuffer(Framebuffer const &) = delete;
    Framebuffer & operator=(Framebuffer const &) = delete;

    static void unbind();
    static void status(BindTarget target = BindTarget::FRAMEBUFFER);

    void bind(BindTarget target = BindTarget::FRAMEBUFFER) const;

    void attach_color(BindTarget target, Renderbuffer const & renderbuffer, int n = 0) const;
    void attach_color(BindTarget target, Texture2D const & texture, int n = 0) const;

    void attach_depth(BindTarget target, Renderbuffer const & renderbuffer) const;
    void attach_depth(BindTarget target, Texture2D const & texture) const;
private:
    std::unique_ptr<GLFramebuffer> gl;
};

#endif
