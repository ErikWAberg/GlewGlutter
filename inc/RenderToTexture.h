#ifndef _RENDER_TEXTURE_H
#define _RENDER_TEXTURE_H

#include <GL/glew.h>
#include <ShaderProgram.h>

class RenderToTexture {

public:

    RenderToTexture(int width, int height, bool makeCubeMap);
    ~RenderToTexture();

    
    void writeToCubeFace(GLenum CubeFace);

    void readFromCubeFace(GLenum TextureUnit);

    bool generateBuffers();

    GLuint getFrameBufferLocation(void);
    GLuint getTexture(void);
    
    void bindForReading(ShaderProgram* shaderProgram);

    void bindForWriting(void);
    

private:

    void clearBuffers();

    int m_width;
    int m_height;
    bool m_makeCubeMap;

    GLuint m_fbo;
    GLuint m_depthTexture;
    GLuint m_shadowCubeTexture;
    GLuint m_texture;

};
#endif