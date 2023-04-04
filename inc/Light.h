#ifndef _LIGHT_H
#define _LIGHT_H

#include <vr/Vec3.h>
#include <State.h>


class Light {

public:
    Light(vr::Vec4 position, vr::Vec3 ambient, vr::Vec3 diffuse, vr::Vec3 specular);

    ~Light() {
    }

    void apply(ShaderProgram* shaderProgram, int lightIndex);

    void move(vr::Vec3 movement);

    vr::Vec4 getPosition();
    vr::Vec3 getPositionVec3();

    void setEnabled(bool value);

    void toggleEnabled(void);

    bool isEnabled(void);
    
    float calcPointLightSphere(void);

private:
    vr::Vec4 m_position;
    vr::Vec3 m_ambient;
    vr::Vec3 m_diffuse;
    vr::Vec3 m_specular;

    float m_constant;
    float m_linear;
    float m_exp;
    float m_diffuseIntensity;
    float m_ambientIntensity;

    bool m_enabled;
};

#endif