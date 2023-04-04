#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <GL/glew.h>
#include <vr/Vec2.h>

#include <string>

class Texture {

public:
    //Texture(const std::string &bmp_image);
    Texture(const char* bmp_image);

    ~Texture();

    GLuint textureObject() const;

    void setTexCoords(const vr::Vec2Vector &texCoords);

    bool hasTexture();

    void apply(GLint uniformLocation, int textureIndex);


private:
    GLuint m_textureObject;

    vr::Vec2Vector m_texCoords;
};

#endif