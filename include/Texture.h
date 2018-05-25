#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Texture {
public:
    Texture(std::string filename, int w, int h);
    ~Texture();

    inline int w() const { return _w; }
    inline int h() const { return _h; }
    inline std::string filename() const { return _filename; }

private:
    int _w, _h;
    std::string _filename;
};

#endif