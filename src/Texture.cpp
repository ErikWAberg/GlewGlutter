#include <Texture.h>

#include <iostream>
#include <SOIL.h>
#include <vr/PNGReader.h>
#include <vr/BMPReader.h>
#include <vr/JPGReader.h>
#include <vr/Image.h>


//Texture::Texture(const std::string &bmp_image) {
Texture::Texture(const char* bmp_image) {
   /* vr::Image *image = NULL;

    std::string fn = bmp_image;

    if(fn.substr(fn.find_last_of(".") + 1) == "png") {
        std::cout << "Reading png " << fn << std::endl;
        vr::PNGReader reader;
      //  image = reader.readImage(bmp_image);
    } else if(fn.substr(fn.find_last_of(".") + 1) == "bmp") {
        std::cout << "Reading bmp " << fn << std::endl;
        vr::BMPReader reader;
      //  image = reader.readImage(bmp_image);
    } else if(fn.substr(fn.find_last_of(".") + 1) == "jpg" || fn.substr(fn.find_last_of(".") + 1) == "jpeg") {
        std::cout << "Reading jpeg " << fn <<  " " << fn.substr(fn.find_last_of("/") + 1) << std::endl;
        vr::JPGReader reader;
    //    image = reader.readImage(bmp_image);
    }

    if (!image) {
       // std::cerr << "Unable to load image: " << bmp_image << std::endl;
        return;
    }

    glGenTextures(1, &m_textureObject);

    glBindTexture(GL_TEXTURE_2D, m_textureObject);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->width(), image->height(),
            GL_RGB, GL_UNSIGNED_BYTE, image->data());

    glBindTexture(GL_TEXTURE_2D, 0);


    m_textureObject = SOIL_load_OGL_texture
            (
                    bmp_image,
                    SOIL_LOAD_AUTO,
                    SOIL_CREATE_NEW_ID,
                    SOIL_FLAG_POWER_OF_TWO
                | SOIL_FLAG_MIPMAPS
                | SOIL_FLAG_MULTIPLY_ALPHA
                | SOIL_FLAG_COMPRESS_TO_DXT
                | SOIL_FLAG_DDS_LOAD_DIRECT
                | SOIL_FLAG_INVERT_Y
            );

    if( m_textureObject > 0 )
        {   
            glEnable( GL_TEXTURE_2D );
            glBindTexture( GL_TEXTURE_2D, m_textureObject );
            
        }
        else {
        std::cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << bmp_image << ")" << std::endl;        
        }
*/
}

Texture::~Texture() {
    glDeleteTextures(1, &m_textureObject);
}

void Texture::apply(GLint uniformLocation, int textureIndex) {
    switch (textureIndex) {
        case 0:
            glActiveTexture(GL_TEXTURE0);
            break;
        case 1:
            glActiveTexture(GL_TEXTURE1);
            break;
        case 2:
            glActiveTexture(GL_TEXTURE2);
            break;
        default:
            break;

    }
    glBindTexture(GL_TEXTURE_2D, m_textureObject);
    glUniform1i(uniformLocation, textureIndex);
}

bool Texture::hasTexture() {
    return m_textureObject != 0;
}

void Texture::setTexCoords(const vr::Vec2Vector &texCoords) {
    m_texCoords = texCoords;
}


GLuint Texture::textureObject() const {
    return m_textureObject;
}

