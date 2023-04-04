#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <vr/ObjLoader.h>
#include <Shader.h>

class State;

class Material {

public:
    Material(const vr::material_t &material);

    ~Material() {
    }

    void apply(ShaderProgram* program);


private:
    vr::material_t m_material;
};

#endif