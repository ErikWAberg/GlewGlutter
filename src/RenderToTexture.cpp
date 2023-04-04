#include "RenderToTexture.h"
#include <iostream>
#include <stdlib.h>

RenderToTexture::RenderToTexture(int width, int height, bool makeCubeMap) {
    m_width = width;
    m_height = height;
    m_makeCubeMap = makeCubeMap;
}

RenderToTexture::~RenderToTexture() {
    clearBuffers();
}

bool RenderToTexture::generateBuffers() {

    int prevFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFrameBuffer);

    glGenFramebuffers(1, &m_fbo);

    if(m_makeCubeMap) {
        glGenTextures(1, &m_depthTexture);
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);


        glGenTextures(1, &m_shadowCubeTexture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowCubeTexture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


        for (int i = 0 ; i < 6 ; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R16F, m_width, m_height, 0, GL_RED, GL_FLOAT, NULL);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
        // Disable writes to the color buffer
        glDrawBuffer(GL_NONE);
           
        // Disable reads from the color buffer
        glReadBuffer(GL_NONE);

    } else {
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);            
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
        
  /*      glGenRenderbuffers(1, &m_texture);
        glBindRenderbuffer(GL_RENDERBUFFER, m_texture);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
*/
        glGenTextures(1, &m_depthTexture);
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
      //  glDrawBuffers(1, DrawBuffers);
    }


    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "RtT FB error, status: 0x" <<  Status << std::endl;
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, prevFrameBuffer);
    return true;
}


void RenderToTexture::clearBuffers() {
    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
    }

    if (m_shadowCubeTexture != 0) {
        glDeleteTextures(1, &m_shadowCubeTexture);
    }   

    if (m_depthTexture != 0) {
        glDeleteTextures(1, &m_depthTexture);
    }
}

void RenderToTexture::writeToCubeFace(GLenum CubeFace)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, CubeFace, m_shadowCubeTexture, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "FB error, status: 0x" <<  Status << std::endl;
    }
}


void RenderToTexture::readFromCubeFace(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowCubeTexture);
}

GLuint RenderToTexture::getFrameBufferLocation(void) {
    return m_fbo;
}

GLuint RenderToTexture::getTexture(void) {
    return m_texture;
}

void RenderToTexture::bindForWriting(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void RenderToTexture::bindForReading(ShaderProgram* shaderProgram) {
  
    glActiveTexture(GL_TEXTURE0);       
    glBindTexture(GL_TEXTURE_2D, m_texture);

    GLint ImageSample = shaderProgram->getUniform("ImageSample");
    glUniform1i(ImageSample, 0);    

}