#ifndef _SIMPLE_ANIMATOR_H
#define _SIMPLE_ANIMATOR_H value

#include <vr/Matrix.h>
#include "State.h"
#include <ctime>
#include "UpdateCallback.h"

class BouncySquashAnimator : public UpdateCallback {

public:
    BouncySquashAnimator(State *state) : UpdateCallback(), m_state(state), m_count(60), varying(-1) {
        m_transMatrix = vr::Matrix();
    }

    ~BouncySquashAnimator() {
    }

    void update();

private:
    vr::Matrix m_transMatrix;
    State *m_state;
    int m_count;
    int varying;


};

#endif