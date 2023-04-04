#ifndef _SHADOW_H
#define _SHADOW_H

#include <Group.h>
#include <RenderToTexture.h>
#include <vr/Vec3.h>
#include <Light.h>
#include <Camera.h>
#include <ShaderProgram.h>
#include <RenderVisitor.h>
#include <UpdateVisitor.h>

struct CameraDirection {
    GLenum cubemapFace;
    vr::Vec3 lookAt;
    vr::Vec3 up;
};


class Shadow : public Group {

public:
    Shadow(int width, int height, ShaderProgram* shadowShader, Light* pointLight);
    ~Shadow();

    //Node
    void accept(NodeVisitor &nodeVisitor);

    void doShadowPass();

    void updateDimensions(int width, int height);

    void loadShadowMap(ShaderProgram* shaderProgram);

private:

    void useShadows();

    RenderToTexture * m_renderTexture;
    Camera* m_pointCamera;
    Light* m_pointLight;
   
    ShaderProgram* m_shadowShader;
    ShaderProgram* m_prevShader;

    RenderVisitor m_renderVisitor;
    UpdateVisitor m_updateVisitor;

    GLint m_shadowPassLoc;
    CameraDirection m_cameraDirections[6];


};

#endif