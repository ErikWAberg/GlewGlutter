#ifndef _STATE_H
#define _STATE_H

#include <Shader.h>
#include <ShaderProgram.h>
#include <Texture.h>
#include <vr/Matrix.h>
#include <Material.h>

#include <memory>
#include <GL/glew.h>
#include <State.h>

class Material;

class Light;

class State {
public:
    State(void)
            : m_hasModel(false),
              m_hasLights(false),
              m_hasTextures(false),
              m_enableTextures(true),
              m_hasMaterial(false),
              m_enableMaterial(true),
              m_enablePolygonMode(false) {
        m_model.makeIdentity();
    }

    ~State(void) {
    }

    void applyState();

    void resetState();

    int applyShader();

    void setShaderProgram(ShaderProgram *shaderProgram);

    int addTexture(const char *bmp_image);    

    void setHasModel(bool hasModel); 
    void setMaterial(Material *material);

    void transform(const vr::Matrix &transformMatrix);

    vr::Matrix getModel();

    void addLight(Light *light);

    bool hasLights();

    std::vector<Light *> getLights();

    void moveLight(size_t index, vr::Vec3 movement);

    void setEnableMaterial(bool value);

    void setEnableTextures(bool value);

    bool hasTexture() {
        return m_hasTextures;
    }

    void enablePolygonMode(bool value, GLenum mode);

    std::string getShaderName();

    ShaderProgram* getShaderProgram();

    void applyModel(void);

    void resetModel(void);

protected:
    ShaderProgram *m_shaderProgram;

    vr::Matrix m_model;
    std::vector<Texture *> m_textures;
    std::vector<Light *> m_lights;

private:
    bool m_hasModel;

    bool m_hasLights;

    bool m_hasTextures;
    bool m_enableTextures;

    bool m_hasMaterial;
    bool m_enableMaterial;

    bool m_enablePolygonMode;

    GLenum m_polygonMode;

    Material *m_material;

    void loadTextures(bool texturesEnabled);

};


#endif