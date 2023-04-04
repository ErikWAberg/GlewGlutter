#include <TransformUpdate.h>


void TransformUpdate::update() {
    m_state->transform(m_transMatrix);
}

void TransformUpdate::updateTransform(const vr::Matrix &transMatrix) {
    m_transMatrix = transMatrix;
}

