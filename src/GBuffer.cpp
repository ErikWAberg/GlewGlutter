#include <GBuffer.h>
#include <iostream>

GBuffer::GBuffer(int width, int height) {
    m_width = width;
    m_height = height;
}

GBuffer::~GBuffer() {
    if (m_fbo != 0) {
        glDeleteFramebuffers(1, &m_fbo);
    }

    if (m_textures[0] != 0) {
        glDeleteTextures(GBuffer::NUM_TYPES, m_textures);
    }

    if (m_depthTexture != 0) {
        glDeleteTextures(1, &m_depthTexture);
    }
}

bool GBuffer::generateBuffers() {

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(GBuffer::NUM_TYPES, m_textures);
    glGenTextures(1, &m_depthTexture);
    

    for (unsigned int i = 0 ; i < GBuffer::NUM_TYPES; i++) {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        if(GBuffer::SPECULAR == i) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);            
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
        }
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
        DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL);
    //glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);


    GLenum fb_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (fb_status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "GB FB error, status: 0x%x" << fb_status << std::endl;
        return false;
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;
}

void GBuffer::bindForWriting() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    glDrawBuffers(GBuffer::NUM_TYPES, DrawBuffers);

}

void GBuffer::bindForReading() {
    glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBuffer::FINAL);
    

}

void GBuffer::startFrame(void) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBuffer::FINAL);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::bindForGeomPass() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    glDrawBuffers(GBuffer::NUM_TYPES, DrawBuffers);
}

void GBuffer::bindForStencilPass() {
    glDrawBuffer(GL_NONE);
}

void GBuffer::bindForLightPass() {
    glDrawBuffer(GL_COLOR_ATTACHMENT0 + GBuffer::FINAL);

    for (unsigned int i = 0 ; i < GBuffer::NUM_TYPES; i++) {
        glActiveTexture(GL_TEXTURE0 + i);       
        glBindTexture(GL_TEXTURE_2D, m_textures[GBuffer::POSITION + i]);
    }
}

void GBuffer::bindForFinalPass() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + GBuffer::FINAL);
}


void GBuffer::setReadingBuffer(GBUFFER_TYPE gBufferType) {

    glReadBuffer(GL_COLOR_ATTACHMENT0 + gBufferType);
}

void GBuffer::loadFinalSample(ShaderProgram* shaderProgram, bool bindSample) {
    glActiveTexture(GL_TEXTURE0 + GBuffer::FINAL);       
    glBindTexture(GL_TEXTURE_2D, m_textures[GBuffer::FINAL]);
    if(bindSample) {
        GLint ImageSample = shaderProgram->getUniform("ImageSample");
        glUniform1i(ImageSample, GBuffer::FINAL);    
    }
    
}

void GBuffer::loadSamples(ShaderProgram* lightShader) {
   for (unsigned int i = 0 ; i < GBuffer::NUM_TYPES; i++) {
        glActiveTexture(GL_TEXTURE0 + i);       
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
    }

    GLint PositionSample = lightShader->getUniform("PositionSample");
    glUniform1i(PositionSample, GBuffer::POSITION);
    GLint DiffuseSample = lightShader->getUniform("DiffuseSample");
    glUniform1i(DiffuseSample, GBuffer::DIFFUSE);
    GLint NormalSample = lightShader->getUniform("NormalSample");
    glUniform1i(NormalSample, GBuffer::NORMAL);
    GLint AmbientSample = lightShader->getUniform("AmbientSample");
    glUniform1i(AmbientSample, GBuffer::AMBIENT);
    GLint SpecularSample = lightShader->getUniform("SpecularSample");
    glUniform1i(SpecularSample, GBuffer::SPECULAR);
    GLint ShadowSample = lightShader->getUniform("ShadowSample");
    glUniform1i(ShadowSample, GBuffer::SHADOW);

}

GLuint GBuffer::getFBO(void) {
    return m_fbo;
}

void GBuffer::drawBuffers(int offset, float sizeScale) {
    GLsizei widthScale = (GLsizei)(m_width / sizeScale);
    GLsizei heightScale = (GLsizei)(m_height / sizeScale);
    

    setReadingBuffer(GBuffer::POSITION);
    glBlitFramebuffer(0, 0, m_width, m_height,
                    offset, offset + heightScale, 
                    offset + widthScale, offset + 2 * widthScale, 
                    GL_COLOR_BUFFER_BIT, GL_LINEAR);

    setReadingBuffer(GBuffer::DIFFUSE);
    glBlitFramebuffer(0, 0, m_width, m_height, 
                    offset, offset + 2 * heightScale + 5, 
                    offset + widthScale, offset + 3 * heightScale, 
                    GL_COLOR_BUFFER_BIT, GL_LINEAR);

    setReadingBuffer(GBuffer::NORMAL);
    glBlitFramebuffer(0, 0, m_width, m_height, 
                    offset, offset + 3 * heightScale + 5,
                    offset + widthScale, offset + 4 * heightScale, 
                    GL_COLOR_BUFFER_BIT, GL_LINEAR);

    setReadingBuffer(GBuffer::AMBIENT);
    glBlitFramebuffer(0, 0, m_width, m_height, 
                    offset, offset + 4 * heightScale + 5,  
                    offset + widthScale, offset + 5 * heightScale, 
                    GL_COLOR_BUFFER_BIT, GL_LINEAR); 
    
    setReadingBuffer(GBuffer::SPECULAR);
    glBlitFramebuffer(0, 0, m_width, m_height, 
                    offset, offset + 5 * heightScale + 5,  
                    offset + widthScale, offset + 6 * heightScale, 
                    GL_COLOR_BUFFER_BIT, GL_LINEAR); 

    setReadingBuffer(GBuffer::SHADOW);
    glBlitFramebuffer(0, 0, m_width, m_height, 
                    offset, offset + 6 * heightScale + 5,  
                    offset + widthScale, offset + 7 * heightScale, 
                    GL_COLOR_BUFFER_BIT, GL_LINEAR); 
}