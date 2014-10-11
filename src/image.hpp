#ifndef IMAGE_HPP_INCLUDED
#define IMAGE_HPP_INCLUDED

#include <vector>

struct Image {
    unsigned int width;
    unsigned int height;
    std::vector<unsigned char> data;
};

#endif
