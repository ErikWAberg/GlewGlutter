#include <iostream>
#include <Light.h>
#include <sstream>
#include <Scene.h>
#include <algorithm>
#include <math.h> 

Light::Light(vr::Vec4 position, vr::Vec3 ambient, vr::Vec3 diffuse, vr::Vec3 specular) {
    m_position.set(position);
    m_ambient.set(ambient);
    m_diffuse.set(diffuse);
    m_specular.set(specular);

    m_constant = 0.0; 
    m_linear = 0.1;
    m_exp = 0.9;
    m_diffuseIntensity = 1.0;
    m_ambientIntensity = 0.0;
    m_enabled = true;
  //  std::cout << "Light sphere size: " << calcPointLightSphere() << std::endl;
}

void Light::setEnabled(bool value) {
    m_enabled = value;
}

void Light::toggleEnabled(void) {
    m_enabled = !m_enabled;
}

bool Light::isEnabled(void) {
    return m_enabled;
}

void Light::move(vr::Vec3 movement) {
    m_position = vr::Vec4(m_position.x() + movement.x(), m_position.y() + movement.y(), m_position.z() + movement.z(), m_position.w());
}

vr::Vec4 Light::getPosition() {
    return m_position;
}

vr::Vec3 Light::getPositionVec3() {
    vr::Vec3 v3Pos = vr::Vec3(m_position.x(), m_position.y(), m_position.z());
    return v3Pos;
}

void Light::apply(ShaderProgram* shaderProgram, int lightIndex) {

    if (m_enabled) {
        /*std::ostringstream sLightIndex;
        sLightIndex << "[";
        sLightIndex << lightIndex;
        sLightIndex << "]";

        GLint Position = shaderProgram->getUniform(std::string("LightPosition" + sLightIndex.str()).c_str());
        glUniform4fv(Position, 1, m_position.ptr());
        if(Scene::instance().isDoingShadowPass()) return;
        GLint Ambient = shaderProgram->getUniform(std::string("LightAmbient" + sLightIndex.str()).c_str());
        glUniform3fv(Ambient, 1, m_ambient.ptr());
        GLint Diffuse = shaderProgram->getUniform(std::string("LightDiffuse" + sLightIndex.str()).c_str());
        glUniform3fv(Diffuse, 1, m_diffuse.ptr());
        GLint Specular = shaderProgram->getUniform(std::string("LightSpecular" + sLightIndex.str()).c_str());
        glUniform3fv(Specular, 1, m_specular.ptr());
        sLightIndex.str("");*/

        GLint Position = shaderProgram->getUniform(std::string("LightPosition").c_str());
        glUniform4fv(Position, 1, m_position.ptr());
        if(Scene::instance().isDoingShadowPass()) return;
        GLint Ambient = shaderProgram->getUniform(std::string("LightAmbient").c_str());
        glUniform3fv(Ambient, 1, m_ambient.ptr());
        GLint Diffuse = shaderProgram->getUniform(std::string("LightDiffuse").c_str());
        glUniform3fv(Diffuse, 1, m_diffuse.ptr());
        GLint Specular = shaderProgram->getUniform(std::string("LightSpecular").c_str());
        glUniform3fv(Specular, 1, m_specular.ptr());
        GLint Constant = shaderProgram->getUniform(std::string("Constant").c_str());
        glUniform1f(Constant, m_constant);
        GLint Linear = shaderProgram->getUniform(std::string("Linear").c_str());
        glUniform1f(Linear, m_linear);
        GLint Exp = shaderProgram->getUniform(std::string("Exp").c_str());
        glUniform1f(Exp, m_linear);
        /*GLint DiffuseIntensity = shaderProgram->getUniform(std::string("DiffuseIntensity").c_str());
        glUniform1f(DiffuseIntensity, m_diffuseIntensity);
        GLint AmbientIntensity = shaderProgram->getUniform(std::string("AmbientIntensity").c_str());
        glUniform1f(AmbientIntensity, m_ambientIntensity);*/
        
    }
}

float Light::calcPointLightSphere(void) {

    float maxColor = std::max(std::max(m_diffuse.x(), m_diffuse.y()), m_diffuse.z());
    float ret = (-m_linear + sqrt(m_linear * m_linear - 4 * m_exp * (m_exp - 256 * maxColor * m_diffuseIntensity))) 
                / 2 * m_exp;        
    return ret ;
}