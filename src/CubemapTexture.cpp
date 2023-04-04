#include <CubemapTexture.h>
#include <vr/BMPReader.h>
#include <iostream>
#include <vr/PNGReader.h>
#include <vr/JPGReader.h>
#include <stdlib.h>
#include <SOIL/SOIL.h>


CubemapTexture::CubemapTexture(const std::string& PosXFilename,
        const std::string& NegXFilename,
        const std::string& PosYFilename,
        const std::string& NegYFilename,
        const std::string& PosZFilename,
        const std::string& NegZFilename)
{

    m_fileNames[0] = PosXFilename;
    m_fileNames[1] = NegXFilename;
    m_fileNames[2] = PosYFilename;
    m_fileNames[3] = NegYFilename;
    m_fileNames[4] = PosZFilename;
    m_fileNames[5] = NegZFilename;

    m_textureObj = 0;

}

bool CubemapTexture::generateBuffers()
{
/*
    m_textureObj = SOIL_load_OGL_cubemap
            (
                    m_fileNames[0].c_str(),
                    m_fileNames[1].c_str(),
                    m_fileNames[3].c_str(),
                    m_fileNames[2].c_str(),
                    m_fileNames[4].c_str(),
                    m_fileNames[5].c_str(),
                    SOIL_LOAD_RGB,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_MIPMAPS
            );*/

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

    return true;
}

void CubemapTexture::bind(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureObj);

}