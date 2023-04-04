#ifndef _SKYBOX_H
#define _SKYBOX_H

#include <string>
#include <CubemapTexture.h>
#include <ShaderProgram.h>
#include <vr/ObjLoader.h>
#include <Geometry.h>

class Skybox
{
public:
    Skybox(ShaderProgram* skyboxShader, Geometry* sphere);

    ~Skybox();

    bool init(const std::string& PosXFilename,
            const std::string& NegXFilename,
            const std::string& PosYFilename,
            const std::string& NegYFilename,
            const std::string& PosZFilename,
            const std::string& NegZFilename);

    void render();

private:

    CubemapTexture* m_cubemapTex;
    ShaderProgram* m_skyboxShader;
    vr::Vec3Vector projPlane;
    GLuint m_QVBO[2], m_QVAO;
    vr::UIntVector projIndices;


};

#endif