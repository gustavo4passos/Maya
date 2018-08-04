#include "../include/Texture.h"

#include "../include/Logger.h"
#include "../include/GLCall.h"

Texture::Texture(const void* data, int width, int height, int nChannels) :
    _w(width),
    _h(height),
    _nChannels(nChannels),
    _textureID(0)
{  

    GLCall(glGenTextures(1, &_textureID));
    GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));

    SetTexFiltering(false, false);
    SetTexRepeatBehavior(true);

    UploadTexture((BYTE*)data);
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &_textureID));
}

void Texture::Bind() {
    GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));
}

void Texture::Unbind() {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::SetTexFiltering(bool magLinearFiltering, bool minLinearFiltering) {
    Bind();
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magLinearFiltering ? GL_LINEAR : GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minLinearFiltering ? GL_LINEAR : GL_NEAREST));
    //TODO(Gustavo): Consider generating mipmaps
}

void Texture::SetTexRepeatBehavior(bool repeat){
    Bind();
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_BORDER));
}

void Texture::UploadTexture(BYTE* data) {
    Bind();

    GLenum format;
    switch(_nChannels) {
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            // Unknow format, triggers an error when uploading the texture
            format = 0;
            break;
    }

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, _w, _h, 0, format, GL_UNSIGNED_BYTE, (const void*)data));
}
