#include <Shadow.h>
#include <float.h>
#include <vr/shaderUtils.h>
#include <Scene.h>
#include <stdexcept>

Shadow::Shadow(int width, int height, ShaderProgram* shadowShader, Light* pointLight) : Group() {

    m_pointLight = pointLight;
    m_shadowShader = shadowShader;

    m_cameraDirections[0].cubemapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    m_cameraDirections[0].lookAt = vr::Vec3(1.0f, 0.0f, 0.0f);
    m_cameraDirections[0].up = vr::Vec3(0.0f, -1.0f, 0.0f); 
    m_cameraDirections[1].cubemapFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    m_cameraDirections[1].lookAt = vr::Vec3(-1.0f, 0.0f, 0.0f);
    m_cameraDirections[1].up = vr::Vec3(0.0f, -1.0f, 0.0f);
    m_cameraDirections[2].cubemapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    m_cameraDirections[2].lookAt = vr::Vec3(0.0f, 1.0f, 0.0f);
    m_cameraDirections[2].up = vr::Vec3(0.0f, 0.0f, -1.0f); 
    m_cameraDirections[3].cubemapFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    m_cameraDirections[3].lookAt = vr::Vec3(0.0f, -1.0f, 0.0f);
    m_cameraDirections[3].up = vr::Vec3(0.0f, 0.0f, -1.0f); //1.0
    m_cameraDirections[4].cubemapFace = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    m_cameraDirections[4].lookAt = vr::Vec3(0.0f, 0.0f, 1.0f);
    m_cameraDirections[4].up = vr::Vec3(0.0f, -1.0f, 0.0f);         
    m_cameraDirections[5].cubemapFace = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
    m_cameraDirections[5].lookAt = vr::Vec3(0.0f, 0.0f, -1.0f);
    m_cameraDirections[5].up = vr::Vec3(0.0f, -1.0f, 0.0f);         


    updateDimensions(width, height);

}

void Shadow::updateDimensions(int width, int height) {
    /*if(m_renderTexture != NULL) {
        m_renderTexture->~RenderToTexture();
    }
    if(m_pointCamera != NULL) {
        m_pointCamera->~Camera();
    }*/
    vr::Vec3 camCenter(0,0,0);
    vr::Vec3 camUp = vr::Vec3(0.0, 1.0, 0.0);
    m_pointCamera = new Camera(width, height, m_pointLight->getPositionVec3(), camCenter, camUp);
    m_pointCamera->enableFishEye();
    m_pointCamera->State::setShaderProgram(m_shadowShader);
    m_pointCamera->attachLight(m_pointLight);
    m_renderTexture = new RenderToTexture(width, height, true);
    if(!m_renderTexture->generateBuffers()) {
        throw std::runtime_error(std::string("Failed to generate buffer for 'RenderToTexture'"));
    }

}

Shadow::~Shadow() {
    m_renderTexture->~RenderToTexture();
    m_pointCamera->~Camera();
}

void Shadow::accept(NodeVisitor &nodeVisitor) {
    nodeVisitor.visit(this);
}

int shadow_prevFrameBuffer;
void Shadow::doShadowPass() {
    if(Scene::instance().isShadowDisabled()) {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &shadow_prevFrameBuffer);
        for(int i = 0; i < 6; i++) {
            m_renderTexture->writeToCubeFace(m_cameraDirections[i].cubemapFace);  
            glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        }
      //  glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, shadow_prevFrameBuffer);
    } else {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &shadow_prevFrameBuffer);

        m_prevShader = this->State::getShaderProgram();
        this->Group::attachShader(m_shadowShader);
        this->State::applyState();
        Scene::instance().setShadowPassActive(true);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_FRONT);
        
        for(int i = 0; i < 6; i++) {
            m_renderTexture->writeToCubeFace(m_cameraDirections[i].cubemapFace);        
            
            glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            
            m_pointCamera->changeLookAt(m_pointLight->getPositionVec3(),
                                m_pointLight->getPositionVec3() + m_cameraDirections[i].lookAt,
                                m_cameraDirections[i].up);
            m_pointCamera->State::applyState();
            m_pointCamera->attachMatricies();
            GLint eyePos = State::m_shaderProgram->getUniform("EyePos");
            glUniform3fv(eyePos, 1, m_pointLight->getPositionVec3().ptr());

            for (std::list<Node *>::const_iterator child = this->Group::children.begin(); child != this->Group::children.end(); child++) {
                (*child)->accept(m_renderVisitor);
            }
            if(i < 5) {
                this->accept(m_updateVisitor);
            }
        }


        this->useShadows();

      //  glBindFramebuffer(GL_FRAMEBUFFER, shadow_prevFrameBuffer);
    }

}

void Shadow::loadShadowMap(ShaderProgram* shaderProgram) {
    /*glReadBuffer(GL_COLOR_ATTACHMENT0 + gBufferType);
    GLint m_shadowMap = shaderProgram->getUniform("ShadowMap");
    glUniform1i(m_shadowMap, 10); //TODO FIX VALUE
    m_renderTexture->readFromCubeFace(GL_TEXTURE10); *///TODO FIX VALUE 1
     //GLint eyePos = State::m_shaderProgram->getUniform("EyePos");
   // glUniform3fv(eyePos, 1, m_pointLight->getPositionVec3().ptr());
}

void Shadow::useShadows() {
    glPopAttrib();
    this->Group::attachShader(m_prevShader);
    this->State::applyState();
    
    Scene::instance().setShadowPassActive(false);
    Scene::instance().getMainCamera()->attachMatricies();
    Scene::instance().getRoot()->applyState();
    
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_prevFrameBuffer);

    GLint m_shadowMap = m_shaderProgram->getUniform("ShadowMap");
    glUniform1i(m_shadowMap, 14); //TODO FIX VALUE

    m_renderTexture->readFromCubeFace(GL_TEXTURE14); //TODO FIX VALUE 1

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    
    GLint eyePos = State::m_shaderProgram->getUniform("EyePos");
    glUniform3fv(eyePos, 1, m_pointLight->getPositionVec3().ptr());

    this->accept(m_updateVisitor);
   

}
