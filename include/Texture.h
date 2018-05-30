#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Texture {
public:
    Texture(std::string filename);
    ~Texture();

    void Bind();
    void Unbind();

    inline int w() const { return _w; }
    inline int h() const { return _h; }
    inline std::string filename() const { return _filename; }
    inline unsigned int id() { return _textureID; }

private:
    typedef unsigned char BYTE;

    // Texture information
    int _w, _h, _nChannels;
    std::string _filename;

    // Texture buffer OpenGL name
    GLuint _textureID;

    // Set mag and min filtering
    void SetTexFiltering(bool magLinearFiltering = false, bool minLinearFiltering = false);

    // Specifies what happens when drawing out of the bounds 
    // of the texture 
    void SetTexRepeatBehavior(bool repeat = true);

    // Uploads texture data to the GPU
    void UploadTexture(BYTE* data);

};

#endif