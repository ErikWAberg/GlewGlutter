#ifndef _LIGHT_ANIMATOR_STATIC_H
#define _LIGHT_ANIMATOR_STATIC_H

#include <vr/Matrix.h>
#include <UpdateCallback.h>
#include <State.h>
#include <ctime>
#include <Group.h>

class LightAnimatorStatic : public UpdateCallback {

public:
    LightAnimatorStatic(Group *groupWithLight)
            : UpdateCallback(), m_count(20), varying(-1), m_group(groupWithLight), m_iterations(0) {
        m_transMatrix = vr::Matrix();
    }

    ~LightAnimatorStatic() {
    }

    void update();

private:
    vr::Matrix m_transMatrix;
    int m_count;
    int varying;
    Group *m_group;
    int m_iterations;

};

#endif