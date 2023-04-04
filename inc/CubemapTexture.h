#ifndef _CUBEMAP_TEXTURE_H
#define _CUBEMAP_TEXTURE_H


#include <string>
#include <GL/glew.h>

class CubemapTexture
{
public:

    CubemapTexture(const std::string& PosXFilename,
            const std::string& NegXFilename,
            const std::string& PosYFilename,
            const std::string& NegYFilename,
            const std::string& PosZFilename,
            const std::string& NegZFilename);

    ~CubemapTexture();



    void bind(GLenum textureUnit);

    bool generateBuffers();

private:

    GLuint m_textureObj;
    std::string m_fileNames[6];

};

#endif