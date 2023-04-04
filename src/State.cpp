#include <State.h>
#include <sstream>
#include <Light.h>
#include <Scene.h>

void State::applyState() {

    if (m_shaderProgram == NULL) {
        std::cerr << "State has no shader program!" << std::endl;
        return;
    }

    if(applyShader()) {
        std::cerr << "Unable to activate shader!" << std::endl;
        return;
    }

    if (m_hasModel) {
        applyModel();
    }

    if (m_enablePolygonMode) {
        glPolygonMode(GL_FRONT_AND_BACK, m_polygonMode);
    }

    if (m_hasLights) {
        /*for (size_t i = 0; i < m_lights.size(); i++) {
            m_lights[i]->apply(m_shaderProgram, i);
        }*/
    }

    if(Scene::instance().isDoingShadowPass())
        return;


    if (m_hasTextures && m_enableTextures) {
        loadTextures(true);
    }

    if (m_hasMaterial && m_enableMaterial) {
        m_material->apply(m_shaderProgram);
        GLint UseMaterial = m_shaderProgram->getUniform("UseMaterial");
        glUniform1i(UseMaterial, 1);
    }
}

void State::resetState() {

    resetModel();

    if (m_hasLights) {

    }

    if (m_enablePolygonMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if(Scene::instance().isDoingShadowPass())
        return;

    if (m_hasTextures && m_enableTextures) {
        loadTextures(false);
    }

    if (m_hasMaterial && m_enableMaterial) {
        GLint UseMaterial = m_shaderProgram->getUniform("UseMaterial");
        glUniform1i(UseMaterial, 0);
    }

}


void State::setHasModel(bool hasModel) {
    m_hasModel = hasModel;
    m_model.makeIdentity();
}


void State::applyModel(void) {
    GLint model = m_shaderProgram->getUniform("model");
    glUniformMatrix4fv(model, 1, GL_FALSE, &m_model.ptr()[0]);
}

void State::resetModel(void) {
    if (m_model.ptr() != NULL) {
        m_model.makeIdentity();
        m_hasModel = false;
    }
}

float g_n = 0;
void State::loadTextures(bool texturesEnabled) {
    GLint useTextureLocation = m_shaderProgram->getUniform("UseTexture");

    if(texturesEnabled) {
        g_n += 0.01;    
        float f = (1 + sinf(g_n)) * 0.5;
        for (size_t i = 0; i < m_textures.size(); i++) {
            std::ostringstream ss;
            ss << "Texture[";
            ss << i;
            ss << "]";
            std::string textureName = ss.str();

            GLint location = m_shaderProgram->getUniform(textureName.c_str());
            m_textures[i]->Texture::apply(location, (int) i);
           
            glUniform1i(useTextureLocation, (GLint) (i + 1));
            //  GLint SinTime = glGetUniformLocation(m_shaderProgram->program, "SinTime");
            //  glUniform1f(SinTime, f);
        }
    } else {

        glUniform1i(useTextureLocation, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

int State::applyShader() {
    if (m_shaderProgram != NULL) {
        m_shaderProgram->useProgram();
        return 0;
    }
    return 1;
}



// -------  Public: ---------


void State::enablePolygonMode(bool value, GLenum mode) {
    m_enablePolygonMode = value;
    m_polygonMode = mode;
}

void State::setEnableTextures(bool value) {
    m_enableTextures = value;
}

void State::addLight(Light *light) {
    m_lights.push_back(light);
    m_hasLights = true;
}

bool State::hasLights() {
    return m_lights.size() > 0;
}

std::vector<Light *> State::getLights() {
    return m_lights;
}

void State::moveLight(size_t index, vr::Vec3 movement) {
    if (!hasLights()) return;
    if (index >= 0 && index < m_lights.size()) {
        m_lights[index]->Light::move(movement);
    }
}


void State::setShaderProgram(ShaderProgram *shaderProgram) {
    m_shaderProgram = shaderProgram;
}

std::string State::getShaderName() {
    return m_shaderProgram->getName();
}

ShaderProgram* State::getShaderProgram() {
    return m_shaderProgram;
}


int State::addTexture(const char *bmp_image) {
    Texture *texture = new Texture(bmp_image);
    if(texture->hasTexture()) {
        m_textures.push_back(texture);
        m_hasTextures = true;
        return 0;    
    }
    
    return 1;
}

void State::setMaterial(Material *material) {
    m_material = material;
    m_hasMaterial = true;
}


void State::setEnableMaterial(bool value) {
    m_enableMaterial = value;
}
/*
void State::setIsShadowNode(bool isShadowNode) {
    m_isShadowNode = isShadowNode;
}*/

void State::transform(const vr::Matrix &transformMatrix) {
    vr::Matrix modelCopy = vr::Matrix();
    modelCopy.vr::Matrix::mult(vr::Matrix(), (m_model));
    m_model.vr::Matrix::mult(modelCopy, transformMatrix);
    m_hasModel = true;
}


vr::Matrix State::getModel() {
    return m_model;
}


