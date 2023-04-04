#ifndef _GBUFFER_H
#define _GBUFFER_H

#include <GL/glew.h>
#include <ShaderProgram.h>

class GBuffer {
    
public:

    enum GBUFFER_TYPE {
        POSITION,
        DIFFUSE,
        NORMAL,
        AMBIENT,
        SPECULAR,
        SHADOW,
        FINAL,
        NUM_TYPES
    };

    GBuffer(int width, int height);
    ~GBuffer();

    bool generateBuffers();

    void bindForWriting();

    void bindForReading();

    void loadFinalSample(ShaderProgram* shaderProgram, bool bindSample);

    GLuint getFBO(void);

    void loadSamples(ShaderProgram* lightShader);

    void setReadingBuffer(GBUFFER_TYPE gBufferType);

    void drawBuffers(int offset, float sizeScale);

    void startFrame();
    
    void bindForGeomPass();

    void bindForStencilPass();

    void bindForLightPass();

    void bindForFinalPass();

private:

    GLuint m_fbo;
    GLuint m_depthTexture;
    GLuint m_textures[NUM_TYPES];
    GLenum DrawBuffers[GBuffer::NUM_TYPES];
    int m_width, m_height;

};


#endif