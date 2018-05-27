#include "../include/Texture.h"

// TODO: stb_image is not robust enough; consider using SOIL or SDL_Image instead
#define STB_IMAGE_IMPLEMENTATION
#include "../extern/stb/stb_image.h"

#include "../include/ErrorHandler.h"
#include "../include/GLCall.h"

Texture::Texture(std::string filename) :
    _filename(filename),
    _textureID(0)
{  
    // TODO: File loading should be done in ResourceManager
    unsigned char* data = stbi_load(filename.c_str(), &_w, &_h, &_nChannels, 0);

    if(data == NULL) {
        LOG_ERROR("Unable to load image file. Filename\"" + _filename + "\"");
        return;
    }    

    GLCall(glGenTextures(1, &_textureID));
    GLCall(glBindTexture(GL_TEXTURE_2D, _textureID));

    SetTexFiltering(false, false);
    SetTexRepeatBehavior(false);

    UploadTexture(data);

    stbi_image_free(data);
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
    // TODO Consider generating mipmaping
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