#include <Skybox.h>
#include <Scene.h>


Geometry* sphere;

Skybox::Skybox(ShaderProgram* skyboxShader, Geometry* spheree) {
    m_skyboxShader = skyboxShader;
    sphere = spheree;

}

Skybox::~Skybox() {

}

bool Skybox::init(const std::string &PosXFilename,
        const std::string &NegXFilename,
        const std::string &PosYFilename,
        const std::string &NegYFilename,
        const std::string &PosZFilename,
        const std::string &NegZFilename) {

    projPlane.push_back(vr::Vec3(-1, 1, 0));
    projPlane.push_back(vr::Vec3(1, 1, 0));
    projPlane.push_back(vr::Vec3(1, -1, 0));
    projPlane.push_back(vr::Vec3(-1, -1, 0));

    projIndices.push_back(0);
    projIndices.push_back(1);
    projIndices.push_back(2);
    projIndices.push_back(0);
    projIndices.push_back(2);
    projIndices.push_back(3);
    

    GLint oldShader;
    glGetIntegerv(GL_CURRENT_PROGRAM, &oldShader);

    m_skyboxShader->useProgram();

    m_cubemapTex = new CubemapTexture(PosXFilename, NegXFilename, PosYFilename, NegYFilename, PosZFilename, NegZFilename);
    bool cubeCreated = m_cubemapTex->generateBuffers();
    if(!cubeCreated) {
        return false;
    }

    glGenVertexArrays(1, &m_QVAO);
    glGenBuffers(2, m_QVBO);
    glBindVertexArray(m_QVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_QVBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vr::Vec3::value_type) * 3 * projPlane.size(), &projPlane[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QVBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, projIndices.size() * sizeof(GLuint), &projIndices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(oldShader);
   
    return true;
}



void Skybox::render() {
    GLint oldCullFaceMode, oldDepthFuncMode, oldShader;
    GLboolean oldDepthMask;
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
    glGetBooleanv(GL_DEPTH_WRITEMASK, &oldDepthMask);
    glGetIntegerv(GL_CURRENT_PROGRAM, &oldShader);

    m_skyboxShader->useProgram();


    m_cubemapTex->bind(GL_TEXTURE10);
    GLint skyCube = m_skyboxShader->getUniform("skyCube");
    glUniform1i(skyCube, 10);
    GLint envMapping = m_skyboxShader->getUniform("envMapping");
    glUniform1i(envMapping, 1);
    

    ShaderProgram* cameraShader = Scene::instance().getMainCamera()->State::getShaderProgram();
    Scene::instance().getMainCamera()->State::setShaderProgram(m_skyboxShader);
    Scene::instance().getMainCamera()->attachMatricies();
    glDepthMask (GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    vr::Matrix scale;
    vr::Matrix translation;
    scale.makeScale(6, 6, 6);
    translation.makeTranslate(vr::Vec3(6,0,-2));
    translation.mult(scale, translation);

    sphere->State::setShaderProgram(m_skyboxShader);
    sphere->State::transform(translation);
    sphere->State::applyModel();
    sphere->draw();
    sphere->State::resetModel();
   
    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glEnable(GL_TEXTURE_2D);
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    envMapping = m_skyboxShader->getUniform("envMapping");
    glUniform1i(envMapping, 0);
    glBindVertexArray(m_QVAO);
    glDrawElements(GL_TRIANGLES, (GLsizei) projIndices.size(), GL_UNSIGNED_INT, ((void*)0));
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glUniform1i(skyCube, 0);



    glUseProgram(oldShader);
    Scene::instance().getMainCamera()->State::setShaderProgram(cameraShader);
    glCullFace(oldCullFaceMode);
    glDepthFunc(oldDepthFuncMode);
    glDepthMask(oldDepthMask);

}

