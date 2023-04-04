#ifndef _TRANSFORM_UPDATE_H
#define _TRANSFORM_UPDATE_H

#include <vr/Matrix.h>
#include <UpdateCallback.h>
#include <State.h>

class TransformUpdate : public UpdateCallback {

public:
    TransformUpdate(const vr::Matrix &transMatrix, State *state)
            : UpdateCallback(), m_transMatrix(transMatrix), m_state(state) {
    }

    ~TransformUpdate() {
    }

    void update();

    void updateTransform(const vr::Matrix &transMatrix);

private:
    vr::Matrix m_transMatrix;
    State *m_state;

};

#endif